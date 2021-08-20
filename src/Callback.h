//
// Created by Ilya on 20.08.2021.
//

#ifndef SOME_REPO_SRC_CALLBACK_H_
#define SOME_REPO_SRC_CALLBACK_H_

#include <functional>
#include <fstream>
#include <vector>

class Callback {
 public:
  Callback() = default;
  explicit Callback(std::function<bool(const std::string &word)> predicate);
  ~Callback();

  void ConnectWithPredicate(std::function<bool(const std::string &word)> predicate);
  void ConnectWithLogFile(const std::string &filename=(std::string)("log.txt"));
  int GetCountPredicates();
  bool operator()(const std::string &word);

 private:
  std::vector<std::function<bool(const std::string &word)>> predicates;
  bool is_connected_with_file = false;
  std::ofstream *log_file = new std::ofstream();
  void CallbackHandler(const std::string &word);
};

#endif //SOME_REPO_SRC_CALLBACK_H_
