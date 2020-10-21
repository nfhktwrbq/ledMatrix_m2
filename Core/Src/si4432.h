/*
 * si4432.h
 *
 *  Created on: Oct 11, 2020
 *      Author: muaddib
 */

#ifndef SRC_SI4432_H_
#define SRC_SI4432_H_

#include "global_inc.h"


#define RX_FIFO_ALMOST_FULL_TRESHOLD 0x08
#define SI4432_PLL 8

#define SI4432_SPI_DEFAULT_TIMEOUT 10000
#define SI4432_IRQ_DEFAULT_TIMEOUT 1000

#define SELPORT   GPIOB
#define SELPIN   GPIO_PIN_0

#define IRQPORT   GPIOB
#define IRQPIN    GPIO_PIN_14

#define POWERPin
#define POWERDDR
#define POWERPORT

#define Device_Type   0x00
#define REG_Device_Version   0x01
#define Device_Status   0x02
#define Interrupt_Status1   0x03
#define Interrupt_Status2   0x04
#define Interrupt_Enable1   0x05
#define Interrupt_Enable2   0x06
#define Operating_Function_Control1   0x07
#define Operating_Function_Control2   0x08
#define Crystal_Oscillator_Load_Capacitance   0x09
#define Microcontroller_Output_Clock   0x0A
#define GPIO0_Configuration   0x0B
#define GPIO1_Configuration   0x0C
#define GPIO2_Configuration   0x0D
#define IO_Port_Configuration   0x0E
#define ADC_Configuration   0x0F
#define ADC_Sensor_Amplifier_Offset   0x10
#define ADC_Value   0x11
#define Temperature_Sensor_Control   0x12
#define Temperature_Value_Offset   0x13
#define Wake_Up_Timer_Period1   0x14
#define Wake_Up_Timer_Period2   0x15
#define Wake_Up_Timer_Period3   0x16
#define Wake_Up_Timer_Value1   0x17
#define Wake_Up_Timer_Value2   0x18
#define Low_Duty_Cycle_Mode_Duration   0x19
#define Low_Battery_Detector_Threshold   0x1A
#define Battery_Voltage_Level   0x1B
#define IF_Filter_Bandwidth   0x1C
#define AFC_Loop_Gearshift_Override   0x1D
#define AFC_Timing_Control   0x1E
#define Clock_Recovery_Gearshift_Override   0x1F
#define Clock_Recovery_Oversampling_Ratio   0x20
#define Clock_Recovery_Offset2   0x21
#define Clock_Recovery_Offset1   0x22
#define Clock_Recovery_Offset0   0x23
#define Clock_Recovery_Timing_Loop_Gain1   0x24
#define Clock_Recovery_Timing_Loop_Gain0   0x25
#define Received_Signal_Strength_Indicator   0x26
#define RSSI_Threshold_For_Clear_Channel_Indicator   0x27
#define Antenna_Diversity_Register1   0x28
#define Antenna_Diversity_Register2   0x29
#define AFC_Limiter   0x2a  //only for rev B1  4431 A0
#define AFC_Correction_Read   0x2b  //only for rev B1
#define OOK_Counter_Value1   0x2c
#define OOK_Counter_Value2   0x2d
#define Slicer_Peak_Hold   0x2e
#define Data_Access_Control   0x30
#define EZmac_Status   0x31
#define Header_Control1   0x32
#define Header_Control2   0x33
#define Preamble_Length   0x34
#define Preamble_Detection_Control   0x35
#define Sync_Word3   0x36
#define Sync_Word2   0x37
#define Sync_Word1   0x38
#define Sync_Word0   0x39
#define Transmit_Header3   0x3A
#define Transmit_Header2   0x3B
#define Transmit_Header1   0x3C
#define Transmit_Header0   0x3D
#define Transmit_Packet_Length   0x3E
#define Check_Header3   0x3F
#define Check_Header2   0x40
#define Check_Header1   0x41
#define Check_Header0   0x42
#define Header_Enable3   0x43
#define Header_Enable2   0x44
#define Header_Enable1   0x45
#define Header_Enable0   0x46
#define Received_Header3   0x47
#define Received_Header2   0x48
#define Received_Header1   0x49
#define Received_Header0   0x4A
#define Received_Packet_Length   0x4B
#define Analog_Test_Bus   0x50
#define Digital_Test_Bus   0x51
#define TX_Ramp_Control   0x52
#define PLL_Tune_Time   0x53
#define PA_Boost   0x54
#define Calibration_Control   0x55
#define Modem_Test   0x56
#define Chargepump_Test   0x57
#define Chargepump_Current_Trimming_Override   0x58
#define Divider_Current_Trimming   0x59
#define VCO_Current_Trimming   0x5A
#define VCO_Calibration_Override   0x5B
#define Synthesizer_Test   0x5C
#define Block_Enable_Override1   0x5D
#define Block_Enable_Override2   0x5E
#define Block_Enable_Override3   0x5F
#define Channel_Filter_Coefficient_Address   0x60
#define Channel_Filter_Coefficient_Value   0x61
#define Crystal_Oscillator_Control_Test   0x62
#define RC_Oscillator_Coarse_Calibration_Override   0x63
#define RC_Oscillator_Fine_Calibration_Override   0x64
#define LDO_Control_Override   0x65
#define LDO_Level_Settings   0x66
#define Deltasigma_ADC_Tuning1   0x67
#define Deltasigma_ADC_Tuning2   0x68
#define AGC_Override1   0x69
#define AGC_Override2   0x6A
#define GFSK_FIR_Filter_Coefficient_Address   0x6B
#define GFSK_FIR_Filter_Coefficient_Value   0x6C
#define TX_Power   0x6D
#define TX_Data_Rate1   0x6E
#define TX_Data_Rate0   0x6F
#define Modulation_Mode_Control1   0x70
#define Modulation_Mode_Control2   0x71
#define Frequency_Deviation   0x72
#define Frequency_Offset   0x73
#define Frequency_Channel_Control   0x74
#define Frequency_Band_Select   0x75
#define Nominal_Carrier_Frequency1   0x76
#define Nominal_Carrier_Frequency0   0x77
#define Misc_Settings   0x78
#define Frequency_Hopping_Channel_Select   0x79
#define Frequency_Hopping_Step_Size   0x7A
#define Turn_Around_And15p4   0x7B
#define TX_FIFO_Control1   0x7C
#define TX_FIFO_Control2   0x7D
#define RX_FIFO_Control   0x7E
#define FIFO_Access   0x7F

#define NIRQ (IRQPIN & IRQPin)

void si4432_reset(void);
void si4432_snit_TX(void);
void si4432_snit_RX(void);
void si4432_snit(void);
void si4432_init_alien(void);
void si4432_send(uint8_t *data);

void si4432_init_AN415(void);
void si4432_init_TX_AN415(void);
void si4432_send_AN415(uint8_t *data, uint8_t len);
void si4432_init_RX_AN415(void);
bool si4432_receive_AN415(uint8_t * buf, uint8_t * len);
void si4432_power_off(void);
void si4432_power_on(void);
uint8_t si4432_get_battery_voltage(void);

#endif /* SRC_SI4432_H_ */
