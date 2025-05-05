#include "functions.h"

int main(int argc, char* argv[]) {
    bool decode_mode = false;

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-decode") {
            decode_mode = true;
        } else if (arg != "-encode") {
            std::cerr << "Usage: Code_Shennon [-decode] [-encode]" << std::endl;
            return 1;
        }
    }
    try {
        if (decode_mode) {
            // Режим декодирования
            std::string infile = "outfile.txt";
            std::string text;
            std::unordered_map <std::string, char> decode_dict;
            Read_Encoded_File(infile, text, decode_dict);
            std::string decoded = Decode_Text(text, decode_dict);
            std::string outfilename = "infile.txt";
            
            std::ofstream outfile(outfilename);
            if (!outfile) {
                throw std::runtime_error("Couldn`t open the file");
            }
            outfile << decoded << "\n";
            
        } else {
            // Режим кодирования
            std::string infilename = "infile.txt";
            std::string text;
            Read_Text(text, infilename);
            
            std::unordered_map <std::string, char> decode_dict;
            std::unordered_map <char, std::string> code_dict;
            
            std::vector <Symbol> symbols = Calculate_Frequencies(text);
            Build_ShannonCodes(symbols, decode_dict, code_dict);
            std::string encoded = Encode_Text(text, code_dict);
            
            std::string outfile = "outfile.txt";
            Save_Dictionary_And_Encoded(outfile, encoded, decode_dict);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
