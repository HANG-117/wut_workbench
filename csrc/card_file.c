#include "card_file.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int saveCardFile(const Card *card, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        return -1;
    }
    fprintf(file, "%s##%s##%d##%ld##%ld##%.2f##%ld##%d##%.2f##%d\n",
            card->aName, card->aPwd, card->nStatus, card->tStart, card->tEnd,
            card->fTotalUse, card->tLast, card->nUseCount, card->fBalance, card->nDel);
    fclose(file);
    return 1;
}

int readCardFile(CardList *head,CardList *tail, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    Card card;
    while (fscanf(file, "%[^#]##%[^#]##%d##%ld##%ld##%f##%ld##%d##%f##%d\n",
                  card.aName, card.aPwd, &card.nStatus, &card.tStart, &card.tEnd,
                  &card.fTotalUse, &card.tLast, &card.nUseCount, &card.fBalance, &card.nDel) == 10) {
        CardNode *newNode = (CardNode*)malloc(sizeof(CardNode));
        if (newNode == NULL) {
            fclose(file);
            return -1;
        }
        newNode->data = card;
        newNode->next = NULL;
        if (*head == NULL) {
            *head = newNode;
            *tail = newNode;
        } else {
            (*tail)->next = newNode;
            *tail = newNode;
        }
    }
    fclose(file);
    return 1;
}