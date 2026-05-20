// ai_util.c
// 共通ユーティリティ 実装
// このファイルは、エラーメッセージ表示、整数入力、SHA-256ハッシュ計算などの
// 共通機能を提供します。

#include "ai_util.h"


// エラーメッセージを表示する関数
// 引数: message - 表示するエラーメッセージ
void error_message(const char* message) {
    fprintf(stderr, "[エラー] %s", message); // 標準エラー出力にメッセージを表示
}

// 整数入力を受け付ける関数（バリデーション付き）
// 引数: prompt - 入力プロンプト
//       min - 入力可能な最小値
//       max - 入力可能な最大値
// 戻り値: 入力された整数
int input_integer(const char* prompt, int min, int max) {
    int value;
    char buffer[64];
    while (1) {
        printf("%s (%d-%d): ", prompt, min, max); // 入力プロンプトを表示
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            error_message("入力エラー\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            error_message("数値を入力してください\n");
            continue;
        }
        if (value < min || value > max) {
            error_message("範囲外の値です\n");
            continue;
        }
        return value; // 正常な値を返す
    }
}

// SHA-256ハッシュを計算する関数
// 引数: input - 入力文字列
//       output - 計算されたハッシュ値（16進数文字列）
// void sha256(const char* input, char* output) {
//     unsigned char hash[SHA256_DIGEST_LENGTH]; // ハッシュ値を格納するバッファ
//     SHA256((unsigned char*)input, strlen(input), hash); // SHA-256を計算

//     // ハッシュ値を16進数文字列に変換
//     for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//         sprintf(output + (i * 2), "%02x", hash[i]);
//     }
//     output[SHA256_DIGEST_LENGTH * 2] = '\0'; // 終端文字を追加
// }