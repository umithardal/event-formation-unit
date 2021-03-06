/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

//#include <common/gccintel.h> // UNUSED macros
#include <cstring>
#include <common/Trace.h>
#include <readout/ReadoutParser.h>

// #undef TRC_LEVEL
// #define TRC_LEVEL TRC_L_DEB

const unsigned int MaxUdpDataSize{8972};
const unsigned int MinDataSize{4 + PAD_SIZE}; // just cookie and version

ReadoutParser::ReadoutParser() {
  std::memset(NextSeqNum, 0, sizeof(NextSeqNum));
}

int ReadoutParser::validate(const char *Buffer, uint32_t Size, uint8_t Type) {
  std::memset(&Packet, 0, sizeof(Packet));

  if (Buffer == nullptr or Size == 0) {
    XTRACE(PROCESS, WAR, "no buffer specified");
    Stats.ErrorBuffer++;
    return -ReadoutParser::EBUFFER;
  }

  if ((Size < MinDataSize) || (Size > MaxUdpDataSize)) {
    XTRACE(PROCESS, WAR, "Invalid data size (%u)", Size);
    Stats.ErrorSize++;
    return -ReadoutParser::ESIZE;
  }

  uint32_t CookieVer = *(uint32_t *)(Buffer + PAD_SIZE);
  // 'E', 'S', 'S', 0x00 - cookie + version 0
  if (CookieVer != 0x00535345) { // 'ESS0' little endian
    Stats.ErrorVersion++;
    return -ReadoutParser::EHEADER;
  }

  // Packet is ESS readout version 0,
  // now we can add more header size checks
  if (Size < sizeof(PacketHeaderV0)) {
    XTRACE(PROCESS, WAR, "Invalid data size for v0 (%u)", Size);
    Stats.ErrorSize++;
    return -ReadoutParser::ESIZE;
  }

  // It is safe to cast packet header v0 strut to data
  Packet.HeaderPtr = (PacketHeaderV0 *)Buffer;

  if (Size < Packet.HeaderPtr->TotalLength or
      Packet.HeaderPtr->TotalLength < sizeof(PacketHeaderV0)) {
    XTRACE(PROCESS, WAR, "Data length mismatch, expected %u, got %u",
           Packet.HeaderPtr->TotalLength, Size);
    Stats.ErrorSize++;
    return -ReadoutParser::ESIZE;
  }

  if (Packet.HeaderPtr->TypeSubType != Type) {
    XTRACE(PROCESS, WAR, "Unsupported data type for v0 (%u)", Type);
    Stats.ErrorTypeSubType++;
    return -ReadoutParser::EHEADER;
  }

  if (Packet.HeaderPtr->OutputQueue >= MaxOutputQueues) {
    XTRACE(PROCESS, WAR, "Output queue %u exceeds max size %u",
           Packet.HeaderPtr->OutputQueue, MaxOutputQueues);
    Stats.ErrorOutputQueue++;
    return -ReadoutParser::EHEADER;
  }

  if (NextSeqNum[Packet.HeaderPtr->OutputQueue] != Packet.HeaderPtr->SeqNum) {
    XTRACE(PROCESS, WAR, "Bad sequence number (expected %u, got %u)",
           NextSeqNum, Packet.HeaderPtr->SeqNum);
    Stats.ErrorSeqNum++;
    NextSeqNum[Packet.HeaderPtr->OutputQueue] = Packet.HeaderPtr->SeqNum;
  }

  NextSeqNum[Packet.HeaderPtr->OutputQueue]++;
  Packet.DataPtr = (char *)(Buffer + sizeof(PacketHeaderV0));
  Packet.DataLength = Packet.HeaderPtr->TotalLength - sizeof(PacketHeaderV0);

  return ReadoutParser::OK;
}
