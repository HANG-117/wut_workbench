#ifndef CARD_FILE_H
#define CARD_FILE_H
    #include "card_service.h"
    #define MAX_CARDS 100
    int saveCardBinary(const Card *card, const char *filename);
    int readCardBinary(CardList *head,CardList *tail, const char *filename);
    int updateCardBinary(const Card *card, const char *filename, int nIndex);
    int saveAllCards(CardList head, const char *filename);
#endif