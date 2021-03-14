#include <iostream>

std::string fileNotFound(const std::string &path) { return "File not found: \"" + path + "\""; }
std::string fileIsEmpty(const std::string &path) { return "File \"" + path + "\" is empty"; }
std::string invalidEndOfFile(const std::string &path) { return "File \"" + path + "\" ended with error"; }
std::string successfullyCompleted(const std::string &inPath, const std::string &outPath) { return "File \"" + inPath + "\" completed success"; }
std::string unsuccessfullyCompleted(const std::string &inPath, const std::string &outPath) { return "File \"" + inPath + "\" completed with errors"; }
std::string invalidLongNum(unsigned int index) { return "Line " + std::to_string(index) + " - error number TLongNum"; }
std::string invalidSign(unsigned int index) { return "Line " + std::to_string(index) + " - error sign"; }
std::string dataOverflowedE(unsigned int index) { return "Line " + std::to_string(index) + " - Overflowed"; }
std::string minusZero(unsigned int index) { return "Line " + std::to_string(index) + " - zero can't be negative"; }