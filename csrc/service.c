#include "card_service.h"
#include <stdio.h>
#include "menu.h"
#include "card_file.h"
#include "global.h"
#include "model.h"
#include "billing_service.h"
#include "billing_file.h"
#include "money_file.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

extern CardList cardList_head;
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
        case 4:
            printf("正在下机...\n");
            settle();
            break;
        case 5:
            printf("正在充值...\n");
            addMoney();
            break;
        case 6:
            printf("正在退费...\n");
            refundMoney();
            break;
        case 7:
            printf("正在查询统计...\n");
            findcard();
            break;
        case 8:
            printf("正在注销卡...\n");
            annual();
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
        return TRUE;
    }
    else{
        // 添加billing失败，需要恢复卡片状态
        pCard->nStatus = CARD_NOT_ON_COMPUTER;
        // 更新文件
        saveAllCards(cardList_head, CARD_PATH);
        printf("添加消费记录失败，上机失败！\n");
        return FALSE;
    }



    return TRUE;
}

double getAmount(time_t tStart)
{
    time_t tEnd = time(NULL);
    double duration = difftime(tEnd, tStart);
    // 两分钟内下机不收取费用
    if(duration <= 2 * 60)
    {
        return 0.0;
    }
    int units = (int)(duration / (UNIT * 60));
    if(duration > 0 && units == 0)
    {
        units = 1;
    }
    return units * CHARGE;
}

int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
    // 查找卡片
    CardList current = cardList_head;
    Card* pCard = NULL;

    while(current != NULL)
    {
        if(strcmp(current->data.aName, pName) == 0)
        {
            if(strcmp(current->data.aPwd, pPwd) == 0)
            {
                if(current->data.nStatus != CARD_ON_COMPUTER)
                {
                    printf("用户未在上机状态，无法下机！\n");
                    return FALSE;
                }
                pCard = &current->data;
                break;
            }
            else
            {
                printf("密码错误！\n");
                return FALSE;
            }
        }
        current = current->next;
    }

    if(pCard == NULL)
    {
        printf("未找到用户！\n");
        return FALSE;
    }

    // 初始化并获取计费信息
    initBillingList();
    if(getBilling() == FALSE)
    {
        releaseBillingList();
        return FALSE;
    }

    // 查找该卡的计费信息
    int nIndex = 0;
    Billing* pBilling = queryBilling(pName, &nIndex);
    if(pBilling == NULL)
    {
        releaseBillingList();
        return FALSE;
    }

    // 计算消费金额
    double fAmount = getAmount(pBilling->tStart);

    // 判断余额是否足够
    if(pCard->fBalance < fAmount)
    {
        printf("余额不足，下机失败！\n");
        releaseBillingList();
        return FALSE;
    }

    // 更新卡片信息
    pCard->fBalance -= fAmount;
    pCard->fTotalUse += fAmount;
    pCard->nUseCount++;
    pCard->tLast = time(NULL);
    pCard->nStatus = CARD_NOT_ON_COMPUTER;

    // 更新计费信息
    pBilling->tEnd = time(NULL);
    pBilling->fAmount = fAmount;
    pBilling->nStatus = SETTLE_BILLING;

    // 保存下机信息
    strcpy(pInfo->aCardName, pCard->aName);
    pInfo->fAmount = fAmount;
    pInfo->fBalance = pCard->fBalance;
    pInfo->tStart = pBilling->tStart;
    pInfo->tEnd = pBilling->tEnd;

    // 更新文件
    saveAllCards(cardList_head, CARD_PATH);

    updateBilling(pBilling, BILLINGPATH, nIndex);

    releaseBillingList();
    return TRUE;
}

int doAddMoney(const char* pName, const char* pPwd, float fAmount, MoneyInfo* pMoneyInfo)
{
    // 查找卡片
    CardList current = cardList_head;
    Card* pCard = NULL;

    while(current != NULL)
    {
        if(strcmp(current->data.aName, pName) == 0)
        {
            if(strcmp(current->data.aPwd, pPwd) == 0)
            {
                if(current->data.nStatus == CARD_DEL)
                {
                    printf("用户已注销，无法充值！\n");
                    return FALSE;
                }
                else if(current->data.nStatus == CARD_INVALID)
                {
                    printf("用户已失效，无法充值！\n");
                    return FALSE;
                }
                pCard = &current->data;
                break;
            }
            else
            {
                printf("密码错误！\n");
                return FALSE;
            }
        }
        current = current->next;
    }

    if(pCard == NULL)
    {
        printf("未找到用户！\n");
        return FALSE;
    }

    if(fAmount <= 0)
    {
        printf("充值金额必须大于0！\n");
        return FALSE;
    }

    // 更新卡片信息
    pCard->fBalance += fAmount;
    pCard->tLast = time(NULL);

    // 保存充值信息
    Money money;
    strcpy(money.aCardName, pCard->aName);
    money.fAmount = fAmount;
    money.nStatus = MONEY_ADD;
    money.tTime = time(NULL);

    if(saveMoney(&money, MONEY_PATH) == FALSE)
    {
        printf("保存充值记录失败！\n");
        pCard->fBalance -= fAmount; // 恢复余额
        return FALSE;
    }

    // 更新文件
    saveAllCards(cardList_head, CARD_PATH);

    // 保存充值信息
    strcpy(pMoneyInfo->aCardName, pCard->aName);
    pMoneyInfo->fAmount = fAmount;
    pMoneyInfo->fBalance = pCard->fBalance;

    return TRUE;
}

int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
    // 查找卡片
    CardList current = cardList_head;
    Card* pCard = NULL;

    while(current != NULL)
    {
        if(strcmp(current->data.aName, pName) == 0)
        {
            if(strcmp(current->data.aPwd, pPwd) == 0)
            {
                if(current->data.nStatus == CARD_DEL)
                {
                    printf("用户已注销，无法退费！\n");
                    return FALSE;
                }
                else if(current->data.nStatus == CARD_INVALID)
                {
                    printf("用户已失效，无法退费！\n");
                    return FALSE;
                }
                else if(current->data.nStatus == CARD_ON_COMPUTER)
                {
                    printf("用户正在上机，无法退费！\n");
                    return FALSE;
                }
                else if(current->data.fBalance <= 0)
                {
                    printf("余额为0，无法退费！\n");
                    return FALSE;
                }
                pCard = &current->data;
                break;
            }
            else
            {
                printf("密码错误！\n");
                return FALSE;
            }
        }
        current = current->next;
    }

    if(pCard == NULL)
    {
        printf("未找到用户！\n");
        return FALSE;
    }

    // 保存退费信息
    Money money;
    strcpy(money.aCardName, pCard->aName);
    money.fAmount = pCard->fBalance;
    money.nStatus = MONEY_REFUND;
    money.tTime = time(NULL);

    if(saveMoney(&money, MONEY_PATH) == FALSE)
    {
        printf("保存退费记录失败！\n");
        return FALSE;
    }

   // 更新卡片信息
    pCard->fBalance = 0;
    pCard->tLast = time(NULL);

    // 更新文件
    saveAllCards(cardList_head, CARD_PATH);


    // 保存退费信息
    strcpy(pMoneyInfo->aCardName, pCard->aName);
    pMoneyInfo->fAmount = money.fAmount;
    pMoneyInfo->fBalance = pCard->fBalance;

    return TRUE;
}

int annualCard(const char* pName, const char* pPwd, AnnulInfo* pAnnulInfo)
{
    // 查找卡片
    CardList current = cardList_head;
    Card* pCard = NULL;

    while(current != NULL)
    {
        if(strcmp(current->data.aName, pName) == 0)
        {
            if(strcmp(current->data.aPwd, pPwd) == 0)
            {
                if(current->data.nStatus == CARD_DEL)
                {
                    printf("用户已注销，无法再次注销！\n");
                    return FALSE;
                }
                else if(current->data.nStatus == CARD_INVALID)
                {
                    printf("用户已失效，无法注销！\n");
                    return FALSE;
                }
                else if(current->data.nStatus == CARD_ON_COMPUTER)
                {
                    printf("用户正在上机，无法注销！\n");
                    return FALSE;
                }
                pCard = &current->data;
                break;
            }
            else
            {
                printf("密码错误！\n");
                return FALSE;
            }
        }
        current = current->next;
    }

    if(pCard == NULL)
    {
        printf("未找到用户！\n");
        return FALSE;
    }

    // 保存退费信息
    Money money;
    strcpy(money.aCardName, pCard->aName);
    money.fAmount = pCard->fBalance;
    money.nStatus = MONEY_REFUND;
    money.tTime = time(NULL);

    if(saveMoney(&money, MONEY_PATH) == FALSE)
    {
        printf("保存退费记录失败！\n");
        return FALSE;
    }

    // 更新卡片信息
    pCard->nStatus = CARD_DEL;
    pCard->tLast = time(NULL);

    // 更新文件
    saveAllCards(cardList_head, CARD_PATH);

    // 保存注销信息
    strcpy(pAnnulInfo->aCardName, pCard->aName);
    pAnnulInfo->fRefund = money.fAmount;
    pAnnulInfo->fBalance = 0;
    pAnnulInfo->tAnnul = time(NULL);

    return TRUE;
}
