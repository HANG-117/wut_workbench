#ifndef MODEL_H
#define MODEL_H
#include <time.h>
typedef struct LogonInfo
{
    char aCardName[18];
    time_t tLogon;
    float fBalance;
}LogonInfo;

typedef struct SettleInfo
{
    char aCardName[18];
    float fAmount;
    float fBalance;
    time_t tStart;
    time_t tEnd;
}SettleInfo;

typedef struct AnnulInfo
{
    char aCardName[18];
    float fRefund;
    float fBalance;
    time_t tAnnul;
}AnnulInfo;

typedef struct Billing
{
    char aCardName[18];
    time_t tStart;
    time_t tEnd;
    float fAmount;
    int nStatus;
    int nDel;
}Billing;

    typedef struct Card {
    char aName[18]; // 用户名，最多17个字符 + 1个结束符
    char aPwd[8]; // 密码，最多7个字符 + 1个结束符
    int nStatus; // 0-未上机 1-正在上机 2-已注销 3-失效
    time_t tStart; // 注册时间
    time_t tEnd; // 截止时间
    float fTotalUse; // 总使用金额
    time_t tLast;    // 最后使用时间
    int nUseCount; // 使用次数
    float fBalance; // 余额
    int nDel;   // 删除标志，0表示未删除，1表示已删除
} Card;
typedef struct CardNode {
    Card data;
    struct CardNode* next;
} CardNode,*CardList;
typedef struct BillingNode {
    Billing data;
    struct BillingNode* next;
} BillingNode,*BillingList;

typedef struct Money
{
    char aCardName[18];
    float fAmount;
    int nStatus; // 0-充值 1-退费
    time_t tTime;
} Money;

typedef struct MoneyInfo
{
    char aCardName[18];
    float fAmount;
    float fBalance;
} MoneyInfo;

typedef struct Rate
{
    float fUnit;  // 计费单位（分钟）
    float fCharge; // 每单位收费金额
    time_t tLastUpdate; // 最后更新时间
} Rate;

#endif