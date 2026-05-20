// ai_main.c
// 自動販売機システム メインエントリ
// このファイルは、プログラムのエントリーポイントであり、
// メインメニューの表示や各機能の呼び出しを制御します。

#include <stdio.h>
#include <stdlib.h>
#include "ai_const.h"
#include "ai_product.h"
#include "ai_purchase.h"
#include "ai_admin.h"
#include "ai_util.h"

// タイトルを表示する関数
// プログラムの最初に呼び出され、システムのタイトルを表示します。
void print_title() {
    printf("==============================\n");
    printf("   飲料自動販売機システム\n");
    printf("==============================\n");
}

// メイン関数
// プログラムのエントリーポイントで、メニューの選択や各機能の呼び出しを行います。
int main() {
    int running = 1; // プログラムが動作中かどうかを示すフラグ
    int menu; // メニュー番号の入力を格納する変数
    Product products[MAX_PRODUCTS]; // 商品情報を格納する配列
    int product_count = 0; // 商品数を管理する変数

    // 商品データをCSVファイルから読み込む
    load_products(AI_PRODUCTS_CSV, products, &product_count);

    // メインループ
    while (running) {
        print_title(); // タイトルを表示
        printf("1: 購入\n2: 在庫管理\n0: 終了\n"); // メニューを表示

        // メニュー番号を入力
        menu = input_integer("メニュー番号を入力してください", 0, 2);

        // メニュー番号に応じた処理を実行
        switch (menu) {
            case 1:
                // 購入処理を呼び出し
                purchase_flow(products, &product_count);
                // 購入後、商品データをCSVに保存
                save_products(AI_PRODUCTS_CSV, products, product_count);
                break;
            case 2:
                // 管理者認証を実行
                if (authenticate_admin(AI_ADMIN_CONF)) {
                    // 認証成功時に在庫管理を実行
                    manage_stock(products, &product_count);
                    // 在庫変更後、商品データをCSVに保存
                    save_products(AI_PRODUCTS_CSV, products, product_count);
                } else {
                    // 認証失敗時にエラーメッセージを表示
                    error_message("管理者認証に失敗しました。\n");
                }
                break;
            case 0:
                // プログラムを終了
                running = 0;
                printf("ご利用ありがとうございました。\n");
                break;
            default:
                // 無効なメニュー番号の場合にエラーメッセージを表示
                error_message("無効なメニュー番号です。\n");
        }
    }
    return 0; // プログラム終了
}
