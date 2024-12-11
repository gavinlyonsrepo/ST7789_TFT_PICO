/*!
	@file     ST7789_TFT.cpp
	@author   Gavin Lyons
	@brief    Source file for ST7789_TFT_PICO library. 
			Contains driver methods for ST7789_TFT display 
	@note  See URL for full details.https://github.com/gavinlyonsrepo/ST7789_TFT_PICO
		
*/

#include "../../include/st7789/ST7789_TFT.hpp"
 
/*!
	@brief Constructor for class ST7789_TFT
*/
ST7789_TFT :: ST7789_TFT(){}

/*!
	@brief : Init Hardware SPI
*/
void ST7789_TFT::TFTHWSPIInitialize(void)
{
	spi_init(_pspiInterface, _speedSPIKHz * 1000); // Initialize SPI port 
	// Initialize SPI pins : clock and data
	TFT_SDATA_SPI_FUNC;
	TFT_SCLK_SPI_FUNC;

    // Set SPI format
    spi_set_format( _pspiInterface,   // SPI instance
                    8,      // Number of bits per transfer
                    SPI_CPOL_0,      // Polarity (CPOL)
                    SPI_CPHA_0,      // Phase (CPHA)
                    SPI_MSB_FIRST);

}

/*!
	@brief: Call when powering down TFT
	@note  Will switch off SPI 
*/
void ST7789_TFT ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	TFT_DC_SetLow;
	TFT_RST_SetLow;
	TFT_CS_SetLow;
	if (_hardwareSPI == true) {
		spi_deinit(_pspiInterface);
	}else{
		TFT_SCLK_SetLow;
		TFT_SDATA_SetLow;
	}
}

/*!
	@brief: Method for Hardware Reset pin control
*/
void ST7789_TFT ::TFTResetPIN() {
	TFT_RST_SetDigitalOutput;
	TFT_RST_SetHigh;
	TFT_MILLISEC_DELAY(10);
	TFT_RST_SetLow;
	TFT_MILLISEC_DELAY(10);
	TFT_RST_SetHigh;
	TFT_MILLISEC_DELAY(10);
}

/*!
	@brief  sets up TFT GPIO
	@param rst reset GPIO 
	@param dc data or command GPIO.
	@param cs chip select GPIO 
	@param sclk Data clock GPIO  
	@param din Data to TFT GPIO 
*/
void ST7789_TFT ::TFTSetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_TFT_SDATA = din;
	_TFT_SCLK = sclk;
	_TFT_RST= rst;
	_TFT_DC = dc;
	_TFT_CS = cs;

	TFT_SDATA_INIT; 
	TFT_SCLK_INIT; 
	TFT_RST_INIT;
	TFT_DC_INIT; 
	TFT_CS_INIT; 
}

/*!
	@brief init routine for ST7789 controller
*/
void ST7789_TFT::TFTST7789Initialize() {
	TFTResetPIN();
	TFT_DC_SetDigitalOutput;
	TFT_DC_SetLow;
	TFT_CS_SetDigitalOutput;
	TFT_CS_SetHigh;
if (_hardwareSPI == false)
{
	TFT_SCLK_SetDigitalOutput;
	TFT_SDATA_SetDigitalOutput;
	TFT_SCLK_SetLow;
	TFT_SDATA_SetLow;
}else{
	TFTHWSPIInitialize();
}
	cmd89();
	AdjustWidthHeight();
	TFTsetRotation(TFT_Degrees_0);
}


/*!
	@brief Toggle the invert mode
	@param invert true invert off , false invert on
*/
void ST7789_TFT ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(ST7789_INVOFF);
	} else {
		writeCommand(ST7789_INVON);
	}
}

/*!
	@brief Toggle the partial display mode
	@param partialDisplay true  on false  off
*/
void ST7789_TFT ::TFTpartialDisplay(bool partialDisplay){
	if(partialDisplay) {
		writeCommand(ST7789_PTLON);
	} else {
		writeCommand(ST7789_NORON);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
*/
void ST7789_TFT ::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(ST7789_DISPON);
	} else {
		writeCommand(ST7789_DISPOFF);
	}
}

/*!
	@brief Toggle the sleep mode
	@param sleepMode true sleep on false sleep off
*/
void ST7789_TFT ::TFTsleepDisplay(bool sleepMode){
	if(sleepMode) {
		writeCommand(ST7789_SLPIN);
		TFT_MILLISEC_DELAY(5);
	} else {
		writeCommand(ST7789_SLPOUT);
		TFT_MILLISEC_DELAY(120);
	}
}

/*!
	@brief Toggle the idle display mode
	@param idleMode true enable on false disable
*/
void ST7789_TFT ::TFTidleDisplay(bool idleMode){
	if( idleMode) {
		writeCommand(ST7789_IDLE_ON);
	} else {
		writeCommand(ST7789_IDLE_OFF);
	}
}

/*!
	@brief return Display to normal mode
	@note used after scroll set for example
*/
void ST7789_TFT::TFTNormalMode(void){writeCommand(ST7789_NORON);}


/*!
	@brief: change rotation of display.
	@param mode TFT_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
*/
void ST7789_TFT ::TFTsetRotation(TFT_rotate_e mode) {
	uint8_t madctl = 0;
	uint8_t rotation;
	rotation = mode % 4;
	switch (rotation) {
		case TFT_Degrees_0 :
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
			_widthTFT =_widthStartTFT;
			_heightTFT = _heightStartTFT;
			_XStart = _colstart;
			_YStart = _rowstart;
			break;
		case TFT_Degrees_90:
			madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart;
			_XStart = _rowstart;
			_widthTFT  =_heightStartTFT;
			_heightTFT = _widthStartTFT;
			break;
		case TFT_Degrees_180:
			madctl = ST7789_MADCTL_RGB;
			_XStart = _colstart;
			_YStart = _rowstart;
			_widthTFT =_widthStartTFT;
			_heightTFT = _heightStartTFT;
			break;
		case TFT_Degrees_270:
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart;
			_XStart = _rowstart;
			_widthTFT =_heightStartTFT;
			_heightTFT = _widthStartTFT;
			break;
	}
	writeCommand(ST7789_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param colOffset Column offset
	@param rowOffset row offset
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void ST7789_TFT  :: TFTInitScreenSize(uint16_t colOffset, uint16_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
{
	_colstart = colOffset; 
	_rowstart = rowOffset;
	_XStart = colOffset; 
	_YStart = rowOffset;
	
	_widthTFT = width_TFT;
	_heightTFT = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
}


/*!
	@brief intialise HW SPI setup
	@param speed_Khz SPI baudrate in Khz , 1000 = 1 Mhz
	@param spi_interface Spi interface, spi0 spi1 etc
	@note method overload used , method 1 hardware SPI 
*/
void ST7789_TFT  :: TFTInitSPIType(uint32_t speed_Khz,  spi_inst_t* spi_interface) 
{
	 _pspiInterface = spi_interface;
	_speedSPIKHz = speed_Khz;
	_hardwareSPI = true;
}

/*!
	@brief intialise SW SPI set
	@param CommDelay SW SPI GPIO delay
	@note method overload used , method 2 software SPI 
*/
void ST7789_TFT ::TFTInitSPIType(uint16_t CommDelay) 
{
	TFTSwSpiGpioDelaySet(CommDelay);
	_hardwareSPI = false;
}

/*!
	@brief Library version number getter
	@return The lib version number eg 171 = 1.7.1
*/
uint16_t ST7789_TFT::TFTLibVerNumGet(void) {return _LibVersionNum;}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ST7789_TFT::TFTSwSpiGpioDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ST7789_TFT::TFTSwSpiGpioDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}


/*!
	@brief Command Initialization sequence for ST7789 display
*/
void ST7789_TFT::cmd89(void)
{
	uint8_t CASETsequence[] {0x00, 0x00, uint8_t(_widthStartTFT  >> 8),  uint8_t(_widthStartTFT  & 0xFF)};
	uint8_t RASETsequence[] {0x00, 0x00, uint8_t(_heightStartTFT >> 8) , uint8_t(_heightStartTFT & 0XFF)};
	
	writeCommand(ST7789_SWRESET);
	TFT_MILLISEC_DELAY (150);
	writeCommand(ST7789_SLPOUT);
	TFT_MILLISEC_DELAY (500);
	writeCommand(ST7789_COLMOD); //Set color mode
	writeData(0x55); // 16 bit color
	TFT_MILLISEC_DELAY (10);

	writeCommand(ST7789_MADCTL); // Mem access ctrl (directions)
	writeData(0x08); // Row/col address, top-bottom refresh

	writeCommand(ST7789_CASET);  //Column address set
	spiWriteDataBuffer(CASETsequence, sizeof(CASETsequence));
	writeCommand(ST7789_RASET);  //Row address set
	spiWriteDataBuffer(RASETsequence, sizeof(RASETsequence));

	writeCommand(ST7789_INVON);
	TFT_MILLISEC_DELAY (10);
	writeCommand(ST7789_NORON);
	TFT_MILLISEC_DELAY (10);
	writeCommand(ST7789_DISPON);
	TFT_MILLISEC_DELAY (10);
}

/*!
	@brief Initialization  width and height code common to all ST7789 displays
	@note ST7789 display require an offset calculation which differs for different size displays
*/
void ST7789_TFT::AdjustWidthHeight() {
	if (_widthTFT == 240 && _heightTFT == 240) {
	// 1.3", 1.54" displays (right justified)
		_rowstart = (320 - _heightTFT);
		_rowstart2 = 0;
		_colstart = _colstart2 = (240 - _widthTFT);
	} else if (_widthTFT == 135 && _heightTFT == 240) {
	// 1.14" display (centered, with odd size)
		_rowstart = _rowstart2 = (int)((320 - _heightTFT) / 2);
		_colstart = (int)((240 - _widthTFT + 1) / 2);
		_colstart2 = (int)((240 - _widthTFT) / 2);
	} else {
	// 1.47", 1.69, 1.9", 2.0" displays (centered)
		_rowstart = _rowstart2 = (int)((320 - _heightTFT) / 2);
		_colstart = _colstart2 = (int)((240 - _widthTFT) / 2);
	}
}

/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x0 Top left corner x coordinate
  @param  y0  Top left corner y coordinate
  @param  x1  Width of window
  @param  y1  Height of window
  @note https://en.wikipedia.org/wiki/Bit_blit
 */
void ST7789_TFT::TFTsetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint8_t x0Higher = (x0 >> 8) + _XStart;
	uint8_t x0Lower  = (x0 &  0xFF) + _XStart;
	uint8_t y0Higher = (y0 >> 8) + _YStart;
	uint8_t y0Lower  = (y0 &  0xFF) + _YStart;
	uint8_t x1Higher = (x1 >> 8) + _XStart;
	uint8_t x1Lower  = (x1 &  0xFF) + _XStart;
	uint8_t y1Higher = (y1 >> 8) + _YStart;
	uint8_t y1Lower  = (y1 &  0xFF) + _YStart;
	uint8_t seqCASET[]    {x0Higher ,x0Lower,x1Higher,x1Lower};
	uint8_t seqRASET[]    {y0Higher,y0Lower,y1Higher,y1Lower};
	writeCommand(ST7789_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ST7789_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(ST7789_RAMWR); // Write to RAM*/
}
//**************** EOF *****************
