/* Berezkin Konstantin Evgenievich
./main st128027@student.spbu.ru*/
#include "functions.h"

// makes a vector consisting of a symbol and its probability
std::vector<Symbol> Calculate_Frequencies(std::string& text) {
    std::map<char, int> freqMap;
    for (char c : text) freqMap[c]++;
    
    std::vector<Symbol> symbols;
    double total = text.length();
    for (auto pair : freqMap) {
        symbols.push_back({pair.first, pair.second/total});
    }
    return symbols;
}

// Produces a Shannon encoding code
void Build_ShannonCodes(std::vector <Symbol>& symbols, std::unordered_map <std::string, char>& decode_dict, std::unordered_map <char, std::string>& code_dict) {
    std::sort(symbols.begin(), symbols.end(), [](Symbol a, Symbol b) {return a.probability > b.probability;});
    if (symbols.size() == 1) {
        std::string code;
        code += '0';
        code_dict[symbols[0].character] = code;
        decode_dict[code] = symbols[0].character;           
    }
    else {
        double cumulativeProb = 0.0;
        for (auto symbol : symbols) {
    
            int len = std::ceil(-std::log2(symbol.probability));
            std::string code;
            double val = cumulativeProb;
        
            for (int j = 0; j < len; ++j) {
                val *= 2;
                if (val >= 1) {
                    code += '1';
                    val -= std::floor(val);
                }
                else {
                    code += '0';
                }
            }
            code_dict[symbol.character] = code;
            decode_dict[code] = symbol.character;
            cumulativeProb += symbol.probability;
        }
    }
}

//Encode text
std::string Encode_Text(std::string& text, std::unordered_map <char, std::string>& code_dict) {
    
    std::string encoded;
    for (char c : text) {
        auto it = code_dict.find(c);
        if (it == code_dict.end()) {
            throw std::runtime_error("Unknown character: " + c);
        }
        encoded += it->second;
    }
    return encoded;
}

// Decode text
std::string Decode_Text(std::string& text, std::unordered_map <std::string, char>& decode_dict)
{
    std::string decoded, current_code;
    for (char c : text) {
        if (c != '1' and c != '0') {
            throw std::runtime_error("Encoded text is incorrected");
        }
    }
    for (char bit : text) {
        current_code += bit;
        auto it = decode_dict.find(current_code);
        if (it != decode_dict.end()) {
            decoded += it->second; // tak kak it - iterator
            current_code.clear();
        }
    }
    return decoded;
}

// Check decode_dict from outfile
void Check_Decode_Dict(std::unordered_map <std::string, char>& decode_dict) {
    for (auto [code, symbol]: decode_dict) {
        for (char c: code) {
            if (c != '0' and c != '1') {
                throw std::runtime_error("The decode_dict is incorrected");
            }
        }
    }
}

// Save decode_dict and encoded text in outfile
void Save_Dictionary_And_Encoded(std::string& outfilename, std::string& encoded, std::unordered_map <std::string, char>& decode_dict) {
    std::ofstream outfile(outfilename);
    if (!outfile) {
        throw std::runtime_error("Couldn`t open the file");
    }
    
    outfile << encoded << "\n";
    
    for (auto [code, symbol] : decode_dict) {
        if (symbol == '\n') {
            outfile << code << " " << "\\n" << "\n";
        }
        else if (symbol == '\t') {
            outfile << code << " " << "\\t" << "\n";
        }
        else if (symbol == ' ') {
            outfile << code << " " << "\\s" << "\n";
        }
        else {
            outfile << code << " " << symbol << "\n";
        }
    }
}

// Read text and decode_dict
void Read_Encoded_File(std::string& infilename, std::string& text, std::unordered_map <std::string, char>& decode_dict) {
    
    std::ifstream infile(infilename);
    if (!infile) {
        throw std::runtime_error("Couldn`t open the file");
    }
    
    std::getline(infile, text);

    std::string line;
    while (std::getline(infile, line)) {

        if (line.find_first_not_of(" \t") == std::string::npos) {
            continue;
        }

        std::istringstream iss(line);
        std::string symbol_part, code;

        if (!(iss >> code >> symbol_part)) {
            throw std::runtime_error("incorrect format - (expect '<symbol> <code>')");
        }

        char symbol;
        if (symbol_part == "\\s") {
            symbol = ' ';
        } else if (symbol_part == "\\t") {
            symbol = '\t';
        } else if (symbol_part == "\\n") {
            symbol = '\n';
        } else if (symbol_part.size() != 1) {
            throw std::runtime_error("incorrect symbol " + symbol_part + " (must be 1 symbol or \\s, \\t, \\n)");
        } else {
            symbol = symbol_part[0];
        }

        decode_dict[code] = symbol;
    }
    
    Check_Decode_Dict(decode_dict);
}
// Read text from file in std::string text
void Read_Text(std::string& text, std::string& infilename) {
    std::ifstream infile(infilename);
    if (!infile) {
        throw std::runtime_error("Couldn`t open the file");
    }
    
    std::stringstream buffer;
    buffer << infile.rdbuf(); // Read all file in stringstream
    text = buffer.str();    // convert to std::string
    
    size_t last_valid_pos = text.find_last_not_of("\n");
    
    // Deleted the last "\n"
    if (last_valid_pos != std::string::npos) {
        text.erase(last_valid_pos + 1);
    } 
    // if file is empty - error
    else {
        throw std::runtime_error(infilename + " is empty");
    }
}
