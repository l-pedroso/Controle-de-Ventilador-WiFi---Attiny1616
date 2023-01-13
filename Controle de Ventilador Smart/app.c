/*
 * app.c
 *
 * Created: 06/12/2022 11:12:07
 *  Author: lpedroso
 */ 

#include "triac_driver/triac_driver.h"
#include "app.h"
#include "atmel_start_pins.h"
#include "tuya_mcu_sdk/protocol.h"
#include "tuya_mcu_sdk/mcu_api.h"


static dir fan_direction = INITIAL_DIRECTION; 
static output_state fan_state = INITIAL_STATE;
static output_state lamp_state = off;
static uint8_t speed = INITIAL_SPEED;
static led_status led_mode = led_off;
volatile uint16_t cnt_direction  = 0;
volatile uint16_t cnt_led = 0;


/****************************************************** 
* LAMP CONTROL                                        *
*******************************************************/
void set_lamp_output(output_state state){
	Rele3_set_level(state);
	lamp_state = state;
	mcu_dp_bool_update(DPID_SWITCH_LED,state);
}

void toggle_lamp_output(){	
	//ToDo zerocross implementation
	if(lamp_state == on){
		set_lamp_output(off);
	}else{
		set_lamp_output(on);
	}
}

output_state get_lamp_output(){
	if(Rele3_get_level()){
		return off;
	}else{
		return on;
	}
}


/******************************************************
* FAN ON/OFF CONTROL                                  *
*******************************************************/
void set_fan_output(output_state state){
	fan_state = state;
	if(fan_state == off){
		set_fan_direction(open);
	}else{
		set_fan_direction_immediately();
	}
	mcu_dp_bool_update(DPID_SWITCH_FAN,state);
	//ToDo -> insert a delay to wait relay on state			
	triac_set_output_state(fan_state);
}

void toggle_fan_output(){
	if(fan_state == on){
		set_fan_output(off);		
	}else {
		set_fan_output(on);
	}
}

output_state get_fan_output(){
	return fan_state;
}

/******************************************************
* FAN DIRECTION CONTROL                               *
*******************************************************/
void set_fan_direction(dir direction){
	
	if(fan_state == on && direction != fan_direction){
		fan_direction = direction;
		cnt_direction = 30000;
		Rele1_set_level(0);
		Rele2_set_level(0);
		mcu_dp_enum_update(DPID_FAN_DIRECTION, fan_direction);
	}else if(direction == open){
		Rele1_set_level(0);
		Rele2_set_level(0);
	}	
}

dir get_fan_direction(){
	return fan_direction;
}

void set_fan_direction_immediately(){
	process_direction_relay();
}


void process_direction_relay(){
	
	if(fan_direction == down){
		Rele1_set_level(1);
		Rele2_set_level(0);
	}
	if(fan_direction == up){
		Rele1_set_level(0);
		Rele2_set_level(1);
	}
}

/******************************************************
* FAN SPEED CONTROL                                   *
*******************************************************/
void increase_speed(){
	if(triac_get_output_state() == on){
		if(speed <= 80) {
			speed += 20;
		}else {
			speed = 100;
		}
		set_fan_speed_percentage(speed);
	}
}

void decrease_speed(){	
	if(triac_get_output_state() == on){
		if(speed >= 20 ){
			 speed -= 20;
		}else{
			 speed = 20;
		}
		set_fan_speed_percentage(speed);
	}	
}

void set_fan_speed_percentage(uint8_t percent_speed){
	
	speed = percent_speed;
	triac_set_angle(MAX_ANGLE - (PERCENT_STEP * percent_speed));	
	mcu_dp_value_update(DPID_FAN_SPEED, speed);
}

uint8_t get_fan_speed_percentage(){
	return speed;
}

/******************************************************
* WIFI LED CONTROL                                    *
*******************************************************/
void set_wifi_led_status(led_status status){
	led_mode = status;

}

void process_wifi_led(){
		switch(led_mode){
			case blink_fast:
			if(cnt_led == 0){
				cnt_led = LED_FAST_PERIOD;
				LED_toggle_level();
			}
			break;
			
			case blink_slow:
			if(cnt_led == 0){
				cnt_led = LED_SLOW_PERIOD;
				LED_toggle_level();
			}
			break;
			
			case led_on:
			LED_set_level(1);
			break;
			
			case led_off:
			LED_set_level(0);
			break;
			
			default:
			LED_set_level(0);
			break;
		}
}