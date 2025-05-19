/* Berezkin Konstantin Evgenievich
./tests st128027@student.spbu.ru*/
#include <gtest/gtest.h>
#include "functions.h"

// tests for Calculate_Frequencies
TEST(Calculate_Frequencies_Test, BasicTest) {
    std::string text = "abracadabra";
    auto symbols = Calculate_Frequencies(text);
    
    EXPECT_EQ(symbols.size(), 5); // a, b, r, c, d
    
    for (const auto& s : symbols) {
        if (s.character == 'a') {
            EXPECT_DOUBLE_EQ(s.probability, 5.0/11.0);
        }
        else if (s.character == 'b') {
            EXPECT_DOUBLE_EQ(s.probability, 2.0/11.0);
        }
        else if (s.character == 'r') {
            EXPECT_DOUBLE_EQ(s.probability, 2.0/11.0);
        }
    }
}

TEST(Calculate_Frequencies_Test, Single_Character) {
    std::string single_char = "aaaaa";
    auto single_symbol = Calculate_Frequencies(single_char);
    
    ASSERT_EQ(single_symbol.size(), 1);
    EXPECT_EQ(single_symbol[0].character, 'a');
    EXPECT_DOUBLE_EQ(single_symbol[0].probability, 1.0);
}

// tests for Build_ShannonCodes
TEST(Build_ShannonCodes_Test, Basic_Test) {
    std::vector<Symbol> symbols = {
        {'a', 0.5}, {'b', 0.25}, {'c', 0.125}, {'d', 0.125}
    };
    
    std::unordered_map<std::string, char> decode_dict;
    std::unordered_map<char, std::string> code_dict;
    
    Build_ShannonCodes(symbols, decode_dict, code_dict);
    
    EXPECT_EQ(code_dict.size(), 4);
    EXPECT_EQ(decode_dict.size(), 4);
    
    EXPECT_EQ(code_dict['a'].length(), 1);
    EXPECT_EQ(code_dict['b'].length(), 2);
    EXPECT_EQ(code_dict['c'].length(), 3);
    EXPECT_EQ(code_dict['d'].length(), 3);
    
}

// test for one symbol
TEST(Build_ShannonCodes_Test, Single_Symbol) {
    std::vector<Symbol> symbols = {{'x', 1.0}};
    std::unordered_map<std::string, char> decode_dict;
    std::unordered_map<char, std::string> code_dict;
    
    Build_ShannonCodes(symbols, decode_dict, code_dict);
    
    EXPECT_EQ(code_dict.size(), 1);
    EXPECT_EQ(decode_dict.size(), 1);
    EXPECT_EQ(code_dict['x'], "0");
    EXPECT_EQ(decode_dict["0"], 'x');
}

// tests for Encode_Text и Decode_Text
TEST(Encoding_Decoding_Test, Basic_Test) {
    std::string text = "abacab";
    std::vector<Symbol> symbols = Calculate_Frequencies(text);
    
    std::unordered_map<std::string, char> decode_dict;
    std::unordered_map<char, std::string> code_dict;
    Build_ShannonCodes(symbols, decode_dict, code_dict);
    
    std::string encoded = Encode_Text(text, code_dict);
    std::string decoded = Decode_Text(encoded, decode_dict);
    
    EXPECT_EQ(decoded, text);
}

TEST(Encoding_Decoding_Test, Unknown_Character) {
    std::string text = "abc";
    std::vector<Symbol> symbols = Calculate_Frequencies(text);
    
    std::unordered_map<std::string, char> decode_dict;
    std::unordered_map<char, std::string> code_dict;
    Build_ShannonCodes(symbols, decode_dict, code_dict);
    
    std::string bad_text = "abcd";
    EXPECT_THROW(Encode_Text(bad_text, code_dict), std::runtime_error);
}

TEST(Encoding_Decoding_Test, Invalid_Decoded_Text_1) {
    std::unordered_map<std::string, char> decode_dict = {{"00", 'a'}, {"01", 'b'}};
    std::string invalid_encoded = "0021";
    EXPECT_THROW(Decode_Text(invalid_encoded, decode_dict), std::runtime_error);
}

TEST(Encoding_Decoding_Test, Invalid_Decoded_Text_2) {
    std::unordered_map<std::string, char> decode_dict = {{"00", 'a'}, {"01", 'b'}};
    std::string invalid_encoded = "0011";
    EXPECT_THROW(Decode_Text(invalid_encoded, decode_dict), std::runtime_error);
}

//random test
TEST(Encoding_Decoding_Test, Random_Test) {
    std::string text = generate_100_char_string();
    std::vector<Symbol> symbols = Calculate_Frequencies(text);
    
    std::unordered_map<std::string, char> decode_dict;
    std::unordered_map<char, std::string> code_dict;
    Build_ShannonCodes(symbols, decode_dict, code_dict);
    
    std::string encoded = Encode_Text(text, code_dict);
    std::string decoded = Decode_Text(encoded, decode_dict);
    
    EXPECT_EQ(decoded, text);
}

// tests for Read and Save
TEST(Save_Read_Test, Test) {

    std::string infilename = "infile.txt";
    std::string text;
                
    Read_Text(text, infilename);
            
    std::unordered_map <std::string, char> decode_dict;
    std::unordered_map <char, std::string> code_dict;
            
    std::vector <Symbol> symbols = Calculate_Frequencies(text);
    Build_ShannonCodes(symbols, decode_dict, code_dict);
            
    std::string encoded = Encode_Text(text, code_dict);
            
    std::string outfile = "outfile.bin";
    Save_Dictionary_And_Encoded(outfile, decode_dict, encoded);
    
    std::unordered_map <std::string, char> decode_dict_then;            
    std::string encoded_then;
            
    Read_Encoded_And_Dict(encoded_then, outfile, decode_dict_then);
            
    std::string decoded = Decode_Text(encoded_then, decode_dict_then); 
            
    std::ofstream infile(infilename);    
    infile << decoded;
    
    EXPECT_EQ(decoded, text);
}

