#include <stdio.h>
#include "Hosoyama.h"

/// @brief 支払い金額を計算する関数
/// @param price 商品の価格
/// @param money 支払った金額
/// @param pay_money 実際に支払う金額
/// @return 不足している金額（支払った金額が足りている場合は0）
int pay(int price, int money) {
    printf("合計: %d円\n", price);
    scanf("投入金額を入力してください。: %d", &money);
    if (money < price) {
        int shortage = price - money;
        printf("金額が不足しています。あと%d円必要です。\n", shortage);
        pay(price, money); // 不足している金額を再度入力させるために再帰呼び出し
    } else {
        printf("お支払いありがとうございます。\n");
    }
    
    return 0;
}

/// @brief 購入した商品名と個数を表示する関数
/// @brief (後々は引数で受け取るように変更予定です。)
/// @param item_name 商品名
/// @param num 個数
/// @return なし
void buy_item_name(char*item_name,int num) {
    printf("後々商品名と個数を引数で受け取るように変更予定です。\n");
    printf("%sを%d個購入しました。\n", item_name, num);
    
}

void change(int price, int money) {
    if (money > price) {
        int change_amount = money - price;
        printf("お釣りは%d円です。\n", change_amount);
    } else {
        printf("ちょうどお支払いありがとうございます。\n");
    }
    
}