あなたはシニアCエンジニアです。
目的は、CUIの自動販売機システムをC言語で実装し、要件を100%満たすことです。
以下の制約、仕様、受け入れ基準を厳守し、実行可能な完全成果物を作成してください。

【最重要指示】

参照ドキュメントの要件を省略・改変せず実装すること。
実装だけでなく、ビルド方法、実行方法、初期CSV、設定ファイル、テスト観点まで揃えること。
曖昧点は勝手に省略せず、合理的なデフォルト値を採用してコード内で一貫させること。
出力は「完成コード一式」と「ファイルごとの全文」。差分ではなく、最終形を提示すること。
【実装対象】

C言語のコンソールアプリ
飲料専用自動販売機
利用者機能: 購入
管理者機能: 在庫管理、商品入れ替え
データ永続化: CSVファイル
管理者認証: SHA-256ハッシュ照合、ハッシュは設定ファイル読み込み
【必須機能】

メインメニュー
1: 購入
2: 在庫管理（認証必須）
3: 商品入れ替え（認証必須）
0: 終了
商品一覧表示
最大50件
1ページ10件のページング
次へ、前へ移動
在庫0は売切表示
購入処理
商品選択
個数入力
合計金額計算（単価×個数）
支払金額入力
支払不足は再入力
釣銭計算
在庫減算
購入結果表示
購入履歴CSVへ追記
在庫管理
管理者認証成功時のみ
商品選択、補充数入力
在庫上限50超過はエラー
妥当時はCSV更新
商品入れ替え
products.csv と replace_products.csv の指定商品番号を入れ替え
複数番号の一括入れ替え対応
存在しない商品番号はエラー記録し、他の番号は処理継続
操作ログを replace_log.csv に記録
管理者認証
入力パスワードをSHA-256でハッシュ化
admin.conf の admin_password_hash と比較
一致で成功、不一致で失敗
設定ファイル読込失敗時は管理機能を利用不可として安全に戻る
【データ仕様】

products.csv
product_id, product_name, temperature, price, stock
product_idは1以上、重複不可
temperatureはhotまたはcool
priceは1以上
stockは0以上50以下
purchase_history.csv
purchased_at, product_name, unit_price, quantity, total_price, paid_amount, change_amount
purchased_atはyyyy-mm-dd hh:mm:ss形式
total_priceはunit_price×quantity
change_amountはpaid_amount-total_price
replace_products.csv
products.csvと同等スキーマを想定し、入れ替え対象データを保持
replace_log.csv
実行日時、対象商品番号、結果、理由を記録
admin.conf
admin_password_hash=（64桁hex）
【入力バリデーションとエラー方針】

数値項目は整数のみ
範囲外入力は再入力
在庫不足、支払不足は当該画面で再入力
ファイルオープン失敗、書込失敗はユーザーに明示
異常時もアプリを落とさず、メニュー復帰可能にする
【実装上の必須条件】

関数を分割実装し、少なくとも以下の責務を持つこと
authenticate_admin
calc_change
calc_total_price
input_integer
load_products
save_products
append_purchase_history
show_main_menu
show_product_list
select_product
input_quantity
input_payment
show_purchase_result
manage_stock
replace_products
error_message
文字コードやWindows環境を考慮し、ビルド可能なコードにすること。
1ファイル実装でも複数ファイル実装でもよいが、可読性と保守性を確保すること。
実在するソフトドリンク名データを初期CSV例に含めること。
【出力フォーマット指定】
以下の順で必ず出力してください。

生成ファイル一覧
各ファイルの全文
ビルドコマンド
実行手順
動作確認チェックリスト（正常系、異常系、境界値）
【受け入れ基準】

全メニューが動作する
CSV読込、更新、追記が正しく行える
管理者認証がハッシュ照合で動作する
ページング表示が動作する
商品入れ替えとログ記録が動作する
不正入力でクラッシュしない
在庫上限50、商品件数上限50を守る
不足がある場合は出力前に自己検証し、要件未達を残さないこと。
最終成果物のみ提示してください。説明のための長文は不要です。