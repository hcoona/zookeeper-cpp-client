#include "zkclient.h"

#include <utility>

#include "zookeeper.h"

namespace hcoona {
namespace zookeeper {

std::string to_string(ErrorCode error_code) {
  return zerror(static_cast<int>(error_code));
}

// Defined in zookeeper.c
std::string to_string(State state) {
  switch (state) {
    case State::kClosed:
      return "ZOO_CLOSED_STATE";
    case State::kConnecting:
      return "ZOO_CONNECTING_STATE";
    case State::kAssociating:
      return "ZOO_ASSOCIATING_STATE";
    case State::kConnected:
      return "ZOO_CONNECTED_STATE";
    case State::kExpiredSession:
      return "ZOO_EXPIRED_SESSION_STATE";
    case State::kAuthFailed:
      return "ZOO_AUTH_FAILED_STATE";
  }
  return "INVALID_STATE";
}

static zhandle_t* to_zoo(void* handle) {
  return reinterpret_cast<zhandle_t*>(handle);
}

Client::Client() = default;

Client::Client(Client&& other) {
  handle_ = other.handle_;
  other.handle_ = nullptr;
}

Client& Client::operator=(Client&& other) {
  if (this != &other) {
    Close();

    handle_ = other.handle_;
    other.handle_ = nullptr;
  }
  return *this;
}

Client::~Client() { Close(); }

int Client::receive_timeout_ms() const {
  return zoo_recv_timeout(to_zoo(handle_));
}

State Client::state() const {
  return static_cast<State>(zoo_state(to_zoo(handle_)));
}

void Client::Close() {
  if (handle_) {
    ErrorCode error_code =
        static_cast<ErrorCode>(zookeeper_close(to_zoo(handle_)));
    (void)error_code;  // TODO: Log it. LOG(ERROR) << to_string(zerror)
  }
}

}  // namespace zookeeper
}  // namespace hcoona
