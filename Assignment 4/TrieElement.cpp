#include "TrieElement.h"

TrieElement::TrieElement(char letter)
{
    c = letter;
}
//--
TrieElement::~TrieElement()
{
    for (int i = 0; i < sizeof(subTries)/sizeof(subTries[0]); i++) // since it's statically allocated, compiler can compute 26 for you, or just use constant
    {
        delete subTries[i];
    }
}