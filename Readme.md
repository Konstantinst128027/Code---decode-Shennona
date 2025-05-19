#(De)code Shennona

##Author
- Berezkin Konstantin Evgenievich 24.b-81mm

##Contacts
- st128027@student.spbu.ru

##Description
- I made a Shannon encoder/decoder. First, he reads the text from "infile.txt" after the text has been encoded, I send the encoded text to "outfile.txt" and I write the text in "outfile.bin". When decoding, I read the text from outfile.txt and I'm reading the dictionary from the "outfile.bin", the encoded string is decoded and then I put it in "infile.txt". How do I write down a dictionary? First I write down its size, then I start writing down the dictionary itself. First, I write the character as a single byte, then I write down the length of the line of code, and then the code itself, which I first supplement to a multiple of 8, so that it is convenient to write it and so that there is less memory. I'm adding zeros from the right, as it will make it easier to trim this line when reading.

##Build encode and run encode
- bash build_encode.sh

##Build decode and run decode
- bash build_decode.sh

##Build tests and run tests
- bash build_tests.sh
