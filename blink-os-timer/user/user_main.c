#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

//Define a GPIO que sera usada
uint32_t gpio5 = BIT5;

static volatile os_timer_t some_timer;

void ICACHE_FLASH_ATTR some_timerfunc(void *arg)
{
    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & gpio5)
    {
        //Set GPIO2 to LOW
        gpio_output_set(0, gpio5, gpio5, 0);
    }
    else
    {
        //Set GPIO2 to HIGH
        gpio_output_set(gpio5, 0, gpio5, 0);
    }
}

//Função NOP (NO OPERATION)
static void ICACHE_FLASH_ATTR user_procTask(os_event_t *events)
{
    os_delay_us(10);
}

//Init function
void ICACHE_FLASH_ATTR user_init()
{
    //Inicialização do modulo de GPIO
    gpio_init();
    //Configura a GPIO5
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
    //Set GPIO2 low
    gpio_output_set(0, gpio5, gpio5, 0);

    //Disarm timer
    os_timer_disarm(&some_timer);

    //Setup timer
    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //1000 is the fire time in ms
    //0 for once and 1 for repeating
    os_timer_arm(&some_timer, 1000, 1);

    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}
