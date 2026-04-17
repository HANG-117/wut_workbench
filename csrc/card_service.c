#include "card_service.h"
#include "card_file.h"
#include "global.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
CardList cardList_head = NULL;
CardList cardList_tail = NULL;

Card newCard;
Card creat_card(){
    
    while(1){
        printf("请输入用户名：");
        scanf("%s",newCard.aName);
        if(strlen(newCard.aName)>17){
            printf("用户名过长，请重新输入！\n");
            continue;
        }
        CardList current = cardList_head;
        int exists = 0;
        while (current != NULL) {
            if (strcmp(current->data.aName, newCard.aName) == 0)
            {
                exists = 1;
                break;
            }
            current = current->next;
        }
        if (exists) {
            printf("用户名已存在，请重新输入！\n");
            continue;
        }
        break;
    }
    while(1){
        printf("请输入密码：(不超过7位且不为纯数字) ");
        scanf("%s",newCard.aPwd);
        if(strlen(newCard.aPwd)>7){
            printf("密码过长，请重新输入！\n");
            continue;
        }
        if(is_digits(newCard.aPwd)){
            printf("密码不能为纯数字，请重新输入！\n");
            continue;
        }
        break;
    }
    newCard.nStatus = CARD_NOT_ON_COMPUTER;
    newCard.tStart = time(NULL);
    newCard.tEnd = 0;
    newCard.fTotalUse = 0.0f;
    newCard.tLast = time(NULL);
    newCard.nUseCount = 0;
    newCard.fBalance = 0.0f;
    newCard.nDel = CARD_EXIST;
    printf("卡创建成功！\n");
    return newCard;
}


int addCard(Card newCard) {
    printf("正在添加卡...\n");
    CardNode *newNode = (CardNode*)malloc(sizeof(CardNode));
    if (newNode == NULL) {
        return -1;
    }
    newNode->data = newCard;
    newNode->next = NULL;
    // 先添加到链表
    if (cardList_head == NULL) {
        cardList_head = newNode;
        cardList_tail = newNode;
    } else {
        cardList_tail->next = newNode;
        cardList_tail = newNode;
    }
    // 保存所有卡片到文件
    if (saveAllCards(cardList_head, CARD_PATH) != 1) {
        free(newNode);
        return -1;
    }
    return 1;
}


int is_digits(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

void printCard(const Card *card) {
    printf("用户：%s\n", card->aName);
    printf("状态：%s\n", card->nStatus == CARD_NOT_ON_COMPUTER ? "未上机" : card->nStatus == CARD_ON_COMPUTER ? "正在上机" : card->nStatus == CARD_DEL ? "已注销" : card->nStatus == CARD_INVALID ? "失效" : "未知");
    printf("余额：%.2f\n", card->fBalance);
    printf("使用次数：%d\n", card->nUseCount);
    printf("总使用金额：%.2f\n", card->fTotalUse);
    printf("最后使用时间：%s\n", ctime(&card->tLast));
    printf("注册时间：%s\n", ctime(&card->tStart));
    printf("删除标志：%s\n", card->nDel == CARD_EXIST ? "未删除" : card->nDel == CARD_NO_EXIST ? "已删除" : "未知");
}

void findcard() {
    
    char searchName[18];
    Card foundCard[100]; // 假设最多有100个匹配的用户
    printf("请输入要查找的用户名：");
    scanf("%s", searchName);
    if(strlen(searchName) > 17){
        printf("搜索失败（用户名过长）\n");
        return;
    }
    CardList current = cardList_head;
    int exactfind = (strlen(searchName) == 17);
    if(exactfind){
        while (current != NULL) {
            if (strcmp(current->data.aName, searchName) == 0) {
                int attemp = 0;
                while(attemp < 3){
                    printf("请输入密码:");
                    char inputPwd[8];
                    scanf("%s",inputPwd);
                    if(strcmp(inputPwd,current->data.aPwd) == 0){
                        printf("密码正确！\n");
                        break;
                    }
                    else{
                        printf("密码错误，请重新输入！\n");
                        attemp++;
                    }
                    if (attemp >= 3) {
                        printf("连续三次密码错误，查询失败！\n");
                        return;
                    }
                }
                printCard(&current->data);
                return;
            }
            current = current->next;
        }
        printf("未找到用户：%s\n", searchName);
    }
    else{
        int count = 0;
        while (current != NULL) {
            if (strstr(current->data.aName, searchName) != NULL) {
                foundCard[count++] = current->data;
                if (count >= 100) {
                    printf("找到 100 个用户，停止搜索！\n");
                    printf("请缩小搜索范围以获得更准确的结果。\n");
                    break;
                }
            }
            current = current->next;
        }
        if (count == 0) {
            printf("未找到包含 '%s' 的用户\n", searchName);
        } 
        else if(count == 1){
            printf("找到 1 个用户：\n");
            printf("用户名: %s\n", foundCard[0].aName);
            char inputPwd[8];
            int attempt = 0;
            while (attempt < 3) {
                printf("请输入密码：");
                scanf("%s", inputPwd);
                if (strcmp(inputPwd, foundCard[0].aPwd) == 0) {
                    printf("密码正确！\n");
                    printCard(&foundCard[0]);
                    return;
                } else {
                    printf("密码错误，请重新输入！\n");
                    attempt++;
                    if(attempt >= 3){
                        printf("连续三次密码错误，查询失败！\n");
                        return;
                    }
                }
            }
        }
        else {
            printf("找到 %d 个用户：\n", count);
            for (int i = 0; i < count; i++) {
                printf("用户 %d:\n", i + 1);
                printf("用户名: %s\n", foundCard[i].aName);
            }
            printf("请输入要查看详情的序号：");
            int index;
            scanf("%d", &index);
            if (index < 1 || index > count) {
                printf("无效的序号！\n");
            } else {
                char inputPwd[8];
                int attempt = 0;
                while (attempt < 3) {
                    printf("请输入密码：");
                    scanf("%s", inputPwd);
                    if (strcmp(inputPwd, foundCard[index - 1].aPwd) == 0) {
                        printf("密码正确！\n");
                        printCard(&foundCard[index - 1]);
                        return;
                    } else {
                        printf("密码错误，请重新输入！\n");
                        attempt++;
                        if(attempt >= 3){
                            printf("连续三次密码错误，查询失败！\n");
                            return;
                        }
                    }
                }
            }
      }
    }
}

Card* check_card(const char* aName, const char* aPwd){
    CardList current = cardList_head;
    int nIndex = 0;
    int find = 0;
    while(current != NULL){
        if(strcmp(current->data.aName, aName) == 0){
            if(strcmp(current->data.aPwd, aPwd) == 0){
                if(current->data.nStatus == CARD_ON_COMPUTER){
                    printf("用户已在上机状态，无法重复上机！\n");
                    return NULL;
                }
                else if(current->data.nStatus == CARD_DEL){
                    printf("用户已注销，无法上机！\n");
                    return NULL;
                }
                else if(current->data.nStatus == CARD_INVALID){
                    printf("用户已失效，无法上机！\n");
                    return NULL;
                }
                else if(current->data.fBalance <2){
                    printf("余额不足2元，无法上机！\n");
                    return NULL;
                }
                else{
                    current->data.nStatus = CARD_ON_COMPUTER;
                    current->data.tLast = time(NULL);
                    find = 1;
                    break;
                }
            }
            else{
                printf("密码错误！\n");
                return NULL;
            }
        }
        current = current -> next;
        nIndex++;
    }
    if(FALSE == find){
        printf("未找到用户！\n");
        return NULL;
    }
    if(TRUE == saveAllCards(cardList_head, CARD_PATH)){
        return &current ->data;
    }
    else{
        return NULL;
    }
}

int deleteCard(const char* pName)
{
    CardList current = cardList_head;
    CardList prev = NULL;
    int found = 0;
    
    while(current != NULL)
    {
        if(strcmp(current->data.aName, pName) == 0)
        {
            found = 1;
            break;
        }
        prev = current;
        current = current->next;
    }
    
    if(found == 0)
    {
        printf("未找到用户：%s\n", pName);
        return 0;
    }
    
    // 检查卡片状态
    if(current->data.nStatus == CARD_ON_COMPUTER)
    {
        printf("用户正在上机，无法删除！\n");
        return 0;
    }
    
    // 将卡片标记为已删除
    current->data.nDel = CARD_NO_EXIST;
    
    // 保存到文件
    if(saveAllCards(cardList_head, CARD_PATH) != 1)
    {
        printf("删除卡片失败：保存文件错误！\n");
        current->data.nDel = CARD_EXIST; // 恢复状态
        return 0;
    }
    
    // 从链表中移除
    if(prev == NULL)
    {
        // 删除的是头节点
        cardList_head = current->next;
        if(cardList_head == NULL)
        {
            cardList_tail = NULL;
        }
    }
    else
    {
        prev->next = current->next;
        if(prev->next == NULL)
        {
            cardList_tail = prev;
        }
    }
    
    free(current);
    printf("卡片删除成功！\n");
    return 1;
}