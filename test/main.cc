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
    ErrorCode error_code;

    std::string created_path;
    created_path.resize(1024);
    error_code = client.CreateSync(
        "/zkclient_test", gsl::as_bytes(gsl::ensure_z("test_value")),
        Acl::kRead, CreateFlag::kEphemeral, &created_path);
    if (error_code != ErrorCode::kOk) {
      std::cerr << "Failed to create zknode: /zkclient_test: "
                << to_string(error_code) << std::endl;
      return 2;
    }
    std::cout << "Successfully created zknode: " << created_path << std::endl;

    std::future<std::tuple<ErrorCode, std::string>> create_future =
        client.CreateAsync("/zkclient_test_async",
                           gsl::as_bytes(gsl::ensure_z("test_value")),
                           Acl::kRead, CreateFlag::kEphemeral);
    std::tie(error_code, created_path) = create_future.get();
    if (error_code != ErrorCode::kOk) {
      std::cerr << "Failed to create zknode: /zkclient_test_async: "
                << to_string(error_code) << std::endl;
      return 2;
    }
    std::cout << "Successfully created zknode: " << created_path << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}
