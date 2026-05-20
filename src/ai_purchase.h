// ai_purchase.h
// 購入処理・購入履歴保存 ヘッダ
#ifndef AI_PURCHASE_H
#define AI_PURCHASE_H

#include "ai_product.h"

// 購入フロー（メインから呼び出し）
void purchase_flow(Product* products, int* count);

#endif
