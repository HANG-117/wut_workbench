#include <service.h>
#include <stdio.h>
#include <card_service.h>
void service_menu(int choice) {
    switch (choice){
        case 1:
            printf("正在添加卡...\n");
            Card newCard = creat_card();
            int k = addCard(newCard);
            if(k==1) printf("卡添加成功！\n");
            else printf("卡添加失败！\n");
            break;
        case 2:
            printf("正在查找卡...\n");
            findcard();
            break;
        default:
            printf("无效的选择，请重新输入！\n");
            break;
    }
}