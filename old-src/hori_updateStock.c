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

/*
 * ここから下は動作確認用に追加したコード
 */

//動作確認用のproducts.csvを作成する関数
void createTestCsv(void) {
    FILE *file;

    file = fopen("products.csv", "w");

    if (file == NULL) {
        printf("動作確認用のproducts.csvを作成できませんでした。\n");
        return;
    }

    fprintf(file, "コーラ,120,10\n");
    fprintf(file, "お茶,100,20\n");
    fprintf(file, "水,80,30\n");

    fclose(file);
}

//products.csvの中身を表示する関数
void showCsv(void) {
    FILE *file;
    char line[255];

    file = fopen("products.csv", "r");

    if (file == NULL) {
        printf("products.csvが開けませんでした。\n");
        return;
    }

    printf("現在のproducts.csvの中身\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    printf("\n");

    fclose(file);
}

//動作確認用のmain関数
int main(void) {
    int result;

    printf("=== 動作確認開始 ===\n\n");

    printf("【準備】動作確認用のproducts.csvを作成\n");
    createTestCsv();
    showCsv();

    printf("【確認1】コーラを3個購入\n");
    result = updateStock("コーラ", 3, PURCHASE);
    printf("戻り値: %d\n", result);
    showCsv();

    printf("【確認2】お茶を5個補充\n");
    result = updateStock("お茶", 5, RESTOCK);
    printf("戻り値: %d\n", result);
    showCsv();

    printf("【確認3】存在しない商品名「ジュース」を購入\n");
    result = updateStock("ジュース", 2, PURCHASE);
    printf("戻り値: %d\n", result);
    showCsv();

    printf("=== 動作確認終了 ===\n");

    return 0;
}