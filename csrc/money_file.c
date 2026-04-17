#include "money_file.h"
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
int saveMoney(const Money* pMoney, const char* pPath)
{
    FILE *fp = fopen(pPath, "ab");
    if(fp == NULL)
    {
        return FALSE;
    }
    fwrite(pMoney, sizeof(Money), 1, fp);
    fclose(fp);
    return TRUE;
}
