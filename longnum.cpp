#include "longnum.h"
#include "messages.h"

bool isDigit(char a, bool withZero = true) { return (withZero ? a >= '0' && a <= '7' : a >= '1' && a <= '7'); }

bool isParsable(std::string& line) {
    unsigned int len = line.length();
    unsigned int i;
    bool isSign = true;
    bool isInteger = false;
    int sign = len > 0 && line[0] == '-' ? 1 : 0;

    if (len <= DATA_SIZE * 3 + sign) {
        for (i = 0; i < len; ++i) {
            if (isSign && ( line[i] == '-' || isDigit( line[i]) ) ) {
                isSign = false;
                isInteger = true;
            }
            else if (isInteger && isDigit(line[i])) { continue; }
            else { break; }
        }
    }
    return i == len && line[i - 1] != '-' && isDigit(line[i - 1]);
}

std::string reverse(std::string& line) {
    std::string reversed;
    for (int i = static_cast<int>( line.length() ) - 1; i >= 0; --i) { reversed += line[i]; }
    return reversed;
}

std::string getTLong(std::string& line) {
    std::string integer;
    bool hasSignificantDigit = false;
    unsigned int len = line.length();
    int startPosition = line[0] == '-' ? 1 : 0;
    int endPosition = static_cast<int>(len) - 1;

    for (int i = startPosition; i <= endPosition; ++i) {
        if (!hasSignificantDigit && isDigit(line[i], false) || i == endPosition) { hasSignificantDigit = true; }
        if (hasSignificantDigit) { integer += line[i]; }
    }
    return integer;
}

void sumTLong(unsigned short& sum, int& remainder, int a, int b) {
    int hundreds, units, tens, q1, q2, q3;
    units = a % 10 + b % 10 + remainder;
    tens = a % 100 / 10 + b % 100 / 10 + (units >= 8 ? 1 : 0);
    hundreds = a / 100 + b / 100 + (tens >= 8 ? 1 : 0);
    q3 = units % 8;
    q2 = tens % 8;
    q1 = hundreds % 8;
    remainder = hundreds >= 8 ? 1 : 0;
    sum = q1 * 100 + q2 * 10 + q3;
}

void subTLong(unsigned short& sub, int& remainder, int a, int b) {
    int hundreds, units, tens, q1, q2, q3;
    units = a % 10 - b % 10 + remainder;
    tens = a % 100 / 10 - b % 100 / 10 - (units >= 0 ? 0 : 1);
    hundreds = a / 100 - b / 100 - (tens >= 0 ? 0 : 1);
    q3 = units >= 0 ? units : 8 + units;
    q2 = tens >= 0 ? tens : 8 + tens;
    q1 = hundreds >= 0 ? hundreds : 8 + hundreds;
    remainder = hundreds >= 0 ? 0 : -1;
    sub = q1 * 100 + q2 * 10 + q3;
}

TLong absSum(TLong& a, TLong& b, int& responseCode) {
    TLong sum;
    int remainder = 0;
    sum.dataSize = a.dataSize > b.dataSize ? a.dataSize : b.dataSize;

    for (int i = 0; i < sum.dataSize; ++i)
        sumTLong(sum.data[i], remainder, a.data[i], b.data[i]);

    if (remainder > 0 && sum.dataSize < DATA_SIZE) { sum.data[sum.dataSize++] = remainder; }
    else if (remainder > 0) { responseCode = dataOverflowed; }

    while (sum.dataSize > 1 && sum.data[sum.dataSize - 1] == 0)
        --sum.dataSize;

    return sum;
}

TLong absSub(TLong& a, TLong& b, int& responseCode) {
    TLong sub;
    int remainder = 0;

    sub.dataSize = a.dataSize > b.dataSize ? a.dataSize : b.dataSize;

    bool less = isLess(a, b, true);
    TLong& lower = less ? a : b;
    TLong& higher = less ? b : a;

    for (int i = 0; i < sub.dataSize; ++i)
        subTLong(sub.data[i], remainder, higher.data[i], lower.data[i]);

    while (sub.dataSize > 1 && sub.data[sub.dataSize - 1] == 0)
        --sub.dataSize;

    responseCode = Success;

    return sub;
}

int readTLong(std::ifstream& inFile, TLong& num) {
    std::string line;
    getline(inFile, line);

    if (static_cast<int>( line.find('\r') ) > -1) { line.erase(line.length() - 1); }

    if ( isParsable(line) ) {
        std::string element;

        num.sign = line[0] != '-';
        std::string integerString = getTLong(line);
        int integerLength = static_cast<int>( integerString.length() );
        bool isIntegerOverflowed = integerLength > DATA_SIZE * 3;

        if (!isIntegerOverflowed) {
            num.dataSize = 0;
            element = "";
            for (int i = integerLength - 1; i >= 0; --i) {
                element += integerString[i];
                if (element.length() == 3 || i == 0) {
                    element = reverse(element);
                    num.data[num.dataSize++] = std::stoi(element);
                    element.clear();
                }
            }

            if (!num.sign && num.dataSize == 1 && num.data[0] == 0) { return MinusZero; }
            else { return Success; }
        } else { return dataOverflowed; }
    }
    return Unparsable;
}

void writeTLong(std::ofstream& outFile, TLong& num) {
    if (!num.sign) { outFile << '-'; }
    for (int i = num.dataSize - 1; i >= 0; --i) {
        if (num.data[i] < 10 && i < num.dataSize - 1) { outFile << "00"; }
        else if (num.data[i] < 100 && i < num.dataSize - 1) { outFile << "0"; }
        outFile << num.data[i];
    }
    outFile << std::endl;
}

bool isEqual(TLong& a, TLong& b, bool abs) {
    if ( (a.sign == b.sign || abs) && a.dataSize == b.dataSize) {
        for (int i = a.dataSize - 1; i >= 0; --i)
            if (a.data[i] != b.data[i]) { return false; }
        return true;
    }
    return false;
}

bool isLess(TLong& a, TLong& b, bool abs) {
    if (a.sign == b.sign || abs) {
        TLong& lower = a.sign || abs ? a : b;
        TLong& higher = a.sign || abs ? b : a;
        if (lower.dataSize == higher.dataSize) {
            for (int i = lower.dataSize - 1; i >= 0; --i)
                if (lower.data[i] != higher.data[i]) { return lower.data[i] < higher.data[i]; }
            return false;
        }
        else { return lower.dataSize < higher.dataSize; }
    }
    return !a.sign && b.sign;
}

TLong sumTL(TLong& a, TLong& b, int& responseCode) {
    TLong sum;
    if (a.sign == b.sign) {
        sum = absSum(a, b, responseCode);
        sum.sign = a.sign;
    }
    else {
        if ( !isEqual(a, b, true) ) {
            bool less = isLess(a, b, true);
            sum = less ? absSub(b, a, responseCode) : absSub(a, b, responseCode);
            sum.sign = less ? b.sign : a.sign;
        }
    }
    if (sum.dataSize == 1 && sum.data[0] == 0 && !sum.sign) { sum.sign = true; }
    return sum;
}

TLong subTL(TLong& a, TLong& b, int& responseCode) {
    TLong sub;
    if (a.sign == b.sign) {
        if ( !isEqual(a, b, true) ) {
            bool less = isLess(a, b, true);
            sub = less ? absSub(b, a, responseCode) : absSub(a, b, responseCode);
            sub.sign = less ? !b.sign : a.sign;
        }
    }
    else {
        sub = absSum(a, b, responseCode);
        sub.sign = a.sign;
    }
    if (sub.dataSize == 1 && sub.data[0] == 0 && !sub.sign) { sub.sign = true; }
    return sub;
}