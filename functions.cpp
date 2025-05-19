/* Berezkin Konstantin Evgenievich
./main st128027@student.spbu.ru*/
#include "functions.h"

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
    for (char bit : text) {
        current_code += bit;
        auto it = decode_dict.find(current_code);
        if (it != decode_dict.end()) {
            decoded += it->second; // tak kak it - iterator
            current_code.clear();
        }
    }
    if (!current_code.empty()) {
        throw std::runtime_error("The decode_dict or text is incorrected");
    }
    return decoded;
}

// Save decode_dict and encoded text in outfile
void Save_Dictionary_And_Encoded(std::string& outfilename, std::unordered_map<std::string, char>& decode_dict, std::string& encoded) {
    std::ofstream outfile(outfilename, std::ios::binary);
    if (!outfile) {
        throw std::runtime_error("Couldn't open the file " + outfilename);
    }
    
    // 1. Write encode_lenght and encoded itself
    uint16_t encoded_size = encoded.size();
    outfile.write(reinterpret_cast<char*>(&encoded_size), sizeof(encoded_size));

    while (encoded.size() % 8 != 0) {
        encoded += '0';
    }
    
    for (size_t i = 0; i < encoded.size(); i += 8) {
        std::bitset<8> byte(encoded.substr(i, 8)); // substr - reduces the string to 8 characters, bitset converts the string to a string of bytes and adds "0" to the beginning
        outfile.put(static_cast<char>(byte.to_ulong())); // to_ulong convert byte line in number, i am doing this because type is bitset
    }
    
    // 2. Write decode_dict size and decode_dict itself
    uint16_t dict_size = decode_dict.size();
    outfile.write(reinterpret_cast<char*>(&dict_size), sizeof(dict_size));

    for (auto [code, symbol] : decode_dict) {
        outfile.put(symbol);

        uint8_t code_length = code.size();
        outfile.put(code_length); // i am not converting in char, because 8 bits is one character and type is uint8_t

        std::string padded_code = code;
        while (padded_code.size() % 8 != 0) { // so that there are full bytes, on the left as it will be easier to read
            padded_code += '0';
        }

        for (size_t i = 0; i < padded_code.size(); i += 8) {
            std::bitset<8> byte(padded_code.substr(i, 8)); // substr - reduces the string to 8 characters, bitset converts the string to a string of bytes and adds "0" to the beginning
            outfile.put(static_cast<char>(byte.to_ulong())); // to_ulong convert byte line in number, i am doing this because type is bitset
        }
    }
}

// Read encode and decode_dict
void Read_Encoded_And_Dict(std::string& encoded, std::string& outfilename, std::unordered_map <std::string, char>& decode_dict) {
    
    std::ifstream outfile(outfilename, std::ios::binary);
    
    if (!outfile) {
        throw std::runtime_error("Couldn`t open the file" + outfilename);
    }
    
    // 1. read size encoded and encoded itself
    uint16_t encoded_size;
    outfile.read(reinterpret_cast<char*>(&encoded_size), sizeof(encoded_size)); // because type is uint16_t
    
    if (outfile.fail()) { // fail checks whether the last input operation ended with an error
            throw std::runtime_error("Failed to read encoded_size");
    }
    
    size_t byte_count_encoded = (encoded_size + 7) / 8; // +7 since there are 8 bits in a byte
    
    for (size_t j = 0; j < byte_count_encoded; ++j) {
            
        uint8_t byte = outfile.get();
            
        if (outfile.fail()) { // fail checks whether the last input operation ended with an error
            throw std::runtime_error("Failed to read byte");
        }
            
        encoded += std::bitset<8>(byte).to_string(); // to_string converted bits line in string
    }

    encoded = encoded.substr(0, encoded_size); // cut encoded to encoded_size symbols

    // checking for dictionary correctness
    for (char character : encoded) {
        if (character != '0' and character != '1') {
            throw std::runtime_error("Encoded_text is incorrected");
        }
    }
    
    // 2. read size dict and dict itself
    uint16_t dict_size;
    outfile.read(reinterpret_cast<char*>(&dict_size), sizeof(dict_size)); // because type is uint16_t
    
    if (outfile.fail()) { // fail checks whether the last input operation ended with an error
            throw std::runtime_error("Failed to read dict_size");
    }
     
    for (uint16_t i = 0; i < dict_size; ++i) { 

        char symbol = outfile.get();
        
        if (outfile.fail()) { // fail checks whether the last input operation ended with an error
            throw std::runtime_error("Failed to read symbol");
        }
        
        uint8_t code_length = outfile.get();
    
        if (outfile.fail()) { // fail checks whether the last input operation ended with an error
            throw std::runtime_error("Failed to read code_length");
        }

        size_t byte_count_code = (code_length + 7) / 8; // +7 since there are 8 bits in a byte
        
        std::string code;

        for (size_t j = 0; j < byte_count_code; ++j) {
            
            uint8_t byte = outfile.get();
            
            if (outfile.fail()) { // fail checks whether the last input operation ended with an error
                throw std::runtime_error("Failed to read byte");
            }
            
            code += std::bitset<8>(byte).to_string(); // to_string converted bits line in string
        }

        code = code.substr(0, code_length); // cut code_bits to code_length symbols

        decode_dict[code] = symbol;
    }    
}

//Generate string of 100 random char
std::string generate_100_char_string() {
    std::string result;
    std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<char> dist(32, 126);
    
    for (int i = 0; i < 100; ++i) {
        result += dist(gen);
    }
    
    return result;
}
