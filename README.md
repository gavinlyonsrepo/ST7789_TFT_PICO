
# This project has been amalgamated into [URL LINK](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO)




## Overview

* Name: ST7789_TFT_PICO

* Description:

0. Library for a TFT SPI LCD, ST7789 Driver
1. Raspberry pi PICO RP2040 library.
2. Inverse colour, rotate, sleep modes supported.
3. 12 fonts included.
4. Graphics + print class included.
5. bi-color, 16 bit and 24 colour Bitmaps supported.
6. Hardware and software SPI

* Author: Gavin Lyons
* Developed on Toolchain:
	1. Raspberry pi PICO RP2040
	2. SDK(2.1.0) C++
	3. compiler G++ for arm-none-eabi(13.2.1)
	4. CMAKE(VERSION 3.18) , VScode(1.95.3)
	5. Linux Mint 22.1


## Test

There are  example files included. User picks the one they want 
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| Filename  | Function  | Note |
| --- | --- | --- |
| ST7789_TFT_HELLO | Hello world  | --- |
| ST7789_TFT_TEXT | Text  + fonts | --- |
| ST7789_TFT_GRAPHICS| Graphics | --- |
| ST7789_TFT_FUNCTIONS_FPS| Functions(like rotate scroll) + FPS test | --- |
| ST7789_TFT_BMP_DATA | 1, 16 & 24 bit colour bitmaps tests +  FPS test | Bitmap data is stored in arrays on PICO |


## Software


### User Options

In the main.cpp example files. There are four sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.


0. USER OPTION 0 SPI_SPEED + TYPE 
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION 


*USER OPTION 0 SPI SPEED* 

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz. There is a file with SPI test results for the FPS tests in extra/doc folder. 2nd parameter is the SPI interface(spi0 spi1 etc). 

If users wants software SPI just call this method 
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI 
which may be beneficial in  some setups.   

*USER OPTION 1 GPIO*

The 5 GPIO pins used, the clock and data lines must be the clock and data lines 
of spi interface chosen in option 0 if using hardware SPI.

*USER OPTION 2 Screen size  + Offsets*

In the main.cpp file, in USER OPTION 2 .
User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function TFTInitScreenSize sets them.

### Fonts

Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Size Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0-0xFF, Full Extended|1275 |Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|
| 11 | $_Mia| 8x16 | 0x20-0x7E |1520|N|
| 12 | $_Dedica | 6x12 |0x20-0x7E |1152|N|

1. $ = TFTFont
2. A print class is available to print out many data types.
3. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
4. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
5. Fonts 9-12 Alphanumeric fonts and cannot be scaled(just one size)
These fonts are optional and can be removed from program
by commenting out the relevant TFT_OPTIONAL_FONT_X define in the  ST7789_TFT_Font.hpp file

Font Methods:

| Font num | Method | Size parameter | Notes |
| ------ | ------ | ------ |  ------ |
| 1-6 | drawChar|Y| draws single  character |
| 1-6 | drawText |Y| draws character array |
| 7-12 | drawChar|N| draws single  character |
| 7-12 | drawText|N| draws character array |
| 1-12 | print |~| Polymorphic print class which will print out many data types |

These functions return an error code in event of an error.

### Bitmap

Functions to support drawing bitmaps, 

| Num | Function Name | Colour support | test bitmap data size |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | TFTdrawIcon | bi-colour | (8 x (0-Max_y))  | Data vertically addressed |
| 2 | TFTdrawBitmap | bi-colour | 2048 bytes  | Data horizontally  addressed |
| 3 | TFTdrawBitmap16Data | 16 bit color 565  | 32768  | Data from array on PICO |
| 4 | TFTdrawBitmap24Data  | 24 bit color  | 49152  | Data from array on PICO, Converted by software to 16-bit color  |
| 5 | TFTdrawSpriteData  | 16 bit color  565 | 32768  | Data from array on PICO, Draws background color transparent | 


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 2-5  assumes 128x128 bitmap.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 and 5 is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

These functions will return an error code in event of an error.

## Hardware

Connections as setup in main.cpp  test file.

| TFT PinNum | Pindesc |  HW SPI |
| --- | --- | --- | 
| 1 | LED | VCC |   
| 2 | SCLK | GPIO18 |
| 3 | SDA | GPIO19 |
| 4 | A0/DC |  GPIO3  |
| 5 | RESET |   GPIO17 |
| 6 | SS/CS |  GPIO2 |
| 7 | GND | GND |
| 8 | VCC |  VCC  |

1. NOTE connect LED backlight pin 1 thru a resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
5. Backlight on/off control is left to user.

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)


