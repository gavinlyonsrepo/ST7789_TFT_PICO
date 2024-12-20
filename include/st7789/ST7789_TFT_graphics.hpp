/*!
	@file     ST7789_TFT_graphics.hpp
	@author   Gavin Lyons
	@brief    Library header file for ST7789_TFT_PICO graphics library.
			  This file handles the graphic methods
*/

#pragma once

// Section: Includes
#include <cstring> //strlen
#include <cstdio>
#include <cstdlib>
#include "hardware/spi.h"
#include "ST7789_TFT_Print.hpp"

// Section defines

/*! Used in graphic shape methods*/
#define _swap_TFT(a, b) \
	{                   \
		int16_t t;      \
		t = a;          \
		a = b;          \
		b = t;          \
	}

/*! Enum to define a standard return code for most functions that return failures*/
enum Display_Return_Codes_e : uint8_t
{
	Display_Success = 0,                 /**< Success, Function ran without defined Error*/
	Display_Reserved = 1,                /**< Reserved */
	Display_WrongFont = 2,               /**< Wrong Font selected*/
	Display_CharScreenBounds = 3,        /**< Text Character is out of Screen bounds, Check x and y*/
	Display_CharFontASCIIRange = 4,      /**< Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.*/
	Display_CharArrayNullptr = 5,        /**< Text Character Array is an invalid pointer object*/
	Display_FontPtrNullptr = 6,          /**< Pointer to the font is an invalid pointer object*/
	Display_FontNotEnabled = 7,          /**< Is the font you selected enabled in _font.hpp? */
	Display_BitmapNullptr = 8,           /**< The Bitmap data array is an invalid pointer object*/
	Display_BitmapScreenBounds = 9,      /**< The Bitmap starting point is outside screen bounds, check x and y*/
	Display_BitmapLargerThanScreen = 10, /**< The Bitmap is larger than screen, check  w and h*/
	Display_BitmapVerticalSize = 11,     /**< A vertical Bitmap's height must be divisible by 8.*/
	Display_BitmapHorizontalSize = 12,   /**< A horizontal Bitmap's width must be divisible by 8*/
	Display_BitmapSize = 13,             /**< Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h*/
	Display_BufferSize = 14,             /**< Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)*/
	Display_BufferNullptr = 15,          /**< The Buffer data array is an invalid pointer object*/
	Display_ShapeScreenBounds = 16,		 /**< Shape is outside screen bounds, check x and y */
	Display_IconScreenWidth = 17,        /**< Icon is greater than screen width , check width w value */
	Display_GenericError = 18            /**< Generic Error */
};

// Section: Classes

/*!
	@brief Class to handle fonts and graphics of ST7789 display
 */
class ST7789_TFT_graphics : public Print
{

public:
	ST7789_TFT_graphics();
	~ST7789_TFT_graphics(){};

	// Enums
	/*! Font type 1-12 */
	enum TFT_Font_Type_e : uint8_t
	{
		TFTFont_Default = 1,	/**< Default Font, Full extended ASCII */
		TFTFont_Thick = 2,		/**< Thick font , no lower case letters*/
		TFTFont_Seven_Seg = 3,	/**< Seven Segment Font */
		TFTFont_Wide = 4,		/**< Wide font no lowercase letters*/
		TFTFont_Tiny = 5,		/**< Tiny font */
		TFTFont_HomeSpun = 6,	/**< HomeSpun Font */
		TFTFont_Bignum = 7,		/**< Bignum numbers only */
		TFTFont_Mednum = 8,		/**< Mednum number only */
		TFTFont_ArialRound = 9, /**< Arial round font */
		TFTFont_ArialBold = 10, /**< Arial bold font */
		TFTFont_Mia = 11,		/**< Mia font */
		TFTFont_Dedica = 12		/**< dedica font */
	};

	virtual size_t write(uint8_t);
	virtual  void TFTsetAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) = 0 ;
	void TFTfillScreen(uint16_t color);
	void TFTsetCursor(int16_t x, int16_t y);

	// Shapes and lines
	void TFTdrawPixel(uint16_t, uint16_t, uint16_t);
	void TFTdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void TFTdrawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
	void TFTdrawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

	void TFTdrawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	Display_Return_Codes_e  TFTfillRectBuffer(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
	void TFTfillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	void TFTdrawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	void TFTfillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);

	void TFTdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void TFTfillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

	void TFTdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void TFTfillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	// Text
	void TFTsetTextWrap(bool w);
	void TFTFontNum(TFT_Font_Type_e FontNumber);
	Display_Return_Codes_e  TFTdrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
	Display_Return_Codes_e  TFTdrawText(uint16_t x, uint16_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);
	Display_Return_Codes_e  TFTdrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg);
	Display_Return_Codes_e  TFTdrawText(uint16_t x, uint16_t y, char *pText, uint16_t color, uint16_t bg);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextSize(uint8_t s);

	// Bitmap & Icon
	Display_Return_Codes_e  TFTdrawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);
	Display_Return_Codes_e  TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, uint8_t *pBmp, uint16_t sizeOfBitmap);
	Display_Return_Codes_e  TFTdrawBitmap24Data(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h);
	Display_Return_Codes_e  TFTdrawBitmap16Data(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h);
	Display_Return_Codes_e  TFTdrawSpriteData(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h, uint16_t backgroundColor);


protected:
	void pushColor(uint16_t color);
	uint16_t Color565(uint16_t, uint16_t, uint16_t);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

	void writeCommand(uint8_t);
	void writeData(uint8_t);
	void spiWrite(uint8_t);
	void spiWriteSoftware(uint8_t spidata);
	void spiWriteDataBuffer(uint8_t *spidata, uint32_t len);

	bool _wrap = true;				/**< wrap text around the screen on overflow*/
	uint16_t _textcolor = 0xFFFF;	/**< ForeGround color for text*/
	uint16_t _textbgcolor = 0x0000; /**< BackGround color for text*/
	uint8_t _textSize = 1;			/**< Size of text , fonts 1-6 only*/

	int16_t _cursorX = 0;	  /**< Current pixel column postion of Cursor*/
	int16_t _cursorY = 0;	  /**< Current pixel row position of Cursor*/

	uint16_t _colstart; /**< Records column offset start postion, never change after first init */
	uint16_t _rowstart; /**< Records row offset start postion, never change after first init */
	uint16_t _colstart2 = 0;   /**< Offset from the right for ST7789*/
	uint16_t _rowstart2 = 0;   /**< Offset from the bottom for ST7789*/
	uint16_t _XStart;   /**< Records column offset postion */
	uint16_t _YStart;   /**< Records row offset postion */

	uint16_t _widthTFT;		  /**< Records width TFT postion */
	uint16_t _heightTFT;	  /**< Records Height TFT postion */
	uint16_t _widthStartTFT;  /**< Records width TFT postion never change after first init*/
	uint16_t _heightStartTFT; /**< Records Height TFT postion never change after first init */

	int8_t _TFT_DC;	   /**< GPIO Data or command line */
	int8_t _TFT_RST;   /**< GPIO Reset line */
	int8_t _TFT_CS;	   /**< GPIO Chip select line */
	int8_t _TFT_SCLK;  /**< GPIO Clock SPI Line */
	int8_t _TFT_SDATA; /**< GPIO MOSI data in  SPI Line */

	bool _hardwareSPI;			  /**< True for Hardware SPI on , false fpr Software SPI on*/
	spi_inst_t *_pspiInterface;	  /**< SPI instance pointer*/
	uint16_t _speedSPIKHz;		  /**< SPI speed value in kilohertz*/
	uint16_t _SWSPIGPIODelay = 0; /**< uS GPIO Communications delay, SW SPI ONLY */
private:
	/*! Width of the font in bits each representing a bytes sized column*/
	enum TFT_Font_width_e : uint8_t
	{
		TFTFont_width_3 = 3,  /**< 3 tiny font */
		TFTFont_width_4 = 4,  /**< 4 seven segment font */
		TFTFont_width_5 = 5,  /**< 5 default font */
		TFTFont_width_6 = 6,  /**< dedica font  */
		TFTFont_width_7 = 7,  /**< 7 homespun & thick font*/
		TFTFont_width_8 = 8,  /**< 8 wide & mia font*/
		TFTFont_width_16 = 16 /**< 16 font 7-10*/
	};

	/*! font offset in the ASCII table*/
	enum TFT_Font_offset_e : uint8_t
	{
		TFTFont_offset_none = 0x00,	 /**< extended ASCII */
		TFTFont_offset_space = 0x20, /**< Starts at Space, alphanumeric */
		TFTFont_offset_minus = 0x2D, /**< Starts at Minus, extended numeric */
		TFTFont_offset_zero = 0x30	 /**< Starts at zero, numeric */
	};

	/*! Height of the font in bits*/
	enum TFT_Font_height_e : uint8_t
	{
		TFTFont_height_8 = 8,	/**< 8 bit font 1-6 at size 1*/
		TFTFont_height_12 = 12, /**< 12 bit font 12 */
		TFTFont_height_16 = 16, /**< 16 bit font 8, 10 11*/
		TFTFont_height_24 = 24, /**< 24 bit font 9 */
		TFTFont_height_32 = 32	/**< 32 bit font 7 */
	};

	/*! Number of ASCII characters in Font */
	enum TFTFontLength_e : uint8_t
	{
		TFTFontLenNumeric = 14,			/**< extended Numeric 0x2D - 0x3A */
		TFTFontLenAlphaNumNoLCase = 59, /**< reduced Alphanumeric 0x20-0x5A*/
		TFTFontLenAlphaNum = 95,		/**< Full Alphanumeric 0x20-0x7E */
		TFTFontLenAll = 255				/**< Full Range  0-0xFF */
	};

	uint8_t _FontNumber = 1;		  /**< Store current font */
	uint8_t _CurrentFontWidth = 5;	  /**< Store current font width */
	uint8_t _CurrentFontoffset = 0;	  /**< Store current offset width */
	uint8_t _CurrentFontheight = 8;	  /**< Store current offset height */
	uint8_t _CurrentFontLength = 254; /**<Store current font number of characters */
};

// ********************** EOF *********************
