#include <iostream>
#include "constants.h"
#include "messages.h"
#include "longnum.h"

bool readSign(std::ifstream& inFile, bool& sign) {
    std::string line;
    getline(inFile, line);
    if (static_cast<int>( line.find('\r') ) > -1) { line.erase(line.length() - 1); }
    if (line.length() == 1 && (line[0] == '+' || line[0] == '-') ) {
        sign = line[0] == '+';
        return true;
    }
    return false;
}

bool process(std::ifstream& inFile, std::ofstream& outFile) {
    bool isFileCorrect = inFile.is_open();

    if (!isFileCorrect) { outFile << fileNotFound(IN_PATH); }
    else {
        LongNum sum;
        int responseCode;
        bool sign = true;
        unsigned int index = 1;

        while (!inFile.eof()) {
            if (index % 2 == 1) {
                LongNum num;
                responseCode = readTLong(inFile, num);
                if (responseCode == Success && isFileCorrect) {
                    if (index == 1) {
                        sum = num;
                    }
                    else {
                        if (sign) {
                            sum = sumTL(sum, num, responseCode);
                        }
                        else {
                            sum = subTL(sum, num, responseCode);
                        }
                        if (responseCode != Success) {
                            isFileCorrect = false;
                            outFile << dataOverflowedE(index) << std::endl;
                        }
                    }
                }
                else {
                    isFileCorrect = false;
                    if (responseCode == Unparsable) {
                        outFile << invalidLongNum(index) << std::endl;
                    }
                    else if (responseCode == MinusZero) {
                        outFile << minusZero(index) << std::endl;
                    }
                    else if (responseCode == dataOverflowed) {
                        outFile << dataOverflowedE(index) << std::endl;
                    }
                }
            }
            else if ( !readSign(inFile, sign) ) {
                outFile << invalidSign(index) << std::endl;
                isFileCorrect = false;
            }
            ++index;
        }

        if (index == 1) {
            outFile << fileIsEmpty(IN_PATH);
            isFileCorrect = false;
        }
        else if (index % 2 == 1 && isFileCorrect) {
            outFile << invalidEndOfFile(IN_PATH);
            isFileCorrect = false;
        }
        else if (isFileCorrect) {
            writeTLong(outFile, sum);
        }
    }

    return isFileCorrect;
}

int main() {
    std::ifstream inFile(IN_PATH);
    std::ofstream outFile(OUT_PATH);

    bool isSuccessfullyCompleted = process(inFile, outFile);

    inFile.close();
    outFile.close();

    if (isSuccessfullyCompleted) { std::cout << successfullyCompleted(IN_PATH, OUT_PATH) << std::endl; }
    else { std::cout << unsuccessfullyCompleted(IN_PATH, OUT_PATH) << std::endl; }

    return 0;
}
