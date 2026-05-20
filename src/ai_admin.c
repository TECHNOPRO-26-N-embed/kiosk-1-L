// ai_admin.c
// 管理者認証 実装
// このファイルは、管理者認証機能を提供します。
// 設定ファイルからパスワードハッシュを読み込み、
// 入力されたパスワードと比較して認証を行います。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai_admin.h"
#include "ai_util.h"

// 設定ファイルから管理者パスワードハッシュを読み込む
// 引数: config_file - 設定ファイルのパス
//       hash - 読み込んだハッシュ値を格納するバッファ
//       hash_size - バッファのサイズ
// 戻り値: 成功=1, 失敗=0
static int load_admin_hash(const char* config_file, char* hash, size_t hash_size) {
    FILE* fp = fopen(config_file, "r"); // 設定ファイルを読み取りモードで開く
    if (!fp) {
        error_message("設定ファイルを開けません\n");
        return 0; // ファイルオープン失敗
    }

    char line[256]; // 1行分のデータを格納するバッファ

    // ファイルの各行を読み込む
    while (fgets(line, sizeof(line), fp)) {
        // ハッシュ値が含まれる行を検索
        if (strncmp(line, "admin_password_hash=", 20) == 0) {
            strncpy(hash, line + 20, hash_size - 1); // ハッシュ値をコピー
            hash[strcspn(hash, "\r\n")] = '\0'; // 改行を削除
            fclose(fp); // ファイルを閉じる
            return 1; // 正常終了
        }
    }

    fclose(fp); // ファイルを閉じる
    error_message("設定ファイルにハッシュが見つかりません\n");
    return 0; // ハッシュ値が見つからなかった場合
}

// 管理者認証
// 引数: config_file - 設定ファイルのパス
// 戻り値: true=認証成功, false=認証失敗
bool authenticate_admin(const char* config_file) {
    char stored_hash[65]; // 設定ファイルから読み込むハッシュ値（SHA-256は64文字)

    // 設定ファイルからハッシュ値を読み込む
    if (!load_admin_hash(config_file, stored_hash, sizeof(stored_hash))) {
        return false; // ハッシュ値の読み込み失敗
    }

    char input_password[64]; // 入力されたパスワードを格納するバッファ
    printf("管理者パスワードを入力してください: ");

    // パスワードを入力
    if (!fgets(input_password, sizeof(input_password), stdin)) {
        error_message("パスワード入力エラー\n");
        return false; // 入力エラー
    }
    input_password[strcspn(input_password, "\r\n")] = '\0'; // 改行を削除

    char input_hash[65]; // 入力されたパスワードのハッシュ値を格納するバッファ
    sha256(input_password, input_hash); // 入力パスワードをハッシュ化

    // 入力ハッシュと設定ファイルのハッシュを比較
    if (strcmp(stored_hash, input_hash) == 0) {
        printf("認証成功\n");
        return true; // 認証成功
    } else {
        error_message("認証失敗\n");
        return false; // 認証失敗
    }
}