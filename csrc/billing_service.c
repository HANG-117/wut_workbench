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