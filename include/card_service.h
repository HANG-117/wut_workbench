#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
    #include "global.h"
    #include "model.h"
    #include <stdio.h>
    #include <string.h>
    #include <time.h>
    #define CARD_NOT_ON_COMPUTER 0
    #define CARD_ON_COMPUTER 1
    #define CARD_DEL 2
    #define CARD_INVALID 3

    #define CARD_EXIST 0
    #define CARD_NO_EXIST 1

   
    Card creat_card();
    int addCard(Card newCard);    
    void findcard();
    Card* check_card(const char* aName, const char* aPwd);
    int deleteCard(const char* pName);
#endif