#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"
#include "w5500_driver.h"
#include "dns.h"
#include "httpClient.h"

#define SOCK_DNS	4
#define SOCK_HTTP_CLIENT	1


#define HTTP_POST_REQUEST "/insert.php"

int8_t process_dns(void);

 typedef enum
 {
	 OFF = 0,
	 ON = 1
 }dns_state_type;


uint8_t flag_process_dns_success =  OFF;


uint8_t dns_server[4] = {8,8,8,8};
uint8_t domain_ip[4] = {};


uint8_t data_buff[DATA_BUF_SIZE];
uint8_t g_send_buff[DATA_BUF_SIZE];
uint8_t g_recv_buff[DATA_BUF_SIZE];


uint8_t domain_name[] = "embedexpertio.batcave.net";
char sense_content[80];
uint16_t sensorvalue = 0;

wiz_PhyConf current_phy_conf;

void check_cable_connection(void)
{
	uint8_t cable_status;

	do{

		printf("\r\nGetting cable status...\r\n");
		ctlwizchip(CW_GET_PHYLINK,(void *)&cable_status);

		if(cable_status ==  PHY_LINK_OFF)
		{
			printf("No cable detected ...\r\n");
			delay(1000);
		}

	}while(cable_status ==  PHY_LINK_OFF);

	printf("Cable connected...\r\n");

}

void display_phy_config(void)
{
	wiz_PhyConf phy_conf;
	ctlwizchip(CW_GET_PHYCONF,(void*)&phy_conf);

	if(phy_conf.by == PHY_CONFBY_HW)
	{
		printf("\n\rPHY is currently configured by hardware.");
	}
	else{
		printf("\n\rPHY is currently configured by software.");

	}
	printf("\r\nSTATUS: Autonegotiation %s",(phy_conf.mode == PHY_MODE_AUTONEGO) ? "Enabled" : "Disabled");
	printf("\r\nSTATUS: Duplex Mode: %s",(phy_conf.duplex == PHY_DUPLEX_FULL) ? "Full Duplex" : "Half Duplex");
	printf("\r\nSTATUS: Speed: %dMbps",(phy_conf.speed == PHY_SPEED_10) ? 10 : 100);
  printf("\r\n...");

}

int main()
{

	uint16_t len = 0;

	/*Enable FPU*/
	fpu_enable();

	/*Initialize timebase*/
	timebase_init();

	/*Initialize debug UART*/
	debug_uart_init();



	/*Initialize LED*/
	led_init();

	/*Initialize Push button*/
	button_init();

	/*Initialize ADC*/
	pa1_adc_init();

	/*Start conversion*/
	start_conversion();

    wizchip_cs_pin_init();
    w5500_spi_init();
    w5500_init();

    check_cable_connection();
    display_phy_config();


    /*Run DNS*/
    if(process_dns())
    {
    	flag_process_dns_success =  ON;
    }

    if(flag_process_dns_success)
    {
	    printf(" # DNS: %s => %d.%d.%d.%d\r\n", domain_name, domain_ip[0], domain_ip[1], domain_ip[2], domain_ip[3]);

    }
    else
    {
    	printf("DNS Failed\r\n");
    }

    /*Initialize the http_client*/
   httpc_init(SOCK_HTTP_CLIENT,domain_ip,80,g_send_buff,g_recv_buff);
	while(1)
	{
		sensorvalue = adc_read();
		sprintf(sense_content,"key=4326&sensorvalue=%d",sensorvalue);

		httpc_connection_handler();

		if(httpc_isSockOpen)
		{
			httpc_connect();
		}

		if(httpc_isConnected)
		{
			/*Prepare http POST request*/
			HttpRequest request;
			request.method =  (uint8_t *)HTTP_POST;
			request.uri    =   (uint8_t *)HTTP_POST_REQUEST;
			request.host   =  (uint8_t *)domain_name;
			request.connection =  (uint8_t *)"keep-alive";
			request.content_type = (uint8_t *)"application/x-www-form-urlencoded";

			/*Calculate the body length*/
			uint16_t body_length =  strlen(sense_content);

			/*Send http request header*/

			len = httpc_send_header(&request, g_send_buff,NULL,body_length);

			/*Check if header was sent successfully*/
			if(len > 0)
			{
				/*Send HTTP request body*/
				len  = httpc_send_body((uint8_t *)sense_content,body_length);

				if(len  > 0 )
				{
					printf("HTTP POST request sent successfully\r\n");
				}
				else
				{
					printf("Failed to send HTTP body \r\n");
				}
			}
			else
			{
				printf("Failed to send HTTP header \r\n");

			}



			delay(1000); //Delay 1 second;

		}

	}
}

int8_t process_dns(void)
{
	int8_t ret = 0;
	uint8_t dns_retry = 0;

	DNS_init(SOCK_DNS, data_buff);

	while(1)
	{
		if((  ret = DNS_run(dns_server, (uint8_t *)domain_name,domain_ip)) == 1)
		{
			break;
		}
		else
		{
			dns_retry++;
			if(dns_retry <= 2)
			{
				printf("DNS timeout occurred retry [%d]\n\r",dns_retry);
			}
			if(dns_retry > 2)
			{
				 break;
			}
		}
	}

	return ret;
}
