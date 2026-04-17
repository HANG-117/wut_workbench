#include "card_file.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// 保存所有卡片到文件（整存）
int saveCardBinary(const Card *card, const char *filename){
    // 这个函数不再使用，改为使用 saveAllCards
    return 1;
}

// 读取所有卡片到链表
int readCardBinary(CardList *head,CardList *tail, const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }
    Card card;
    while (fread(&card, sizeof(Card), 1, file) == 1) {
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

// 更新单个卡片（整存整取）
int updateCardBinary(const Card *card, const char *filename, int nIndex){
    // 这个函数不再使用，改为使用 saveAllCards
    return 1;
}

// 保存所有卡片到文件（整存）
int saveAllCards(CardList head, const char *filename){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return -1;
    }
    CardList current = head;
    while (current != NULL) {
        if (fwrite(&current->data, sizeof(Card), 1, file) != 1) {
            fclose(file);
            return -1;
        }
        current = current->next;
    }
    fclose(file);
    return 1;
}
