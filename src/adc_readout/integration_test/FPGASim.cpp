/* Copyright (C) 2017-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#include "FPGASim.h"
#include <ciso646>
#include <functional>
#include <iostream>

using UdpEndpoint = asio::ip::udp::endpoint;
using namespace std::chrono_literals;

struct QueryResult {
  explicit QueryResult(asio::ip::udp::resolver::iterator &&Endpoints)
      : EndpointIterator(std::move(Endpoints)) {
    while (EndpointIterator != asio::ip::udp::resolver::iterator()) {
      auto CEndpoint = *EndpointIterator;
      EndpointList.push_back(CEndpoint);
      ++EndpointIterator;
    }
    std::sort(EndpointList.begin(), EndpointList.end(), [](auto &a, auto &b) {
      return a.address().is_v6() < b.address().is_v6();
    });
  }
  asio::ip::udp::endpoint getNextEndpoint() {
    if (NextEndpoint < EndpointList.size()) {
      return EndpointList[NextEndpoint++];
    }
    return {};
  }
  bool isDone() const { return NextEndpoint >= EndpointList.size(); }
  asio::ip::udp::resolver::iterator EndpointIterator;
  std::vector<UdpEndpoint> EndpointList;
  unsigned int NextEndpoint{0};
};

FPGASim::FPGASim(std::string DstAddress, std::uint16_t DstPort,
                 asio::io_service &Service)
    : Address(std::move(DstAddress)), Port(DstPort),
      Socket(Service, UdpEndpoint()), Resolver(Service),
      ReconnectTimeout(Service), HeartbeatTimeout(Service),
      DataPacketTimeout(Service),
      TransmitBuffer(std::make_unique<DataPacket>(MaxPacketSize)),
      SharedStandbyBuffer(std::make_unique<DataPacket>(MaxPacketSize)) {
  resolveDestination();

  IdlePacket.Head.Version = Protocol::VER_1;
  IdlePacket.Head.Type = PacketType::Idle;
  IdlePacket.Head.ReadoutLength = 20;
  IdlePacket.Head.ReadoutCount = 0;
  IdlePacket.Head.ClockMode = Clock::Clk_Ext;
  IdlePacket.Head.OversamplingFactor = 1;
  IdlePacket.Idle.TimeStamp = {0, 0};
  IdlePacket.fixEndian();
}

void FPGASim::resolveDestination() {
  asio::ip::udp::resolver::query Query(asio::ip::udp::v4(), Address,
                                       std::to_string(Port));
  auto ResolveHandlerGlue = [this](auto &Error, auto EndpointIterator) {
    this->handleResolve(Error, EndpointIterator);
  };
  Resolver.async_resolve(Query, ResolveHandlerGlue);
}

void FPGASim::reconnectWait() {
  auto HandlerGlue = [this](auto &) { this->resolveDestination(); };
  ReconnectTimeout.expires_after(1s);
  ReconnectTimeout.async_wait(HandlerGlue);
}

void FPGASim::tryConnect(QueryResult AllEndpoints) {
  UdpEndpoint CurrentEndpoint = AllEndpoints.getNextEndpoint();
  auto HandlerGlue = [this, AllEndpoints](auto &Error) {
    this->handleConnect(Error, AllEndpoints);
  };
  Socket.async_connect(CurrentEndpoint, HandlerGlue);
}

void FPGASim::handleResolve(const asio::error_code &Error,
                            asio::ip::udp::resolver::iterator EndpointIter) {
  if (Error) {
    reconnectWait();
    return;
  }
  QueryResult AllEndpoints(std::move(EndpointIter));
  tryConnect(AllEndpoints);
}

void FPGASim::handleConnect(const asio::error_code &Error,
                            QueryResult const &AllEndpoints) {
  if (!Error) {
    startHeartbeatTimer();
    return;
  }
  Socket.close();
  if (AllEndpoints.isDone()) {
    reconnectWait();
    return;
  }
  tryConnect(AllEndpoints);
}

void FPGASim::startHeartbeatTimer() {
  HeartbeatTimeout.expires_after(4s);
  HeartbeatTimeout.async_wait([this](auto &Error) {
    if (not Error) {
      transmitHeartbeat();
      startHeartbeatTimer();
    }
  });
}

void FPGASim::startPacketTimer() {
  DataPacketTimeout.expires_after(500ms);
  DataPacketTimeout.async_wait([this](auto &Error) {
    if (not Error) {
      swapAndTransmitSharedStandbyBuffer();
      startHeartbeatTimer();
    }
  });
}

void FPGASim::transmitHeartbeat() {
  TimeStamp IdleTS{CurrentRefTimeNS + RefTimeDeltaNS,
                   TimeStamp::ClockMode::External};
  auto IdleRawTS = RawTimeStamp{IdleTS.getSeconds(), IdleTS.getSecondsFrac()};
  IdleRawTS.fixEndian();
  IdlePacket.Head.ReferenceTimeStamp = IdleRawTS;
  IdlePacket.Idle.TimeStamp = IdleRawTS;

  transmitPacket(&IdlePacket, sizeof(IdlePacket));
}

void FPGASim::transmitPacket(const void *DataPtr, const size_t Size) {
  Socket.async_send(asio::buffer(DataPtr, Size),
                    [this](auto &, auto) { this->incNumSentPackets(); });
}

// used by the generators
void FPGASim::addSamplingRun(void const *const DataPtr, size_t Bytes,
                             TimeStamp Timestamp) {
  if (CurrentRefTimeNS == 0) {
    CurrentRefTimeNS = Timestamp.getTimeStampNS();
  }
  while (Timestamp.getTimeStampNS() > CurrentRefTimeNS + RefTimeDeltaNS) {
    CurrentRefTimeNS += RefTimeDeltaNS;
  }

  bool Success =
      SharedStandbyBuffer->addSamplingRun(DataPtr, Bytes, CurrentRefTimeNS);
  std::pair<size_t, size_t> BufferSizes = SharedStandbyBuffer->getBufferSizes();
  size_t Size = BufferSizes.first;
  size_t MaxSize = BufferSizes.second;

  if (not Success or MaxSize - Size < 20) {
    swapAndTransmitSharedStandbyBuffer();
    assert(SharedStandbyBuffer->addSamplingRun(DataPtr, Bytes, CurrentRefTimeNS));
  } else {
    startPacketTimer();
  }
  ++SamplingRuns;
  startHeartbeatTimer();
}

void FPGASim::swapAndTransmitSharedStandbyBuffer() {
  std::swap(TransmitBuffer, SharedStandbyBuffer);
  SharedStandbyBuffer->resetPacket();
  auto DataInfo = TransmitBuffer->getBuffer(PacketCount);
  PacketCount++;
  transmitPacket(DataInfo.first, DataInfo.second);
}
