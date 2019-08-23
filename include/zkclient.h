#pragma once

#include <string>

#if __cplusplus > 201703L
#include <string_view>
#else
#include "absl/strings/string_view.h"
#endif  // C++17

namespace hcoona {
namespace zookeeper {

#if __cplusplus > 201703L
using std::string_view;
#else
using absl::string_view;
#endif  // C++17

// Defined in zookeeper.h
enum class LogLevel { kError = 1, kWarning = 2, kInformation = 3, kDebug = 4 };

// Defined in zookeeper.h
enum class ErrorCode : int {
  kOk = 0,
  kSystemError = -1,
  kRuntimeInconsistency = -2,
  kDataInconsistency = -3,
  kConnectionLoss = -4,
  kMarshallingError = -5,
  kUnimplemented = -6,
  kOperationTimeout = -7,
  kBadArguments = -8,
  kInvalidState = -9,
  kApiError = -100,
  kNoNode = -101,
  kNoAuth = -102,
  kBadVersion = -103,
  kNoChildrenForEphemerals = -108,
  kNodeExists = -110,
  kNotEmpty = -111,
  kSessionExpired = -112,
  kInvalidCallback = -113,
  kInvalidAcl = -114,
  kAuthFailed = -115,
  kClosing = -116,
  kNothing = -117,
  kSessionMoved = -118
};

// Defined in zk_adaptor.h
enum class State : int {
  kExpiredSession = -112,
  kAuthFailed = -113,
  kClosed = 0,
  kConnecting = 1,
  kAssociating = 2,
  kConnected = 3
};

// Defined in zookeeper.h
enum class WatchEventType : int {
  kError = 0,
  kCreated = 1,
  kDeleted = 2,
  kChanged = 3,
  kChild = 4,
  kSession = -1,
  kNotWatching = -2,
};

// Defined in zookeeper.c
enum class CreateFlag : int { kEphemeral = 1 << 0, kSequence = 1 << 1 };

std::string to_string(ErrorCode error_code);

std::string to_string(State state);

std::string to_string(WatchEventType type);

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

  ErrorCode Create(absl::string_view path);

 private:
  void Close();
  void Callback(WatchEventType type, State state, string_view path);

  void* handle_;
};

}  // namespace zookeeper
}  // namespace hcoona
