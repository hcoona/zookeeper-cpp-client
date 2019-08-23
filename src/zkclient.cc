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
std::string to_string(WatchEventType type) {
  switch (type) {
    case WatchEventType::kError:
      return "ZOO_ERROR_EVENT";
    case WatchEventType::kCreated:
      return "ZOO_CREATED_EVENT";
    case WatchEventType::kDeleted:
      return "ZOO_DELETED_EVENT";
    case WatchEventType::kChanged:
      return "ZOO_CHANGED_EVENT";
    case WatchEventType::kChild:
      return "ZOO_CHILD_EVENT";
    case WatchEventType::kSession:
      return "ZOO_SESSION_EVENT";
    case WatchEventType::kNotWatching:
      return "ZOO_NOTWATCHING_EVENT";
  }
  return "INVALID_EVENT";
}

static zhandle_t* to_zoo(void* handle) {
  return reinterpret_cast<zhandle_t*>(handle);
}

Client::Client() = default;

// TODO(hcoona): Persist & Reload client_id from file.
Client::Client(string_view host) {
  handle_ = zookeeper_init(
      host.data(),
      +[](zhandle_t* zh, int type, int state, const char* path,
          void* watcherCtx) {
        Client* client = reinterpret_cast<Client*>(watcherCtx);
        WatchEventType watch_type = static_cast<WatchEventType>(type);
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

void Client::SetDebugLevel(LogLevel log_level) {
  zoo_set_debug_level(static_cast<ZooLogLevel>(log_level));
}

void Client::EnableDeterministicConnectOrder(bool yes_or_no) {
  zoo_deterministic_conn_order(yes_or_no);
}

int Client::receive_timeout_ms() const {
  return zoo_recv_timeout(to_zoo(handle_));
}

State Client::state() const {
  return static_cast<State>(zoo_state(to_zoo(handle_)));
}

ErrorCode Client::Create(string_view path, gsl::span<const gsl::byte> value,
                         const std::vector<struct ACL>& acl, int flags,
                         std::string* created_path) {
  return static_cast<ErrorCode>(to_zoo(handle_), path.data(), value.data())
}

void Client::Close() {
  if (handle_) {
    ErrorCode error_code =
        static_cast<ErrorCode>(zookeeper_close(to_zoo(handle_)));
    if (error_code != ErrorCode::kOk) {
      LOG_ERROR(("[handle=%x] Failed to close zookeeper session, error_code=%s",
                 handle_, to_string(error_code).data()));
    }
  }
}

void Client::Callback(WatchEventType type, State state, string_view path) {
  LOG_DEBUG(("[handle=%x] type=%s, state=%s, path=%s", handle_,
             to_string(type).data(), to_string(state).data(), path.data()));

  if (type == WatchEventType::kSession) {
    LOG_WARN(("[handle=%x] Session state changed to %s", handle_,
              to_string(state).data()));
    return;
  }
}

}  // namespace zookeeper
}  // namespace hcoona
