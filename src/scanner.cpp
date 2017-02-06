#include <iomanip>
#include <sstream>

#include "scanner.h"


using namespace std;


string Character::toString() {
    ostringstream os;
    string printable;

    switch(cargo) {
        case ' ': printable = "   space"; break;
        case '\n': printable = "   newline"; break;
        case '\t': printable = "   tab"; break;
        case ENDMARK: printable = "   eof"; break;
        default: printable.push_back(cargo); break;
    }

    os << setw(4) << lineIndex << " " << setw(4) << colIndex << " " << printable << " ";
    return os.str();
}

// -----------------------------------------------------------------------------

Scanner::Scanner(string newSourceText) {
    sourceText = newSourceText;
    lastIndex = (int) sourceText.length() - 1;
    sourceIndex = -1;
    lineIndex = 1;
    colIndex = -1;
}

Character *Scanner::get() {
    sourceIndex += 1;

    if (sourceIndex > 0) {
        if (sourceText[sourceIndex - 1] == '\n') {
            lineIndex += 1;
            colIndex = -1;
        }
    }

    colIndex += 1;

    char cargo;
    if (sourceIndex > lastIndex) {
        cargo = Character::ENDMARK;
    } else {
        cargo = sourceText[sourceIndex];
    }

    return new Character(cargo, sourceIndex, lineIndex, colIndex);
}


Character *Scanner::lookahead(int steps) {
    if (sourceIndex + steps >= sourceText.length()) {
        return new Character(Character::ENDMARK, 0, 0, 0);
    }

    return new Character(sourceText[sourceIndex + steps], 0, 0, 0);
}
