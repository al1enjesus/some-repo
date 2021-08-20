#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
#include <functional>
#include "Callback.h"
#include <gtest/gtest.h>

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

void SplitIntoWords(const string &file_name, vector<vector<string>> &lines){
  fstream file(file_name, ios::in);
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
}

template <typename Predicate>
void PrepareFile(const string &file_name, Predicate predicate, Callback &callback) {
  vector<vector<string>> lines;
  SplitIntoWords(file_name, lines);

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
  fstream file(file_name, ios::out);
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
  if (argc < 2) {
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


TEST(WorkingWithFileTests, IncorrectFileName){
  bool checking_value = false;
  string file_name = "dont_exists.txt";
  fstream input_file;
  input_file.open(file_name, ios::in);
  if (!input_file){
    cout << "Error with opening file." << endl;
    checking_value = true;
  }
  input_file.close();

  EXPECT_EQ(true, checking_value);
}

TEST(WorkingWithFileTests, CorrectFileName){
  bool checking_value = false;
  string file_name = "file.txt";
  fstream input_file;
  input_file.open(file_name, ios::in);
  if (!input_file){
    cout << "Error with opening file." << endl;
    checking_value = true;
  }
  cout << "Input file opened correctly." << endl;
  input_file.close();
  EXPECT_EQ(false, checking_value);
}

TEST(PrepareFileTests, CorrectWorkWithDigitsPredicate){
  string file_name = "temp";
  fstream input_file;
  Callback callback;
  callback.ConnectWithLogFile();
  std::string answer;


  input_file.open(file_name, ios::out);
  input_file << "123 dawdw AW2" << endl;
  input_file.close();
  PrepareFile(file_name, ConsistsOfDigits, callback);
  input_file.open(file_name, ios::in);
  getline(input_file, answer);
  input_file.close();
  EXPECT_EQ(answer, "dawdw AW2");


  input_file.open(file_name, ios::out);
  input_file << "AAA bbb aAB2" << endl;
  input_file.close();
  PrepareFile(file_name, ConsistsOfDigits, callback);
  input_file.open(file_name, ios::in);
  getline(input_file, answer);
  input_file.close();
  EXPECT_EQ(answer, "AAA bbb aAB2");
}

TEST(PrepareFileTests, CorrectWorkWithUpperCasePredicate){
  string file_name = "temp";
  fstream input_file;
  Callback callback;
  callback.ConnectWithLogFile();
  std::string answer;


  input_file.open(file_name, ios::out);
  input_file << "123 dawdw AWW" << endl;
  input_file.close();
  PrepareFile(file_name, ConsistsOfUpperCase, callback);
  input_file.open(file_name, ios::in);
  getline(input_file, answer);
  input_file.close();
  EXPECT_EQ(answer, "123 dawdw");


  input_file.open(file_name, ios::out);
  input_file << "bbb 22 aAB2" << endl;
  input_file.close();
  PrepareFile(file_name, ConsistsOfUpperCase, callback);
  input_file.open(file_name, ios::in);
  getline(input_file, answer);
  input_file.close();
  EXPECT_EQ(answer, "bbb 22 aAB2");
}

TEST(SplitFileIntoWordsTests, CorrectRegExWork){
  string file_name = "temp";
  fstream input_file;
  vector<vector<string>> lines;
  vector<vector<string>> answer;

  input_file.open(file_name, ios::out);
  input_file << "This is my message, with punctuation signs!? Yes." << endl;
  input_file.close();
  answer.push_back(vector<string>{"This", "is", "my", "message", "with", "punctuation", "signs", "Yes"});
  SplitIntoWords(file_name, lines);
  EXPECT_EQ(lines, answer);

  input_file.open(file_name, ios::out);
  input_file << "Simple            strange    . ! . ? .... message" << endl;
  input_file.close();
  answer.clear();
  lines.clear();
  answer.push_back(vector<string>{"Simple", "strange", "message"});
  SplitIntoWords(file_name, lines);
  EXPECT_EQ(lines, answer);
}