#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//商品の種類数
#define item_count 50

// 自動販売機の構造体（商品名と金額のみ）
typedef struct {
    char name[item_count];
    int price;
} vending_machine;


int MenuChoice() {

    //ありえないくらい文字化けするのでshift-jisに変更
    //int result = system("chcp 932");
    //if(result == -1) {
    //    printf("コマンドの実行に失敗しました。\n");
    //}else{
    //    printf("コマンドが正常に実行されました。（終了コード:%d)\n", result);
    //}  

    vending_machine vending[item_count];
    for (int i = 0; i < item_count; i++) {
        vending[i].price = 0; //初期化
        vending[i].name[0] = '\0'; //初期化
    }
    
    int item = 0;
    
    //csvファイルを開く
    FILE *fp = fopen("ktest.csv", "r");
    if (fp == NULL) {
        printf("ファイルが開けませんでした。.\n");
        return 1;
    }
    
    //csvファイルから必要な値を読み取るための仮配列
    char tmp[255];

    //csvファイルから商品名と金額を読み取る
    while (fgets(tmp, sizeof(tmp), fp) != NULL && item < item_count){
        //csvの1列目
        //最初の文字列にはchar型の文字列が必要なのでtmpを渡す
        char* p1 = strtok(tmp,",");
        //csvの2列目
        //2列目以降はNULLを入れるのでNULLを挿入(これでtmpの次の文字列を取得できる)
        char* p2 = strtok(NULL,",");
        //csvの3列目
        char* p3 = strtok(NULL,",");
        //csvの4列目
        char* p4 = strtok(NULL,",");
        
        if ( p2 != NULL && p4 != NULL){
            strcpy(vending[item].name, p2);
            vending[item].price = atoi(p4);
            item++;
        }
    }
    fclose(fp);
    for ( int a = 0; a < sizeof(vending)/sizeof(vending[0]); a++){
        if( vending[a].price != 0){
            printf("%s %d円 %d番\n", vending[a].name, vending[a].price, a+1);
        }
    }
    return 0;
}