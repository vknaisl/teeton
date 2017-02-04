#include "utils.h"

using namespace std;

void runtimeError(string err) {
    cout << "RuntimeError: " << err << endl;
    exit(1);
}
