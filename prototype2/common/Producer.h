/* Copyright (C) 2016-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
/// \brief Wrapper class for sending data to Kafka broker
///
//===----------------------------------------------------------------------===//

#pragma once

#include <librdkafka/rdkafkacpp.h>

///
class ProducerBase {
public:
  ProducerBase() = default;
  virtual ~ProducerBase() = default;
  virtual int produce(char *buffer, int length) = 0;
};

class Producer : public ProducerBase, public RdKafka::DeliveryReportCb {
public:
  /// \brief Construct a producer object.
  /// \param broker 'URL' specifying host and port, example "127.0.0.1:9009"
  /// \param topicstr Name of Kafka topic according to agreement, example
  /// "T-REX_detectors"
  Producer(std::string broker, std::string topicstr);

  /// \brief cleans up by deleting allocated structures
  ~Producer();

  /// \brief Function called to send data to a broker
  /// \param buffer Pointer to char buffer containing data to be tx'ed
  /// \param length Size of buffer data in bytes
  int produce(char *buffer, int length) override;

  /// \brief set kafka configuration and check result
  void setConfig(std::string configName, std::string configValue);

  /// \brief check kafka configuration result
  void checkConfig(RdKafka::Conf::ConfResult configResult);


  /// \brief Kafka callback function for delivery reports
  void dr_cb(RdKafka::Message &message) override;

  /// \brief Kafka callback function for events
  void event_cb(RdKafka::Event &event);

  struct {
    uint64_t ev_errors;
    uint64_t ev_others;
    // uint64_t ev_log;
    // uint64_t ev_stats;
    // uint64_t ev_throttle;
    uint64_t dr_errors;
    uint64_t dr_noerrors;
    uint64_t produce_fails;
  } stats = {};

private:
  std::string kafkaErrstr;
  std::string topicString;
  RdKafka::Conf *conf{0};
  RdKafka::Conf *tconf{0};
  RdKafka::Topic *topic{0};
  RdKafka::Producer *producer{0};
};
