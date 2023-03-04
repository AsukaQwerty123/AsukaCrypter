#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

// Функция для генераций случайного текста
std::string generate_random_text(int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charset_size = sizeof(charset) - 1;
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++) {
        result += charset[rand() % charset_size];
    }
    return result;
}
// Функция для шифрования данных в буфере с помощью ключа
void xorEncrypt(char* buffer, int size, const char* key, int keySize) {
    for (int i = 0; i < size; i++) {
        buffer[i] ^= key[i % keySize];
    }
}

int main() {
    const char* inputFile = "input.exe"; // имя входного файла
    const char* outputFile = "output.h"; // имя выходного заголовочного файла
    const int length = 58;
    std::string text = generate_random_text(length);
    const char* key = text.c_str(); // пароль

    // Открытие входного файла в двоичном режиме
    std::ifstream ifs(inputFile, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return 1;
    }

    // Получение размера входного файла
    ifs.seekg(0, std::ios::end);
    int fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    // Выделение памяти под буфер для хранения данных
    char* buffer = new char[fileSize];

    // Чтение данных из файла в буфер
    ifs.read(buffer, fileSize);
    ifs.close();

    // Шифрование данных в буфере с помощью ключа
    xorEncrypt(buffer, fileSize, key, strlen(key));

    // Открытие выходного файла в текстовом режиме
    std::ofstream ofs(outputFile);
    if (!ofs.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return 1;
    }

    // Запись зашифрованных данных в заголовочный файл
    ofs << "#pragma once\n\n";
    ofs << "const char* key = \042" << key << "\042;\n\n";
    ofs << "const unsigned char encryptedData[] = {\n";
    for (int i = 0; i < fileSize; i++) {
        ofs << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
        if (i != fileSize - 1) {
            ofs << ",";
        }
        if ((i + 1) % 16 == 0) {
            ofs << "\n";
        }
        else {
            ofs << " ";
        }
    }
    ofs << "\n};\n";
    ofs.close();

    // Освобождение памяти, выделенной под буфер
    delete[] buffer;

    std::cout << "Encryption completed successfully." << std::endl;

    return 0;
}