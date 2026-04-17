#ifndef BILLING_SERIVCE_H
#define BILLING_SERIVCE_H
#include "model.h"
#include "global.h"
    int addBilling(Billing *Billing);
    void initBillingList();
    void releaseBillingList();
    int getBilling();
    Billing* queryBilling(const char* pName, int* pIndex);
#endif