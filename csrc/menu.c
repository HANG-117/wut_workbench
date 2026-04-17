#include <stdio.h>
#include "menu.h"
#include "card_service.h"
#include "service.h"
#include "billing_service.h"
#include "rate_service.h"
#include <string.h>
void displayMainMenu() {
    printf("欢迎进入计费管理系统\n");
    printf("======菜单======\n");
    printf("1. 添加卡片\n");
    printf("2. 查询卡片\n");
    printf("3. 上机\n");
    printf("4. 下机\n");
    printf("5. 充值\n");
    printf("6. 退费\n");
    printf("7. 查询统计\n");
    printf("8. 注销卡\n");
    printf("0. 退出\n");
    printf("================\n");
    printf("请输入您的选择: ");
}
int getChoice() {
    int choice;
    scanf("%d", &choice);
    if(choice == 999) {
        // 隐藏菜单触发码
        return 999;
    }
    if(choice < 0 || choice > 8) {
        printf("%d无效的选择，请重新输入！\n",choice);
        return -1;
    }
    return choice;
}
void logon(){
    LogonInfo logonInfo;
    Billing billing;
    int nResult = dologon(&billing,&logonInfo);
    if(nResult != 0){
        printf("上机成功！\n");
        printf("卡号：%s\n",logonInfo.aCardName);
        printf("余额：%.2f\n",logonInfo.fBalance);
        printf("上机时间：%s\n",ctime(&logonInfo.tLogon));        
    }
    else{
        printf("上机失败！\n");
    }
}

void settle(){
    SettleInfo settleInfo;
    char aName[18];
    char aPwd[8];
    printf("请输入卡号：");
    scanf("%s", aName);
    printf("请输入密码：");
    scanf("%s", aPwd);
    
    int nResult = doSettle(aName, aPwd, &settleInfo);
    if(nResult != 0){
        printf("下机成功！\n");
        printf("卡号：%s\n", settleInfo.aCardName);
        printf("消费：%.2f\n", settleInfo.fAmount);
        printf("余额：%.2f\n", settleInfo.fBalance);
        printf("上机时间：%s", ctime(&settleInfo.tStart));
        printf("下机时间：%s", ctime(&settleInfo.tEnd));
    }
    else{
        printf("下机失败！\n");
    }
}

void addMoney(){
    MoneyInfo moneyInfo;
    char aName[18];
    char aPwd[8];
    float fAmount;
    printf("请输入卡号：");
    scanf("%s", aName);
    printf("请输入密码：");
    scanf("%s", aPwd);
    printf("请输入充值金额：");
    scanf("%f", &fAmount);
    
    if(fAmount <= 0){
        printf("充值金额必须大于0！\n");
        return;
    }
    
    int nResult = doAddMoney(aName, aPwd, fAmount, &moneyInfo);
    if(nResult != 0){
        printf("充值成功！\n");
        printf("卡号：%s\n", moneyInfo.aCardName);
        printf("充值金额：%.2f\n", moneyInfo.fAmount);
        printf("余额：%.2f\n", moneyInfo.fBalance);
    }
    else{
        printf("充值失败！\n");
    }
}

void refundMoney(){
    MoneyInfo moneyInfo;
    char aName[18];
    char aPwd[8];
    printf("请输入卡号：");
    scanf("%s", aName);
    printf("请输入密码：");
    scanf("%s", aPwd);
    
    int nResult = doRefundMoney(aName, aPwd, &moneyInfo);
    if(nResult != 0){
        printf("退费成功！\n");
        printf("卡号：%s\n", moneyInfo.aCardName);
        printf("退费金额：%.2f\n", moneyInfo.fAmount);
        printf("余额：%.2f\n", moneyInfo.fBalance);
    }
    else{
        printf("退费失败！\n");
    }
}

void annual(){
    AnnulInfo annulInfo;
    char aName[18];
    char aPwd[8];
    printf("请输入卡号：");
    scanf("%s", aName);
    printf("请输入密码：");
    scanf("%s", aPwd);

    int nResult = annualCard(aName, aPwd, &annulInfo);
    if(nResult != 0){
        printf("注销卡成功！\n");
        printf("卡号：%s\n", annulInfo.aCardName);
        printf("退费金额：%.2f\n", annulInfo.fRefund);
        printf("注销时间：%s", ctime(&annulInfo.tAnnul));
    }
    else{
        printf("注销卡失败！\n");
    }
}

void statistics(){
    float fDayRevenue = 0.0f;
    float fMonthRevenue = 0.0f;
    float fYearRevenue = 0.0f;

    int nResult = getRevenueStatistics(&fDayRevenue, &fMonthRevenue, &fYearRevenue);
    if(nResult != 0){
        printf("===== 营收统计 =====\n");
        printf("当日营收：%.2f 元\n", fDayRevenue);
        printf("当月营收：%.2f 元\n", fMonthRevenue);
        printf("当年营收：%.2f 元\n", fYearRevenue);
        printf("===================\n");
    }
    else{
        printf("获取营收统计失败！\n");
    }
}

void displayHiddenMenu() {
    printf("===== 隐藏菜单 =====\n");
    printf("1. 设置费率\n");
    printf("2. 查看当前费率\n");
    printf("3. 删除卡\n");
    printf("0. 返回正常菜单\n");
    printf("===================\n");
    printf("请输入您的选择: ");
}

int getHiddenMenuChoice() {
    int choice;
    scanf("%d", &choice);
    if(choice < 0 || choice > 3) {
        printf("%d无效的选择，请重新输入！\n",choice);
        return -1;
    }
    return choice;
}

void hiddenMenu() {
    int choice;
    while(1) {
        displayHiddenMenu();
        choice = getHiddenMenuChoice();
        if(choice == -1) {
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("设置费率...\n");
                {
                    float fUnit, fCharge;
                    printf("请输入计费单位（分钟）：");
                    scanf("%f", &fUnit);
                    printf("请输入每单位收费金额（元）：");
                    scanf("%f", &fCharge);
                    setRate(fUnit, fCharge);
                }
                break;
            case 2:
                printf("查看当前费率...\n");
                {
                    Rate rate;
                    if(getCurrentRate(&rate) == 1) {
                        printf("===== 当前费率 =====\n");
                        printf("计费单位：%.0f 分钟\n", rate.fUnit);
                        printf("每单位收费：%.2f 元\n", rate.fCharge);
                        printf("更新时间：%s", ctime(&rate.tLastUpdate));
                        printf("===================\n");
                    }
                }
                break;
            case 3:
                printf("删除卡...\n");
                {
                    char aName[18];
                    printf("请输入要删除的卡号：");
                    scanf("%s", aName);
                    deleteCard(aName);
                }
                break;
            case 0:
                printf("返回正常菜单...\n");
                return;
            default:
                printf("无效的选择，请重新输入！\n");
                break;
        }
    }
}
