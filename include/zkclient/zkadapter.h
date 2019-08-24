#pragma once

#include <string>
#include "gsl/gsl"

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

// Defined in zookeeper.c
enum class PermissionFlag : int32_t {
  kRead = 1 << 0,
  kWrite = 1 << 1,
  kCreate = 1 << 2,
  kDelete = 1 << 3,
  kAdmin = 1 << 4,
  kAll = 0x1f,
};

std::string to_string(LogLevel log_level);

std::string to_string(ErrorCode error_code);

std::string to_string(State state);

std::string to_string(WatchEventType event_type);

std::string to_string(CreateFlag create_flag);

// Must keep align with `struct Id` in "zookeeper.jute.h".
// Don't use virtual function.
class Id {
 public:
  Id();
  Id(string_view scheme, string_view id);
  Id(const Id& other);
  Id& operator=(const Id& other);
  Id(Id&& other);
  Id& operator=(Id&& other);
  ~Id();

  static const Id kAnyone;                // ZOO_ANYONE_ID_UNSAFE
  static const Id kCurrentAuthenticated;  // ZOO_AUTH_IDS

  string_view scheme() const { return scheme_; }
  string_view id() const { return id_; };

 private:
  char* scheme_;
  char* id_;
};

// Must keep align with `struct ACL` in "zookeeper.jute.h"
// Don't use virtual function
class Acl {
 public:
  Acl() = default;
  Acl(PermissionFlag perms, Id id);
  Acl(const Acl& other) = default;
  Acl& operator=(const Acl& other) = default;
  Acl(Acl&& other) = default;
  Acl& operator=(Acl&& other) = default;
  ~Acl() = default;

  static const gsl::span<Acl> kOpen;        // ZOO_OPEN_ACL_UNSAFE
  static const gsl::span<Acl> kRead;        // ZOO_READ_ACL_UNSAFE
  static const gsl::span<Acl> kCreatorAll;  // ZOO_CREATOR_ALL_ACL

  PermissionFlag permissions() const { return perms_; }
  const Id& id() const { return id_; }

 private:
  PermissionFlag perms_;
  Id id_;
};

}  // namespace zookeeper
}  // namespace hcoona
