#include <iostream>
#include <string>
#include <ctime>
#include "BoggleBoard.h"
using namespace std;

int main()
{
    //comment this out while debugging to get the same board over and over again
    srand(time(0));

    BoggleBoard bb("bigDictionary.txt");
    bb.printBoard();
    bb.solve();

/*
from https://google.github.io/styleguide/cppguide.html:
// Assignment operators always have spaces around them.
x = 0;

// Other binary operators usually have spaces around them, but it's
// OK to remove spaces around factors.  Parentheses should have no
// internal padding.
v = w * x + y / z;
v = w*x + y/z;
v = w * (x + z);

// No spaces separating unary operators and their arguments.
x = -5;
++x;
if (x && !y)
  ...
  */

    cout << "exited"; // also spaces are added before and after <<. It does help readability, but not a big deal.
    return 0;
}