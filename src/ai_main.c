// ai_main.c
// 自動販売機システム メインエントリ
// 参照: 要件定義・基本設計・詳細設計
// ビルド: gcc -finput-charset=utf-8 -fexec-charset=cp932 ai_main.c ai_product.c ai_purchase.c ai_admin.c ai_util.c -o app.exe

#include <stdio.h>
#include <stdlib.h>
#include "ai_const.h"
#include "ai_product.h"
#include "ai_purchase.h"
#include "ai_admin.h"
#include "ai_util.h"

void print_title() {
    printf("==============================\n");
    printf("   飲料自動販売機システム\n");
    printf("==============================\n");
}

int main() {
    int running = 1;
    int menu;
    Product products[MAX_PRODUCTS];
    int product_count = 0;
    load_products(AI_PRODUCTS_CSV, products, &product_count);

    while (running) {
        print_title();
        printf("1: 購入\n2: 在庫管理\n0: 終了\n");
        menu = input_integer("メニュー番号を入力してください", 0, 2);
        switch (menu) {
            case 1:
                purchase_flow(products, &product_count);
                save_products(AI_PRODUCTS_CSV, products, product_count);
                break;
            case 2:
                if (authenticate_admin(AI_ADMIN_CONF)) {
                    manage_stock(products, &product_count);
                    save_products(AI_PRODUCTS_CSV, products, product_count);
                } else {
                    error_message("管理者認証に失敗しました。\n");
                }
                break;
            case 0:
                running = 0;
                printf("ご利用ありがとうございました。\n");
                break;
            default:
                error_message("無効なメニュー番号です。\n");
        }
    }
    return 0;
}
