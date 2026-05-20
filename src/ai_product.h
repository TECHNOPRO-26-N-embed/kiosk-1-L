// ai_product.h
// 商品情報・在庫管理・商品入れ替え ヘッダ
#ifndef AI_PRODUCT_H
#define AI_PRODUCT_H

#include "ai_const.h"

// 商品構造体
typedef struct {
    int product_id;           // 商品識別子
    char product_name[64];    // 商品名
    char temperature[8];      // 温度区分（hot/cool）
    int price;                // 単価
    int stock;                // 在庫数
} Product;

// 商品CSV読込
typedef enum { LOAD_OK=0, LOAD_FILE_ERROR, LOAD_PARSE_ERROR } LoadResult;
LoadResult load_products(const char* filename, Product* products, int* count);

// 商品CSV書込
int save_products(const char* filename, Product* products, int count);

// 商品一覧表示
void show_product_list(Product* products, int count, int page);

// 在庫管理（補充）
void manage_stock(Product* products, int* count);

// 商品入れ替え
void replace_products(Product* main_products, int* main_count, const char* replace_csv, const char* log_csv);

#endif
