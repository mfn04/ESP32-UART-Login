#include "string.h"
#include "esp_log.h"

struct User_Data {
    const char* username;
    size_t username_len;
    const char* password;
    size_t password_len;
};

typedef struct User_Data User_t;

const int USERS_SIZE = 10;
int current_user_index = 0;
User_t* users[10];

static bool register_user(const char* username, const char* password){
    if(current_user_index < USERS_SIZE) {
        User_t* user = (User_t*)malloc(sizeof(User_t));
        user->username = strdup(username);
        user->password = strdup(password);
        users[current_user_index++] = user;
        ESP_LOGI("Users", "Successfully added user '%s' to database.", username);
        return true;
    } else {
        ESP_LOGW("Users", "Failed to added user '%s' to database.", username);
        return false;
    }
}

static User_t* user_exists(const char* username) {
    // Linear search
    for(int i = 0; i < USERS_SIZE; i++){
        User_t* user = users[i];
        if(user != NULL){
            if(strcmp(user->username,username) == 0) {
                return user;
            }
        }
    }
    return NULL;
}

static User_t* user_exists_from_buff(char* user_buff, size_t user_buff_size) {
    // Linear search
    for(int i = 0; i < USERS_SIZE; i++){
        User_t* user = users[i];
        if(user != NULL){
            if(strncmp(user->username,user_buff,user_buff_size) == 0) {
                return user;
            }
        }
    }
    return NULL;
}

/*static bool check_password(User_t* user, const char* password){
    if(user == NULL){
        return false;
    }
    if(strcmp(user->password,password) == 0){
        return true;
    }
    return false;
}*/

static bool check_password_from_buff(User_t* user, char* pass_buff, size_t pass_buff_size){
    if(user == NULL){
        return false;
    }
    if(strncmp(user->password,pass_buff,pass_buff_size) == 0){
        return true;
    }
    return false;
}

/*static bool check_credentials(const char* username, const char* password){
    User_t* user_found = user_exists(username);
    if(user_found == NULL){
        return false;
    }
    if(strcmp(user_found->password,password) == 0){
        return true;
    }
    return false;
}*/

static void clear_users(){
    for(int i = 0; i < USERS_SIZE; i++) {
        User_t* user = users[i];
        if(users[i] != NULL) {
            free(user->username);
            free(user->password);
            free(user);
        }
    }
}