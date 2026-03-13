#ifndef CARD_FILE_H
#define CARD_FILE_H

#include <time.h>
typedef struct Card {
    char aName[18];
    char aPwd[8];
    int nStatus;
    time_t tStart;
    time_t tEnd;
    float fTotalUse;
    time_t tLast;
    int nUseCount;
    float fBalance;
    int nDel;
} Card;

#endif