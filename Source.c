
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "Header.h"

int main() {
	int num;
	char id[LEN_NAME + 1];
	char dummy;

	while (1) {
		printf("[%d] 名簿 - 表示    \t", SHOW_USERS);
		printf("[%d] 給与 - 表示\n", SHOW_SALARY);
		printf("[%d] 名簿 - 新規登録\t", REGISTER_USERS);
		printf("[%d] 給与 - 情報入力\n", REGISTER_SALARY);
		printf("[%d] 名簿 - 編集    \t", UPDATE_USERS);
		printf("[%d] 給与 - 編集    \n", UPDATE_SALARY);
		printf("[%d] 名簿 - 登録削除\t", DELETE_USERS);
		printf("[%d] 給与 - 情報削除\n", DELETE_SALARY);
		printf("[%d] 検索          \t", SEARCH);
		printf("[%d] ファイル出力    \n", FILE_OUT);
		printf("[%d] 終了\n", EXIT);
		scanf("%d", &num);

		if (num == SHOW_USERS) {
			show_users();
		}
		else if (num == SHOW_SALARY){
			printf("給与を表示したいIDを入力してください\n");
			scanf("%s", id);

			show_salary(id);
		}
		else if (num == REGISTER_USERS) {
			User user;
			int confirm;

			do {
				printf("IDを入力してください\n");
				scanf("%s", user.id);
				printf("名前を入力してください\n");
				scanf("%s", user.name);
				printf("住所を入力してください\n");
				scanf("%s", user.address);
				printf("誕生日を入力してください (例: 1990/01/01)\n");
				scanf("%s", user.birth);
				printf("性別を選択してください\n");
				printf("[0] 男\n[1] 女\n");
				scanf("%d", &user.gender);

				do{
					printf("以下の内容を登録しますか?\n");
					printf("    ID: %s 名前: %s 住所: %s 生年月日: %s 性別: %d\n",
						user.id, user.name, user.address, user.birth, user.gender
					);
					printf("[0] 登録する\n");
					printf("[1] 入力し直す\n");
					printf("[2] キャンセル\n");
					scanf("%c", &dummy);
					scanf("%d", &confirm);
				}while(confirm<0 || confirm>2); // 0,1,2以外を弾く
			} while (confirm==1); // 0,2を選んだとき下に進む
			if (confirm == 0) { // 0のとき登録
				register_users(user);
			}
			else { // 2のとき何もしない
				printf("登録をキャンセルしました\n");
			}
		}
		else if (num == REGISTER_SALARY){
			Salary salary;
			int confirm;

			do{
				printf("登録対象のIDを入力してください\n");
				scanf("%s", salary.user_id);
				printf("支給月を入力してください (例 2020/03)\n");
				scanf("%s", salary.month);
				printf("金額を入力してください\n");
				scanf("%d", &salary.num);

				do{
					printf("以下の内容を登録しますか?\n");
					printf("    金額: %d\t支給月: %s\n",
						salary.num, salary.month
					);
					printf("[0] 登録する\n");
					printf("[1] 入力し直す\n");
					printf("[2] キャンセル\n");
					scanf("%c", &dummy);
					scanf("%d", &confirm);
				} while (confirm<0 || confirm>2); // 0,1,2以外弾く
			} while (confirm==1); // 0,2を選んだとき下に進む
			if (confirm == 0) { // 0のとき登録
				strcat(salary.month, "-01");
				register_salary(salary);
			}
			else { // 2のとき何もしない
				printf("登録をキャンセルしました\n");
			}
		}
		else if (num == UPDATE_USERS) {
			printf("編集したいIDを入力してください\n");
			scanf("%s", id);
			update_users(id);
		}
		else if (num == UPDATE_SALARY){
			printf("編集したいIDを入力してください\n");
			scanf("%s", id);
			update_salary(id);
		}
		else if (num == DELETE_USERS) {
			printf("削除したいIDを入力してください\n");
			scanf("%s", id);

			delete_users(id);
		}
		else if (num == DELETE_SALARY){
			printf("削除したいIDを入力してください\n");
			scanf("%s", id);
			
			delete_salary(id);
		}
		else if(num == SEARCH){
			char str[101];

			printf("検索したい名前の一部を入力してください\n");
			scanf("%s", str);
			search(str);
		}
		else if(num == FILE_OUT){
			file_out();
		}
		else if (num == EXIT) {
			printf("終了します\n");
			break;
		}
		else {
			printf("無効な番号です\n");
		}
	}
	return 0;
}