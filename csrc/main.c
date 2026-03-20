#include"menu.h"
#include"service.h"
#include"card_service.h"
#include"global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(){
    int choice;
    while(1){
        static int count = 0;
        displayMainMenu();
        choice = getChoice();
        count++;
        if(count > 100) {
            printf("程序已运行超过100次，自动退出。\n");
            break;
        }
        choice = distinguishchoice(choice);
        if(choice == -1) {
            exit(0);
        }
        else if(choice == 0) {
           continue;
        }
        else if(choice >= 1) {
            service_menu(choice);
        }

    }
    return 0;
}