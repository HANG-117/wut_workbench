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
int updateCardFile(const Card *card, const char *filename,int nIndex){
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    Card cards[MAX_CARDS]; // 假设最多有100张卡
    int count = 0;
    while (fscanf(file, "%[^#]##%[^#]##%d##%ld##%ld##%f##%ld##%d##%f##%d\n",
                  cards[count].aName, cards[count].aPwd, &cards[count].nStatus, &cards[count].tStart, &cards[count].tEnd,
                  &cards[count].fTotalUse, &cards[count].tLast, &cards[count].nUseCount, &cards[count].fBalance, &cards[count].nDel) == 10) {
        count++;
        if(count >= MAX_CARDS){
            printf("卡数量超过%d，无法更新文件！\n", MAX_CARDS);
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    if(nIndex < 1 || nIndex > count){
        printf("索引超出范围，无法更新文件！\n");
        return -1;
    }
    cards[nIndex - 1] = *card; // 更新指定索引的卡信息
    file = fopen(filename, "w");
    if (!file) {
        return -1;
    }
    for(int i = 0; i < count; i++){
        fprintf(file, "%s##%s##%d##%ld##%ld##%.2f##%ld##%d##%.2f##%d\n",
                cards[i].aName, cards[i].aPwd, cards[i].nStatus, cards[i].tStart, cards[i].tEnd,
                cards[i].fTotalUse, cards[i].tLast, cards[i].nUseCount, cards[i].fBalance, cards[i].nDel);
    }
    fclose(file);
    return 1;
}