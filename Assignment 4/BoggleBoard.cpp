#include "BoggleBoard.h"

BoggleBoard::BoggleBoard(const string& pathToDictionary)
: words(pathToDictionary)
{
    // it's important not to have magic numbers especially if it's duplicated, I can't tell if 25 here is the same as 25 there and if I need to change both, etc.
    // More importantly by simply adding this line you can then make the whole board configurable to any size through an argument in a constructor in the future

    string faces[c_boardDim*c_boardDim][6]
    {
        { "N", "S", "C", "T", "E", "C" },
        { "A", "E", "A", "E", "E", "E" },
        { "H", "H", "L", "R", "O", "D" },
        { "O", "R", "W", "V", "G", "R" },
        { "S", "P", "R", "I", "Y", "Y" },
        { "S", "U", "E", "N", "S", "S" },
        { "M", "E", "A", "U", "E", "G" },
        { "S", "E", "P", "T", "I", "C" },
        { "D", "H", "H", "O", "W", "N" },
        { "L", "E", "P", "T", "I", "S" },
        { "S", "T", "L", "I", "E", "I" },
        { "A", "R", "S", "I", "Y", "F" },
        { "T", "E", "T", "I", "I", "I" },
        { "O", "T", "T", "T", "M", "E" },
        { "N", "M", "N", "E", "G", "A" },
        { "N", "N", "E", "N", "A", "D" },
        { "O", "U", "O", "T", "T", "O" },
        { "B", "Z", "J", "B", "X", "K" },
        { "A", "A", "F", "A", "S", "R" },
        { "T", "O", "O", "U", "W", "N" },
        { "O", "T", "H", "D", "D", "N" },
        { "R", "A", "A", "S", "F", "I" },
        { "H", "O", "D", "R", "L", "N" },
        { "E", "E", "E", "E", "A", "M" },
        { "He", "Qu", "Th", "In", "Er", "An" }
    };

    // randomly shuffles postion of die board.
    random_shuffle(faces[0], faces[c_boardDim-1]);
    foundWords.resize(c_boardDim*c_boardDim);
    int k = 0;

    for (int i = 0; i < c_boardDim; i++)
    {
        for (int j = 0; j < c_boardDim; j++)
        {
            Die* a = new Die;
            assert(a && "failure to allocate a Die"); // though very unlikely must respect is gained by checking for memory failures, and most importantly pointer values before dereferencing.
            // picks random side of die (result of a die roll)
            a->text = faces[k][rand() % 6];
            k++; // no need to save on vertical space, people read code faster if it's written in a way they are used to seeing
            a->text[0] = char(int(a->text[0]) + 32);
            a->x = j;
            a->y = i;
            board[i][j] = a;
        }
    }
}
//--
BoggleBoard::~BoggleBoard()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            delete board[i][j];
        }
    }
}
//--
void BoggleBoard::solve()
{
    TrieElement* c = words.p_root;
    unordered_set < Die* > travelled;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            solveHelper(board[i][j], traverse(board[i][j], c), travelled);
        }
    }
    printWords();
}
//--
void BoggleBoard::solveHelper(const Die* d, const TrieElement* c, unordered_set < Die* > travelled)
{
    assert(d && c && "Invalid input argument");
    travelled.insert(d);
    if (!c) return;
    if (c->isCompletedWord)
    {
        foundWords[c->wordSoFar.length() - 4].insert(c->wordSoFar);
    }
    Die* dir[8] {west(d), southWest(d), south(d), southEast(d), east(d), northEast(d), north(d), northWest(d)};

    for (int i = 0; i < 8; i++)
    {
        if (dir[i])
        {
            if (!travelled.count(dir[i]))
            {
                solveHelper(dir[i], traverse(dir[i], c), travelled);
            }   
        }
    }
    return;
}
//--
void BoggleBoard::printBoard()
{
    for (int i = 0; i < c_boardDim; i++)
    {
        for (int j = 0; j < c_boardDim; j++)
        {
            cout<<char(int(board[i][j]->text[0])-32)<<" ";
        }
        cout<<endl;
    }
}
//--
void BoggleBoard::printWords() const //not big deal - small but mature addition of const to indicate that the function does not change anything. 
{
    int sum = 0;

    for (int i = 0; i < c_boardDim * c_boardDim; i++)
    {
        if (foundWords[i].size() > 0)
        {
            sum += foundWords[i].size();
            cout<<i+4<<" Letter Words: \n"; // Not clear what + 4 means and why we use it

            for (auto j : foundWords[i])
            {
                for (int k = 0; k < j.length(); k++)
                {
                    cout<<char(int(j[k])-32);
                }
                cout<<" ";
            }
            cout<<endl;
        }
    }
    cout<<"I found "<<sum<<" words!\n";
}
//--
/* this method traverses from a TrieElement c through the text on a given die d.
For example if c is root->a and you give it the text "He", it will give you
a pointer to root->a->h->e if it exists. */
TrieElement* BoggleBoard::traverse(cosnt Die* d, const TrieElement* c) // lack of const would tell me that I should expect arguments to be modified. use const if it's input parameter, and non-const of out or in/out
{
    assert(c && d && "invalid input argument"); // important to at least add asserts on input arguments, here and every other function
    if (d->text.length() == 2)
    {
        if (c->subTries[int(d->text[0]) - 97])
        {
            if (c->subTries[int(d->text[0]) - 97]->subTries[int(d->text[1]) - 97])
            {
                return c->subTries[int(d->text[0]) - 97]->subTries[int(d->text[1]) - 97];
            }
        }
    }
    else if (d->text.length())
    {
        if (c->subTries[int(d->text[0]) - 97])
        {
            return c->subTries[int(d->text[0]) - 97];
        }
    }
    return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::west(BoggleBoard::Die* d)
{
    if (d->x > 0)
    {
        return board[d->y][d->x - 1];
    }
    
    return NULL; // not really anything wrong here, but makes for cleaner code additions in the future.
}
//--
BoggleBoard::Die* BoggleBoard::southWest(const BoggleBoard::Die* d)
{
    if (south(d) and west(d))
    {
        return board[d->y + 1][d->x - 1];
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::south(const BoggleBoard::Die* d)
{
    if (d->y < 4)
    {
        return (board[d->y + 1][d->x]);
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::southEast(const BoggleBoard::Die* d)
{
    if (south(d) and (east(d)))
    {
        return board[d->y + 1][d->x + 1];
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::east(const BoggleBoard::Die* d)
{
    if (d->x < 4)
    {
        return board[d->y][d->x + 1];
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::northEast(BoggleBoard::Die* d)
{
    if (north(d) and east(d))
    {
        return board[d->y - 1][d->x + 1];
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::north(BoggleBoard::Die* d)
{
    if (d->y > 0)
    {
        return board[d->y - 1][d->x];
    }
    else return NULL;
}
//--
BoggleBoard::Die* BoggleBoard::northWest(BoggleBoard::Die* d)
{
    if (north(d) and west(d))
    {
        return board[d->y - 1][d->x - 1];
    }
    else return NULL;
}