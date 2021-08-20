//
// Created by Ilya on 20.08.2021.
//

#include "Callback.h"
#include <iostream>

[[maybe_unused]] Callback::Callback(std::function<bool(const std::string &word)> predicate){
  predicates.emplace_back(std::move(predicate));
}

Callback::~Callback(){
  (*log_file).close();
  delete log_file;
  if (!is_connected_with_file){
    this->PrintBannedWords();
  }
}

void Callback::ConnectWithPredicate(std::function<bool(const std::string &word)> predicate){
  predicates.emplace_back(std::move(predicate));
}

void Callback::ConnectWithLogFile(const std::string &filename)
{
  if ((*log_file).is_open()){
    (*log_file).close();
  }
  (*log_file).open(filename, std::ios::out);
  (*log_file) << "Banned words: ";
  is_connected_with_file = true;
}

void Callback::PrintBannedWords(){
  if (!banned_words.empty()) {
    std::cout << "Banned words: ";
    for (auto &word: banned_words) {
      std::cout << word << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "There aren't banned words." << std::endl;
  }
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
    banned_words.emplace_back(word);
  }
}