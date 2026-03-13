#include"menu.h"
int main(){
    int choice;
    while(1){
        static int count = 0;
        displayMainMenu();
        int choice = getChoice();
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
            funct(choice);
        }

    }
    return 0;
}