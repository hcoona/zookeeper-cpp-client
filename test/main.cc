#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "gsl/gsl"
#include "zkclient/zkclient.h"
// TODO(hcoona): Do not expose `struct ACL` & IDs.
#include "zookeeper.h"  // NOLINT

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Please provide Zookeeper hosts." << std::endl;
    return 1;
  }

  hcoona::zookeeper::Client::SetDebugLevel(hcoona::zookeeper::LogLevel::kDebug);
  {
    hcoona::zookeeper::Client client(argv[1]);

    std::string path;
    path.resize(1024);
    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_READ, ZOO_ANYONE_ID_UNSAFE}};
    hcoona::zookeeper::ErrorCode error_code = client.CreateSync(
        "/zkclient_test", gsl::as_bytes(gsl::ensure_z("test_value")),
        CREATE_ONLY_ACL, hcoona::zookeeper::CreateFlag::kEphemeral, &path);
    if (error_code != hcoona::zookeeper::ErrorCode::kOk) {
      std::cerr << "Failed to create zknode: /zkclient_test: "
                << hcoona::zookeeper::to_string(error_code) << std::endl;
      return 2;
    }
    std::cout << "Successfully created zknode: " << path << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(20));
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));
  return 0;
}
