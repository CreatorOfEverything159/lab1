#include <iostream>
#include <fstream>
#include "constants.h"

typedef struct TLong {
    bool sign;
    unsigned short data[DATA_SIZE]{};
    unsigned short dataSize{};

    TLong() {
        sign = true;
        dataSize = 1;
    }
} LongNum;

enum codeNotifications {
    Success,
    Unparsable,
    MinusZero,
    dataOverflowed
};

int readTLong(std::ifstream& inFile, TLong& num);
void writeTLong(std::ofstream& outFile, TLong& num);
bool isEqual(TLong& a, TLong& b, bool abs = false);
bool isLess(TLong& a, TLong& b, bool abs = false);
TLong sumTL(TLong& a, TLong& b, int& responseCode);
TLong subTL(TLong& a, TLong& b, int& responseCode);