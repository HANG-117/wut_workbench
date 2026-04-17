#include"menu.h"
#include"service.h"
#include"card_service.h"
#include"card_file.h"
#include"global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern CardList cardList_head;
extern CardList cardList_tail;

int main(){
    readCardFile(&cardList_head, &cardList_tail, CARD_FILE);
    int choice;
    while(1){
        static int count = 0;
        count++;
        if(count > 100) {
            printf("程序已运行超过100次，自动退出。\n");
            break;
        }
        displayMainMenu();
        choice = getChoice();
        if(choice == -1) {
            printf("无效的选择，请重新输入！\n");
            continue;
        }
       
        service_menu(choice);
    }
    return 0;
}