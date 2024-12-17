/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Example cpp file for ST7789_TFT_PICO library.
			 Graphics methods testing
	@note  See USER OPTIONS 0-2 in SETUP function

	@test
		-# Test201  pixels and lines
		-# Test202  rectangles
		-# Test203  Circle
		-# Test204  Triangles
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7789/ST7789_TFT.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

// Section :: Globals
ST7789_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);	// setup + user options
void Test201(void);	// pixels and lines
void Test202(void);	// rectangles
void Test203(void);	// Circle
void Test204(void);	// Triangles
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{
	Setup();
	Test201();
	Test202();
	Test203();
	Test204();
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

/*!
	@brief setup + user options
*/
void Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	TFT_MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true)
	{								// hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//**********************************************************

	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19;
	int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;
	int8_t RST_TFT = 17;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;	   // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 240;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	myTFT.TFTST7789Initialize();
}


/*!
	@brief   pixels and lines
*/
void Test201(void)
{
	TFT_MILLISEC_DELAY(TEST_DELAY1);
	myTFT.TFTfillScreen(ST7789_BLACK);

	myTFT.TFTdrawPixel(85, 55, ST7789_WHITE);
	myTFT.TFTdrawPixel(87, 59, ST7789_WHITE);
	myTFT.TFTdrawPixel(110, 79, ST7789_WHITE);
	myTFT.TFTdrawLine(80, 80, 40, 40, ST7789_RED);
	myTFT.TFTdrawFastVLine(100, 100, 100, ST7789_GREEN);
	myTFT.TFTdrawFastHLine(160, 60, 70, ST7789_YELLOW);

	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTfillScreen(ST7789_BLACK);
}

/*!
	@brief  rectangles
*/
void Test202(void)
{
	myTFT.TFTdrawRectWH(65, 65, 20, 20, ST7789_RED);
	if(myTFT.TFTfillRectBuffer(105, 75, 20, 20, ST7789_YELLOW) != 0) //uses spiwrite
	{
		printf("Error Test202 1: Error in the TFTfillRectangle function\r\n");
	}
	myTFT.TFTfillRect(160, 55, 20, 20, ST7789_GREEN);
	myTFT.TFTdrawRoundRect(15, 160, 50, 50, 5, ST7789_CYAN);
	myTFT.TFTfillRoundRect(70, 160, 50, 50, 10, ST7789_WHITE);

	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTfillScreen(ST7789_BLACK);
}

/*!
	@brief  circles
*/
void Test203(void)
{
	myTFT.TFTdrawCircle(90, 160, 15, ST7789_GREEN);
	myTFT.TFTfillCircle(140, 80, 15, ST7789_YELLOW);
}

/*!
	@brief  triangles
*/
void Test204(void)
{
	myTFT.TFTdrawTriangle(35, 80, 85, 40, 115, 80, ST7789_CYAN);
	myTFT.TFTfillTriangle(55, 120, 100, 90, 127, 120, ST7789_RED);

	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTfillScreen(ST7789_BLACK);
}


/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.TFTFontNum(myTFT.TFTFont_Default);
	myTFT.TFTfillScreen(ST7789_BLACK);
	myTFT.TFTdrawText(5, 50, teststr1, ST7789_GREEN, ST7789_BLACK, 2);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over");
}