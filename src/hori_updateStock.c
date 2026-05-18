#include <stdio.h>
#include <string.h>

#define PURCHASE 0
#define RESTOCK 1

//csvファイルの形式：商品名,単価,在庫数
/* 引数:
 * productName : 在庫を変更したい商品名
 * quantity    : 変更する個数
 * mode        : PURCHASEなら購入、RESTOCKなら補充
 */

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
