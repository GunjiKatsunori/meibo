#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "Header.h"

Machine	dynabook = {"192.168.10.103", "dyna", "diemeistersinger"};
Machine	macBookAir = {"192.168.10.104", "mac", "diemeistersinger"};
Machine	local = {"localhost", "root", "diemeistersinger"};

char *gender[2] = {"男", "女"};

/* usersテーブルから全データを取得して表示 */
void show_users() {
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	int users_exist = 0;
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}
	
	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n");
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// usersテーブルからデータを取得
	sprintf(sql_st, "SELECT * FROM users, address WHERE users.id = address.id");
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n"); 
		fprintf(fp, "%s ユーザーデータの取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}

	// 表示
	resp = mysql_use_result(mysql);
	while((row = mysql_fetch_row(resp)) != NULL ){
		printf( "ID: %s 名前: %s 住所: %s 生年月日: %s 性別: %s\n",
			row[USER_ID], row[NAME], row[COLUMNS_USERS + ADDRESS], row[BIRTH], gender[atoi(row[GENDER])]
		);
		users_exist++;
	}

	if(users_exist==0){
		printf("データがありません。\n");
	}

	// MySQLとファイルを閉じる
	mysql_free_result(resp);
	mysql_close(mysql);
	fclose(fp);

	printf("\n");
}

/* int show_salary(in char *id)
 * idで指定されたuserの給与を表示
 * 戻り値 該当する給与データがなければ0
 *       データがあれば0以外
 */
int show_salary(char *id){
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	int salary_exists = 0;
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return salary_exists;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// salaryテーブルからデータを取得
	sprintf(sql_st,
			"SELECT id, name FROM users WHERE users.id='%s'",
			id
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n"); 
		fprintf(fp, "%s ユーザーデータの取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return salary_exists;
	}
	// 表示
	resp = mysql_use_result(mysql);
	if((row = mysql_fetch_row(resp)) != NULL ){
		printf( "%sさん(id: %s)の給与データ\n", row[NAME], row[USER_ID]);
	}
	else{
		printf("該当するデータがありません\n");
		
		mysql_free_result(resp);
		mysql_close(mysql);
		return salary_exists;
	}

	// メモリ解放
	mysql_free_result(resp);
	mysql_close(mysql);

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// salaryテーブルからデータを取得
	sprintf(sql_st,
			"SELECT num, DATE_FORMAT(month, '%%Y'), DATE_FORMAT(month, '%%m') FROM salary WHERE user_id='%s'",
			id
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("給与データの取得に失敗しました\n"); 
		fprintf(fp, "%s Salaryテーブルからのデータの取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return salary_exists;
	}
	// 表示
	resp = mysql_use_result(mysql);
	while((row = mysql_fetch_row(resp)) != NULL ){
		printf( "金額: %s円\t %s/%s月分\n", row[0], row[1], row[2]);
		salary_exists++;
	}
	if(salary_exists==0){
		printf("該当するデータがありません\n");
	}

	// MySQLとファイルを閉じる
	mysql_free_result(resp);
	mysql_close(mysql);
	fclose(fp);

	printf("\n");
	
	return salary_exists;
}

/* void register_users(in User user)
 * User の情報をテーブルに登録
 */
void register_users(User user) {
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, CLIENT_MULTI_STATEMENTS
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// users, addressテーブルに登録
	sprintf(sql_st,
			"INSERT INTO users SET id='%s', name='%s', birth='%s', gender=%d;\
			 INSERT INTO address SET id='%s', address='%s'",
			user.id, user.name, user.birth, user.gender,
			user.id, user.address
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーの新規登録に失敗しました\n"); 
		mysql_rollback(mysql);
		fprintf(fp, "%s User, Addressテーブルへの書き込みに失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}
	else {
		mysql_close(mysql);
		printf("データを登録しました\n");
	}
	printf("\n");

	// ファイルを閉じる
	fclose(fp);
}

/* void register_salary(in Salary salary)
 * Salary の情報をテーブルに登録 */
void register_salary(Salary salary){
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// salaryテーブルに登録
	sprintf(sql_st,
			"INSERT INTO salary SET user_id='%s', num=%d, month='%s'",
			salary.user_id, salary.num, salary.month
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("変更データの保存に失敗しました\n"); 
		fprintf(fp, "%s Salaryテーブルの更新に失敗\n", timestamp(ts));
		mysql_rollback(mysql);
		mysql_close(mysql);
		return;
	}
	else {
		printf("データを登録しました\n");
	}
	printf("\n");

	// MySQLとファイルを閉じる
	mysql_close(mysql);
	fclose(fp);
}

/* void update_users(in char *id)
 * id で指定されたユーザーの個人情報を更新 */
void update_users(char *id) {
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	User user;
	int choice=1, end;
	char dummy;
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続(SELECT処理)
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// DBから条件に合うデータを取得
	sprintf(sql_st, "SELECT * FROM users LEFT JOIN (address) ON (users.id=address.id) WHERE users.id='%s'", id);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n"); 
		fprintf(fp, "%s Usersテーブルからのデータ取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}
	resp = mysql_use_result(mysql);
	row = mysql_fetch_row(resp);
	if(row == NULL ){
		printf("該当するデータがありません。\n");
		mysql_free_result(resp); // メモリ解放
		mysql_close(mysql);
		return;
	}

	do {
		// Userインスタンスの代入と更新
		strcpy(user.id, row[USER_ID]);
		strcpy(user.name, row[NAME]);
		strcpy(user.birth, row[BIRTH]);
		user.gender  = atoi(row[GENDER]);
		strcpy(user.address, row[COLUMNS_USERS + ADDRESS]);


		do{
			printf("どの項目を変更しますか?\n");
			printf("[1] 名前\n");
			printf("[2] 生年月日\n");
			printf("[3] 性別\n");
			printf("[4] 住所\n");
			scanf("%d", &choice);
		} while(choice<1 ||choice>4 );
		
		if (choice == 1) {
			printf("訂正後の名前を入力してください\n");
			scanf("%s", user.name);
		}
		else if (choice == 2) {
			printf("訂正後の生年月日を入力してください (例: 1990/01/01)\n");
			scanf("%s", user.birth);
		}
		else if (choice == 3) {
			printf("訂正後の性別を入力してください\n");
			scanf("%d", &user.gender);
		}
		else if (choice == 4) {
			printf("新しい住所を入力してください\n");
			scanf("%s", user.address);
		}

		do{
			printf("以下の内容に変更しますか?\n");
			printf("    ID: %s 名前: %s 住所: %s 生年月日: %s 性別: %s\n",
				user.id, user.name, user.address, user.birth, gender[user.gender]
			);
			printf("[0] 変更する\n");
			printf("[1] 入力し直す\n");
			printf("[2] キャンセル\n");
			scanf("%c", &dummy);
			scanf("%d", &end);
		}while(end<0 || end>2); // 0,1,2以外を弾く
	} while (end == 1); // 0,2を選んだとき下に進む
	if(end == 2){ // 2のとき保存しないで終了
		mysql_free_result(resp);
		mysql_close(mysql);
		printf("変更をキャンセルしました\n");
		return;
	}
	
	// メモリ解放(SELECT処理)
	mysql_free_result(resp);
	mysql_close(mysql);

	// 初期化と接続(UPDATE処理)
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, CLIENT_MULTI_STATEMENTS
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// テーブルの情報を更新
	sprintf(sql_st,
			"UPDATE users SET name='%s', birth='%s', gender=%d WHERE id='%s';\
			 UPDATE address SET address='%s' WHERE id='%s'",
			user.name, user.birth, user.gender, user.id,
			user.address, user.id
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの保存に失敗しました\n"); 
		fprintf(fp, "%s Users, Addressテーブルの更新に失敗\n", timestamp(ts));
		mysql_rollback(mysql);
		mysql_close(mysql);

		return;
	}
	else {
		printf("変更を保存しました\n");
	}
	printf("\n");

	// MySQLとファイルを閉じる
	mysql_close(mysql);
	fclose(fp);
}

/* void update_salary(in char *id) */
void update_salary(char *id){
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	char month[11] = "";
	Salary salary;
	int choice, end;
	char dummy;
	int salary_exists = 0;
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 給与情報がなければ関数終了
	salary_exists = show_salary(id);
	if(salary_exists==0){
		return;
	}

	printf("変更したい支給月を入力してください (例: 2020/04)\n");
	scanf("%s", month);
	strcat(month, "/01");

	// 初期化と接続(SELECT処理)
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// DBから条件に合うデータを取得
	sprintf(sql_st, "SELECT * FROM salary WHERE user_id='%s' AND month='%s'", id, month);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("給与データの取得に失敗しました\n"); 
		fprintf(fp, "%s Salaryテーブルからのデータ取得に失敗\n", timestamp(ts));
		mysql_close(mysql);

		return;
	}
	resp = mysql_use_result(mysql);
	row = mysql_fetch_row(resp);
	if(row == NULL ){
		printf("該当するデータがありません。\n");
		mysql_free_result(resp); // メモリ解放
		mysql_close(mysql);
		
		return;
	}

	do {
		// Salaryインスタンスの代入と更新
		strcpy(salary.user_id, row[USER_ID]);
		salary.id  = atoi(row[SALARY_ID]);
		salary.num = atoi(row[NUM]);
		strcpy(salary.month, row[MONTH]);
	
		printf("どの項目を変更しますか?\n");
		printf("[1] 金額\n");
		printf("[2] 支給月\n");
		scanf("%d", &choice);

		if (choice == 1) {
			printf("訂正後の金額を入力してください\n");
			scanf("%d", &salary.num);
		}
		else if (choice == 2) {
			printf("訂正後の支給月を入力してください (例: 2020/04)\n");
			scanf("%s", salary.month);
		}
		else {
			printf("無効な番号です\n");
		}

		do {
			printf("以下の内容に変更しますか? \n");
			printf("    ユーザーID: %s 金額: %d 支給月: %s\n",
				salary.user_id, salary.num, salary.month
			);
			printf("[0] 変更する\n");
			printf("[1] 入力し直す\n");
			printf("[2] キャンセル\n");
			scanf("%c", &dummy);
			scanf("%d", &end);
		}while(end<0 || end>2); // 0,1,2以外を弾く
	} while (end == 1); // 0,2を選んだとき下に進む
	if(end == 2){ // 2のとき保存しないで終了
		mysql_free_result(resp);
		mysql_close(mysql);
		printf("変更をキャンセルしました\n");
		return;
	}

	// メモリ解放(SELECT処理)
	mysql_free_result(resp);
	mysql_close(mysql);

	// 初期化と接続(UPDATE処理)
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// テーブルの情報を更新
	sprintf(sql_st,
			"UPDATE salary SET num=%d, month='%s/01' WHERE id='%d'",
			salary.num, salary.month, salary.id
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("変更データの保存に失敗しました\n"); 
		fprintf(fp, "%s Salaryテーブルの更新に失敗\n", timestamp(ts));
		mysql_rollback(mysql);
		mysql_close(mysql);

		return;
	}
	else {
		printf("変更を保存しました\n");
	}
	printf("\n");

	// MySQLとファイルを閉じる
	mysql_close(mysql);
	fclose(fp);
}

/* void delete_users(in char *id)
 * id で指定したユーザーを登録削除 */
void delete_users(char *id) {
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n"); 
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// idに該当する情報を削除
	sprintf(sql_st,
			"DELETE FROM users WHERE id='%s'",
			id
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーの削除に失敗しました\n");
		fprintf(fp, "%s Usersテーブルのデータ削除に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}

	if(mysql_affected_rows(mysql)==0){
		printf("データがありません\n");
	}
	else {
		printf("データを削除しました\n");
	}
	printf("\n");

	// MySQLとファイルを閉じる
	mysql_close(mysql);
	fclose(fp);
}

/* void delete_salary(in char *id)
 * id で指定したユーザーのyear/month月の給与情報を削除 */
void delete_salary(char *id) {
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	char month[11];
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 指定したユーザの給与表示
	show_salary(id);
	printf("削除したい支給月を入力 (例: 2020/03)\n");
	scanf("%s", month);
	strcat(month, "/01");

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n");
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// idに該当する情報を削除
	sprintf(sql_st,
			"DELETE FROM salary WHERE user_id='%s' AND month='%s'",
			id, month
	);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("給与データの削除に失敗しました\n");
		fprintf(fp, "%s Salaryテーブルからのデータ削除に失敗\n", timestamp(ts));
		mysql_rollback(mysql);
		mysql_close(mysql);
		return;
	}

	if(mysql_affected_rows(mysql)==0){
		printf("データがありません\n");
	}
	else {
		printf("データを削除しました\n");
	}
	printf("\n");

	// MySQLとファイルを閉じる
	mysql_close(mysql);
	fclose(fp);
}

/* void search(in char* str) */
void search(char* str){
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	int data_exist = 0;
	FILE *fp;
	char ts[20];

	// ログファイルオープン
	fp = fopen("log.txt", "a");
	if(fp == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n");
		fprintf(fp, "%s MySQLへの接続に失敗\n", timestamp(ts));
		exit(-1);
	}

	// usersテーブルからデータを取得
	sprintf(sql_st, "SELECT * FROM users, address WHERE users.id=address.id AND users.name LIKE '%%%s%%'", str);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n");
		fprintf(fp, "%s Users, Addressテーブルからのデータ取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}

	// 表示
	resp = mysql_use_result(mysql);
	while((row = mysql_fetch_row(resp)) != NULL ){
		printf( "ID: %s 名前: %s 住所: %s 生年月日: %s 性別: %s\n",
			row[USER_ID], row[NAME], row[COLUMNS_USERS + ADDRESS], row[BIRTH], gender[atoi(row[GENDER])]
		);
		data_exist++;
	}
	if(data_exist==0){
		printf("データがありません。\n");
	}

	// MySQLとファイルを閉じる
	mysql_free_result(resp);
	mysql_close(mysql);
	fclose(fp);
}

void file_out(){
    FILE *fp_log, *fp_data;
    User user_out[100] = {'\0'};
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	int data_exist=0;
	char ts[20];

	// ログファイルを開く
	fp_log = fopen("log.txt", "a");
	if(fp_log == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// データファイルを開く
	fp_data = fopen("data.txt", "w");
	if(fp_data == NULL) {
		printf("ファイルが開けません\n");
		fprintf(fp_log, "%s データファイルへのアクセスに失敗\n", timestamp(ts));
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n");
		exit(-1);
	}

	// usersテーブルからデータを取得
	sprintf(sql_st, "SELECT * FROM users, address WHERE users.id=address.id");
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n");
		fprintf(fp_log, "%s Users, Addressテーブルからの情報取得に失敗\n", timestamp(ts));
		mysql_close(mysql);
		return;
	}

	// ファイル出力
	resp = mysql_use_result(mysql);
	while((row = mysql_fetch_row(resp)) != NULL ){
		fprintf(fp_data, "ID: %s 名前: %s 住所: %s 生年月日: %s 性別: %s\n",
			row[USER_ID], row[NAME], row[COLUMNS_USERS + ADDRESS], row[BIRTH], gender[atoi(row[GENDER])]
		);
		fprintf(fp_data, "給与データ\n");
		file_out_salary(fp_data, row[USER_ID]);
		fprintf(fp_data, "======================================\n");
		data_exist++;
	}
	if(data_exist==0){
		printf("データがありません。\n");
	}

	// MySQLとファイルを閉じる
	mysql_free_result(resp);
	mysql_close(mysql);
	fclose(fp_data);
	fclose(fp_log);
}

/* void file_out_salary(in FILE *fp, in char *id) */
void file_out_salary(FILE *fp_data, char *id){
	MYSQL* mysql = NULL;
	MYSQL* ret = NULL;
	MYSQL_RES* resp = NULL;
	MYSQL_ROW row;
	int qr_ret = 0;
	char* db_name = "Meibo";
	char sql_st[200] = "";
	FILE *fp_log;
	char ts[20];

	// ログファイルオープン
	fp_log = fopen("log.txt", "a");
	if(fp_log == NULL) {
		printf("ファイルが開けません\n");
		return;
	}

	// 初期化と接続
	mysql = mysql_init(NULL);
	ret = mysql_real_connect(
		mysql,
		local.ip,
		local.name,
		local.pw,
		db_name,
		0, NULL, 0
	);
	if(!ret){
		printf("MySQLへの接続に失敗\n");
		exit(-1);
	}

	// usersテーブルからデータを取得
	sprintf(sql_st, "SELECT num, DATE_FORMAT(month, '%%Y'), DATE_FORMAT(month, '%%m') FROM salary WHERE user_id=%s", id);
	qr_ret = mysql_query(mysql , sql_st);
	if(qr_ret){
		printf("%s\n", mysql_error(mysql));
		printf("ユーザーデータの取得に失敗しました\n");
		mysql_close(mysql);
		return;
	}

	// ファイル書き込み
	resp = mysql_use_result(mysql);
	while((row = mysql_fetch_row(resp)) != NULL ){
		fprintf(fp_data, "金額: %s円\t%s/%s月\n",
			row[0], row[1], row[2]
		);
	}

	// MySQLとログファイルを閉じる
	mysql_free_result(resp);
	mysql_close(mysql);
	fclose(fp_log);
}

/* char *timestamp(out char *ts) */
char *timestamp(char *ts){
	time_t t;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);
	sprintf(ts, "%04d/%02d/%02d %02d:%02d:%02d\n",
			tm->tm_year + 1900, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec
	);
	return ts;
}