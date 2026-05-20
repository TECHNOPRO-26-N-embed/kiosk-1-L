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

// 設定ファイルから管理者パスワード（平文）を読み込む
// 引数: config_file - 設定ファイルのパス
//       password - 読み込んだパスワードを格納するバッファ
//       password_size - バッファのサイズ
// 戻り値: 成功=1, 失敗=0
static int load_admin_password(const char* config_file, char* password, size_t password_size) {
    FILE* fp = fopen(config_file, "r");
    if (!fp) {
        error_message("設定ファイルを開けません\n");
        return 0;
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "admin_password=", 15) == 0) {
            strncpy(password, line + 15, password_size - 1);
            password[strcspn(password, "\r\n")] = '\0';
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    error_message("設定ファイルにパスワードが見つかりません\n");
    return 0;
}

// 管理者認証（平文パスワード比較）
// 引数: config_file - 設定ファイルのパス
// 戻り値: true=認証成功, false=認証失敗
bool authenticate_admin(const char* config_file) {
    char stored_password[64];
    if (!load_admin_password(config_file, stored_password, sizeof(stored_password))) {
        return false;
    }
    char input_password[64];
    printf("管理者パスワードを入力してください: ");
    if (!fgets(input_password, sizeof(input_password), stdin)) {
        error_message("パスワード入力エラー\n");
        return false;
    }
    input_password[strcspn(input_password, "\r\n")] = '\0';
    if (strcmp(stored_password, input_password) == 0) {
        printf("認証成功\n");
        return true;
    } else {
        error_message("認証失敗\n");
        return false;
    }
}