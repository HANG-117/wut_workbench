#include <stdio.h>
#include "menu.h"
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
    return choice;
}
int distinguishchoice(int choice) {
    int cor = 0;
    switch(choice) {
        case 1:
            printf("添加卡片功能\n");
            cor = 1;
            break;
        case 2:
            printf("查询卡片功能\n");
            cor = 2;
            break;
        case 3:
            printf("上机功能\n");
            cor = 3;
            break;
        case 4:
            printf("下机功能\n");
            cor = 4;
            break;
        case 5:
            printf("充值功能\n");
            cor = 5;
            break;
        case 6:
            printf("退费功能\n");
            cor = 6;
            break;
        case 7:
            printf("查询统计功能\n");
            cor = 7;
            break;
        case 8:
            printf("注销卡功能\n");
            cor = 8;
            break;
        case 0:
            printf("退出程序\n");
            cor =-1;
            break;
        default:
            printf("非法输入，请重新输入。\n");
            cor = 0;
            break;
    }
    return cor; 
}