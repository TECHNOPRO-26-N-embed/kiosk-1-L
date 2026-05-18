#include <stdio.h>
#include <time.h>

// 引数：商品名、商品の単価、購入した商品の個数、購入した商品の合計金額、ユーザーの支払金額、お釣り
// 購入日時と一緒にcsvファイルへ追記
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

