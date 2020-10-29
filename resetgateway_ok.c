

/* standard headers */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "mraa/uart.h"
#include <time.h>


// transmit
static uint8_t reset_gateway[]         =   {0xe9, 0xff, 0x02};
static uint8_t setpro_internal[]       =   {0xe9, 0xff, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33, 0x44, 0x02, 0x00};
static uint8_t admit_pro_internal[]    =   {0xe9, 0xff, 0x0d, 0x01, 0x00, 0xff, 0xfb, 0xeb, 0xbf, 0xea, 0x06, 0x09, 0x00, 0x52, 0x90, 0x49, 0xf1, 0xf1, 0xbb, 0xe9, 0xeb};// trả về unicast tiếp theo của con đèn cần thêm vào




#define UART 1
#define UART0 0
mraa_uart_context uart;
mraa_uart_context uart0;

void UART_Init()
{
    uart = mraa_uart_init(UART);
    mraa_uart_set_mode(uart,8, 0, 1);
    mraa_uart_set_baudrate(uart, 115200);
    uart0 = mraa_uart_init(UART0);
    mraa_uart_set_mode(uart0,8, 0, 1);
    mraa_uart_set_baudrate(uart0, 115200);
}
void RESET_GW()
{
	mraa_uart_write(uart,reset_gateway,3);
	puts("reset_gw...");
}
void SETPRO_INTERNAL()
{
	srand((int)time(0));
	int random;
	int i;
	for(i=0;i<16;i++)
	{
		random=rand()%256;
		setpro_internal[i+3]=random;
	}

	for(i=0;i<28;i++)
	{
		printf ("%x ",setpro_internal[i]);
	}
	puts("setpro...");
	mraa_uart_write(uart,setpro_internal,28);
}
void ADMITPRO_INTERNAL()
{
	int i;
	for(i=0;i<21;i++)
	{
		printf ("%x ",admit_pro_internal[i]);
	}
	puts("admitpro...");
	mraa_uart_write(uart,admit_pro_internal,21);
}
int main(int argc, char** argv)
{
	mraa_init();
	UART_Init();
	RESET_GW();
	sleep(3);
	SETPRO_INTERNAL();sleep(2);
	ADMITPRO_INTERNAL();sleep(2);
	puts("DONE!");
    while(1)
    {
    }
}
