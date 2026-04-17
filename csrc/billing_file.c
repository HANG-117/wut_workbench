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