/*
 * File:   lora.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../engine.h"

#define _XTAL_FREQ	1000000UL

#define RF_RXpin     LATBbits.LATB4                  // antenna switch RX control pin
#define RF_TXpin     LATBbits.LATB3                  // antenna switch TX control pin
#define RF_RESETpin  LATCbits.LATC2                  // Reset input

#define RF_RXpin_DIR       TRISBbits.TRISB4          // direction bit for RX control line
#define RF_TXpin_DIR       TRISBbits.TRISB3          // direction bit for TX control line
#define RF_RESETpin_DIR    TRISCbits.TRISC2          // direction bit for Reset

#define PAYLOAD_LENGTH 7                             // for transmission: number of bytes to transmit
                                                     // (this value will be stored before transmission in REG_PAYLOAD_LENGTH_LORA register of SX1272 chip)
#define POUT 14                                      // output power (in dBm)
                                                     // (used to compute the data to store in REG_PA_CONFIG register during configuration of SX1272 chip)

#define SPI_SCK_DIR             TRISDbits.TRISD0
#define SPI_MISO_DIR            TRISDbits.TRISD1
#define SPI_MOSI_DIR            TRISDbits.TRISD4
//
// Slave Select is wired on E0
//
#define SPI_SS_DIR              TRISCbits.TRISC0
#define SPI_SS_LAT              LATCbits.LATC0

#define SPI_SS_DISABLE          1
#define SPI_SS_ENABLE           0

#define        REG_FIFO        					0x00
#define        REG_OP_MODE        				0x01
#define        REG_BITRATE_MSB    				0x02
#define        REG_BITRATE_LSB    				0x03
#define        REG_FDEV_MSB   					0x04
#define        REG_FDEV_LSB    					0x05
#define        REG_FRF_MSB    					0x06
#define        REG_FRF_MID    					0x07
#define        REG_FRF_LSB    					0x08
#define        REG_PA_CONFIG    				0x09
#define        REG_PA_RAMP    					0x0A
#define        REG_OCP    						0x0B
#define        REG_LNA    						0x0C
#define        REG_RX_CONFIG    				0x0D
#define        REG_FIFO_ADDR_PTR  				0x0D
#define        REG_RSSI_CONFIG   				0x0E
#define        REG_FIFO_TX_BASE_ADDR 		    0x0E
#define        REG_RSSI_COLLISION    			0x0F
#define        REG_FIFO_RX_BASE_ADDR   			0x0F
#define        REG_RSSI_THRESH    				0x10
#define        REG_FIFO_RX_CURRENT_ADDR   		0x10
#define        REG_RSSI_VALUE_FSK	    		0x11
#define        REG_IRQ_FLAGS_MASK    			0x11
#define        REG_RX_BW		    			0x12
#define        REG_IRQ_FLAGS	    			0x12
#define        REG_AFC_BW		    			0x13
#define        REG_RX_NB_BYTES	    			0x13
#define        REG_OOK_PEAK	    				0x14
#define        REG_RX_HEADER_CNT_VALUE_MSB  	0x14
#define        REG_OOK_FIX	    				0x15
#define        REG_RX_HEADER_CNT_VALUE_LSB  	0x15
#define        REG_OOK_AVG	 					0x16
#define        REG_RX_PACKET_CNT_VALUE_MSB  	0x16
#define        REG_RX_PACKET_CNT_VALUE_LSB  	0x17
#define        REG_MODEM_STAT	  				0x18
#define        REG_PKT_SNR_VALUE	  			0x19
#define        REG_AFC_FEI	  					0x1A
#define        REG_PKT_RSSI_VALUE	  			0x1A
#define        REG_AFC_MSB	  					0x1B
#define        REG_RSSI_VALUE_LORA	  			0x1B
#define        REG_AFC_LSB	  					0x1C
#define        REG_HOP_CHANNEL	  				0x1C
#define        REG_FEI_MSB	  					0x1D
#define        REG_MODEM_CONFIG1	 		 	0x1D
#define        REG_FEI_LSB	  					0x1E
#define        REG_MODEM_CONFIG2	  			0x1E
#define        REG_PREAMBLE_DETECT  			0x1F
#define        REG_SYMB_TIMEOUT_LSB  			0x1F
#define        REG_RX_TIMEOUT1	  				0x20
#define        REG_PREAMBLE_MSB_LORA  			0x20
#define        REG_RX_TIMEOUT2	  				0x21
#define        REG_PREAMBLE_LSB_LORA  			0x21
#define        REG_RX_TIMEOUT3	 				0x22
#define        REG_PAYLOAD_LENGTH_LORA		 	0x22
#define        REG_RX_DELAY	 					0x23
#define        REG_MAX_PAYLOAD_LENGTH 			0x23
#define        REG_OSC		 					0x24
#define        REG_HOP_PERIOD	  				0x24
#define        REG_PREAMBLE_MSB_FSK 			0x25
#define        REG_FIFO_RX_BYTE_ADDR 			0x25
#define        REG_PREAMBLE_LSB_FSK 			0x26
#define        REG_SYNC_CONFIG	  				0x27
#define        REG_SYNC_VALUE1	 				0x28
#define        REG_SYNC_VALUE2	  				0x29
#define        REG_SYNC_VALUE3	  				0x2A
#define        REG_SYNC_VALUE4	  				0x2B
#define        REG_SYNC_VALUE5	  				0x2C
#define        REG_SYNC_VALUE6	  				0x2D
#define        REG_SYNC_VALUE7	  				0x2E
#define        REG_SYNC_VALUE8	  				0x2F
#define        REG_PACKET_CONFIG1	  			0x30
#define        REG_PACKET_CONFIG2	  			0x31
#define        REG_DETECT_OPTIMIZE              0x31
#define        REG_PAYLOAD_LENGTH_FSK			0x32
#define        REG_NODE_ADRS	  				0x33
#define        REG_INVERT_IQ	  				0x33
#define        REG_BROADCAST_ADRS	 		 	0x34
#define        REG_FIFO_THRESH	  				0x35
#define        REG_SEQ_CONFIG1	  				0x36
#define        REG_SEQ_CONFIG2	  				0x37
#define        REG_DETECTION_THRESHOLD          0x37
#define        REG_TIMER_RESOL	  				0x38
#define        REG_TIMER1_COEF	  				0x39
#define        REG_SYNC_WORD	  				0x39
#define        REG_TIMER2_COEF	  				0x3A
#define        REG_IMAGE_CAL	  				0x3B
#define        REG_TEMP		  					0x3C
#define        REG_LOW_BAT	  					0x3D
#define        REG_IRQ_FLAGS1	  				0x3E
#define        REG_IRQ_FLAGS2	  				0x3F
#define        REG_DIO_MAPPING1	  				0x40
#define        REG_DIO_MAPPING2	  				0x41
#define        REG_VERSION	  					0x42
#define        REG_AGC_REF	  					0x43
#define        REG_AGC_THRESH1	  				0x44
#define        REG_AGC_THRESH2	  				0x45
#define        REG_AGC_THRESH3	  				0x46
#define        REG_PLL_HOP	  					0x4B
#define        REG_TCXO		  					0x58
#define        REG_PA_DAC		  				0x5A
#define        REG_PLL		  					0x5C
#define        REG_PLL_LOW_PN	  				0x5E
#define        REG_FORMER_TEMP	  				0x6C
#define        REG_BIT_RATE_FRAC	  			0x70

#define        LORA_SLEEP_MODE                  0x80
#define        LORA_STANDBY_MODE                0x81
#define        LORA_TX_MODE                     0x83
#define        LORA_RX_CONTINUOUS_MODE          0x85
#define        LORA_RX_SINGLE_MODE              0x86
#define        LORA_CAD_MODE                    0x87
#define        LORA_STANDBY_FSK_REGS_MODE       0xC1

#define        FSK_SLEEP_MODE                   0x00
#define        FSK_STANDBY_MODE                 0x01
#define        FSK_TX_MODE                      0x03
#define        FSK_RX_MODE                      0x05

void InitRFLoRaPins(void);                          // configure pins for RF Solutions module
void ResetRFModule(void);                           // configure pins for RF Solutions module
void AntennaTX(void);                               // connect antenna to module output
void AntennaRX(void);                               // connect antenna to module input

void SPIInit(void);                                                         // init SPI in master mode
void SPITransfer (uint8_t data_out);                                        // send a byte
uint8_t SPIReceive (uint8_t data_out);                                      // receive a byte and send another byte

void WriteSXRegister(uint8_t address, uint8_t data);        // write data in a SX1272 register
uint8_t ReadSXRegister(uint8_t address);                    // read data which is in a SX1272 register
void GetMode (void);                                        // read operating mode
void InitModule (void);                                     // initialize the module (for TX or RX)
void PrintSXRegContent(uint8_t address);                    // read SX1272 register and send its content to serial output
void CheckConfiguration (void);                             // for debugging purpose: read configuration registers and send information to serial output
        
uint8_t UARTReadByte(void);                 // read a byte from UART
void UARTWriteByte(uint8_t data);           // send a byte to UART
void UARTWriteStr(char *Str);               // output strings to UART
void UARTWriteStrLn(char *Str);             // output strings to UART
void UARTWriteByteHex(uint8_t data);        // send the hexadecimal value of a byte so that it is readable in a terminal window
void UARTWriteByteDec(uint8_t data);        // send the decimal value of a byte so that it is readable in a terminal window

void WriteSXRegister(uint8_t address, uint8_t data);        // write data in a SX1272 register
uint8_t ReadSXRegister(uint8_t address);                    // read data which is in a SX1272 register
void GetMode (void);                                        // read operating mode
void InitModule (void);                                     // initialize the module (for TX or RX)
void PrintSXRegContent(uint8_t address);                    // read SX1272 register and send its content to serial output
                             
// init UART with specified baud rate
void UARTInit(uint16_t baudRate) {

    TRISCbits.TRISC6 = 1;                     // RC6 is TxD (but should be declared first as input)
    TRISCbits.TRISC7 = 1;                     // RC7 is RxD
    ANSELCbits.ANSC7 = 0;                 // disable analog input

    TXSTA1 = 0;                             // reset USART registers
    RCSTA1 = 0;

    TXSTA1bits.SYNC = 0;                    // asynchronous mode
    TXSTA1bits.TX9 = 0;                     // 8-bit mode
    RCSTA1bits.RX9 = 0;
    RCSTA1bits.CREN = 1;                      // continous receive
    RCSTA1bits.ADDEN = 0;                       // address detect disable
//    RCSTA1bits.ADDEN = SET;                     // address detect enable
    PIE1bits.RC1IE = 1;                       // interrupt on receive
    PIE1bits.TX1IE = 0;                     // no interrupt on transmission

    TXSTA1bits.BRGH = 1;                      // baud rate select (high speed)
    BAUDCON1bits.BRG16 = 1;                   // see PIC18F44K22 Silicon Errata (5. Module: EUSART, page 2)
    //SPBRG1 = (UINT8_T)(((_XTAL_FREQ / (4 * baudRate)) - 1));
    //SPBRG1 = 25;                                // baudrate = 9600 bps
    SPBRG1 = 12;                                // baudrate = 19200 bps
    SPBRGH1 = 0;

//    SPBRG1 = (UINT8_T)((_XTAL_FREQ / baudRate) / 16 - 1);   // set baud rate divider

    TXSTA1bits.TXEN = 1;                   // enable transmitter
    RCSTA1bits.SPEN = 1;                   // enable serial port

    RCIF = 0;                               // reset RX pin flag
//    RCIP = CLEAR;                               // not high priority
    RCIE = 1;                              // enable RX interrupt
    PEIE = 1;                              // enable pheripheral interrupt (serial port is a pheripheral)
}

void UARTWriteByte(uint8_t data) {

    while (TXSTA1bits.TRMT == 0);           // wait for transmission idle

    TXREG1 = data;
}

uint8_t UARTReadByte(void)
{
    return(RCREG1);
}

void UARTWriteStr(char *string)
{
  uint8_t i = 0;

  while (string[i])
    UARTWriteByte(string[i++]);
}

void UARTWriteStrLn(char *string)
{
  uint8_t i = 0;

  while (string[i])
    UARTWriteByte(string[i++]);
  
  UARTWriteByte(0x0D);      // write Carriage Return
  UARTWriteByte(0x0A);      // write Line Feed (New Line)
}

void UARTWriteByteHex(uint8_t data)
{
    char *hexa = "0123456789ABCDEF";
    
    UARTWriteByte(hexa[data / 16]);     // write ASCII value of hexadecimal high nibble
    UARTWriteByte(hexa[data % 16]);     // write ASCII value of hexadecimal low nibble
    UARTWriteByte(0x0D);      // write Carriage Return
    UARTWriteByte(0x0A);      // write Line Feed (New Line))
}

void UARTWriteByteDec(uint8_t data)
{

    UARTWriteByte((data / 100) + '0');              // write ASCII value of hundreds digit
    UARTWriteByte(((data % 100) / 10) + '0');       // write ASCII value of tenths digit
    UARTWriteByte((data % 10) + '0');       // write ASCII value of tenths digit

    UARTWriteByte(0x0D);      // write Carriage Return
    UARTWriteByte(0x0A);      // write Line Feed (New Line))
}

void InitRFLoRaPins(void) {
    RF_RESETpin_DIR = 1;             // on power-up, RESET must be high impedance, so configured as input (see SX1272 datasheet page 114)
    RF_TXpin = 0;                      // on power-up, no transmission
    RF_TXpin_DIR = 0;
    RF_RXpin = 0;                      // on power-up, no reception
    RF_RXpin_DIR = 0;
}

void ResetRFModule(void) {          // apply sequence given on SX1272 datasheet page 115:
    RF_RESETpin = 1;
    RF_RESETpin_DIR = 0;         // RESET = High for 50 ms
    __delay_ms(50);
    RF_RESETpin_DIR = 1;          // then RESET back to high-imedance
    __delay_ms(50);
}

void AntennaTX(void) {              // control antenna swith and connect antenna to RF module output
    RF_RXpin = 0;
    RF_TXpin = 0;
    __delay_ms(10);
    RF_TXpin = 1;
}

void AntennaRX(void) {              // control antenna swith and connect antenna to RF module input
    RF_RXpin = 0;
    RF_TXpin = 0;
    __delay_ms(10);
    RF_RXpin = 1;
}

void SPIInit(void) {

    SPI_SCK_DIR = 0;                     // SCK = output
    ANSELDbits.ANSD0 = 0;             // disable analog input for SCK
    SPI_MISO_DIR = 1;                     // SDI = input
    ANSELDbits.ANSD1 = 0;             // disable analog input for MISO
    SPI_MOSI_DIR = 0;                    // MOSI

    SPI_SS_DIR = 0;                      // SS = output
    SPI_SS_LAT = SPI_SS_DISABLE;            // disable SS (SS = High)

    SSP2STATbits.SMP = 0;               // SMP = 0 (input sampled at middle of output)
    SSP2STATbits.CKE = 1;                 // CKE = 1 (transmit occurs from active (1) to idle (0) SCK state, and sampling occurs from idle to active state)
    SSP2CON1 = 0b00100000;                  // WCOL not used in SPI mode, SSPOV not used in SPI master mode
                                            // SSPEN set (enable serial port), CKP cleared (idle state is low level)
                                            // SSP2CON1<3:0> = 0000 (clock = FOSC / 4) = 250kHz
}

void SPITransfer(uint8_t data_out) {        // Warning: Slave Select is not managed in this function
                                            // don't forget to control SS before and after calling this function
    uint8_t dummy_byte;
    dummy_byte = SSP2BUF;                   // clear BF (Buffer Full Status bit)                    
    PIR3bits.SSP2IF = 0;                // clear interrupt flag
    SSP2BUF = data_out;                     // transmit data
    while (!PIR3bits.SSP2IF);               // wait until transmission is complete
}

uint8_t SPIReceive(uint8_t data_out) {      // Warning: Slave Select is not managed in this function
                                            // don't forget to control SS before and after calling this function
    uint8_t data_in, dummy_byte;
    dummy_byte = SSP2BUF;                   // clear BF (Buffer Full Status bit) 
    PIR3bits.SSP2IF = 0;                // clear interrupt flag
    SSP2BUF = data_out;                     // transmit data
    while (!PIR3bits.SSP2IF);               // wait until transmission is complete
    data_in = SSP2BUF;                      // store received data
    return(data_in);
}

void WriteSXRegister(uint8_t address, uint8_t data) {

    SPI_SS_LAT = SPI_SS_ENABLE;             // enable slave
    address = address | 0x80;               // MSB of address must be high for write operation
                                            // (see SX1272 datasheet page 76)
    SPITransfer(address);
    SPITransfer(data);
    SPI_SS_LAT = SPI_SS_DISABLE;            // disable slave
}

uint8_t ReadSXRegister(uint8_t address) {
    uint8_t RegValue;
    SPI_SS_LAT = SPI_SS_ENABLE;             // enable slave
    address = address & 0x7F;               // MSB of address must be low for read operation
                                            // (see SX1272 datasheet page 76)
    SPITransfer(address);                   // send register address
    RegValue = SPIReceive(0x00);            // send dummy data and receive register content
    SPI_SS_LAT = SPI_SS_DISABLE;            // disable slave
    return RegValue;
}

// read REG_OP_MODE register to check operating mode
// and send information to serial ouput
void GetMode (void){
    uint8_t reg, masked_reg;
    reg = ReadSXRegister(REG_OP_MODE);
    
    // for debugging: send reg value to terminal
        UARTWriteStr("REG_OP_MODE = 0x");
        UARTWriteByteHex(reg);
    
    masked_reg = reg & 0x80;        // to check bit n°7
    if (masked_reg)
        {
        // MSB of RegOpMode is 1, so mode = LoRa
        masked_reg = reg & 0x40;        // to check bit n°6
        if (!masked_reg)
            UARTWriteStrLn("mode = LoRa");
        else
            UARTWriteStrLn("mode = LoRa with FSK registers access");
        }
    else
        // MSB of RegOpMode is 0, so mode = FSK
        UARTWriteStrLn("mode = FSK");
            
    masked_reg = reg & 0x07;       // test bits 2-0 of RegOpMode
        switch (masked_reg){
        case 0x00:
            UARTWriteStrLn("sleep mode");
            break;
        case 0x01:
            UARTWriteStrLn("standby mode");
            break;
        case 0x02:
            UARTWriteStrLn("frequency synthesis TX");
            break;
        case 0x03:
            UARTWriteStrLn("TX mode");
            break;
        case 0x04:
            UARTWriteStrLn("frequency synthesis RX");
            break;
        case 0x05:
            UARTWriteStrLn("continuous receive mode");
            break;
        case 0x06:
            UARTWriteStrLn("single receive mode");
            break;
        case 0x07:
            UARTWriteStrLn("Channel Activity Detection");
            break;
        }
            
}

void InitModule (void){
    
  WriteSXRegister(REG_FIFO, 0x00);

  WriteSXRegister(REG_FRF_MSB, 0xD8); // center freq = 865.2MHz
  WriteSXRegister(REG_FRF_MID, 0x4C);
  WriteSXRegister(REG_FRF_LSB, 0xCC);

  WriteSXRegister(REG_PA_CONFIG, 0x01); // out=RFIO, Pout = 0dBm

  WriteSXRegister(REG_PA_RAMP, 0x19); // low cons PLL TX&RX, 40us

  WriteSXRegister(REG_OCP, 0b00101011); //OCP enabled, 100mA

  WriteSXRegister(REG_LNA, 0b00100011); // max gain, BOOST on

  WriteSXRegister(REG_FIFO_ADDR_PTR, 0x00);     // pointer to access FIFO through SPI port (read or write)
  WriteSXRegister(REG_FIFO_TX_BASE_ADDR, 0x80);   // top half
  WriteSXRegister(REG_FIFO_RX_BASE_ADDR, 0x00);   // bottom half

  WriteSXRegister(REG_IRQ_FLAGS_MASK, 0x00);  // activate all IRQs

  WriteSXRegister(REG_IRQ_FLAGS, 0xFF);  // clear all IRQs

  // in Explicit Header mode, CRC enable or disable is not relevant in case of RX operation: everything depends on TX configuration
  WriteSXRegister(REG_MODEM_CONFIG1, 0b10001000); //BW=500k, CR=4/5, explicit header, CRC disable, LDRO disabled
  //writeRegister(REG_MODEM_CONFIG1, 0b10001010); //BW=500k, CR=4/5, explicit header, CRC enable, LDRO disabled

  WriteSXRegister(REG_MODEM_CONFIG2, 0b11000111); // SF=12, normal TX mode, AGC auto on, RX timeout MSB = 11

  WriteSXRegister(REG_SYMB_TIMEOUT_LSB, 0xFF);  // max timeout

  WriteSXRegister(REG_PREAMBLE_MSB_LORA, 0x00); // default value
  WriteSXRegister(REG_PREAMBLE_LSB_LORA, 0x08);

  WriteSXRegister(REG_MAX_PAYLOAD_LENGTH, 0x80); // half the FIFO

  WriteSXRegister(REG_HOP_PERIOD, 0x00); // freq hopping disabled

  WriteSXRegister(REG_DETECT_OPTIMIZE, 0xC3); // pour SF=12

  WriteSXRegister(REG_INVERT_IQ, 0x27); // default value, IQ not inverted

  WriteSXRegister(REG_DETECTION_THRESHOLD, 0x0A); // pour SF=12

  WriteSXRegister(REG_SYNC_WORD, 0x12);   // default value 0x12 --> 0x6F
}

// read a register and send its content to serial output
void PrintSXRegContent(uint8_t address){
    uint8_t RegValue;
    SPI_SS_LAT = SPI_SS_ENABLE;             // enable slave
    address = address & 0x7F;               // MSB of address must be low for read operation
                                            // (see SX1272 datasheet page 76)
    SPITransfer(address);                   // send register address
    RegValue = SPIReceive(0x00);            // send dummy data and receive register content
    SPI_SS_LAT = SPI_SS_DISABLE;            // disable slave
    UARTWriteByteHex(RegValue);             // send to serial output received register content
}

// check the content of configuration registers
// for debugging purpose: read configuration registers and send information to serial output
void CheckConfiguration (void){
    
    UARTWriteStrLn(" ");
    UARTWriteStrLn("configuration registers content:");
    
    UARTWriteStr("REG_FRF_MSB = 0x");
    PrintSXRegContent(REG_FRF_MSB);
    UARTWriteStr("REG_FRF_MID = 0x");
    PrintSXRegContent(REG_FRF_MID);
    UARTWriteStr("REG_FRF_LSB = 0x");
    PrintSXRegContent(REG_FRF_LSB);
    UARTWriteStr("REG_PA_CONFIG = 0x");
    PrintSXRegContent(REG_PA_CONFIG);
    UARTWriteStr("REG_PA_RAMP = 0x");
    PrintSXRegContent(REG_PA_RAMP);
    UARTWriteStr("REG_OCP = 0x");
    PrintSXRegContent(REG_OCP);
    UARTWriteStr("REG_LNA = 0x");
    PrintSXRegContent(REG_LNA);
    UARTWriteStr("REG_FIFO_ADDR_PTR = 0x");
    PrintSXRegContent(REG_FIFO_ADDR_PTR);
    UARTWriteStr("REG_FIFO_TX_BASE_ADDR = 0x");
    PrintSXRegContent(REG_FIFO_TX_BASE_ADDR);
    UARTWriteStr("REG_FIFO_RX_BASE_ADDR = 0x");
    PrintSXRegContent(REG_FIFO_RX_BASE_ADDR);
    UARTWriteStr("REG_IRQ_FLAGS_MASK = 0x");
    PrintSXRegContent(REG_IRQ_FLAGS_MASK);
    UARTWriteStr("REG_IRQ_FLAGS = 0x");
    PrintSXRegContent(REG_IRQ_FLAGS);
    UARTWriteStr("REG_MODEM_CONFIG1 = 0x");
    PrintSXRegContent(REG_MODEM_CONFIG1);
    UARTWriteStr("REG_MODEM_CONFIG2 = 0x");
    PrintSXRegContent(REG_MODEM_CONFIG2);
    UARTWriteStr("REG_SYMB_TIMEOUT_LSB = 0x");
    PrintSXRegContent(REG_SYMB_TIMEOUT_LSB);
    UARTWriteStr("REG_PREAMBLE_MSB_LORA = 0x");
    PrintSXRegContent(REG_PREAMBLE_MSB_LORA);
    UARTWriteStr("REG_PREAMBLE_LSB_LORA = 0x");
    PrintSXRegContent(REG_PREAMBLE_LSB_LORA);
    UARTWriteStr("REG_MAX_PAYLOAD_LENGTH = 0x");
    PrintSXRegContent(REG_MAX_PAYLOAD_LENGTH);
    UARTWriteStr("REG_HOP_PERIOD = 0x");
    PrintSXRegContent(REG_HOP_PERIOD);
    UARTWriteStr("REG_DETECT_OPTIMIZE = 0x");
    PrintSXRegContent(REG_DETECT_OPTIMIZE);
    UARTWriteStr("REG_INVERT_IQ = 0x");
    PrintSXRegContent(REG_INVERT_IQ);
    UARTWriteStr("REG_DETECTION_THRESHOLD = 0x");
    PrintSXRegContent(REG_DETECTION_THRESHOLD);
    UARTWriteStr("REG_SYNC_WORD = 0x");
    PrintSXRegContent(REG_SYNC_WORD);
}

void send_text(uint8_t txMsg[]){
    uint8_t reg_val;      // reg_val is used to store temporarily the content of a register after reading
    int16_t i;            // i: general purpose variable (used in loops)
    uint8_t txBuffer[256];
    
    UARTInit(19200);            // init UART @ 19200 bps
    InitRFLoRaPins();           // configure pins for RF Solutions LoRa module   
    SPIInit();                  // init SPI   
    ResetRFModule();            // reset the RF Solutions LoRa module (should be optional since Power On Reset is implemented)
    
    AntennaTX();                // connect antenna to module output
    
    // put module in LoRa mode (see SX1272 datasheet page 107)
    UARTWriteStrLn("set mode to LoRa standby");

    WriteSXRegister(REG_OP_MODE, FSK_SLEEP_MODE);       // SLEEP mode required first to change bit n°7
    WriteSXRegister(REG_OP_MODE, LORA_SLEEP_MODE);      // switch from FSK mode to LoRa mode
    WriteSXRegister(REG_OP_MODE, LORA_STANDBY_MODE);    // STANDBY mode required fot FIFO loading
    __delay_ms(100);
    GetMode();
    
    // initialize the module
    UARTWriteStrLn("initialize module ");
    InitModule();
    
    // for debugging purpose only: check configuration registers content
    CheckConfiguration();
    
    strcpy(( char* )txBuffer, ( char* )txMsg);          // load txBuffer with content of txMsg
                                                        // txMsg is a table of constant values, so it is stored in Flash Memory
                                                        // txBuffer is a table of variables, so it is stored in RAM

    // load FIFO with data to transmit
    UARTWriteStrLn(" ");
    UARTWriteStrLn("step 1: load FIFO");
    WriteSXRegister(REG_FIFO_ADDR_PTR, ReadSXRegister(REG_FIFO_TX_BASE_ADDR));      // FifoAddrPtr takes value of FifoTxBaseAddr
    WriteSXRegister(REG_PAYLOAD_LENGTH_LORA, PAYLOAD_LENGTH);                       // set the number of bytes to transmit (PAYLOAD_LENGTH is defined in RF_LoRa868_SO.h)

    for (i = 0; i < PAYLOAD_LENGTH; i++) {
        WriteSXRegister(REG_FIFO, txBuffer[i]);         // load FIFO with data to transmit  
    }
    
    while(1){
        // set mode to LoRa TX
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 2: set mode to LoRa TX");
        WriteSXRegister(REG_OP_MODE, LORA_TX_MODE);
        __delay_ms(100);                                    // delay required to start oscillator and PLL
        GetMode();
        
        // wait end of transmission
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        while ((reg_val & 0x08) == 0x00) {                    // wait for end of transmission (wait until TxDone is set)
            reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        }
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 3: TxDone flag set");
        
        __delay_ms(200);        // delay is required before checking mode: it takes some time to go from TX mode to STDBY mode
        GetMode();              // check that mode is back to STDBY
        
        // reset all IRQs
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 4: clear flags");
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        UARTWriteStr("before clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);
        
        WriteSXRegister(REG_IRQ_FLAGS, 0xFF);           // clear flags: writing 1 clears flag

        // check that flags are actually cleared (useless if not debugging)
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        UARTWriteStr("after clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);
        
        // wait before next transmission
        for (i = 0; i < 10; i++) {
           __delay_ms(500);  
        }
    }
}

void send_best_score(uint8_t address_best_score){
    uint8_t reg_val;
    int16_t i;
    
    const uint8_t txMsg[256];
    memset(txMsg, 0, sizeof(txMsg));
    sprintf(txMsg, "%u", e_read_eeprom(address_best_score));
    
    UARTInit(19200);            // init UART @ 19200 bps
    InitRFLoRaPins();           // configure pins for RF Solutions LoRa module   
    SPIInit();                  // init SPI   
    ResetRFModule();            // reset the RF Solutions LoRa module (should be optional since Power On Reset is implemented)
    
    AntennaTX();                // connect antenna to module output
    
    // put module in LoRa mode (see SX1272 datasheet page 107)
    UARTWriteStrLn("set mode to LoRa standby");

    WriteSXRegister(REG_OP_MODE, FSK_SLEEP_MODE);       // SLEEP mode required first to change bit n°7
    WriteSXRegister(REG_OP_MODE, LORA_SLEEP_MODE);      // switch from FSK mode to LoRa mode
    WriteSXRegister(REG_OP_MODE, LORA_STANDBY_MODE);    // STANDBY mode required fot FIFO loading
    __delay_ms(100);
    GetMode();
    
    // initialize the module
    UARTWriteStrLn("initialize module ");
    InitModule();
    
    // for debugging purpose only: check configuration registers content
    CheckConfiguration(); 

    // load FIFO with data to transmit
    UARTWriteStrLn(" ");
    UARTWriteStrLn("step 1: load FIFO");
    WriteSXRegister(REG_FIFO_ADDR_PTR, ReadSXRegister(REG_FIFO_TX_BASE_ADDR));      // FifiAddrPtr takes value of FifoTxBaseAddr
    WriteSXRegister(REG_PAYLOAD_LENGTH_LORA, PAYLOAD_LENGTH);                       // set the number of bytes to transmit (PAYLOAD_LENGTH is defined in RF_LoRa868_SO.h)

    for (i = 0; i < PAYLOAD_LENGTH; i++) {
        WriteSXRegister(REG_FIFO, txMsg[i]);         // load FIFO with data to transmit  
    }
    
    while(1){
        // set mode to LoRa TX
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 2: set mode to LoRa TX");
        WriteSXRegister(REG_OP_MODE, LORA_TX_MODE);
        __delay_ms(100);                                    // delay required to start oscillator and PLL
        GetMode();
        
        // wait end of transmission
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        while (reg_val & 0x08 == 0x00) {                    // wait for end of transmission (wait until TxDone is set)
            reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        }
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 3: TxDone flag set");
        
        __delay_ms(200);        // delay is required before checking mode: it takes some time to go from TX mode to STDBY mode
        GetMode();              // check that mode is back to STDBY
        
        // reset all IRQs
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 4: clear flags");
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        UARTWriteStr("before clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);
        
        WriteSXRegister(REG_IRQ_FLAGS, 0xFF);           // clear flags: writing 1 clears flag

        // check that flags are actually cleared (useless if not debugging)
        reg_val = ReadSXRegister(REG_IRQ_FLAGS);
        UARTWriteStr("after clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);
        
        // wait before next transmission
        for (i = 0; i < 4; i++) {
           __delay_ms(500);  
        }
    }
}