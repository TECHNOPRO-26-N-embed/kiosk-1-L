#include <stdio.h>
#include <string.h>

//商品の種類数
#define item_count 50

// 自動販売機の構造体（商品名と金額のみ）
typedef struct {
    char name[50];
    int price;
} vending_machine;


int main() {

    vending_machine vending[item_count];
    int i = 0;
    
    //csvファイルを開く
    FILE *fp = fopen("a.csv", "r");
    if (fp == NULL) {
        printf("ファイルが開けませんでした。.\n");
        return 1;
    }
    
    //csvファイルから必要な値を読み取るための仮配列
    char tmp[255];

    while (fgets(tmp, sizeof(tmp), fp) != NULL && i < item_count){
        char *token = strtok(tmp, ",");
        if (token != NULL) {
            strncpy(vending[i].name, token, sizeof(vending[i].name) - 1);
            vending[i].name[sizeof(vending[i].name) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            vending[i].price = atoi(token);
        }
        i++;
    }

    
    return 0;
}