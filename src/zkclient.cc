#include "zkclient.h"

#include <utility>

#include "zookeeper.h"

namespace hcoona {
namespace zookeeper {

ZClient::ZClient() = default;

ZClient::ZClient(ZClient&& other) {
  handle_ = other.handle_;
  other.handle_ = nullptr;
}

ZClient& ZClient::operator=(ZClient&& other) {
  if (this != &other) {
    Close();

    handle_ = other.handle_;
    other.handle_ = nullptr;
  }
  return *this;
}

ZClient::~ZClient() { Close(); }

void ZClient::Close() {
  if (handle_) {
    ZErrorCode error_code = static_cast<ZErrorCode>(
        zookeeper_close(reinterpret_cast<zhandle_t*>(handle_)));
  }
}

}  // namespace zookeeper
}  // namespace hcoona
