/**
 * \file
 *
 * \brief Driver ISR.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <driver_init.h>
#include <compiler.h>
#include "app.h"
#include "triac_driver/triac_driver.h"

bool flag= 0;
extern uint16_t cnt_direction;
extern uint16_t cnt_led;

ISR(RTC_CNT_vect)
{

	/* Insert your RTC Compare interrupt handling code */
	touch_timer_handler();

	/* Compare interrupt flag has to be cleared manually */
	RTC.INTFLAGS = RTC_CMP_bm;
}

ISR(TCA0_OVF_vect)
{
	/* Insert your TCA overflow interrupt handling code */

	/* The interrupt flag has to be cleared manually */
	
	if(cnt_direction  > 0){
		flag = 1;
		cnt_direction--;
		}else if(flag){
		flag = 0;
		process_direction_relay();
	}
		
	if(cnt_led > 0) cnt_led--;
	process_wifi_led();
		
	triac_driver_tick();
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
