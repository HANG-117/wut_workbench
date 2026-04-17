#include <stdio.h>
#include "menu.h"
#include "card_service.h"
#include "service.h"
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
