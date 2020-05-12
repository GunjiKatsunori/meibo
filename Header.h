#pragma once
#define DATA_FILE "data.bin"
#define LEN_NAME 20
#define SIZE 256
#define COLUMNS_USERS 5
//#define DEBUG

enum chioce {SHOW_USERS=1, REGISTER_USERS, UPDATE_USERS, DELETE_USERS,
							SHOW_SALARY, REGISTER_SALARY, UPDATE_SALARY, DELETE_SALARY,
              SEARCH, FILE_OUT, EXIT};
enum column_users {USER_ID, NAME, BIRTH, GENDER, USER_TS};
enum column_address {ADDRESS_ID, ADDRESS, ADDRESS_TS};
enum column_salary {SALARY_ID=1, NUM, MONTH, SALARY_TS};

typedef struct Machine{
	char ip[20];
	char name[20];
	char pw[20];
}Machine;
typedef struct User {
	char id[LEN_NAME + 1];
	char name[LEN_NAME + 1];
	char address[LEN_NAME + 1];
	char birth[11];
	int gender;
}User;
typedef struct Salary {
	char user_id[LEN_NAME + 1];
  int id;
	int num;
	char month[11];
}Salary;

/* int show_salary(in char *id)
 * void register_users(in User user)
 * void register_salary(in Salary salary)
 * void update_users(in char *id)
 * void update_salary(in char *id)
 * void delete_users(in char *id)
 * void delete_salary(in char *id) 
 * void search(in char *str) 
 * void file_out_salary(in FILE *fp, in char *id)
 * char *timestamp(out char *ts) */

void show_users();
int show_salary(char *id);
void register_users(User user);
void register_salary(Salary salary);
void update_users(char *id);
void update_salary(char *id);
void delete_users(char *id);
void delete_salary(char *id);
void search(char* str);
void file_out();
void file_out_salary(FILE *fp, char *id);
char *timestamp(char *ts);