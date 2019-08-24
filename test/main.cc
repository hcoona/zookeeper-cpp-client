#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "gsl/gsl"
#include "zkclient/zkclient.h"

using namespace hcoona::zookeeper;  // NOLINT

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Please provide Zookeeper hosts." << std::endl;
    return 1;
  }

  Client::SetDebugLevel(LogLevel::kDebug);
  {
    Client client(argv[1]);

    std::string path;
    path.resize(1024);
    ErrorCode error_code = client.CreateSync(
        "/zkclient_test", gsl::as_bytes(gsl::ensure_z("test_value")),
        Acl::kRead, CreateFlag::kEphemeral, &path);
    if (error_code != ErrorCode::kOk) {
      std::cerr << "Failed to create zknode: /zkclient_test: "
                << to_string(error_code) << std::endl;
      return 2;
    }
    std::cout << "Successfully created zknode: " << path << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}
