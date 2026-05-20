// ai_product.c
// 商品情報・在庫管理・商品入れ替え 実装
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai_product.h"
#include "ai_util.h"

// 商品CSV読込
// 指定CSVファイルから商品情報を読み込み、構造体配列に格納する
LoadResult load_products(const char* filename, Product* products, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return LOAD_FILE_ERROR; // ファイルオープン失敗
    char line[256];
    int idx = 0;
    // 1行目はヘッダ行なのでスキップ
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (idx >= MAX_PRODUCTS) break; // 上限超過防止
        char* token;
        // 商品ID
        token = strtok(line, ",");
        if (!token) continue;
        products[idx].product_id = atoi(token);
        // 商品名
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(products[idx].product_name, token, sizeof(products[idx].product_name)-1);
        // 温度区分
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(products[idx].temperature, token, sizeof(products[idx].temperature)-1);
        // 単価
        token = strtok(NULL, ",");
        if (!token) continue;
        products[idx].price = atoi(token);
        // 在庫数
        token = strtok(NULL, ",\r\n");
        if (!token) continue;
        products[idx].stock = atoi(token);
        idx++;
    }
    fclose(fp);
    *count = idx;
    return LOAD_OK;
}

// 商品CSV書込
// 構造体配列の内容をCSVファイルに保存する
int save_products(const char* filename, Product* products, int count) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return 0; // ファイルオープン失敗
    // ヘッダ行出力
    fprintf(fp, "product_id,product_name,temperature,price,stock\n");
    // 各商品データを1行ずつ出力
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d\n", products[i].product_id, products[i].product_name, products[i].temperature, products[i].price, products[i].stock);
    }
    fclose(fp);
    return 1;
}

// 商品一覧表示（ページング対応）
// 指定ページの商品を10件ずつ一覧表示する。売切/在庫有も表示。
void show_product_list(Product* products, int count, int page) {
    int per_page = 10;
    int start = (page-1) * per_page;
    int end = start + per_page;
    if (start >= count) {
        printf("商品がありません。\n");
        return;
    }
    printf("\n--- 商品一覧 (ページ%d) ---\n", page);
    printf("番号  商品名           温度   価格  在庫\n");
    // 商品ごとに表示
    for (int i = start; i < end && i < count; i++) {
        printf("%2d  %-14s %-5s %4d  %s\n", products[i].product_id, products[i].product_name, products[i].temperature, products[i].price, products[i].stock == 0 ? "売切" : "在庫有");
    }
    // ページ遷移案内
    if (end < count) printf("n:次ページ  ");
    if (page > 1) printf("p:前ページ  ");
    printf("q:戻る\n");
}

// 在庫管理（補充）
// 管理者が商品を選択し、補充数を入力して在庫を加算する
void manage_stock(Product* products, int* count) {
    printf("\n--- 在庫管理 ---\n");
    show_product_list(products, *count, 1);
    // 商品番号入力
    int id = input_integer("補充する商品番号を入力", 1, *count);
    int idx = -1;
    // 入力番号に該当する商品を検索
    for (int i = 0; i < *count; i++) {
        if (products[i].product_id == id) idx = i;
    }
    if (idx == -1) {
        error_message("商品番号が不正です。\n");
        return;
    }
    // 補充数入力（在庫上限を超えない範囲）
    int add = input_integer("補充数を入力(1-50)", 1, 50-products[idx].stock);
    if (products[idx].stock + add > 50) {
        error_message("在庫上限を超えます。\n");
        return;
    }
    // 在庫加算
    products[idx].stock += add;
    printf("%sの在庫を%d個補充しました。\n", products[idx].product_name, add);
}

// 商品入れ替え（設計例のみ、詳細実装は省略）
// main_products: メイン商品配列, main_count: 商品数, replace_csv: 追加CSV, log_csv: ログファイル
void replace_products(Product* main_products, int* main_count, const char* replace_csv, const char* log_csv) {
    // ここに商品入れ替え処理を実装
    // 設計書のアルゴリズムに従い、
    // 1. 追加CSV読込
    // 2. 入れ替え対象商品番号で内容を入れ替え
    // 3. ログ記録
    // 4. 上限超過チェック
    // ...
}
