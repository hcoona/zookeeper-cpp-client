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
    case State::Closed:
      return "ZOO_CLOSED_STATE";
    case State::Connecting:
      return "ZOO_CONNECTING_STATE";
    case State::Associating:
      return "ZOO_ASSOCIATING_STATE";
    case State::Connected:
      return "ZOO_CONNECTED_STATE";
    case State::ExpiredSession:
      return "ZOO_EXPIRED_SESSION_STATE";
    case State::AuthFailed:
      return "ZOO_AUTH_FAILED_STATE";
  }
  return "INVALID_STATE";
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

void Client::Close() {
  if (handle_) {
    ErrorCode error_code = static_cast<ErrorCode>(
        zookeeper_close(reinterpret_cast<zhandle_t*>(handle_)));
    // TODO: Log it. LOG(ERROR) << to_string(zerror)
  }
}

}  // namespace zookeeper
}  // namespace hcoona
