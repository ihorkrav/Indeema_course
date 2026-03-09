/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

 #include"UART.h"

#define ECHO_TEST_TXD           1
#define ECHO_TEST_RXD           3
#define ECHO_TEST_RTS           UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS           UART_PIN_NO_CHANGE

#define ECHO_UART_PORT_NUM      0
#define ECHO_UART_BAUD_RATE     115200
#define ECHO_TASK_STACK_SIZE    2048

static const char *TAG = "UART TEST";

#define BUF_SIZE (1024)

 void uart_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM,
                                 ECHO_TEST_TXD,
                                 ECHO_TEST_RXD,
                                 ECHO_TEST_RTS,
                                 ECHO_TEST_CTS));
}


int uart_read_servo(void)
{
    static char buffer[16];
    static int index = 0;

    uint8_t ch;
    int len = uart_read_bytes(ECHO_UART_PORT_NUM, &ch, 1, 20 / portTICK_PERIOD_MS);

    if (len <= 0) {
        return -1;
    }

    if (ch == '\n' || ch == '\r') {
        buffer[index] = '\0';

        int angle = atoi(buffer);

        index = 0;   // reset buffer

        if (angle < 0) angle = 0;
        if (angle > 180) angle = 180;

        ESP_LOGI(TAG, "Servo angle received: %d", angle);

        return angle;
    }

    if (index < sizeof(buffer) - 1) {
        buffer[index++] = ch;
    }

    return -1;
}