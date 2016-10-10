// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include <stdio.h>
#include "rom/ets_sys.h"
#include "soc/gpio_reg.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#include "driver/gpio_sig_map.h"
#include "driver/spi_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5

#define LCD_SEL_CMD()   gpio_set_level(PIN_NUM_DC,0) // Low to send command 
#define LCD_SEL_DATA()  gpio_set_level(PIN_NUM_DC,1) // High to send data
#define LCD_RST_SET()   gpio_set_level(PIN_NUM_RST,1) 
#define LCD_RST_CLR()   gpio_set_level(PIN_NUM_RST,0)
#define LCD_BKG_ON()    gpio_set_level(PIN_NUM_BCKL,1) // Backlight ON
#define LCD_BKG_OFF()   gpio_set_level(PIN_NUM_BCKL,0) //Backlight OFF


#define SPI_NUM  0x3


static void LCD_WriteCommand(const uint8_t cmd)
{
    LCD_SEL_CMD();

	spi_data_t pDat;
    pDat.cmd = cmd;
    pDat.cmdLen = 1;
    pDat.addr = NULL;//&addrIn;
    pDat.addrLen = 0;
    pDat.txDataLen = 0;
    pDat.txData = NULL;
    pDat.rxDataLen = 0;

    spi_master_send_data(SPI_NUM, &pDat);
}

static void LCD_WriteData(const uint8_t data)
{
    LCD_SEL_DATA();

    spi_data_t pDat;
    pDat.cmd = data;
    pDat.cmdLen = 1;
    pDat.addr = NULL;
    pDat.addrLen = 0;
    pDat.txDataLen = 0;
    pDat.txData = NULL;
    pDat.rxDataLen = 0;

    spi_master_send_data(SPI_NUM, &pDat);
}

static void  ILI9341_INITIAL ()
{
	LCD_BKG_ON();
	//------------------------------------Reset Sequence-----------------------------------------//

	LCD_RST_SET();
	ets_delay_us(100000);                                                              

	LCD_RST_CLR();
	ets_delay_us(200000);                                                              

	LCD_RST_SET();
	ets_delay_us(200000);                                                             

	//************* Start Initial Sequence **********//
	LCD_WriteCommand(0xCF);
	LCD_WriteData(0x00);
	LCD_WriteData(0x83);
	LCD_WriteData(0X30);

	LCD_WriteCommand(0xED);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);

	LCD_WriteCommand(0xE8);
	LCD_WriteData(0x85);
	LCD_WriteData(0x01); //i
	LCD_WriteData(0x79); //i

	LCD_WriteCommand(0xCB);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);

	LCD_WriteCommand(0xF7);
	LCD_WriteData(0x20);

	LCD_WriteCommand(0xEA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0xC0);    //Power control
	LCD_WriteData(0x26); //i  //VRH[5:0]

	LCD_WriteCommand(0xC1);    //Power control
	LCD_WriteData(0x11);   //i //SAP[2:0];BT[3:0]

	LCD_WriteCommand(0xC5);    //VCM control
	LCD_WriteData(0x35); //i
	LCD_WriteData(0x3E); //i

	LCD_WriteCommand(0xC7);    //VCM control2
	LCD_WriteData(0xBE); //i   //»òÕß B1h

	LCD_WriteCommand(0x36);    // Memory Access Control
	LCD_WriteData(0x28); //i //was 0x48

	LCD_WriteCommand(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteCommand(0xB1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x1B); //18
    
	LCD_WriteCommand(0xF2);    // 3Gamma Function Disable
	LCD_WriteData(0x08);

	LCD_WriteCommand(0x26);    //Gamma curve selected
	LCD_WriteData(0x01);
		
	LCD_WriteCommand(0xE0);    //Set Gamma
	LCD_WriteData(0x1F);
	LCD_WriteData(0x1A);
	LCD_WriteData(0x18);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x06);
	LCD_WriteData(0x45);
	LCD_WriteData(0X87);
	LCD_WriteData(0x32);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x07);
	LCD_WriteData(0x02);
	LCD_WriteData(0x07);
	LCD_WriteData(0x05);
	LCD_WriteData(0x00);
 
    LCD_WriteCommand(0XE1);    //Set Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x25);
	LCD_WriteData(0x27);
	LCD_WriteData(0x05);
	LCD_WriteData(0x10);
	LCD_WriteData(0x09);
	LCD_WriteData(0x3A);
	LCD_WriteData(0x78);
	LCD_WriteData(0x4D);
	LCD_WriteData(0x05);
	LCD_WriteData(0x18);
	LCD_WriteData(0x0D);
	LCD_WriteData(0x38);
	LCD_WriteData(0x3A);
	LCD_WriteData(0x1F);

    LCD_WriteCommand(0x2A);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0xEF);

	LCD_WriteCommand(0x2B);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x3f);
	LCD_WriteCommand(0x2C);
	
    LCD_WriteCommand(0xB7); 
	LCD_WriteData(0x07); 
	
	LCD_WriteCommand(0xB6);    // Display Function Control
	LCD_WriteData(0x0A); //8 82 27
	LCD_WriteData(0x82);
	LCD_WriteData(0x27);
	LCD_WriteData(0x00);

	//LCD_WriteCommand(0xF6); //not there
	//LCD_WriteData(0x01);
	//LCD_WriteData(0x30);

	LCD_WriteCommand(0x11);    //Exit Sleep
	ets_delay_us(100000);
	LCD_WriteCommand(0x29);    //Display on
    ets_delay_us(100000);
}
//.............LCD API END----------

static void ili_gpio_init()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO21_U,2);   //DC PIN
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO18_U,2);   //RESET PIN
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,2);   //BKL PIN
	WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG, BIT21|BIT18|BIT5);
}

static void spi_master_init()
{
	ets_printf("lcd spi pin mux init ...\r\n");
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO19_U,2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO23_U,2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO25_U,2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO22_U,2);

	WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG, BIT19|BIT23|BIT22);

    ets_printf("lcd spi signal init\r\n");
	gpio_matrix_in(PIN_NUM_MISO, VSPIQ_IN_IDX,0);
    gpio_matrix_out(PIN_NUM_MOSI, VSPID_OUT_IDX,0,0);
	gpio_matrix_out(PIN_NUM_CLK, VSPICLK_OUT_MUX_IDX,0,0);
    gpio_matrix_out(PIN_NUM_CS, VSPICS0_OUT_IDX,0,0);
    ets_printf("Hspi config\r\n");
    spi_attr_t hSpiAttr;
	hSpiAttr.mode     = SpiMode_Master;
    hSpiAttr.subMode  = SpiSubMode_0;
//	hSpiAttr.speed    = SpiSpeed_20MHz; //In theory, this is the maximum the LCD can work with (20MHz)
	hSpiAttr.speed    = 2;				//...but overclocking to 40MHz works as well.
    hSpiAttr.bitOrder = SpiBitOrder_MSBFirst;
	hSpiAttr.halfMode = SpiWorkMode_Half;
    spi_init(SPI_NUM, &hSpiAttr);
}



void ili9341_send_data(int x, int y, int width, int height, const uint16_t *data) {
	int i, l;
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,2);   //BKL PIN
	WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG, BIT5);
        gpio_set_level(5,1);

	LCD_WriteCommand(0x2A);
	LCD_WriteData(x>>8);
	LCD_WriteData(x&0xff);
	LCD_WriteData((x+(width-1))>>8);
	LCD_WriteData((x+(width-1))&0xff);

	LCD_WriteCommand(0x2B);
	LCD_WriteData(0);
	LCD_WriteData(y);
	LCD_WriteData(0);
	LCD_WriteData(y+(height-1));
	LCD_WriteCommand(0x2C);
/*
	l=height*width;
	for (i=0; i<l; i++) {
		LCD_WriteData(*data>>8);
		LCD_WriteData(*data&0xff);
		data++;
	}
*/
    LCD_SEL_DATA();

	l=height*width;
	while(l) {
		unsigned char buf[64];
		for (i=0; i<32; i++) {
			buf[i*2]=data[i]>>8;
			buf[i*2+1]=data[i]&0xff;
		}
		spi_data_t pDat;
		pDat.cmd = 0;
		pDat.cmdLen = 0;
		pDat.addr = NULL;
		pDat.addrLen = 0;
		pDat.txDataLen = 64;
		pDat.txData = (void*)buf;
		pDat.rxDataLen = 0;

		spi_master_send_data(SPI_NUM, &pDat);
		l-=32;
		data+=32;
	}
        gpio_set_level(4,0);

}


void ili9341_init()
{
	spi_master_init();
	ili_gpio_init();
	ILI9341_INITIAL ();
}





