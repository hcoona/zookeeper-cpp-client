#pragma once

namespace hcoona {
namespace zookeeper {

enum class ZErrorCode {
  ZOK = 0,
  ZSYSTEMERROR = -1,
  ZRUNTIMEINCONSISTENCY = -2,
  ZDATAINCONSISTENCY = -3,
  ZCONNECTIONLOSS = -4,
  ZMARSHALLINGERROR = -5,
  ZUNIMPLEMENTED = -6,
  ZOPERATIONTIMEOUT = -7,
  ZBADARGUMENTS = -8,
  ZINVALIDSTATE = -9,
  ZAPIERROR = -100,
  ZNONODE = -101,
  ZNOAUTH = -102,
  ZBADVERSION = -103,
  ZNOCHILDRENFOREPHEMERALS = -108,
  ZNODEEXISTS = -110,
  ZNOTEMPTY = -111,
  ZSESSIONEXPIRED = -112,
  ZINVALIDCALLBACK = -113,
  ZINVALIDACL = -114,
  ZAUTHFAILED = -115,
  ZCLOSING = -116,
  ZNOTHING = -117,
  ZSESSIONMOVED = -118
};

class ZClient {
 public:
  ZClient();
  ZClient(const ZClient&) = delete;
  ZClient& operator=(const ZClient&) = delete;
  ZClient(ZClient&&);
  ZClient& operator=(ZClient&& other);
  virtual ~ZClient();

 private:
  void Close();

  void* handle_;
};

}  // namespace zookeeper
}  // namespace hcoona
