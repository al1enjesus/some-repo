//
// Created by Ilya on 20.08.2021.
//

#include "Callback.h"
#include <iostream>

Callback::Callback(std::function<bool(const std::string &word)> predicate){
  predicates.emplace_back(std::move(predicate));
}

Callback::~Callback(){
  (*log_file).close();
  delete log_file;
}

void Callback::ConnectWithPredicate(std::function<bool(const std::string &word)> predicate){
  predicates.emplace_back(std::move(predicate));
}

[[maybe_unused]] void Callback::ConnectWithLogFile(const std::string &filename)
{
  if ((*log_file).is_open()){
    (*log_file).close();
  }
  (*log_file).open(filename, std::ios::out);
  is_connected_with_file = true;
}

[[maybe_unused]] int Callback::GetCountPredicates() {
  return predicates.size();
}

bool Callback::operator()(const std::string &word){
  if (std::any_of(predicates.begin(), predicates.end(), [&word](auto predicate){return predicate(word);}))
  {
    CallbackHandler(word);
    return true;
  }
  return false;
}

void Callback::CallbackHandler(const std::string &word){
  if (is_connected_with_file){
    *log_file << word << " ";
  } else {
    std::cout << word << " ";
  }
}