#include "billing_file.h"
#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
int saveBillingFile(const Billing *pBilling, const char *pPath) 
{
    FILE *fp = fopen(pPath, "ab");
    if(fp == NULL)
        {
            return FALSE;
        }
    fwrite(pBilling, sizeof(Billing), 1, fp);
    fclose(fp);
    return TRUE;
}

int readBilling(Billing* pBilling, const char* pPath)
{
    FILE *fp = fopen(pPath, "rb");
    if(fp == NULL)
    {
        return FALSE;
    }
    if(fread(pBilling, sizeof(Billing), 1, fp) != 1)
    {
        fclose(fp);
        return FALSE;
    }
    fclose(fp);
    return TRUE;
}

int getBillingCount(const char* pPath)
{
    FILE *fp = fopen(pPath, "rb");
    if(fp == NULL)
    {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fclose(fp);
    return fileSize / sizeof(Billing);
}

int updateBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
    FILE *fp = fopen(pPath, "rb+");
    if(fp == NULL)
    {
        return FALSE;
    }
    fseek(fp, nIndex * sizeof(Billing), SEEK_SET);
    if(fwrite(pBilling, sizeof(Billing), 1, fp) != 1)
    {
        fclose(fp);
        return FALSE;
    }
    fclose(fp);
    return TRUE;
}