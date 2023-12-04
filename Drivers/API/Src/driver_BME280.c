/*
 * BME280.c
 *
 *  Created on: Nov 26, 2023
 *      Author: hollerller
 */


#include <driver_BME280.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"



#define BME280_ADDRESS 0xEC
#define TIMEOUT 1000

extern I2C_HandleTypeDef hi2c1;

uint16_t firstMemAddres = 0x88;
uint16_t memAddressSize = 1;



uint16_t dig_T1, dig_P1, dig_H1, dig_H3;
int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2, dig_H4, dig_H5, dig_H6;

int32_t tADC, hADC;

typedef int32_t BME280_S32_t;
typedef uint32_t BME280_U32_t;


float temp, hum, pres;

// Read the calibration parameters stored in the sensor NVM

void trimmingParametersRead (void) {

uint8_t calibData1[26];
uint8_t calibData2[7];

	HAL_I2C_Mem_Read(&hi2c1, BME280_ADDRESS, 0x88, 1, calibData1, 25, TIMEOUT);

	HAL_I2C_Mem_Read(&hi2c1, BME280_ADDRESS, 0xE1, 1, calibData2, 7, TIMEOUT);


	dig_T1 = (calibData1[1] << 8) | calibData1[0];
	dig_T2 = (calibData1[3] << 8) | calibData1[2];
	dig_T3 = (calibData1[5] << 8) | calibData1[4];
	dig_P1 = (calibData1[7] << 8) | calibData1[6];
	dig_P2 = (calibData1[9] << 8) | calibData1[8];
	dig_P3 = (calibData1[11] << 8) | calibData1[10];
	dig_P4 = (calibData1[13] << 8) | calibData1[12];
	dig_P5 = (calibData1[15] << 8) | calibData1[14];
	dig_P6 = (calibData1[17] << 8) | calibData1[16];
	dig_P7 = (calibData1[19] << 8) | calibData1[18];
	dig_P8 = (calibData1[21] << 8) | calibData1[20];
	dig_P9 = (calibData1[23] << 8) | calibData1[22];
	dig_H1 = calibData1[24];
	dig_H2 = (calibData2[1] << 8) | calibData2[0];
	dig_H3 = calibData2[2];
	dig_H4 = (calibData2[3] << 4) | (calibData2[4] & 0x0F);
	dig_H5 = (calibData2[4] << 4) | (calibData2[5] >> 4);
	dig_H6 = calibData2[6];

}

void BME280_init(void) {


	trimmingParametersRead();


	uint8_t resetSeq = 0xB6;
	uint8_t ctrlHum = 0x01;
	uint8_t ctrlMeas = 0b10101011;
	uint8_t config = 0b00010000;

	// Soft Reset

	HAL_I2C_Mem_Write(&hi2c1, BME280_ADDRESS, RESET_REG, 1, &resetSeq , 1, 1000);

	HAL_Delay(100);

	// ctrl_hum

	HAL_I2C_Mem_Write(&hi2c1, BME280_ADDRESS, CTRL_HUM, 1, &ctrlHum , 1, 1000);

	HAL_Delay(100);

	// ctrl_meas

	HAL_I2C_Mem_Write(&hi2c1, BME280_ADDRESS, CTRL_MEAS, 1, &ctrlMeas , 1, 1000);

	HAL_Delay(100);

	// config parameters

	HAL_I2C_Mem_Write(&hi2c1, BME280_ADDRESS, CONFIG_REG, 1, &config , 1, 1000);

	HAL_Delay(100);


}

void BME280_read(void) {

	uint8_t sensorData[8];


	HAL_I2C_Mem_Read(&hi2c1, BME280_ADDRESS, 0xF7, 1, sensorData, 8, 1000);

	tADC = (sensorData[3] << 12) | (sensorData[4] << 4) | (sensorData[5] >> 4);
	hADC = (sensorData[6] << 8) | sensorData[7];


}


// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value

BME280_S32_t t_fine;

BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T) {
	BME280_S32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((BME280_S32_t) dig_T1 << 1)))
			* ((BME280_S32_t) dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((BME280_S32_t) dig_T1))
			* ((adc_T >> 4) - ((BME280_S32_t) dig_T1))) >> 12)
			* ((BME280_S32_t) dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}


// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH

BME280_U32_t bme280_compensate_H_int32(BME280_S32_t adc_H) {
	BME280_S32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((BME280_S32_t) 76800));
	v_x1_u32r =
			(((((adc_H << 14) - (((BME280_S32_t) dig_H4) << 20)
					- (((BME280_S32_t) dig_H5) * v_x1_u32r))
					+ ((BME280_S32_t) 16384)) >> 15)
					* (((((((v_x1_u32r * ((BME280_S32_t) dig_H6)) >> 10)
							* (((v_x1_u32r * ((BME280_S32_t) dig_H3)) >> 11)
									+ ((BME280_S32_t) 32768))) >> 10)
							+ ((BME280_S32_t) 2097152))
							* ((BME280_S32_t) dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r
			- (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
							* ((BME280_S32_t) dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (BME280_U32_t) (v_x1_u32r >> 12);
}




void BME280_calculate(void){

	BME280_read();

	temp = BME280_compensate_T_int32(tADC) / 100.0;
	hum = bme280_compensate_H_int32(hADC) / 1024.0;


}

