#pragma once

#include <string>

namespace hcoona {
namespace zookeeper {

// Defined in zookeeper.h
enum class ErrorCode : int {
  Ok = 0,
  SystemError = -1,
  RuntimeInconsistency = -2,
  DataInconsistency = -3,
  ConnectionLoss = -4,
  MarshallingError = -5,
  Unimplemented = -6,
  OperationTimeout = -7,
  BadArguments = -8,
  InvalidState = -9,
  ApiError = -100,
  NoNode = -101,
  NoAuth = -102,
  BadVersion = -103,
  NoChildrenForEphemerals = -108,
  NodeExists = -110,
  NotEmpty = -111,
  SessionExpired = -112,
  InvalidCallback = -113,
  InvalidAcl = -114,
  AuthFailed = -115,
  Closing = -116,
  Nothing = -117,
  SessionMoved = -118
};

// Defined in zk_adaptor.h
enum class State : int {
  ExpiredSession = -112,
  AuthFailed = -113,
  Closed = 0,
  Connecting = 1,
  Associating = 2,
  Connected = 3
};

// Defined in zookeeper.c
enum class CreateFlag : int { Ephemeral = 1 << 0, Sequence = 1 << 1 };

std::string to_string(ErrorCode error_code);

std::string to_string(State state);

class Client {
 public:
  Client();
  explicit Client(const std::string& host);
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;
  Client(Client&&);
  Client& operator=(Client&& other);
  virtual ~Client();

 private:
  void Close();

  void* handle_;
};

}  // namespace zookeeper
}  // namespace hcoona
