#ifndef CARD_FILE_H
#define CARD_FILE_H
    #include "card_service.h"
    #define CARD_FILE "data/card.txt"
    #define MAX_CARDS 100
    int saveCardFile(const Card *card, const char *filename);
    int readCardFile(CardList *head,CardList *tail, const char *filename);
    int updateCardFile(const Card *card, const char *filename,int nIndex);
#endif