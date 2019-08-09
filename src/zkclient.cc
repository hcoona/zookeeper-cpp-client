#include "zkclient.h"

#include <utility>

#include "zookeeper.h"
#include "zookeeper_log.h"

namespace hcoona {
namespace zookeeper {

// TODO(hcoona): Move them into configuration class.
static constexpr int kReceiveTimeoutMs = 30000;

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

// Defined in zookeeper.c
std::string to_string(WatchType type) {
  switch (type) {
    case WatchType::kError:
      return "ZOO_ERROR_EVENT";
    case WatchType::kCreated:
      return "ZOO_CREATED_EVENT";
    case WatchType::kDeleted:
      return "ZOO_DELETED_EVENT";
    case WatchType::kChanged:
      return "ZOO_CHANGED_EVENT";
    case WatchType::kChild:
      return "ZOO_CHILD_EVENT";
    case WatchType::kSession:
      return "ZOO_SESSION_EVENT";
    case WatchType::kNotWatching:
      return "ZOO_NOTWATCHING_EVENT";
  }
  return "INVALID_EVENT";
}

static zhandle_t* to_zoo(void* handle) {
  return reinterpret_cast<zhandle_t*>(handle);
}

Client::Client() = default;

// TODO(hcoona): Persist & Reload client_id from file.
Client::Client(const std::string& host) {
  handle_ = zookeeper_init(
      host.c_str(),
      +[](zhandle_t* zh, int type, int state, const char* path,
          void* watcherCtx) {
        Client* client = reinterpret_cast<Client*>(watcherCtx);
        WatchType watch_type = static_cast<WatchType>(type);
        State zk_state = static_cast<State>(state);

        client->Callback(watch_type, zk_state, path);
      },
      kReceiveTimeoutMs, nullptr, this, 0);
}

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

void Client::SetDebugLevel(LogLevel logLevel) {
  zoo_set_debug_level(static_cast<ZooLogLevel>(logLevel));
}

void Client::EnableDeterministicConnectOrder(bool yesOrNo) {
  zoo_deterministic_conn_order(yesOrNo);
}

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
    if (error_code != ErrorCode::kOk) {
      LOG_ERROR(("Failed to close zookeeper session, handle=%x, error_code=%s",
                 handle_, to_string(error_code).c_str()));
    }
  }
}

void Client::Callback(WatchType type, State state, const char* path) {
  LOG_DEBUG(("handle=%x,type=%s,state=%s", handle_, to_string(type).c_str(),
             to_string(state).c_str()));
}

}  // namespace zookeeper
}  // namespace hcoona
