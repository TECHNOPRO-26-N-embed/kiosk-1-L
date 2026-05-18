#include <stdio.h>
#include <locale.h>

typedef struct {
    int id;
    char name[32];
    int price;
    int stock;
} Product;

int main(void) {
    
    int choice;
    Product products[] = {
		{1, "水", 120, 5},
		{2, "お茶", 140, 5},
		{3, "コーヒー", 150, 5},
		{4, "ジュース", 160, 5},
        {5, "エナジードリンク", 200, 5},
        {6, "コーラ", 130, 5},
        {7, "レモネード", 150, 5},
	};

    while (1){
        printf("=== 自販機シミュレーション ===\n");
        printf("商品一覧:\n");
        for (int i = 0; i < 7; i++) {
            printf("%d. %s - %d円 (在庫: %d)\n", products[i].id, products[i].name, products[i].price, products[i].stock);
        }
        printf("購入する商品番号を入力してください (終了するには0): ");
        scanf("%d", &choice);
        if (choice == 0) {
            printf("終了します。\n");
            break;
        }
        if (choice < 1 || choice > 7) {
            printf("無効な商品番号です。もう一度入力してください。\n");
            continue;
        }
        if (products[choice - 1].stock > 0) {
            products[choice - 1].stock--;
            printf("%sを購入しました。残りの在庫: %d\n", products[choice - 1].name, products[choice - 1].stock);
        } else {
            printf("%sは在庫切れです。\n", products[choice - 1].name);
        }
    }
    
    return 0;
}