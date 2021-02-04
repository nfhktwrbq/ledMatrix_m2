/*
 * si4432.c
 *
 *  Created on: Oct 11, 2020
 *      Author: muaddib
 */
#include "si4432.h"


volatile uint8_t IStatus1 = 0;
volatile uint8_t IStatus2 = 0;
volatile uint8_t Status1 = 0;
volatile uint8_t Status2 = 0;
uint8_t PL_Length = 0;

void si4432_select_spi(void)
{
	HAL_GPIO_WritePin(SELPORT, SELPIN, GPIO_PIN_RESET);
}

void si4432_release_spi(void)
{
	HAL_GPIO_WritePin(SELPORT, SELPIN, GPIO_PIN_SET);
}

GPIO_PinState si4432_get_irq_pin(void){
	return HAL_GPIO_ReadPin(IRQPORT, IRQPIN);
}

void si4432_write_register(uint8_t reg, uint8_t value)
{
	si4432_select_spi();

    uint8_t data[2];

    data[0] = (reg|0x80);
    data[1] = value;

    HAL_SPI_Transmit(&hspi1, data, sizeof(data), SI4432_SPI_DEFAULT_TIMEOUT);

    si4432_release_spi();
}

uint8_t si4432_read_register (uint8_t reg)
{
    uint8_t result;
    si4432_select_spi();

    HAL_SPI_Transmit(&hspi1, &reg, sizeof(reg), SI4432_SPI_DEFAULT_TIMEOUT);
    HAL_SPI_Receive(&hspi1, &result, sizeof(result), SI4432_SPI_DEFAULT_TIMEOUT);

    si4432_release_spi();

	return result;
}

void si4432_power_on(void){

}

void si4432_power_off(void){

}

void si4432_reset(void)
{
	uint32_t timer;
    si4432_power_on();

	//Software reset
	si4432_write_register(Operating_Function_Control1, 0x80);

	timer = HAL_GetTick();
	while(si4432_get_irq_pin() == GPIO_PIN_SET && HAL_GetTick() - timer < SI4432_IRQ_DEFAULT_TIMEOUT );

	//read statuses
	Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);
}


void si4432_init_AN415(void){
	//Set the Center Frequency to 915 MHz
	si4432_write_register(Frequency_Band_Select, 0x75);
	si4432_write_register(Nominal_Carrier_Frequency1, 0xbb);
	si4432_write_register(Nominal_Carrier_Frequency0, 0x80);

	//Set the Desired TX data rate (9.6 kbps)
	si4432_write_register(TX_Data_Rate1, 0x4e);
	si4432_write_register(TX_Data_Rate0, 0xa5);
	si4432_write_register(Modulation_Mode_Control1, 0x2c);

	//set the TX power
	si4432_write_register(TX_Power, 0x1f);

	//Set desired TX deviation (+-45 kHz)
	si4432_write_register(Frequency_Deviation, 0x48);

	//set the modem parameters according to the exel calculator(parameters: 9.6 kbps, deviation: 45 kHz, channel filter BW: 102.1 kHz
	si4432_write_register(IF_Filter_Bandwidth, 0x1e);
	si4432_write_register(Clock_Recovery_Oversampling_Ratio, 0xD0);  //write 0xD0 to the Clock Recovery Oversampling Ratio register
	si4432_write_register(Clock_Recovery_Offset2, 0x00);    //write 0x00 to the Clock Recovery Offset 2 register
	si4432_write_register(Clock_Recovery_Offset1, 0x9D);        //write 0x9D to the Clock Recovery Offset 1 register
	si4432_write_register(Clock_Recovery_Offset0, 0x49);                                                           //write 0x49 to the Clock Recovery Offset 0 register
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain1, 0x00);                                                           //write 0x00 to the Clock Recovery Timing Loop Gain 1 register
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain0, 0x24);                                                           //write 0x24 to the Clock Recovery Timing Loop Gain 0 register
	si4432_write_register(AFC_Loop_Gearshift_Override, 0x40);                                                           //write 0x40 to the AFC Loop Gearshift Override register
	si4432_write_register(AFC_Timing_Control, 0x0A);                                                           //write 0x0A to the AFC Timing Control register
	si4432_write_register(AFC_Limiter, 0x20);                                                           //write 0x20 to the AFC Limiter register

	//Set the Preamble length to 5 bytes
	si4432_write_register(Preamble_Length, 0x0a);

	 //set preamble detection threshold to 20bits
	si4432_write_register(0x35, 0x2A);                                                       //write 0x2A to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	si4432_write_register(Header_Control2, 0x02);

	//Set the Sync Word Pattern
	si4432_write_register(Sync_Word3, 0x2D);
	si4432_write_register(Sync_Word2, 0xD4);

	//enable the TX packet handler and CRC
	si4432_write_register(Data_Access_Control, 0x8d);
	si4432_write_register(Header_Control1, 0x00);

	//enable FIFO mode and GFSK modulation
	si4432_write_register(Modulation_Mode_Control2, 0x63);

	//set the TX FIFO almost empty threshold to 10 bytes
	si4432_write_register(0x7D, 0x01);
	//set the RX FIFO almost full threshold to 54 bytes
	si4432_write_register(0x7E, RX_FIFO_ALMOST_FULL_TRESHOLD);

	//Configure control to RF Switch
	si4432_write_register(GPIO1_Configuration, 0x15);//TX state
	si4432_write_register(GPIO2_Configuration, 0x12);//RX state

	//set AGC Override1 Register
	si4432_write_register(AGC_Override1, 0x60);
	//set Crystal Oscillator Load Capacitance register
	si4432_write_register(Crystal_Oscillator_Load_Capacitance, 0xb7);
}

void si4432_init_TX_AN415(void){
	si4432_init_AN415();
    //disable the receiver chain (but keep the XTAL running to have shorter TX on time!)
    si4432_write_register(Operating_Function_Control1, 0x01);

    si4432_write_register(Interrupt_Enable1, 0x04);
    si4432_write_register(Interrupt_Enable2, 0x00);

    Status1 = si4432_read_register(Interrupt_Status1);
    Status2 = si4432_read_register(Interrupt_Status2);
}

void si4432_init_RX_AN415(void){
	si4432_init_AN415();

	//enable low bat and enable the receiver chain
	si4432_write_register(Operating_Function_Control1, 0x05);

    Status1 = si4432_read_register(Interrupt_Status1);
    Status2 = si4432_read_register(Interrupt_Status2);

    DEBUG_SI4432_PRINT("S1 = 0x%X, S2 = 0x%X\n", Status1, Status2);

    si4432_write_register(Interrupt_Enable1, 0x13);
    si4432_write_register(Interrupt_Enable2, 0x00);

    //default 0b00011110
    si4432_write_register(RSSI_Threshold_For_Clear_Channel_Indicator, 0x20);

    Status1 = si4432_read_register(Interrupt_Status1);
    Status2 = si4432_read_register(Interrupt_Status2);

    DEBUG_SI4432_PRINT("S1 = 0x%X, S2 = 0x%X, V = %u\n", Status1, Status2, si4432_get_battery_voltage());


}


bool si4432_receive_AN415(uint8_t * buf, uint8_t * len){
	uint8_t length = 0;
	uint8_t temp8 = 0;

	if(si4432_get_irq_pin() == GPIO_PIN_RESET)
	{
		//read interrupt status registers
		Status1 = si4432_read_register(Interrupt_Status1);
		Status2 = si4432_read_register(Interrupt_Status2);                                            //read the Interrupt Status2 register

		//CRC Error interrupt occured
		if( (Status1 & 0x01) == 0x01 )
		{
			//reset the RX FIFO
			si4432_write_register(Operating_Function_Control2, 0x02); //write 0x02 to the Operating Function Control 2 register
			si4432_write_register(Operating_Function_Control2, 0x00); //write 0x00 to the Operating Function Control 2 register
		}

		//RX FIFO almost full interrupt occured
		/*if( (Status1 & 0x10) == 0x10 )
		{
			//read 32bytes from the FIFO
			for(temp8 = pointer; temp8 < pointer + RX_FIFO_ALMOST_FULL_TRESHOLD; temp8++)
			{
				buf[temp8] = si4432_read_register(FIFO_Access);
			}
			//update receive buffer pointer
			pointer = pointer + RX_FIFO_ALMOST_FULL_TRESHOLD;
		}*/


		/*packet received interrupt occured*/
		if( (Status1 & 0x02) == 0x02 || (Status1 & 0x10) == 0x10 )
		{
			//disable the receiver chain
			si4432_write_register(Operating_Function_Control2, 0x01);

			//Read the length of the received payload
			length = si4432_read_register(Received_Packet_Length);                                             //read the Received Packet Length register

			//get the remaining 32bytes from the RX FIFO
			for(temp8 = 0; temp8 < (length > SI4432_PLL ? SI4432_PLL : length); temp8++)
			{
				buf[temp8] = si4432_read_register(FIFO_Access);
			}

			*len = length;


			//reset the RX FIFO
			si4432_write_register(Operating_Function_Control2, 0x02);
			si4432_write_register(Operating_Function_Control2, 0x00);
			//enable the receiver chain again
			si4432_write_register(Operating_Function_Control1, 0x05);

			//read interrupt status registers
					Status1 = si4432_read_register(Interrupt_Status1);
					Status2 = si4432_read_register(Interrupt_Status2);
			return true;
		}
	}
	return false;
}

void si4432_send_AN415(uint8_t *data, uint8_t len){
    int k = 0;

    //clear Tx FIFO
    si4432_write_register(Operating_Function_Control2, 0x03);
    si4432_write_register(Operating_Function_Control2, 0x00);

    //only TX interrupt enable
    si4432_write_register(Interrupt_Enable1, 0x04);
    si4432_write_register(Interrupt_Enable2, 0x00);

    //_delay_ms(1);

    Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);

    si4432_write_register(Transmit_Packet_Length, len);

    for (uint8_t i = 0; i < len; i++)
	{
		si4432_write_register(FIFO_Access, data[i]);
	}

    Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);

    si4432_write_register(Operating_Function_Control1, 0x09);

    while(si4432_get_irq_pin() == GPIO_PIN_SET);

    Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);

    while(!(Status1 & 0x04) && k < 100){
        //_delay_ms(2);
        Status1 = si4432_read_register(Interrupt_Status1);
        Status2 = si4432_read_register(Interrupt_Status2);
        k++;
    }

    si4432_write_register(Operating_Function_Control1, 0x01);

    Status1 = si4432_read_register(Device_Status);

    if(Status1 & 0x80){
        //clear FIFO
        si4432_write_register(Operating_Function_Control2, 0x03);
        si4432_write_register(Operating_Function_Control2, 0x00);
    }
}

void si4432_init_TX(void)
{
	//errata?
	si4432_write_register(LDO_Control_Override, 0xA1);

	//Set RF Parameters
	//Set the Center Frequency to 915 MHz
	si4432_write_register(Frequency_Band_Select, 0x75);
	si4432_write_register(Nominal_Carrier_Frequency1, 0xBB);
	si4432_write_register(Nominal_Carrier_Frequency0, 0x80);

	//Set the Desired TX data rate (9.6 kbps)
	si4432_write_register(TX_Data_Rate1, 0x4e);
	si4432_write_register(TX_Data_Rate0, 0xa5);
	si4432_write_register(Modulation_Mode_Control1, 0x2c);

	//Set desired TX deviation (+-45 kHz)
	si4432_write_register(Frequency_Deviation, 0x48);

	//Set the Preamble length to 5 bytes
	si4432_write_register(Preamble_Length, 0x09);
	si4432_write_register(Header_Control2, 0x02);

	//Set the Sync Word Pattern
	si4432_write_register(Sync_Word3, 0x2D);
	si4432_write_register(Sync_Word2, 0xD4);

	//enable the TX packet handler and CRC
	si4432_write_register(Data_Access_Control, 0x0d);

	//enable FIFO mode and GFSK modulation
	si4432_write_register(Modulation_Mode_Control2, 0x63);

	//Configure control to RF Switch
	si4432_write_register(GPIO0_Configuration, 0x12);//TX state
	si4432_write_register(GPIO1_Configuration, 0x15);//RX state

	//
	si4432_write_register(VCO_Current_Trimming, 0x7f);
	si4432_write_register(Divider_Current_Trimming, 0x40);

	//Set Crystal OscollatorLoad Capacitance
	si4432_write_register(Crystal_Oscillator_Load_Capacitance, 0xd7);

	//TX Power
	//si4432_write_register(TX_Power, 0x18 );

}

void si4432_send(unsigned char *data)
{
	/*//Disable the receiver chain
	si4432_write_register(Operating_Function_Control1, 0x01);

	//Set TX deviation
	si4432_write_register(Frequency_Deviation, 0x48);*/

	for (unsigned char i = 0; i < SI4432_PLL; i++)
	{
		si4432_write_register(FIFO_Access, data[i]);
	}

	//errata?
	si4432_write_register(LDO_Control_Override, 0xA1);

	//Disable all interrupts besides packet sentinterruptt
	si4432_write_register(Interrupt_Enable1, 0x04);
	si4432_write_register(Interrupt_Enable2, 0x00);

	//read statuses
	Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);

	//sent packet
	si4432_write_register(Operating_Function_Control1, 0x09);

	while(si4432_get_irq_pin() == GPIO_PIN_SET);

	//read statuses
	Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);
	/*si4432_write_register(Interrupt_Enable1, 0x03);
	si4432_write_register(Interrupt_Enable2, 0x00);
	Status1 = si4432_read_register(Interrupt_Status1);
	Status2 = si4432_read_register(Interrupt_Status2);

	//Set RX deviation
	si4432_write_register(Frequency_Deviation, 0x48);

	//Enable the receiver chain
	si4432_write_register(Operating_Function_Control1, 0x05);*/
}

void si4432_init_RX(void)
{
	//errata?
	si4432_write_register(LDO_Control_Override, 0xA1);

	//Set RF Parameters
	//Set the Center Frequency to 915 MHz
	si4432_write_register(Frequency_Band_Select, 0x75);
	si4432_write_register(Nominal_Carrier_Frequency1, 0xBB);
	si4432_write_register(Nominal_Carrier_Frequency0, 0x80);

	//Configure the Modem
	si4432_write_register(IF_Filter_Bandwidth, 0x05);
	si4432_write_register(Clock_Recovery_Oversampling_Ratio, 0xa1);
	si4432_write_register(Clock_Recovery_Offset2, 0x20);
	si4432_write_register(Clock_Recovery_Offset1, 0x4e);
	si4432_write_register(Clock_Recovery_Offset0, 0xa5);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain1, 0x00);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain0, 0x13);
	si4432_write_register(AFC_Loop_Gearshift_Override, 0x40);
	//si4432_write_register(AFC_Timing_Control, 0x0a);

	si4432_write_register(AFC_Limiter, 0x20);
	si4432_write_register(Clock_Recovery_Gearshift_Override, 0x03);

	//Configure receive packet handler
	si4432_write_register(Header_Control2, 0x0a);//fix payload adn synch word 3 2
	si4432_write_register(Header_Control1, 0x00);
	si4432_write_register(Transmit_Packet_Length, SI4432_PLL);//8 butes payload

	//Set the Sync Word Pattern
	si4432_write_register(Sync_Word3, 0x2D);
	si4432_write_register(Sync_Word2, 0xD4);

	//enable the TX packet handler and CRC
	si4432_write_register(Data_Access_Control, 0x85);

	//enable FIFO mode and GFSK modulation
	si4432_write_register(Modulation_Mode_Control2, 0x63);

	//Set preamble detection treshold to 20 bit
	si4432_write_register(Preamble_Detection_Control, 0x28);

	//Configure control to RF Switch
	si4432_write_register(GPIO0_Configuration, 0x12);//TX state
	si4432_write_register(GPIO1_Configuration, 0x15);//RX state

	//Special considerations for rev B1
	si4432_write_register(AGC_Override1, 0x60);

	//Set Crystal OscollatorLoad Capacitance
	si4432_write_register(Crystal_Oscillator_Load_Capacitance, 0xd7);
}

void si4432_init(void)
{
	//Set RF Parameters
	//Set the Center Frequency to 915 MHz
	si4432_write_register(Frequency_Band_Select, 0x75);
	si4432_write_register(Nominal_Carrier_Frequency1, 0xBB);
	si4432_write_register(Nominal_Carrier_Frequency0, 0x80);

	//Set the Desired TX data rate (9.6 kbps)
	si4432_write_register(TX_Data_Rate1, 0x4e);
	si4432_write_register(TX_Data_Rate0, 0xa5);
	si4432_write_register(Modulation_Mode_Control1, 0x2c);

	//Configure the Modem
	si4432_write_register(IF_Filter_Bandwidth, 0x05);
	si4432_write_register(Clock_Recovery_Oversampling_Ratio, 0xa1);
	si4432_write_register(Clock_Recovery_Offset2, 0x20);
	si4432_write_register(Clock_Recovery_Offset1, 0x4e);
	si4432_write_register(Clock_Recovery_Offset0, 0xa5);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain1, 0x00);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain0, 0x13);
	si4432_write_register(AFC_Loop_Gearshift_Override, 0x40);
	//si4432_write_register(AFC_Timing_Control, 0x0a);
	si4432_write_register(Frequency_Deviation, 0x1f);

	//Set the Preamble length to 5 bytes
	si4432_write_register(Preamble_Length, 0x09);
	si4432_write_register(Header_Control2, 0x0a);//fix payload adn synch word 3 2
	si4432_write_register(Transmit_Packet_Length, SI4432_PLL);//8 butes payload

	//Set preamble detection treshold to 20 bit
	si4432_write_register(Preamble_Detection_Control, 0x28);

	//Set the Sync Word Pattern
	si4432_write_register(Sync_Word3, 0x2D);
	si4432_write_register(Sync_Word2, 0xD4);

	//enable the TX packet handler and CRC
	si4432_write_register(Data_Access_Control, 0x8d);

	//Disable the receive header filters
	si4432_write_register(Header_Control1, 0x00);

	//enable FIFO mode and GFSK modulation
	si4432_write_register(Modulation_Mode_Control2, 0x63);

	//Configure control to RF Switch
	si4432_write_register(GPIO0_Configuration, 0x12);//TX state
	si4432_write_register(GPIO1_Configuration, 0x15);//RX state

	//
	si4432_write_register(VCO_Current_Trimming, 0x7f);
	si4432_write_register(Chargepump_Current_Trimming_Override, 0x80);
	si4432_write_register(Divider_Current_Trimming, 0x40);
	si4432_write_register(AGC_Override2, 0x0b);
	si4432_write_register(Deltasigma_ADC_Tuning2, 0x04);
	si4432_write_register(Clock_Recovery_Gearshift_Override, 0x03);

	//Set Crystal OscollatorLoad Capacitance
	si4432_write_register(Crystal_Oscillator_Load_Capacitance, 0xd7);
}

void si4432_init_alien(void)
{
	// read interrupt status
	si4432_read_register(Interrupt_Status1);
	si4432_read_register(Interrupt_Status2);
	// SW reset -> wait for POR interrupt
	si4432_write_register(Operating_Function_Control1, 0x80);

	// Enable the POR interrupt
	// Wait for the POR interrupt
	//_delay_ms(100);

	// read interrupt status
	si4432_read_register(Interrupt_Status1);
	if(((si4432_read_register(Interrupt_Status2)) & 0x03) == 0x03)
	{
		si4432_write_register(Operating_Function_Control1, 0x00);
	}
	else
	{

	}

	si4432_write_register(LDO_Control_Override, 0xA1);


	// disable all ITs, except 'ichiprdy'
	si4432_write_register(Interrupt_Enable1, 0x00);
	si4432_write_register(Interrupt_Enable2, 0x02);
	si4432_read_register(Interrupt_Status1);
	si4432_read_register(Interrupt_Status2);

	// set the non-default Si4432 registers
	// set TX Power +11dBm
	si4432_write_register(TX_Power, 0x00);
	// set VCO
	si4432_write_register(VCO_Current_Trimming, 0x7F);
	si4432_write_register(Divider_Current_Trimming, 0x40);
	// set the AGC
	si4432_write_register(AGC_Override2, 0x0B);
	// set ADC reference voltage to 0.9V
	si4432_write_register(Deltasigma_ADC_Tuning2, 0x04);
	// set cap. bank
	si4432_write_register(Crystal_Oscillator_Load_Capacitance, 0x7F);
	// reset digital testbus, disable scan test
	si4432_write_register(Digital_Test_Bus, 41);//0x00);
	// select nothing to the Analog Testbus
	si4432_write_register(Analog_Test_Bus, 0x0B);
	// set frequency
	si4432_write_register(Frequency_Band_Select, 0x53);
	si4432_write_register(Nominal_Carrier_Frequency1, 0x64);
	si4432_write_register(Nominal_Carrier_Frequency0, 0x00);
	// disable RX-TX headers,
	si4432_write_register(Header_Control1, 0x00);
	si4432_write_register(Header_Control2, 0x02);
	// set the sync word
	si4432_write_register(Sync_Word3, 0x2D);
	si4432_write_register(Sync_Word2, 0xD4);

	// set GPIO0 to MCLK
	//si4432_write_register((REG_WRITE | GPIO0Configuration), 0x17);
	// set GPIO0 to TX State & GPIO1 to RX State
	si4432_write_register(GPIO0_Configuration, 0x12);
	si4432_write_register(GPIO1_Configuration, 0x15);

	// set modem and RF parameters according to the selected DATA rate
	// setup the internal digital modem according the selected RF settings (data rate)
	si4432_write_register(IF_Filter_Bandwidth, 0x01);
	si4432_write_register(Clock_Recovery_Oversampling_Ratio, 0x83);
	si4432_write_register(Clock_Recovery_Offset2, 0xc0);
	si4432_write_register(Clock_Recovery_Offset1, 0x13);
	si4432_write_register(Clock_Recovery_Offset0, 0xa9);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain1, 0x00);
	si4432_write_register(Clock_Recovery_Timing_Loop_Gain0, 0x05);
	si4432_write_register(TX_Data_Rate1, 0x13);
	si4432_write_register(TX_Data_Rate0, 0xa9);
	si4432_write_register(Modulation_Mode_Control1, 0x20);
	si4432_write_register(Frequency_Deviation, 0x3a);
	si4432_write_register(AFC_Loop_Gearshift_Override, 0x40);
	si4432_write_register(Chargepump_Current_Trimming_Override, 0x80);

	// enable packet handler & CRC16
	si4432_write_register(Data_Access_Control, 0x8D);
	si4432_write_register(Modulation_Mode_Control2, 0x63);

	// set preamble length & detection threshold
	si4432_write_register(Preamble_Length, 0x09);
	si4432_write_register(Preamble_Detection_Control, 0x28);
	si4432_write_register(Clock_Recovery_Gearshift_Override, 0x03);

	// reset digital testbus, disable scan test
	si4432_write_register(Digital_Test_Bus, 0x00);
	// select nothing to the Analog Testbus
	si4432_write_register(Analog_Test_Bus, 0x0B);
}

uint8_t si4432_get_battery_voltage(void){
    uint8_t voltage = (si4432_read_register(Interrupt_Status1) & 0x1f);

    return  voltage;
}
