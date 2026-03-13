#include "funct.h"
#include "menu.h"
#include "card_service.h"
#include "card_file.h"
void funct(int choice) {
    switch (choice)
    {
    case 1:
        addCard();
        break;
    case 2:
        findCard();
        break;
    default:
        break;
    }
}