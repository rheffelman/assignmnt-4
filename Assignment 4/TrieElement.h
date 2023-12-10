#pragma once
#include <string>
#include <vector>
using namespace std;

class TrieElement
{
public:

    TrieElement(char letter);
    ~TrieElement();
    
    // can we add private here?
    char c;
    string wordSoFar;
    bool isCompletedWord = false;
    TrieElement* subTries[26] {NULL};
    
};