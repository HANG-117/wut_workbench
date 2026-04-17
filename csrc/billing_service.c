#include "billing_file.h"
#include "model.h"
#include "billing_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int addBilling(Billing *Billing)
{ 
    return saveBillingFile(Billing,BILLINGPATH);
}

BillingList billingList_head = NULL;
BillingList billingList_tail = NULL;

void initBillingList()
{
    billingList_head = NULL;
    billingList_tail = NULL;
}

void releaseBillingList()
{
    BillingList current = billingList_head;
    while(current != NULL)
    {
        BillingList temp = current;
        current = current->next;
        free(temp);
    }
    billingList_head = NULL;
    billingList_tail = NULL;
}

int getBilling()
{
    int count = getBillingCount(BILLINGPATH);
    if(count <= 0)
    {
        return FALSE;
    }
    
    FILE *fp = fopen(BILLINGPATH, "rb");
    if(fp == NULL)
    {
        return FALSE;
    }
    
    Billing billing;
    while(fread(&billing, sizeof(Billing), 1, fp) == 1)
    {
        BillingNode *newNode = (BillingNode*)malloc(sizeof(BillingNode));
        if(newNode == NULL)
        {
            fclose(fp);
            return FALSE;
        }
        newNode->data = billing;
        newNode->next = NULL;
        
        if(billingList_head == NULL)
        {
            billingList_head = newNode;
            billingList_tail = newNode;
        }
        else
        {
            billingList_tail->next = newNode;
            billingList_tail = newNode;
        }
    }
    fclose(fp);
    return TRUE;
}

Billing* queryBilling(const char* pName, int* pIndex)
{
    BillingList current = billingList_head;
    int index = 0;
    
    while(current != NULL)
    {
        if(strcmp(current->data.aCardName, pName) == 0 && current->data.nStatus == NOT_SETTLE_BILLING)
        {
            *pIndex = index;
            return &current->data;
        }
        current = current->next;
        index++;
    }
    return NULL;
}

int getRevenueStatistics(float* fDayRevenue, float* fMonthRevenue, float* fYearRevenue)
{
    // 初始化计费列表
    initBillingList();
    if(getBilling() == FALSE)
    {
        releaseBillingList();
        return FALSE;
    }

    // 获取当前时间
    time_t tNow = time(NULL);
    struct tm* tmNow = localtime(&tNow);
    int nCurrentYear = tmNow->tm_year + 1900;
    int nCurrentMonth = tmNow->tm_mon + 1;
    int nCurrentDay = tmNow->tm_mday;

    // 初始化统计值
    *fDayRevenue = 0.0f;
    *fMonthRevenue = 0.0f;
    *fYearRevenue = 0.0f;

    // 遍历计费记录
    BillingList current = billingList_head;
    while(current != NULL)
    {
        // 只统计已结算的记录
        if(current->data.nStatus == SETTLE_BILLING)
        {
            struct tm* tmEnd = localtime(&current->data.tEnd);
            int nYear = tmEnd->tm_year + 1900;
            int nMonth = tmEnd->tm_mon + 1;
            int nDay = tmEnd->tm_mday;

            // 统计当年营收
            if(nYear == nCurrentYear)
            {
                *fYearRevenue += current->data.fAmount;

                // 统计当月营收
                if(nMonth == nCurrentMonth)
                {
                    *fMonthRevenue += current->data.fAmount;

                    // 统计当日营收
                    if(nDay == nCurrentDay)
                    {
                        *fDayRevenue += current->data.fAmount;
                    }
                }
            }
        }
        current = current->next;
    }

    releaseBillingList();
    return TRUE;
}