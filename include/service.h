#ifndef SERVICE_H
#define SERVICE_H
#include "model.h"
void service_menu(int choice );
int dologon(Billing *billing , LogonInfo* pInfo);
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
double getAmount(time_t tStart);
int doAddMoney(const char* pName, const char* pPwd, float fAmount, MoneyInfo* pMoneyInfo);
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);

#endif