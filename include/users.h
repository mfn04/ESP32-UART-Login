#ifndef _USERS_H_
#define _USERS_H_

#include "stdbool.h"
#include "string.h"
#include "esp_log.h"

#define USERS_SIZE 10


struct User_Data;

typedef struct User_Data User_t;

extern int current_user_index;
extern User_t* users[USERS_SIZE];

bool register_user(const char* username, const char* password);

User_t* user_exists(const char* username);

User_t* user_exists_from_buff(char* user_buff, size_t user_buff_size);

bool check_password(User_t* user, const char* password);

bool check_password_from_buff(User_t* user, char* pass_buff, size_t pass_buff_size);

bool check_credentials(const char* username, const char* password);

void clear_users();

#endif