// ai_admin.h
// 管理者認証 ヘッダ
#ifndef AI_ADMIN_H
#define AI_ADMIN_H

#include <stdbool.h>

// 管理者認証（平文パスワード比較）
// 戻り値: true=認証成功, false=認証失敗
bool authenticate_admin(const char* config_file);

#endif