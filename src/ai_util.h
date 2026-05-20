// ai_util.h
// 共通ユーティリティ ヘッダ
#ifndef AI_UTIL_H
#define AI_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// エラーメッセージを表示する関数
void error_message(const char* message);

// 整数入力を受け付ける関数（バリデーション付き）
int input_integer(const char* prompt, int min, int max);

// SHA-256ハッシュを計算する関数
void sha256(const char* input, char* output);

#endif