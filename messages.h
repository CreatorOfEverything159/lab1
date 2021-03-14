#include <iostream>

std::string fileNotFound(const std::string &path);
std::string fileIsEmpty(const std::string &path);
std::string invalidEndOfFile(const std::string &path);
std::string successfullyCompleted(const std::string &inPath, const std::string &outPath);
std::string unsuccessfullyCompleted(const std::string &inPath, const std::string &outPath);
std::string invalidLongNum(unsigned int index);
std::string invalidSign(unsigned int index);
std::string dataOverflowedE(unsigned int index);
std::string minusZero(unsigned int index);