#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>

bool ConsistsOfDigits(const std::string &word)
{
    if (std::count_if(word.begin(), word.end(), [](const char &c){
        return std::isdigit(c);
    }) == word.size()) {
        return true;
    }
    return false;
}

template <typename Predicate>
void PrepareFile(const std::string &file_name, Predicate predicate) {
    // region Splitting file into words with regex and save them into "lines"
    std::fstream file(file_name, std::ios::in);
    std::vector<std::vector<std::string>> lines;
    std::string row;
    while (std::getline(file, row)) {
        // Splitting with regular expression
        std::cmatch result;
        // There may be more separators here.
        std::regex pattern(",|\\.|!|\\?|\\s+");
        std::sregex_token_iterator iter(row.begin(), row.end(), pattern, -1);
        std::sregex_token_iterator end;

        std::vector<std::string> words_in_line;
        for (; iter != end; iter++) {
            if (iter->length()) {
                words_in_line.push_back(*iter);
            }
        }
        lines.push_back(words_in_line);
    }
    file.close();
    // endregion


    // Easy way with std::partition for divide banwords and whitewords
    /*
        auto newEnd = std::partition(line.begin(), line.end(), predicate);
        for (auto it = newEnd; it != line.end(); ++it){
            std::cout << *it << " ";
        }
    */


    // region Removing ban words
    for (auto &line : lines)
    {
        line.erase(std::remove_if(line.begin(), line.end(), predicate), line.end());
    }
    // endregion


    // region Editing file
    file.open(file_name, std::ios::out);
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines[i].size(); j++){
            if (j == lines[i].size() - 1){
                file << lines[i][j];
            } else {
                file << lines[i][j] << " ";
            }
        }
        if (i != lines.size() - 1){
            file << std::endl;
        }
    }
    file.close();
    // endregion
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect input." << std::endl;
        return 0;
    }
    std::string file_name = argv[1];
    std::cout << "Recognized file name: " << file_name << std::endl;

    std::fstream input_file;
    input_file.open(file_name, std::ios::in);
    if (!input_file){
        std::cout << "Error with opening file." << std::endl;
        return 0;
    }
    std::cout << "Input file opened correctly." << std::endl;
    input_file.close();

    PrepareFile(file_name, ConsistsOfDigits);

    std::cout << "The program worked correctly." << std::endl;
    return 0;
}
