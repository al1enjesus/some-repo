#include <iostream>
#include <fstream>
#include <algorithm>

bool ConsistsOfDigits(const std::string &word)
{
    if (std::count_if(word.begin(), word.end(), [](const char &c){
        return std::isdigit(c);
    }) == word.size()) {
        return true;
    }
    return false;
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

    std::cout << "The program worked correctly." << std::endl;
    return 0;
}
