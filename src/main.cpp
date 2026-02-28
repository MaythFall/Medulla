#include "Server.hpp"
#include <iostream>


void decrypter(unsigned char& dest, unsigned char in, int shift) {
    if (in >= 'a' && in <= 'z') {
        dest = ((in - 'a' - (shift % 26) + 26) % 26) + 'a'; 
    } else if (in >= 'A' && in <= 'Z') {
        dest = ((in - 'A' - (shift % 26) + 26) % 26) + 'A'; 
    } else {
        dest = in;
    }
}


int main() {
    try {
        auto decrypt = [](medulla::Server& client, medulla::SecureBuffer<uint8_t>& buffer) {
            if (buffer.empty()) return;
            std::string cmd(buffer.begin(), buffer.end());
            medulla::SecureBuffer<uint8_t> plain_text;
            plain_text.resize(buffer.size()-1);
            std::cout << "Ciphertext: ";
            for (int i = 0; i < buffer.size()-1; ++i) {
                decrypter(plain_text[i], buffer[i], buffer.back());
                std::cout << buffer[i];
            }
            std::cout << "\nPlain text: ";
            for (int i = 0; i < plain_text.size(); ++i) {
                std::cout << plain_text[i];
            }
            std::cout << '\n';
        };
        medulla::SecureBuffer<uint8_t> buff;
        medulla::Server server = medulla::Server();
        server.runSocket(decrypt, buff);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}