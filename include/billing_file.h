#ifndef BILLING_FILE_H
#define BILLING_FILE_H
    #include <time.h>
    typedef struct Billing {
        char aCardName[18];
        time_t tStart;
        time_t tEnd;
        float fTotalUse;
        int nStatus;
        int nDel;
    } Billing;

#endif