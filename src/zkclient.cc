#include "zkclient/zkclient.h"

#include <utility>
#include "zookeeper.h"      // NOLINT
#include "zookeeper_log.h"  // NOLINT

namespace hcoona {
namespace zookeeper {

// TODO(hcoona): Move them into configuration class.
static constexpr int kReceiveTimeoutMs = 30000;

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

ErrorCode Client::CreateSync(string_view path, gsl::span<const gsl::byte> value,
                             gsl::span<Acl> acl, CreateFlag flags,
                             std::string* created_path) {
  struct ACL_vector z_acls {
    gsl::narrow_cast<int>(acl.size()), reinterpret_cast<::ACL*>(acl.data())
  };
  return static_cast<ErrorCode>(zoo_create(
      to_zoo(handle_), path.data(), reinterpret_cast<const char*>(value.data()),
      value.size_bytes(), &z_acls, static_cast<int>(flags),
      const_cast<char*>(created_path->data()), created_path->size()));
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
