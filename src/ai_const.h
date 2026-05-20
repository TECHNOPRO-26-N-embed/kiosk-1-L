// ai_const.h
// 定数定義ヘッダ
// 最大商品数、在庫上限、ファイル名などの定数を定義します。

#ifndef AI_CONST_H
#define AI_CONST_H

// 最大商品数
#define MAX_PRODUCTS 100

// 在庫数の上限
#define MAX_STOCK 1000

// 商品データCSVファイル名
#define PRODUCT_CSV "data/ai_products.csv"

// 商品入れ替えCSVファイル名
#define REPLACE_CSV "data/ai_replace_products.csv"

// 商品入れ替えログCSVファイル名
#define REPLACE_LOG_CSV "data/ai_replace_log.csv"

// 購入履歴CSVファイル名
#define PURCHASE_HISTORY_CSV "data/ai_purchase_history.csv"

// 管理者ハッシュファイル名
#define ADMIN_HASH_FILE "data/ai_admin.conf"

#endif // AI_CONST_H
