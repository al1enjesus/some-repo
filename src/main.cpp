#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <functional>

bool ConsistsOfDigits(const std::string &word)
{
    if (std::count_if(word.begin(), word.end(), [](const char &c){
        return std::isdigit(c);
    }) == word.size()) {
        return true;
    }
    return false;
}

bool ConsistsOfUpperCase(const std::string &word)
{
    if (std::count_if(word.begin(), word.end(), [](const char &c){
        return std::isupper(c);
    }) == word.size()){
        return true;
    }
    return false;
}

class CallBack
{
public:
    CallBack() = default;
    explicit CallBack(std::function<bool(const std::string &word)> predicate){
        predicates.emplace_back(std::move(predicate));
    }
    void ConnectWithPredicate(std::function<bool(const std::string &word)> predicate){
        predicates.emplace_back(std::move(predicate));
    }
    void ConnectWithLogFile(const std::string &filename=(std::string)("log.txt"))
    {
        if ((*log_file).is_open()){
            (*log_file).close();
        }
        (*log_file).open(filename, std::ios::out);
        (*log_file) << "Banned words: ";
        is_connected_with_file = true;
    }
    bool operator()(const std::string &word)
    {
        if (std::any_of(predicates.begin(), predicates.end(), [&word](auto predicate){return predicate(word);}))
        {
            CallBackHandler(word);
            return true;
        }
        return false;
    };

    ~CallBack(){
        (*log_file).close();
        delete log_file;
        if (!is_connected_with_file){
            this->PrintBannedWords();
        }
    }

    void PrintBannedWords(){
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
private:
    std::vector<std::function<bool(const std::string &word)>> predicates;
    bool is_connected_with_file = false;
    std::ofstream *log_file = new std::ofstream();
    std::vector<std::string> banned_words{};

    void CallBackHandler(const std::string &word){
        // Flagging handler
        if (is_connected_with_file){
            *log_file << word << " ";
        } else {
            banned_words.emplace_back(word);
        }
    }
};

template <typename Predicate, typename CallBack>
void PrepareFile(const std::string &file_name, Predicate predicate, CallBack callback) {
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

    // Connect our CallBack with predicate
    callback.ConnectWithPredicate(predicate);

    // If you want to use more than 1 predicate just add
    // example at the next line
    // callback.ConnectWithPredicate(ConsistsOfUpperCase);

    for (auto &line : lines)
    {
        // std::bind helps avoid CallBack's copy constructor
        line.erase(std::remove_if(line.begin(), line.end(), std::bind(std::ref(callback), std::placeholders::_1)),
                   line.end());
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


    CallBack callback;
    // Connect with log file, if you want to write file into output file
    // if you'll commit the next line, you'll see banned words in console
    callback.ConnectWithLogFile();
    PrepareFile(file_name, ConsistsOfDigits, callback);

    std::cout << "The program worked correctly." << std::endl;
    return 0;
}
