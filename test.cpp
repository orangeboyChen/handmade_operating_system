
#include <iostream>
using namespace std;

struct TS
{
    char updateMap[50];
};

void setBitInUpdateMap(struct TS *ts, unsigned int index, unsigned int value)
{
    char *butch = &(ts->updateMap[index / sizeof(char)]);
    unsigned int indexInChar = index - (index / sizeof(char)) * sizeof(char);
    if (value == true)
    {
        *butch = *butch | (1 << indexInChar);
    }
    else
    {
        *butch = *butch & (!(1 << indexInChar));
    }
}

unsigned int getBitInUpdateMap(struct TS *ts, unsigned int index)
{
    char *butch = &ts->updateMap[index / sizeof(char)];
    unsigned int indexInChar = index - (index / sizeof(char)) * sizeof(char);
    return (*butch & (1 << indexInChar)) >> indexInChar;
}

int main()
{
    struct TS *t = (struct TS *)malloc(sizeof(TS));
    setBitInUpdateMap(t, 0, 0);
    setBitInUpdateMap(t, 1, 1);
    setBitInUpdateMap(t, 2, 1);
    setBitInUpdateMap(t, 3, 0);
    setBitInUpdateMap(t, 4, 1);
    setBitInUpdateMap(t, 5, 0);
    setBitInUpdateMap(t, 6, 1);
    setBitInUpdateMap(t, 7, 0);
    setBitInUpdateMap(t, 8, 0);
    setBitInUpdateMap(t, 9, 1);
    setBitInUpdateMap(t, 10, 1);
    setBitInUpdateMap(t, 11, 0);
    setBitInUpdateMap(t, 12, 0);
    for (int i = 0; i < 13; i++)
    {
        cout << getBitInUpdateMap(t, i);
    }
}