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

struct Symbol {
    char character;
    double probability;
};

std::vector<Symbol> Calculate_Frequencies(std::string& text);
void Build_ShannonCodes(std::vector<Symbol>& symbols, std::unordered_map <std::string, char>& decode_dict, std::unordered_map <char, std::string>& code_dict);

std::string Encode_Text(std::string& text, std::unordered_map <char, std::string>& code_dict);
void Save_Dictionary_And_Encoded(std::string& outfilename, std::string& encoded, std::unordered_map <std::string, char>& decode_dict);

std::string Decode_Text(std::string& encoded, std::unordered_map <std::string, char>& decode_dict);
void Check_Decode_Dict(std::unordered_map <std::string, char>& decode_dict);

void Read_Encoded_File(std::string& infilename, std::string& text, std::unordered_map <std::string, char>& decode_dict);
void Read_Text(std::string& text, std::string& infilename);

#endif
