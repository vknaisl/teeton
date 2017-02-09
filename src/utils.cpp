#include <algorithm>
#include <sstream>

#include "utils.h"

using namespace std;

void runtimeError(string err) {
    ostringstream os;
    os << "RuntimeError: " << err;
    throw new TeetonError(os.str());
}

void parseError(string err, int lineIndex, int colIndex) {
    ostringstream os;
    os << "Parse error: " << err << " [line: " << lineIndex << ", col: " << colIndex << "] ";
    throw new TeetonError(os.str());
}

bool contains(const string &str, char c) {
    return str.find(c) != string::npos;
}

bool contains(const vector<string> &vec, string s) {
    return find(vec.begin(), vec.end(), s) != vec.end();
}
