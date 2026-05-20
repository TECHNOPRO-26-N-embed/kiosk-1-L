// ai_product.c
// 商品情報・在庫管理・商品入れ替え 実装
// このファイルは、商品データの読み込み、保存、表示、在庫管理、
// 商品入れ替えなどの機能を提供します。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ai_product.h"
#include "ai_util.h"

// 商品IDから配列インデックスを検索
static int find_product_index_by_id(Product* products, int count, int product_id) {
    for (int i = 0; i < count; i++) {
        if (products[i].product_id == product_id) {
            return i;
        }
    }
    return -1;
}

// 商品CSV読込
// 指定されたCSVファイルから商品情報を読み込み、構造体配列に格納します。
// 戻り値: 読み込み結果（成功、ファイルエラー、パースエラー）
LoadResult load_products(const char* filename, Product* products, int* count) {
    FILE* fp = fopen(filename, "r"); // ファイルを読み取りモードで開く
    if (!fp) return LOAD_FILE_ERROR; // ファイルが開けなかった場合のエラー処理

    char line[256]; // 1行分のデータを格納するバッファ
    int idx = 0; // 商品数のカウンタ

    // 1行目はヘッダ行なのでスキップ
    fgets(line, sizeof(line), fp);

    // ファイルの各行を読み込む
    while (fgets(line, sizeof(line), fp)) {
        if (idx >= MAX_PRODUCTS) break; // 商品数が上限を超えた場合は終了

        char* token;
        // 商品IDを取得
        token = strtok(line, ",");
        if (!token) continue;
        products[idx].product_id = atoi(token);

        // 商品名を取得
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(products[idx].product_name, token, sizeof(products[idx].product_name)-1);

        // 温度区分を取得
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(products[idx].temperature, token, sizeof(products[idx].temperature)-1);

        // 単価を取得
        token = strtok(NULL, ",");
        if (!token) continue;
        products[idx].price = atoi(token);

        // 在庫数を取得
        token = strtok(NULL, ",\r\n");
        if (!token) continue;
        products[idx].stock = atoi(token);

        idx++; // 商品数をインクリメント
    }

    fclose(fp); // ファイルを閉じる
    *count = idx; // 読み込んだ商品数を設定
    return LOAD_OK; // 正常終了
}

// 商品CSV書込
// 構造体配列の内容を指定されたCSVファイルに保存します。
// 戻り値: 成功=1, 失敗=0
int save_products(const char* filename, Product* products, int count) {
    FILE* fp = fopen(filename, "w"); // ファイルを上書きモードで開く
    if (!fp) return 0; // ファイルが開けなかった場合のエラー処理

    // ヘッダ行を出力
    fprintf(fp, "product_id,product_name,temperature,price,stock\n");

    // 各商品データを出力
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d\n",
            products[i].product_id,
            products[i].product_name,
            products[i].temperature,
            products[i].price,
            products[i].stock);
    }

    fclose(fp); // ファイルを閉じる
    return 1; // 正常終了
}

// 商品一覧表示（ページング対応）
// 商品情報を10件ずつ表示します。ページ番号を指定可能です。
void show_product_list(Product* products, int count, int page) {
    int per_page = 10; // 1ページあたりの表示件数
    int start = (page-1) * per_page; // 表示開始インデックス
    int end = start + per_page; // 表示終了インデックス

    if (start >= count) {
        printf("商品がありません。\n");
        return;
    }

    printf("\n--- 商品一覧 (ページ%d) ---\n", page);
    printf("番号  商品名           温度   価格  在庫\n");

    // 商品情報を表示
    for (int i = start; i < end && i < count; i++) {
        printf("%2d  %-14s %-5s %4d  %d\n",
            products[i].product_id,
            products[i].product_name,
            products[i].temperature,
            products[i].price,
            products[i].stock);
    }

    // ページ遷移案内を表示
    if (end < count) printf("n:次ページ  ");
    if (page > 1) printf("p:前ページ  ");
    printf("q:戻る\n");
}

// 在庫管理（補充）
// 管理者が商品を選択し、補充数を入力して在庫を加算します。
void manage_stock(Product* products, int* count) {
    while (1) {
        printf("\n--- 在庫管理 ---\n");
        printf("1: 在庫補充\n2: 商品入れ替え\n0: 戻る\n");
        int menu = input_integer("操作を選択", 0, 2);

        if (menu == 0) {
            return;
        }

        if (menu == 1) {
            printf("\n--- 商品一覧 ---\n");
            for (int i = 0; i < *count; i++) {
                printf("%2d  %-14s %-5s %4d  在庫: %d\n",
                    products[i].product_id,
                    products[i].product_name,
                    products[i].temperature,
                    products[i].price,
                    products[i].stock);
            }

            // 補充する商品番号を入力
            int id = input_integer("補充する商品IDを入力", 1, 50);
            int idx = find_product_index_by_id(products, *count, id);

            if (idx == -1) {
                error_message("商品IDが不正です。\n");
                continue;
            }

            if (products[idx].stock >= 50) {
                error_message("この商品はすでに在庫上限です。\n");
                continue;
            }

            // 補充数を入力（在庫上限を超えない範囲）
            int max_add = 50 - products[idx].stock;
            printf("補充数を入力(1-%d): ", max_add);
            int add = input_integer("", 1, max_add);
            products[idx].stock += add;
            printf("%sの在庫を%d個補充しました。\n", products[idx].product_name, add);
            continue;
        }

        // menu == 2
        replace_products(products, count, REPLACE_CSV, REPLACE_LOG_CSV);
    }
}

// 商品入れ替え
// main_products: メイン商品配列, main_count: 商品数, replace_csv: 追加CSV, log_csv: ログファイル
void replace_products(Product* main_products, int* main_count, const char* replace_csv, const char* log_csv) {
    Product replace_products_list[MAX_PRODUCTS];
    int replace_count = 0;

    if (load_products(replace_csv, replace_products_list, &replace_count) != LOAD_OK) {
        error_message("入れ替え商品CSVの読み込みに失敗しました。\n");
        return;
    }
    if (*main_count <= 0 || replace_count <= 0) {
        error_message("商品データが不足しているため入れ替えできません。\n");
        return;
    }

    printf("\n--- メイン商品一覧 ---\n");
    for (int i = 0; i < *main_count; i++) {
        printf("%2d  %-14s %-5s %4d  %s\n",
            main_products[i].product_id,
            main_products[i].product_name,
            main_products[i].temperature,
            main_products[i].price,
            main_products[i].stock == 0 ? "売切" : "在庫有");
    }

    printf("\n--- 入れ替え候補一覧 ---\n");
    for (int i = 0; i < replace_count; i++) {
        printf("%2d  %-14s %-5s %4d  %s\n",
            replace_products_list[i].product_id,
            replace_products_list[i].product_name,
            replace_products_list[i].temperature,
            replace_products_list[i].price,
            replace_products_list[i].stock == 0 ? "売切" : "在庫有");
    }

    int main_id = input_integer("入れ替え対象のメイン商品IDを入力", 1, 9999);
    int replace_id = input_integer("入れ替え候補の商品IDを入力", 1, 9999);

    int main_idx = find_product_index_by_id(main_products, *main_count, main_id);
    int replace_idx = find_product_index_by_id(replace_products_list, replace_count, replace_id);

    if (main_idx == -1 || replace_idx == -1) {
        error_message("指定した商品IDが見つかりません。\n");
        return;
    }

    Product old_main = main_products[main_idx];
    Product old_replace = replace_products_list[replace_idx];

    // 商品IDは各CSV側で維持し、商品内容のみ入れ替える
    strncpy(main_products[main_idx].product_name, old_replace.product_name, sizeof(main_products[main_idx].product_name) - 1);
    main_products[main_idx].product_name[sizeof(main_products[main_idx].product_name) - 1] = '\0';
    strncpy(main_products[main_idx].temperature, old_replace.temperature, sizeof(main_products[main_idx].temperature) - 1);
    main_products[main_idx].temperature[sizeof(main_products[main_idx].temperature) - 1] = '\0';
    main_products[main_idx].price = old_replace.price;
    main_products[main_idx].stock = old_replace.stock;

    strncpy(replace_products_list[replace_idx].product_name, old_main.product_name, sizeof(replace_products_list[replace_idx].product_name) - 1);
    replace_products_list[replace_idx].product_name[sizeof(replace_products_list[replace_idx].product_name) - 1] = '\0';
    strncpy(replace_products_list[replace_idx].temperature, old_main.temperature, sizeof(replace_products_list[replace_idx].temperature) - 1);
    replace_products_list[replace_idx].temperature[sizeof(replace_products_list[replace_idx].temperature) - 1] = '\0';
    replace_products_list[replace_idx].price = old_main.price;
    replace_products_list[replace_idx].stock = old_main.stock;

    // 入れ替え候補CSVを保存（失敗時はメイン側を戻す）
    if (!save_products(replace_csv, replace_products_list, replace_count)) {
        main_products[main_idx] = old_main;
        error_message("入れ替え商品CSVの保存に失敗しました。\n");
        return;
    }

    // 入れ替えログを追記
    FILE* log_fp = fopen(log_csv, "a");
    if (log_fp) {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char replaced_at[20];
        snprintf(replaced_at, sizeof(replaced_at), "%04d-%02d-%02d %02d:%02d:%02d",
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

        fprintf(log_fp, "%s,%d,%s,%s,%s,%d,%d\n",
                replaced_at,
                old_main.product_id,
                old_main.product_name,
                main_products[main_idx].product_name,
                main_products[main_idx].temperature,
                main_products[main_idx].price,
                main_products[main_idx].stock);
        fclose(log_fp);
    } else {
        error_message("入れ替えログファイルを開けませんでした。\n");
    }

    printf("商品を入れ替えました: %s -> %s\n",
           old_main.product_name,
           main_products[main_idx].product_name);
}
