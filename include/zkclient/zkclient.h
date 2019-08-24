#pragma once

#include <string>
#include "gsl/gsl"
#include "zkclient/zkadapter.h"

namespace hcoona {
namespace zookeeper {

class Client {
 public:
  Client();
  explicit Client(string_view host);
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;
  Client(Client&&);
  Client& operator=(Client&& other);
  virtual ~Client();

  static void SetDebugLevel(LogLevel log_level);
  static void EnableDeterministicConnectOrder(bool yes_or_no);

  // Milliseconds according to calculate_interval in zookeeper.c
  int receive_timeout_ms() const;
  State state() const;

  // TODO(hcoona): Do not expose `struct ACL` in `zookeeper.jute.h`.
  ErrorCode CreateSync(string_view path, gsl::span<const gsl::byte> value,
                       gsl::span<Acl> acl, CreateFlag flags,
                       std::string* created_path);

 private:
  void Close();
  void Callback(WatchEventType type, State state, string_view path);

  void* handle_;
};

}  // namespace zookeeper
}  // namespace hcoona
