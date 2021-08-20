#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <functional>
#include "Callback.h"

using namespace std;

[[maybe_unused]] bool ConsistsOfDigits(const string &word)
{
  if (count_if(word.begin(), word.end(), [](const char &c){return isdigit(c);})
      == word.size()){
    return true;
  }
  return false;
}

[[maybe_unused]] bool ConsistsOfUpperCase(const string &word)
{
  if (count_if(word.begin(), word.end(), [](const char &c){return isupper(c);})
      == word.size()){
    return true;
  }
  return false;
}

template <typename Predicate>
void PrepareFile(const string &file_name, Predicate predicate, Callback callback) {
  // region Splitting file into words with regex and save them into "lines"
  fstream file(file_name, ios::in);
  vector<vector<string>> lines;
  string row;
  while (getline(file, row)) {
    // Splitting with regular expression
    cmatch result;
    // There may be more separators here.
    regex pattern(R"(,|\.|!|\?|\s+)");
    sregex_token_iterator iter(row.begin(), row.end(), pattern, -1);
    sregex_token_iterator end;

    vector<string> words_in_line;
    for (; iter != end; iter++) {
      if (iter->length()) {
        words_in_line.push_back(*iter);
      }
    }
    lines.push_back(words_in_line);
  }
  file.close();
  // endregion

  // region Removing ban words

  // Connect our CallBack with predicate
  callback.ConnectWithPredicate(predicate);

  // If you want to use more than 1 predicate just add
  // example at the next line
  // callback.ConnectWithPredicate(ConsistsOfUpperCase);

  for (auto &line : lines)
  {
    // bind helps avoid CallBack's copy constructor
    line.erase(remove_if(line.begin(), line.end(), bind(ref(callback), placeholders::_1)),
               line.end());
  }
  // endregion


  // region Editing file
  file.open(file_name, ios::out);
  for (int i = 0; i < lines.size(); i++){
    for (int j = 0; j < lines[i].size(); j++){
      if (j == lines[i].size() - 1){
        file << lines[i][j];
      } else {
        file << lines[i][j] << " ";
      }
    }
    if (i != lines.size() - 1){
      file << endl;
    }
  }
  file.close();
  // endregion
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Incorrect input." << endl;
    return 1;
  }
  string file_name = argv[1];
  cout << "Recognized file name: " << file_name << endl;

  fstream input_file;
  input_file.open(file_name, ios::in);
  if (!input_file){
    cout << "Error with opening file." << endl;
    return 1;
  }
  cout << "Input file opened correctly." << endl;
  input_file.close();


  Callback callback;
  // Connect with log file, if you want to write file into output file
  // if you'll commit the next line, you'll see banned words in console
  callback.ConnectWithLogFile();
  PrepareFile(file_name, ConsistsOfDigits, callback);

  cout << "The program worked correctly." << endl;
  return 0;
}
