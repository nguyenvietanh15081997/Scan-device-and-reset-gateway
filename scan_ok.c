/*after 15 scan, print mac of all device scanned*/
/*
 ============================================================================
 Name        : test.c
 Author      : anh
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014, 2015 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Prints "Hello Mraa!" recursively. Press Ctrl+C to exit
 *
 */

/* standard headers */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "mraa/uart.h"


// transmit
char scan_device[]           =   {0xe9, 0xff, 0x00};
char select_mac[]            =   {0xe9, 0xff, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char select_mac2[]           =   {0xe9, 0xff, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char stop_scan[]             =   {0xe9, 0xff, 0x01};

// buffer receive
char ans_scan_device[38];
char mac_Array[600];
char mac[6];
char outputMac[6];
char buffer[128];


// variable
unsigned int TEMP=0,LAN1=0;
double count=0,Time=0;
int check_mac=0;
int nho1, nho2;

#define UART 1
#define UART0 0

mraa_uart_context uart;
mraa_uart_context uart0;

void UART_Init()
{
    uart = mraa_uart_init(UART);
    mraa_uart_set_mode(uart,8, 0, 1);
    mraa_uart_set_baudrate(uart, 115200);
    //mraa_init();
    uart0 = mraa_uart_init(UART0);
    mraa_uart_set_mode(uart0,8, 0, 1);
    mraa_uart_set_baudrate(uart0, 115200);
}

void CLEAR_DATA()
{
	while(mraa_uart_data_available(uart, 0)==1)
	{
		mraa_uart_read(uart, buffer, 128);
	}
}

void CHECK_MAC2(char *inMac,char * outMac)
{
	int i,j,dem=6,DEM=0;
	for(i=0;i<600;i+=6)
	{
		for(j=0;j<6;j++)
		{
			if(inMac[j]==mac_Array[j+i]) dem--;
		}
		if (dem==0) break;
		else {DEM++;dem=6;}
	}
	if(DEM==100) // khong giong
	{
		nho1=nho1+6;
		for(i= nho1-6 ; i < nho1 ; i++)
		{
			mac_Array[i]=inMac[i-(nho1-6)];
			outMac[i-(nho1-6)]=inMac[i-(nho1-6)];
		}
		check_mac=0;
	}
	else check_mac=1;
}

void Scan_vr2()
{
		CLEAR_DATA();
		mraa_uart_write(uart,scan_device,3);
		while(mraa_uart_data_available(uart, 100)==0)
		{
			count++;
			if(count==65500)
				{
					Time++;
					count=0;
				}
		}
		if(Time<200000)
		{
			mraa_uart_read(uart,ans_scan_device,38);
			//mraa_uart_write(uart0, ans_scan_device, 38);
			int i;
			if(ans_scan_device[0]==36 && ans_scan_device[1]==0)   // 0x24 and 0x00
			{
				for (i=0;i<6;i++)
				{
					mac[i]=ans_scan_device[i+4];                  // select new mac
				}
				//mraa_uart_write(uart0, mac, 6);
				CHECK_MAC2(mac,outputMac);
				if(check_mac==0)
				{
					puts("new");
					for(i=0;i<6;i++)
					{
						select_mac[i+3]=outputMac[i];
					}
					//mraa_uart_write(uart0, select_mac, 9);
				        mraa_uart_write(uart0, outputMac, 6);
				}
				else puts("old");
			}
		}
		else
		{
			mraa_uart_write(uart, stop_scan, 3);
			puts("stop");
		}
}

int main(int argc, char** argv)
{
    mraa_init();
	UART_Init();
	sleep(3);
	mac_Array[600]='0';
	LAN1=0;
	nho1=0;
	for(LAN1=0;LAN1<15;LAN1++)
	{
		Scan_vr2();sleep(2);
	}
    mraa_uart_write(uart, stop_scan, 3);
	puts("done");
    while(1)
    {
    }
}

