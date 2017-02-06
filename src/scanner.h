#ifndef TEETON_SCANNER_H
#define TEETON_SCANNER_H

#include <string>


class Character {
public:
    static const char ENDMARK = '\0';
    char cargo;
    int sourceIndex;
    int lineIndex;
    int colIndex;

    Character(char cargo, int sourceIndex, int lineIndex, int colIndex) : cargo(cargo), sourceIndex(sourceIndex),
                                                                          lineIndex(lineIndex), colIndex(colIndex) { };

    std::string toString();
};


class Scanner {
public:
    Scanner(std::string newSourceText);

    Character *get();

    Character *lookahead(int steps);

private:
    std::string sourceText;
    int sourceIndex;
    int lastIndex;
    int lineIndex;
    int colIndex;
};


#endif //TEETON_SCANNER_H
