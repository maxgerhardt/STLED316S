/******************************************************************************
 * @file STLED316S.h
 * @brief Arduino Library for STLED316S LED controller with keyscan (Header)
 * @author David Leval
 * @version 1.0.2
 * @date 17/06/2021
 * 
 * Resources:
 * Uses SPI.h for SPI operation
 *
 * The STLED316S is a LED controller and driver that interface microprocessors 
 * to 7-segments LED displays through a serial 3-wire interface (compatible SPI)\n
 * Features :\n
 * - LED driver with 14 outputs (8 segments/6 digits common-anode)
 * - A single LED digit output (DIG1_LED) can be used to drive up to 8 discrete LEDs
 * - 8-step dimming circuit to control brightness of individual LEDs for LED digit
 * - Power 5VDC / can operate with 3.3 V interface voltages
 * \n
 * 
 * Release :
 * 		- v1.0.0 (08/04/2020) : Initial version
 * 		- v1.0.1 (02/06/2021) : Addition of a private variable to save the state of the LEDs 
 * 								(Contribution of Giovani Luis Franco)
 * 		- v1.0.2 (17/06/2021) : Fix vtable linker error (default implementation of virtual functions )
 * 
 * STLED316S library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * <http://www.gnu.org/licenses/>.
 * 
 ******************************************************************************/

#ifndef __STLED316S_h
#define __STLED316S_h

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/

#ifdef ARDUINO
  #if (ARDUINO >= 100)
  #include <Arduino.h>
  #else
  #include <WProgram.h>
  #include <pins_arduino.h>
#endif

#endif
#include <stdint.h>
#include <SPI.h>  // Arduino SPI library

/******************************************************************************/
/* Platform specificity                                                       */
/******************************************************************************/
/*#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) //Arduino Uno
 	//
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__) //Arduino Leonardo
	//
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) //Arduino Mega
	//
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAMD_ZERO) //Arduino Due or Zero
	//
#elif defined(ARDUINO_ARCH_STM32F0) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32L4) //STM32
	//
#elif defined(ARDUINO_ARCH_ESP32)
	//
#elif defined(ARDUINO_ARCH_ESP8266)
	//
#else
  #error Unsupported Hardware.
#endif*/


/******************************************************************************/
/* Configuration                                                              */
/******************************************************************************/
#define STLED316S_DEFAULT_BRIGHTNESS 	2      //!< Brightness Default (0 to 7)

/******************************************************************************/
/* STLED316S Register                                                         */
/******************************************************************************/
#define STLED316S_DISP_ON_CMD   0x0D
#define STLED316S_DISP_OFF_CMD  0x0E
    
#define STLED316S_DATA_WR      0x00
#define STLED316S_DATA_RD      0x40
#define STLED316S_ADDR_INC     0x00
#define STLED316S_ADDR_FIXED   0x20
#define STLED316S_CONF1_PAGE   0x10
#define STLED316S_DIG_PAGE     0x00
#define STLED316S_LED_PAGE     0x08
#define STLED316S_BRT_LED_PAGE 0x18
#define STLED316S_READ_PAGE    0x08

#define STLED316S_ADDR_LED_DATA			0x00
#define STLED316S_ADDR_KEY_DATA1		0x01
#define STLED316S_ADDR_KEY_DATA2		0x02

#define STLED316S_CONF_BRT_CONSTANT     0x18
#define STLED316S_CONF_BRT_VARIABLE     0x00

#define SEG1	0x01
#define SEG2	0x02
#define SEG3	0x04
#define SEG4	0x08
#define SEG5	0x10
#define SEG6	0x20
#define SEG7	0x40
#define SEG8	0x80


/******************************************************************************/
/* Typedef                                                                    */
/******************************************************************************/
typedef enum {
	DIGITall = 0,
	DIGITn1 = 1,
	DIGITn2 = 2,
	DIGITn3 = 3,
	DIGITn4 = 4,
	DIGITn5 = 5,
	DIGITn6 = 6
} DIGITnum_t;

typedef enum {
	LEDall = 0x00,
	LEDn1 = 0x01,
	LEDn2 = 0x02,
	LEDn3 = 0x04,
	LEDn4 = 0x08,
	LEDn5 = 0x10,
	LEDn6 = 0x20,
	LEDn7 = 0x40,
	LEDn8 = 0x80
} LEDnum_t;


/******************************************************************************/
/* Class declaration                                                          */
/******************************************************************************/
class STLED316S_Common
{
	private:
		uint8_t _dispDataBuffer[7]; //!< Memory buffer used for display
		uint8_t _digit_table[16] = {0x77,0x14,0xB3,0xB6,0xD4,0xE6,0xE7,0x34,0xF7,0xF6,0xF5,0xC7,0x63,0x97,0xE3,0xE1};
		uint8_t _digit_brightness[3];
		uint8_t _LED_brightness[4];
		uint8_t _digDP;
		uint8_t _LED_state;  //!< Memory of LEDs state
		uint8_t _nbrOfDigit;

	public:
		STLED316S_Common(uint8_t nbrOfDigit);
		void begin(void);
		void begin(uint8_t digA, uint8_t digB, uint8_t digC, uint8_t digD, uint8_t digE, uint8_t digF, uint8_t digG, uint8_t digDP);
		void displayON(void);
		void displayOFF(void);
		void setBrightness(DIGITnum_t DIGITnum, uint8_t brightness);
		void clearDisplay(void);
		void dispRAW(DIGITnum_t DIGITnum, uint8_t raw);
		void dispRAW(uint8_t *raw);
		void dispUdec(uint32_t nbr);
		void dispHex(uint32_t data);
		void setDP(DIGITnum_t DIGITnum, uint8_t state);
		void setBrightnessLED(LEDnum_t LEDnum, uint8_t brightness);
		void setLED(LEDnum_t LEDnum, bool state);

		virtual void writeData(uint8_t *data, uint8_t lenght) = 0;
		virtual uint8_t readData(uint8_t address) = 0;
};

class STLED316S_SPI : public STLED316S_Common
{
	private :
		uint8_t _STBpin = 10; 		//!< Chip Select Pin (STB) : Default 10
		uint8_t SwapBit(uint8_t byte);

	public:
		STLED316S_SPI(uint8_t nbrOfDigit);
		STLED316S_SPI(uint8_t nbrOfDigit, uint8_t STBpin);
		void writeData(uint8_t *data, uint8_t lenght) override;
};

class STLED316S : public STLED316S_Common
{
	private :
		uint8_t _STBpin;
		uint8_t _CLKpin;
		uint8_t _DATApin;
		void sendSW_SPI(uint8_t data);

	public:
		STLED316S(uint8_t nbrOfDigit, uint8_t STBpin, uint8_t CLKpin, uint8_t DATApin);
		void writeData(uint8_t *data, uint8_t lenght) override;
		uint8_t readData(uint8_t address) override;
};

#endif //__STLED316S_h
