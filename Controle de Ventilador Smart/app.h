/*
 * app.h
 *
 * Created: 06/12/2022 11:12:26
 *  Author: lpedroso
 */ 


#ifndef APP_H_
#define APP_H_

#include "triac_driver/triac_driver.h"
#include <compiler.h>

typedef enum {
	down = 0,
	up,
	open
} dir;

typedef enum{
	blink_fast = 0,
	blink_slow,
	led_on,
	led_off
} led_status;

#define INITIAL_DIRECTION (dir)down
#define INITIAL_STATE  0
#define INITIAL_SPEED  100
#define PERCENT_STEP (MAX_ANGLE / 100.0)
#define LED_FAST_PERIOD 2500
#define LED_SLOW_PERIOD 5000

void set_lamp_output(output_state state);
void toggle_lamp_output();
output_state get_lamp_output();
void set_fan_output(output_state state);
void toggle_fan_output();
output_state get_fan_output();
void set_fan_direction(dir direction);
dir get_fan_direction();
void set_fan_direction_immediately();
void process_direction_relay();
void increase_speed();
void decrease_speed();
void set_fan_speed_percentage(uint8_t percent_speed);
uint8_t get_fan_speed_percentage();
void set_wifi_led_status(led_status status);
void process_wifi_led();

#endif /* APP_H_ */