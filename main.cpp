#include <iostream>
#include <vector>
#include <thread>
#include <sstream>

#include "threadfuncs.h"

int main() {
  about();

  // Open log file
  Logger logger("output.log");

  logger.writeLine("main: pid = " + std::to_string(getThreadID())
                 + ", opened file: 'output.log'");

  // args for threads
  std::vector<ThreadArgs> args(COUNT_THREADS);
for (int i = 0; i < COUNT_THREADS; ++i) {
    std::ostringstream oss;
    oss << "T" << i;
    args[i].id = i;
    args[i].tag = oss.str();
}

  // thread are starting
  std::vector<std::thread> threads;
  threads.reserve(COUNT_THREADS);

  // promise/future для первого потока
std::promise<std::string> prom;
std::future<std::string> fut = prom.get_future();

threads.emplace_back(funcThread, std::cref(args[0]), std::ref(logger), std::move(prom));

for (int i = 1; i < COUNT_THREADS; ++i) {
    std::promise<std::string> dummy;
    threads.emplace_back(funcThread, std::cref(args[i]), std::ref(logger), std::move(dummy));
}

  // wait for stop all threads
  for (auto& t : threads) {
    if (t.joinable()) t.join();         
  }

  std::string result = fut.get();
  std::cout << "Поток вернул: " << result << "\n";

  std::cout << "counter = " << counter << "\n";
  // close file automatically
 // std::cout << "main: all threads finished, file closed\n";
  return 0;
}
