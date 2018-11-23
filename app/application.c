#include <application.h>

#define PING "PING"
#define PONG "PONG"

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

typedef struct
{
    uint8_t id;

} config_t;

config_t config;

uint8_t *rx_buffer;
uint8_t *tx_buffer;

bc_scheduler_task_id_t spirit1_tx_task_id;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 100);

        tx_buffer[0] = config.id;
        strcpy((char *) tx_buffer + 1, PING);
        bc_spirit1_set_tx_length(1 + sizeof(PING));
        bc_spirit1_tx();

        bc_log_debug("TX: %s", PING);
    }
}

void spirit1_tx_task(void *param)
{
    tx_buffer[0] = config.id;
    strcpy((char *) tx_buffer + 1, PONG);
    bc_spirit1_set_tx_length(1 + sizeof(PONG));
    bc_spirit1_tx();

    bc_log_debug("TX: %s", PONG);
}

void spirit1_event_handler(bc_spirit1_event_t event, void *event_param)
{
    if (event == BC_SPIRIT1_EVENT_RX_DONE)
    {
        bc_led_pulse(&led, 30);

        bc_log_dump(rx_buffer, bc_spirit1_get_rx_length(), "RX length=%d", bc_spirit1_get_rx_length());

        if (strncmp((char *) rx_buffer + 1, PING, bc_spirit1_get_rx_length()) == 0)
        {
            bc_tick_t delay = config.id * 30 + 10;
            bc_scheduler_plan_from_now(spirit1_tx_task_id, delay);
        }
    }
    else if (event == BC_SPIRIT1_EVENT_TX_DONE)
    {
        bc_spirit1_rx();
    }
}

bool save_config(void)
{
    return bc_eeprom_write(0, &config, sizeof(config_t));
}

bool at_id_set(bc_atci_param_t *param)
{
    if (param->length != 1 && param->length != 2)
    {
        return false;
    }

    if (!isdigit(param->txt[0]) || (param->length == 2 && !isdigit(param->txt[1])))
    {
        return false;
    }

    int id = atoi(param->txt);

    if ((id < 0) || id > 256)
    {
        return false;
    }

    config.id = id;

    return save_config();
}

bool at_id_read(void)
{
    bc_atci_printf("$ID: %d", config.id);

    return true;
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 2000);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // bc_system_pll_enable();
    // bc_scheduler_disable_sleep();

    bc_spirit1_init();
    bc_spirit1_set_event_handler(spirit1_event_handler, NULL);
    rx_buffer = bc_spirit1_get_rx_buffer();
    tx_buffer = bc_spirit1_get_tx_buffer();
    bc_spirit1_rx();

    spirit1_tx_task_id = bc_scheduler_register(spirit1_tx_task, NULL, BC_TICK_INFINITY);

    static bc_atci_command_t commands[] = {
        BC_ATCI_COMMAND_CLAC,
        {"$ID", NULL, at_id_set, at_id_read, NULL, "Id set/read"},
        BC_ATCI_COMMAND_HELP
    };

    bc_atci_init(commands, sizeof(commands)/sizeof(commands[0]));

    bc_eeprom_read(0, &config, sizeof(config_t));
}

// void application_task(void)
// {
//     // Logging in action
//     bc_log_debug("application_task run");

//     // Plan next run this function after 1000 ms
//     bc_scheduler_plan_current_from_now(1000);
// }
