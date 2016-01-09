/*
The MIT License (MIT)

Copyright (c) 2015 Pedro Minatel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "osapi.h"
#include "os_type.h"
#include <user_interface.h>
#include "driver/uart.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

#define DEBUG_SERIAL

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void loop(os_event_t *events);

static void ICACHE_FLASH_ATTR loop(os_event_t *events){
    os_delay_us(10000);
    system_os_post(user_procTaskPrio, 0, 0 );
}

void ICACHE_FLASH_ATTR user_init(void) {

#ifdef DEBUG_SERIAL
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	system_set_os_print(TRUE);
#else
	system_set_os_print(FALSE);
#endif

	char ssid[32] = "LHC";
	char pass[64] = "tijolo22";

	wifi_setup(&ssid,&pass, STATION_MODE);

    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );
}