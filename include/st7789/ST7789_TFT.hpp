/*!
	@file     ST7789_TFT.hpp
	@author   Gavin Lyons
	@brief    Library header file for ST7789_TFT_PICO library.
				Contains driver methods for ST7789_TFT display
	@note  See URL for full details.https://github.com/gavinlyonsrepo/ST7789_TFT_PICO

*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ST7789_TFT_graphics.hpp"

// Section:  Defines

// ST7789 registers + Commands

// Section:  Defines

// ST7789 registers + Commands

// ST7789 general purpose
#define ST7789_NOP     0x00 /**< Non operation */
#define ST7789_SWRESET 0x01 /**< Soft Reset */

// ST7789 Modes
#define ST7789_SLPIN    0x10 /**< Sleep ON */
#define ST7789_SLPOUT   0x11 /**< Sleep OFF */
#define ST7789_PTLON    0x12 /**< Partial mode */
#define ST7789_NORON    0x13 /**< Normal Display */
#define ST7789_INVOFF   0x20 /**< Display invert off */
#define ST7789_INVON    0x21 /**< Display Invert on */
#define ST7789_DISPOFF  0x28 /**< Display off */
#define ST7789_DISPON   0x29 /**< Display on */
#define ST7789_IDLE_ON  0x39 /**< Idle Mode ON */
#define ST7789_IDLE_OFF 0x38 /**< Idle Mode OFF */

// ST7789 Addressing
#define ST7789_CASET    0x2A /**< Column address set */
#define ST7789_RASET    0x2B /**<  Page address set */
#define ST7789_RAMWR    0x2C /**< Memory write */
#define ST7789_RAMRD    0x2E /**< Memory read */
#define ST7789_PTLAR    0x30 /**< Partial Area */
#define ST7789_VSCRDEF  0x33 /**< Vertical scroll def */
#define ST7789_SRLBTT   0x28 /**< Scroll direction bottom to top */
#define ST7789_SRLTTB   0x30 /**< Scroll direction top to bottom */
#define ST7789_COLMOD   0x3A /**< Interface Pixel Format */
#define ST7789_MADCTL   0x36 /**< Memory Access Control */
#define ST7789_VSCRSADD 0x37 /**< Vertical Access Control */

// Frame Rate Control
#define ST7789_FRMCTR1 0xB1 /**< Normal */
#define ST7789_FRMCTR2 0xB2 /**< idle */
#define ST7789_FRMCTR3 0xB3 /**< Partial */

#define ST7789_INVCTR  0xB4 /**< Display Inversion control */
#define ST7789_DISSET5 0xB6 /**< Display Function set */

#define ST7789_RDID1   0xDA /**< read ID1 */
#define ST7789_RDID2   0xDB /**< read ID2 */
#define ST7789_RDID3   0xDC /**< read ID3 */
#define ST7789_RDID4   0xDD /**< read ID4 */

// ST7789 color control
#define ST7789_GMCTRP1 0xE0 /**< Positive Gamma Correction Setting */
#define ST7789_GMCTRN1 0xE1 /**< Negative Gamma Correction Setting */

// Memory Access Data Control  Register
#define ST7789_MADCTL_MY  0x80 /**< Row Address Order */
#define ST7789_MADCTL_MX  0x40 /**< Column Address Order */
#define ST7789_MADCTL_MV  0x20 /**< Row/Column Order (MV) */
#define ST7789_MADCTL_ML  0x10 /**< Vertical Refresh Order */
#define ST7789_MADCTL_RGB 0x00 /**< RGB order */
#define ST7789_MADCTL_BGR 0x08 /**< BGR order */
#define ST7789_MADCTL_MH  0x04  /**< Horizontal Refresh Order */

// Color definitions 16-Bit Color Values R5G6B5
#define ST7789_BLACK   0x0000
#define ST7789_BLUE    0x001F
#define ST7789_RED     0xF800
#define ST7789_GREEN   0x07E0
#define ST7789_CYAN    0x07FF
#define ST7789_MAGENTA 0xF81F
#define ST7789_YELLOW  0xFFE0
#define ST7789_WHITE   0xFFFF
#define ST7789_TAN     0xED01
#define ST7789_GREY    0x9CD1
#define ST7789_BROWN   0x6201
#define ST7789_DGREEN  0x01C0
#define ST7789_ORANGE  0xFC00
#define ST7789_NAVY    0x000F
#define ST7789_DCYAN   0x03EF
#define ST7789_MAROON  0x7800
#define ST7789_PURPLE  0x780F
#define ST7789_OLIVE   0x7BE0
#define ST7789_LGREY   0xC618
#define ST7789_DGREY   0x7BEF
#define ST7789_GYELLOW 0xAFE5
#define ST7789_PINK    0xFC18

// GPIO	Abstractions , for portability purposes
#define TFT_DC_INIT gpio_init(_TFT_DC)
#define TFT_RST_INIT gpio_init(_TFT_RST)
#define TFT_CS_INIT gpio_init(_TFT_CS)
#define TFT_SCLK_INIT gpio_init(_TFT_SCLK)
#define TFT_SDATA_INIT gpio_init(_TFT_SDATA)

#define TFT_DC_SetHigh gpio_put(_TFT_DC, true)
#define TFT_DC_SetLow gpio_put(_TFT_DC, false)
#define TFT_RST_SetHigh gpio_put(_TFT_RST, true)
#define TFT_RST_SetLow gpio_put(_TFT_RST, false)
#define TFT_CS_SetHigh gpio_put(_TFT_CS, true)
#define TFT_CS_SetLow gpio_put(_TFT_CS, false)
#define TFT_SCLK_SetHigh gpio_put(_TFT_SCLK, true)
#define TFT_SCLK_SetLow gpio_put(_TFT_SCLK, false)
#define TFT_SDATA_SetHigh gpio_put(_TFT_SDATA, true)
#define TFT_SDATA_SetLow gpio_put(_TFT_SDATA, false)

#define TFT_DC_SetDigitalOutput gpio_set_dir(_TFT_DC, GPIO_OUT)
#define TFT_RST_SetDigitalOutput gpio_set_dir(_TFT_RST, GPIO_OUT)
#define TFT_CS_SetDigitalOutput gpio_set_dir(_TFT_CS, GPIO_OUT)
#define TFT_SCLK_SetDigitalOutput gpio_set_dir(_TFT_SCLK, GPIO_OUT)
#define TFT_SDATA_SetDigitalOutput gpio_set_dir(_TFT_SDATA, GPIO_OUT)

#define TFT_SCLK_SPI_FUNC gpio_set_function(_TFT_SCLK, GPIO_FUNC_SPI)
#define TFT_SDATA_SPI_FUNC gpio_set_function(_TFT_SDATA, GPIO_FUNC_SPI)

// Delays
#define TFT_MILLISEC_DELAY busy_wait_ms
#define TFT_MICROSEC_DELAY busy_wait_us

/*!
	@brief Class to control ST7789 TFT basic functionality.
*/

class ST7789_TFT : public ST7789_TFT_graphics
{

public:
	ST7789_TFT();
	~ST7789_TFT(){};

	//  Enums


	/*! TFT rotate modes in degrees*/
	enum TFT_rotate_e : uint8_t
	{
		TFT_Degrees_0 = 0, /**< No rotation 0 degrees*/
		TFT_Degrees_90,	   /**< Rotation 90 degrees*/
		TFT_Degrees_180,   /**< Rotation 180 degrees*/
		TFT_Degrees_270	   /**< Rotation 270 degrees*/
	};

	TFT_rotate_e TFT_rotate = TFT_Degrees_0; /**< Enum to hold rotation */

	virtual void TFTsetAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;

	void TFTSetupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t);
	void TFTInitScreenSize(uint16_t xOffset, uint16_t yOffset, uint16_t w, uint16_t h);
	void TFTST7789Initialize(void);
	void TFTInitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void TFTInitSPIType(uint16_t CommDelay);
	void TFTPowerDown(void);

	void TFTsetRotation(TFT_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTpartialDisplay(bool m);
	void TFTenableDisplay(bool m);
	void TFTidleDisplay(bool m);
	void TFTsleepDisplay(bool m);
	void TFTNormalMode(void);

	uint16_t TFTLibVerNumGet(void);
	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);

private:
	void TFTHWSPIInitialize(void);
	void TFTResetPIN(void);
	void cmd89(void);
	void AdjustWidthHeight(void);

	const uint16_t _LibVersionNum = 101; /**< library version number eg 171 1.7.1*/

}; // end of class

// ********************** EOF *********************