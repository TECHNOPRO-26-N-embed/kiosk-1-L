#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "Hosoyama.h"
#define PURCHASE 0
#define RESTOCK 1

//商品の種類数
#define item_count 50


typedef struct {
    int id;
    char name[32];
    int price;
    int stock;
} Product;

int savePurchase(
    const char *productName,
    int unitPrice,
    int quantity,
    int totalPrice,
    int payment,
    int change
) {
    FILE *file;
    time_t now;

    // 年、月、日、時、分、秒などに分解された時刻情報を扱うための変数。
    // localtime関数の戻り値を受け取る。
    struct tm *t;

    // csvに書き込むための日時文字列を保存する配列
    char dateStr[20];

    // purchase_history.csv を追記モードで開く
    file = fopen("purchase_history.csv", "a");

    if (file == NULL) {
        printf("購入履歴ファイルを開けませんでした。\n");
        return -1; // ファイルオープンエラー
    }

    // 現在の日時を取得
    now = time(NULL);

    // 現在時刻をローカル時刻に変換
    t = localtime(&now);

    if (t == NULL) {
        printf("日時の取得に失敗しました。\n");
        fclose(file);
        return -1;
    }

    // 日時を文字列に変換
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", t);

    // CSVファイルに1行追加
    if (fprintf(
            file,
            "%s,%s,%d,%d,%d,%d\n",
            dateStr, // 購入日時
            productName, // 商品名
            unitPrice, // 商品の単価
            quantity, // 購入した商品の個数
            totalPrice, // 購入した商品の合計金額            
            payment, // 支払金額
            change // お釣り
        ) < 0) {

        printf("購入履歴の書き込みに失敗しました。\n");
        fclose(file);
        return -1;
    }

    // ファイルを閉じる
    fclose(file);

    printf("購入履歴を保存しました。\n");

    return 0; // 成功
}

int ProductParchase(){
//    while (1){
    //1---------------
    Product vending[item_count];
    for (int i = 0; i < item_count; i++) {
        vending[i].price = 0; //初期化
        vending[i].name[0] = '\0'; //初期化
        vending[i].stock = 0; //初期化
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
        //csvの５列目
        char* p5 = strtok(NULL,",");
        //csvの６列目
        char* p6 = strtok(NULL,",");
        //csvの７列目
        char* p7 = strtok(NULL,",");
            
        if ( p2 != NULL && p4 != NULL){
            strcpy(vending[item].name, p2);
            vending[item].price = atoi(p4);
            vending[item].stock = atoi(p5);
            item++;
        }
    }
    fclose(fp);

    //2---------------
    int choice;
    int sum = 0;

    while(1){
        printf("=== 自販機シミュレーション ===\n");
        printf("商品一覧:\n");
        for (int i = 0; i < item_count; i++) {
            if (vending[i].price != 0) {
                printf("%d. %s - %d円\n", i + 1, vending[i].name, vending[i].price);
            }
        }
        printf("購入する商品番号を入力してください (終了するには0): ");
        scanf("%d", &choice);
        if (choice == 0) {
            printf("終了します。\n");
            pay(sum);
            break;
        }
        if (choice < 1 || choice > item_count) {
            printf("無効な商品番号です。もう一度入力してください。\n");
            continue;
        }
        if (vending[choice - 1].stock > 0) {
            vending[choice - 1].stock--;
            printf("%sを購入しました。残りの在庫: %d\n", vending[choice - 1].name, vending[choice - 1].stock);
            sum += vending[choice - 1].price;
            savePurchase(vending[choice - 1].name, vending[choice - 1].price, 1, sum, 1000, 0);
        } else {
            printf("%sは在庫切れです。\n", vending[choice - 1].name);
        }
    }
    
    // pay(100,1000);
    // change(100, 1000);
    // buy_item_name(NULL, 0);
    // updateStock(NULL, 0, PURCHASE);
    // savePurchase(NULL, 0, 0, 0, 0, 0);
//}
}

int pay(int price) {
    int money;  //入力される金額を格納する変数
    printf("合計: %d円\n", price);
    printf("投入金額を入力してください。: ");
    scanf("%d", &money);

    if (money < price) {
        int shortage = price - money;
        printf("金額が不足しています。あと%d円必要です。\n", shortage);
        pay(price); // 不足している金額を再度入力させるために再帰呼び出し
    } else {
        printf("お支払いありがとうございます。\n");
        change(price, money);
    }
    
    return 0;
}

void buy_item_name(char*item_name,int num) {
    printf("後々商品名と個数を引数で受け取るように変更予定です。\n");
    printf("%sを%d個購入しました。\n", item_name, num);
    
}

void change(int price, int money) {
    if (money > price) {
        int change_amount = money - price;
        printf("お釣りは%d円です。\n", change_amount);
    } else {
        printf("ちょうどお支払いありがとうございます。\n");
    }
    
}



int updateStock(const char *productName, int quantity, int mode) {
    FILE *readFile;
    FILE *writeFile;

    //csvから1行読み込むための配列
    char line[255];

    //csvから読み取った商品名を保存する配列
    char csvProductName[100];

    //csvから読み取った商品の単価を保存する変数
    int unitPrice;

    //csvから読み取った商品の在庫数を保存する変数
    int stock;

    //変更後の在庫数を保存する変数
    int newStock;

    //products.csvを読み込みモードで開く
    readFile = fopen("products.csv", "r");

    if (readFile == NULL) {
        printf("products.csvが開けませんでした。\n");
        return -1; // ファイルオープンエラー
    }

    //products_temp.csvを新規作成して書き込みモードで開く
    writeFile = fopen("products_temp.csv", "w");

    if (writeFile == NULL) {
        printf("products_temp.csvが開けませんでした。\n");
        fclose(readFile);
        return -1; // ファイルオープンエラー
    }

    //読み込める行がある間繰り返す
    while (fgets(line, sizeof(line), readFile) != NULL) {

        //sscanfを使って、カンマ区切りの文字列を分解する
        if (sscanf(line, "%99[^,],%d,%d", csvProductName, &unitPrice, &stock) != 3) {
            printf("products.csvの形式が正しくありません。\n");
            fclose(readFile);
            fclose(writeFile);
            return -1; // ファイル形式エラー
        }

        //商品名が一致する場合、在庫数を更新する
        if (strcmp(csvProductName, productName) == 0) {
            if (mode == PURCHASE) {
                newStock = stock - quantity;
            } else if (mode == RESTOCK) {
                newStock = stock + quantity;
            } else {
                printf("無効なモードです。\n");
                fclose(readFile);
                fclose(writeFile);
                return -1; // 無効なモードエラー
            }
        } else {
            newStock = stock;
        }

        //更新後の情報を一時ファイルに書き込む
        fprintf(writeFile, "%s,%d,%d\n", csvProductName, unitPrice, newStock);
    }

    fclose(readFile);
    fclose(writeFile);

    //元のファイルを削除し、一時ファイルをリネームする
    remove("products.csv");
    rename("products_temp.csv", "products.csv");

    return 0; // 成功
}


int main() {

    
    //ありえないくらい文字化けするのでshift-jisに変更
    int result = system("chcp 932");
    if(result == -1) {
       printf("コマンドの実行に失敗しました。\n");
    }else{
       printf("コマンドが正常に実行されました。（終了コード:%d)\n", result);
    }  


    int menu_choice  = 0;
    printf("=== メニュー画面 ===\n");
    printf("1. 商品の購入\n");
    printf("2. 商品の補充\n");
    printf("3. 在庫管理\n");
    printf("終了するには0を入力してください。\n");
    scanf("%d", &menu_choice);

    if (menu_choice == 1) {
        ProductParchase();
    }else if (menu_choice == 2) {
        // 商品の補充に関する処理をここに追加
    } else if (menu_choice == 3) {
        // 在庫管理に関する処理をここに追加
    } else if (menu_choice == 0) {
        printf("終了します。\n");
    } else {
        printf("無効な選択です。もう一度入力してください。\n");
    }
}