#include "w5500_driver.h"



/*MOSI   = PA7
 *MISO   = PA6
 *SCLK   = PA5
 *SCS    = PA4*/

SPI_HandleTypeDef hspi1;


char temp_buffer[170];


/*Define DHCP Socket*/
#define DHCP_SOCKET		0

/*DHCP Buffer*/
uint8_t dhcp_buffer[1024];


volatile bool is_ip_assigned =  false;


void ip_assigned_callback(void)
{
	is_ip_assigned =  true;
}

void ip_update_callback(void)
{
	is_ip_assigned =  true;
}
void ip_conflict_callback(void)
{
	is_ip_assigned =  false;
}


wiz_NetInfo net_info ={
	.mac = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},/*MAC Addres*/
    .dhcp =  NETINFO_DHCP
};
/*Initialize CS Pin*/

void wizchip_cs_pin_init(void)
{
	/*Enable clock for GPIOA*/
	RCC->AHB1ENR |=(1U <<0);

	/*Set PA4 as output pin*/
	GPIOA->MODER |=(1U<<8);
	GPIOA->MODER &=~(1U<<9);

}
/*Select W5500 by pulling CS pin low*/
void wizchip_pin_cs_select(void)
{
	GPIOA->ODR &=~(1U<<4);
}

/*Unselect W5500 by pulling CS pin high*/
void wizchip_pin_cs_deselect(void)
{
	GPIOA->ODR |= (1U<<4);
}

/*Spi read multiple bytes*/
void wizchip_read_burst(uint8_t *buffer,uint16_t length)
{
	 spi_receive(&hspi1,buffer, length, MAX_DELAY);

}

/*Spi read single byte*/

uint8_t wizchip_read_byte(void)
{
	uint8_t byte;
	wizchip_read_burst(&byte,sizeof(byte));
	return byte;
}
/*Spi write multiple bytes*/
void wizchip_write_burst(uint8_t *buffer,uint16_t length)
{
	 spi_transmit(&hspi1,buffer, length, MAX_DELAY);

}

/*Spi write single byte*/
void wizchip_write_byte(uint8_t byte)
{
	wizchip_write_burst(&byte,sizeof(byte));
}
 void w5500_spi_init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction =  SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity =  SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase =  SPI_PHASE_1EDGE;
	hspi1.Init.NSS =  SPI_NSS_SOFT;
	hspi1.Init.BuadRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	/*Initialize spi*/
	spi_init(&hspi1);

}


 void w5500_init(void)
 {
     //Set RX and TX Buffer sizes

	 /*Set 2KB buffer for each of the 8 sockets*/
	 uint8_t rx_tx_buff_sizes[]= {2,2,2,2,2,2,2,2};

	 /*Initialize the W5500 with the RX and TX buffer sizes*/
	 wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

	 //Register callback functions
	 reg_wizchip_cs_cbfunc(wizchip_pin_cs_select,wizchip_pin_cs_deselect);
	 reg_wizchip_spi_cbfunc(wizchip_read_byte,wizchip_write_byte);
	 reg_wizchip_spiburst_cbfunc(wizchip_read_burst,wizchip_write_burst);

	 /*Set MAC Address*/
	 setSHAR(net_info.mac);

	 /*Initialize DHCP*/
	 DHCP_init(DHCP_SOCKET,dhcp_buffer);

	 //Register DHCP Callbacks
	 reg_dhcp_cbfunc(ip_assigned_callback,ip_update_callback,ip_conflict_callback);

	 /*Wait for IP assignment from DHCP*/
	 uint16_t timeout_counter =  10000;
	 while((!is_ip_assigned) && (timeout_counter> 0))
	 {
		 DHCP_run();
		 timeout_counter--;
	 }

	 if(!is_ip_assigned)
	 {
		 return;
	 }

	 /*Get network settings from DHCP*/
	 getIPfromDHCP(net_info.ip);
	 getGWfromDHCP(net_info.gw);
	 getSNfromDHCP(net_info.sn);


	/* Format and print the assigned IP, Gateway, and Subnet mask */
	sprintf(temp_buffer,
		"IP Address:  %d.%d.%d.%d\r\nGateway:  %d.%d.%d.%d\r\nSubnet mask: %d.%d.%d.%d\r\n",
		net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
		net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
		net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3]
	);

    printf("*************Network Information************************\r\n");
    printf("%s", temp_buffer);

    /*Apply the network configurations to the W5500*/
    wizchip_setnetinfo(&net_info);

 }



