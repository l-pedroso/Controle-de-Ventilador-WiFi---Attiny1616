#include <atmel_start.h>
#include "touch_example.h"
#include "triac_driver/triac_driver.h"
#include "atmel_start_pins.h"
#include "tuya_mcu_sdk/mcu_api.h"
#include "tuya_mcu_sdk/wifi.h"
#include "usart_basic.h"
#include "app.h"


void tuya_wifi_rx_cb(void){
	uint8_t data;
	data = USART0.RXDATAL;
	uart_receive_input(data);
}

int main(void)
{
	triac_driver_config config;
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	config.get_zero_pin_state_func = Zerocross_get_level;
	config.triac_control_func = Triac_set_level;
	config.trigger_angle = 0;
	config.triac_state = off;
	triac_driver_init(config);

	USART_0_set_ISR_cb(&tuya_wifi_rx_cb, RX_CB);
	wifi_protocol_init();
	/* Replace with your application code */
   //mcu_set_wifi_mode(SMART_CONFIG);
	
	while (1) {
		touch_example();
		
		wifi_uart_service();
		
		switch(mcu_get_wifi_work_state())
		{
			case SMART_CONFIG_STATE:
			// In EZ mode, the LED flickers quickly.
			set_wifi_led_status(blink_fast);
			break;
			case AP_STATE:
			// In AP mode, the LED flickers slowly.
			set_wifi_led_status(blink_slow);
			break;
			case WIFI_NOT_CONNECTED:
			// The Wi-Fi network has been set up and is connecting to the router. The LED is off.
			set_wifi_led_status(led_off);
			break;
			case WIFI_CONNECTED:
			set_wifi_led_status(led_on);
			break;
			default:
			break;
		}
	}
}
