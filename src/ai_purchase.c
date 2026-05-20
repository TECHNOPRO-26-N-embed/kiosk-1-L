// ai_purchase.c
// 購入処理・購入履歴保存 実装
// このファイルは、購入フローの実装と購入履歴の保存機能を提供します。
// 商品選択、購入個数入力、支払金額入力、履歴保存などの処理を行います。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ai_purchase.h"
#include "ai_util.h"

// 購入履歴CSVファイル名
//#define PURCHASE_HISTORY_CSV "ai_purchase_history.csv"

// 購入情報構造体
// 購入履歴を管理するためのデータ構造です。
typedef struct {
    char purchased_at[20];   // 購入日時 yyyy-mm-dd hh:mm:ss
    char product_name[64];   // 商品名
    int unit_price;          // 単価
    int quantity;            // 購入個数
    int total_price;         // 合計金額
    int paid_amount;         // 支払金額
    int change_amount;       // 釣銭
} PurchaseHistory;

// 購入履歴をCSVに追記する
// 引数: history - 購入履歴データ
// 戻り値: 1=成功, 0=失敗
int append_purchase_history(const PurchaseHistory* history) {
    FILE* fp = fopen(PURCHASE_HISTORY_CSV, "a"); // CSVファイルを追記モードで開く
    if (!fp) {
        error_message("購入履歴ファイル書込失敗\n");
        return 0; // ファイルオープン失敗
    }

    // 購入履歴データをCSV形式で出力
    fprintf(fp, "%s,%s,%d,%d,%d,%d,%d\n",
        history->purchased_at,
        history->product_name,
        history->unit_price,
        history->quantity,
        history->total_price,
        history->paid_amount,
        history->change_amount);

    fclose(fp); // ファイルを閉じる
    return 1; // 正常終了
}

// 購入フロー（商品選択→個数→継続確認→一括会計→履歴保存）
// 複数商品を選択してから最後にまとめて会計します。
void purchase_flow(Product* products, int* count) {
    printf("\n--- 購入処理 ---\n");
    int page = 1; // 現在のページ番号
    int total_pages = (*count + 9) / 10; // 総ページ数（1ページ10件）
    int selected_qty[MAX_PRODUCTS] = {0}; // 商品ごとの選択個数
    int quantity = 0; // 直近で入力した購入個数
    int paid = 0; // 支払金額
    int total = 0; // 合計金額
    int change = 0; // 釣銭
    int continue_buy = 1; // 購入継続フラグ
    char cmd[8]; // 入力コマンド

    // 商品選択ループ
    while (continue_buy) {
        show_product_list(products, *count, page); // 商品一覧を表示
        printf("商品番号を入力 (n:次ページ p:前ページ q:戻る): ");
        fgets(cmd, sizeof(cmd), stdin); // 商品番号を入力
        if (cmd[0] == 'q') {
            if (total == 0) {
                return; // 何も選んでいなければ購入画面を終了
            }
            break; // 1件以上選んでいれば会計へ進む
        }
        if (cmd[0] == 'n') {
            if (page < total_pages) {
                page++;
            } else {
                error_message("これ以上次のページはありません\n");
            }
            continue;
        }
        if (cmd[0] == 'p') {
            if (page > 1) {
                page--;
            } else {
                error_message("これ以上前のページはありません\n");
            }
            continue;
        }

        int id = atoi(cmd); // 入力を整数に変換
        if (id <= 0) {
            error_message("商品番号が不正です\n");
            continue;
        }

        // 商品番号チェック
        int idx = -1;
        for (int i = 0; i < *count; i++) {
            if (products[i].product_id == id) idx = i;
        }
        if (idx == -1) {
            error_message("商品番号が不正です\n");
            continue; // 無効な商品番号の場合
        }
        if (products[idx].price == 0) {
            error_message("0円の商品は購入できません\n");
            continue; // 0円商品は購入不可
        }
        if (products[idx].stock - selected_qty[idx] == 0) {
            error_message("売切です\n");
            continue; // 在庫がない場合
        }

        // 購入個数入力（すでに選んだ個数を差し引いた残在庫内）
        quantity = input_integer("購入個数を入力", 1, products[idx].stock - selected_qty[idx]);
        selected_qty[idx] += quantity;
        total += products[idx].price * quantity;
        printf("現在の合計金額: %d円\n", total);

        // さらに商品を選ぶか確認
        continue_buy = input_integer("購入を続けますか? (1:はい 0:いいえ)", 0, 1);
    }

    if (total == 0) {
        return;
    }

    // 合計金額表示
    printf("合計金額: %d円\n", total);

    // 支払金額入力
    paid = input_integer("支払金額を入力", total, 100000);

    // 釣銭計算
    change = paid - total;

    // 購入日時は会計単位で共通化する
    time_t now = time(NULL); // 現在時刻を取得
    struct tm* t = localtime(&now);
    char purchased_at[20];
    snprintf(purchased_at, sizeof(purchased_at), "%04d-%02d-%02d %02d:%02d:%02d",
        t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    // 選択された商品ごとに在庫を更新し、履歴を追記
    for (int i = 0; i < *count; i++) {
        if (selected_qty[i] > 0) {
            products[i].stock -= selected_qty[i];

            PurchaseHistory history;
            strncpy(history.purchased_at, purchased_at, sizeof(history.purchased_at) - 1);
            history.purchased_at[sizeof(history.purchased_at) - 1] = '\0';
            strncpy(history.product_name, products[i].product_name, sizeof(history.product_name) - 1);
            history.product_name[sizeof(history.product_name) - 1] = '\0';
            history.unit_price = products[i].price;
            history.quantity = selected_qty[i];
            history.total_price = products[i].price * selected_qty[i];
            history.paid_amount = paid;
            history.change_amount = change;
            append_purchase_history(&history);
        }
    }

    // 結果表示
    printf("\n--- 購入完了 ---\n");
    printf("購入内容:\n");
    for (int i = 0; i < *count; i++) {
        if (selected_qty[i] > 0) {
            printf("- %s x %d\n", products[i].product_name, selected_qty[i]);
        }
    }
    printf("合計: %d円\n支払: %d円\n釣銭: %d円\n", total, paid, change);
    printf("ありがとうございました！\n");
}
