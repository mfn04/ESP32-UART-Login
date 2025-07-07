#include "esp_log.h"
#include "driver/uart.h"
#include "users.h"
#include "string.h"

static void get_input(char* buff, size_t buff_size){
    char c;

    int index = 0;

    while(1){
        int err = 0;
        if( (err = uart_read_bytes(UART_NUM_0, &c, sizeof(char), pdMS_TO_TICKS(100))) > 0 ){
            if( (c != '\n') && (index < (buff_size-1)) ){
                buff[index] = c;
                index++;
            } else {
                buff[index] = '\0';
                break;
            }
        } else if ( err < 0 ) {
            ESP_LOGW("ERROR", "An error occured while reading UART bytes. Error Code: %d", err);
            break;
        }
    }
}

void app_main() {
    register_user("Player1", "123456");
    register_user("Player2", "123456");

    uart_config_t conf = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .source_clk = UART_SCLK_DEFAULT,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    QueueHandle_t queue_handle;

    uart_param_config(UART_NUM_0, &conf);

    uart_driver_install(UART_NUM_0, 1024, 1024, 10, &queue_handle, 0);

    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    ESP_LOGI("INPUT", "Insert Username: ");

    const int USERNAME_SIZE = 32;
    char username[USERNAME_SIZE] = {};

    get_input(username, USERNAME_SIZE);

    User_t* user = user_exists_from_buff(username,USERNAME_SIZE);

    while(user == NULL){
        ESP_LOGW("WARN", "User '%s' was not found in the database!", username);
        ESP_LOGI("INPUT", "Insert Username: ");
        get_input(username, USERNAME_SIZE);
        user = user_exists_from_buff(username,USERNAME_SIZE);
    }

    ESP_LOGI("INPUT", "Insert Password: ");

    const int PASS_SIZE = 64;
    char pass[PASS_SIZE] = {};

    get_input(pass, PASS_SIZE);

    ESP_LOGI("INFO", "Attempting to connect to, %s. Password attempt is: %s", username, pass);

    bool correct_pass = check_password_from_buff(user, pass,PASS_SIZE);

    if(correct_pass) {
        ESP_LOGI("INFO", "Welcome back, %s!", username);
    } else {
        ESP_LOGW("WARN", "Bad credentials, goodbye!");
    }

    clear_users();
}