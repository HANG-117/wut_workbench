#include "card_service.h"
#include <stdio.h>
#include "menu.h"
#include "card_file.h"
#include "global.h"
#include "model.h"
#include "billing_service.h"
#include "billing_file.h"
#include <assert.h>
#include <stdlib.h>
void service_menu(int choice) {
    switch (choice){
        case 1:
            printf("正在添加卡...\n");
            Card newCard = creat_card();
            int k = addCard(newCard);
            if(k==1) printf("卡添加成功！\n");
            else printf("卡添加失败！\n");
            break;
        case 2:
            printf("正在查找卡...\n");
            findcard();
            break;
        case 3:
            printf("正在上机...\n");
            logon();
            break;
        case 0:
            exit(0);
        default:
            printf("无效的选择，请重新输入！\n");
            break;
    }
}

int dologon(Billing *billing,LogonInfo *pInfo){
    Card* pCard = NULL;
    char aName[18];
    char aPwd[8];
    printf("请输入用户名：");
    scanf("%s", aName);
    printf("请输入密码：");
    scanf("%s", aPwd);
    pCard = check_card(aName, aPwd);
    if(pCard == NULL){
        return FALSE;
    }
    strcpy(billing->aCardName, pCard->aName);
    billing->nStatus = NOT_SETTLE_BILLING;
    billing->tStart = time(NULL);
    billing->nDel = FALSE;
    
    if(addBilling(billing)==TRUE){
        strcpy(pInfo->aCardName, pCard->aName);
        pInfo->fBalance = pCard->fBalance;
        pInfo->tLogon = time(NULL);
        free(billing);
    }
    else{
        assert(0);
    }
    

    
    return TRUE;
}