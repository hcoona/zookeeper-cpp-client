#include <chrono>
#include <iostream>
#include <thread>

#include "zkclient.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Please provide Zookeeper hosts." << std::endl;
    return -1;
  }

  hcoona::zookeeper::Client::SetDebugLevel(hcoona::zookeeper::LogLevel::kDebug);
  {
    hcoona::zookeeper::Client client(argv[1]);
    std::this_thread::sleep_for(std::chrono::seconds(20));
  }
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return 0;
}
