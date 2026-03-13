#include "card_service.h"
#include "card_file.h"
#include "global.h"
#include <stdio.h>
#include <string.h>
int cardCount = 0;
Card cardList[MAX_CARD_NUM];
int is_all_digits(const char* str) {
    if (str == NULL || *str == '\0') return 0; 
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}
void addCard() {
    while(1){
        char Name[20];
        char Pwd[10];
        while(1){
            printf("请输入卡号\n");
            scanf("%s", Name);
            if (strlen(Name) > 18) {
                printf("卡号长度不能超过18位，请重新输入\n");
                continue;
            }
            break;
        }
        int exists = 0;
        for(int i = 0;i<cardCount;i++){
            if(strcmp(Name, cardList[i].aName) == 0){
                printf("卡号已存在，请重新输入\n");
                exists = 1;
            }
        }
        if(exists == 1){
            continue;
        }
        while(1){
            strcpy(cardList[cardCount].aName, Name);
            printf("请输入密码(不能为纯数字)\n");
            scanf("%s", Pwd);
            if (strlen(Pwd) > 8) {
                printf("密码长度不能超过8位，请重新输入\n");
                continue;
            }
            if(is_all_digits(Pwd)) {
                printf("密码不能为纯数字，请重新输入\n");
                continue;
            }
            break;
        }
        
        strcpy(cardList[cardCount].aPwd, Pwd);
        cardList[cardCount].nStatus = 0;
        cardList[cardCount].tStart = time(NULL);
        cardList[cardCount].tEnd = time(NULL) + 365 * 24;
        cardList[cardCount].fTotalUse = 0.0;
        cardList[cardCount].tLast = time(NULL);
        cardList[cardCount].nUseCount = 0;
        cardList[cardCount].fBalance = 0.0;
        cardList[cardCount].nDel = 0;
        cardCount++;
        printf("卡号：%s 添加成功\n", cardList[cardCount - 1].aName);
        if (cardCount >= MAX_CARD_NUM) {
            printf("卡号已满，无法继续添加\n");
            break;
        }

        printf("是否继续添加？(y/n)");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'n' || choice == 'N') {
            break;}
        else if (choice == 'y' || choice == 'Y') {
            continue;
        }
        else {
            printf("无效输入，默认不再添加\n");
            break;
        }
    }
}
void findCard(){
    char Name[20];
    while(1){
        int is_exact = 0;
        printf("请输入要查询的卡号：");
        scanf("%s", Name);
        if (strlen(Name) > 18) {
            printf("卡号长度不能超过18位，请重新输入\n");
            continue;
        }
        int found = 0;
        if (strlen(Name) == 18) is_exact = 1;
        if(is_exact)
        {
            Card card_find;
            for (int i = 0; i < cardCount; i++) {
                if (strcmp(Name, cardList[i].aName) == 0) {
                    card_find = cardList[i];
                    found = 1;
                    break;
                }
            }
            if (found) {
                while(1){
                    printf("请输入密码");
                    char password[10];
                    scanf("%s", password);
                    if (strcmp(password, card_find.aPwd) == 0) {
                        printf("密码正确\n");
                        break;
                    }
                    else {
                        printf("密码错误，请重新输入\n");
                        continue;
                    }
                }
                printf("卡号：%s\n", card_find.aName);
                printf("密码：%s\n", card_find.aPwd);
                printf("状态：%d\n", card_find.nStatus);
                printf("开始时间：%s", ctime(&card_find.tStart));
                printf("结束时间：%s", ctime(&card_find.tEnd));
                printf("总使用金额：%.2f\n", card_find.fTotalUse);
                printf("上次使用时间：%s", ctime(&card_find.tLast));
                printf("使用次数：%d\n", card_find.nUseCount);
                printf("余额：%.2f\n", card_find.fBalance);
                printf("是否删除：%d\n", card_find.nDel);
            }
            else {
                printf("未找到该卡号\n");
            }
        }
        else {
            Card cards_find[MAX_CARD_NUM];
            int count = 0;
            for (int i = 0; i < cardCount; i++) {
                if (strstr(cardList[i].aName, Name) != NULL) {
                    cards_find[count++] = cardList[i];
                }
            }
            if (count > 0) {
                printf("找到 %d 个匹配的卡号：\n", count);
                for (int i = 0; i < count; i++) {
                    printf("卡号：%s\n", cards_find[i].aName);
                }
                int index;
                while(1){
                    printf("请输入要查询的卡号索引：");
                    scanf("%d", &index);
                    if (index >= 0 && index < count) {
                        printf("卡号：%s\n", cards_find[index].aName);
                        while(1){
                            printf("请输入密码");
                            char password[10];
                            scanf("%s", password);
                            if (strcmp(password, cards_find[index].aPwd) == 0) {
                                printf("密码正确\n");
                                break;
                            }
                            else {
                                printf("密码错误，请重新输入\n");
                                continue;
                            }
                        }
                    }
                    else{
                        printf("索引无效，请重新输入\n");
                        continue;
                    }
                    printf("卡号：%s\n", cards_find[index].aName);
                    printf("密码：%s\n", cards_find[index].aPwd);
                    printf("状态：%d\n", cards_find[index].nStatus);
                    printf("开始时间：%s", ctime(&cards_find[index].tStart));
                    printf("结束时间：%s", ctime(&cards_find[index].tEnd));
                    printf("总使用金额：%.2f\n", cards_find[index].fTotalUse);
                    printf("上次使用时间：%s", ctime(&cards_find[index].tLast));
                    printf("使用次数：%d\n", cards_find[index].nUseCount);
                    printf("余额：%.2f\n", cards_find[index].fBalance);
                    printf("是否删除：%d\n", cards_find[index].nDel);
                    break;
                }
                
            }
            else {
                printf("未找到该卡号\n");
            }
        }
        char choice;
        printf("是否继续查询？(y/n)");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            continue;
        }
        else{
            break;
        }
    }
        
}