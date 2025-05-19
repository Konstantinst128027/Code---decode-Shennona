/* Berezkin Konstantin Evgenievich
./main st128027@student.spbu.ru*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <random>
#include <ctime>
#include <bitset>

struct Symbol {
    char character;
    double probability;
};

void Read_Text(std::string& text, std::string& infilename);

std::vector<Symbol> Calculate_Frequencies(std::string& text);
void Build_ShannonCodes(std::vector<Symbol>& symbols, std::unordered_map <std::string, char>& decode_dict, std::unordered_map <char, std::string>& code_dict);

std::string Encode_Text(std::string& text, std::unordered_map <char, std::string>& code_dict);
std::string Decode_Text(std::string& encoded, std::unordered_map <std::string, char>& decode_dict);

void Save_Dictionary_And_Encoded(std::string& outfilename, std::unordered_map<std::string, char>& decode_dict, std::string& encoded);
void Read_Encoded_And_Dict(std::string& encoded, std::string& outfilename, std::unordered_map <std::string, char>& decode_dict);

std::string generate_100_char_string();
#endif
