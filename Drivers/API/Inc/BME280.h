/*
 * BME280.h
 *
 *  Created on: Nov 26, 2023
 *      Author: hollerller
 */

#ifndef API_INC_BME280_H_
#define API_INC_BME280_H_

extern float temp;  // Declaración externa de la variable temp
extern float hum;   // Declaración externa de la variable hum

enum OSRS {
	OSRS_0 = 0x00,
	OSRS_1 = 0x01,
	OSRS_2 = 0x02,
	OSRS_4 = 0x03,
	OSRS_8 = 0x04,
	OSRS_16 = 0x05
};

enum MODE {
	SLEEP = 0x00,
	FORCED = 0x01,
	NORMAL = 0x03
};

#define CHIP_ID 0xD0
#define RESET_REG 0xE0
#define CTRL_HUM 0xF2
#define STATUS 0xF3
#define CTRL_MEAS 0xF4
#define CONFIG_REG 0xF5
#define PRESS_MSB 0xF7

void trimmingParametersRead (void);
void BME280_init(void);
void BME280_calculate(void);


/*
void TrimRead(void);
int BME280_Config (uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h, uint8_t mode, uint8_t t_sb, uint8_t filter);
int BMEReadRaw(void);
void BME280_Measure (void);
*/

#endif /* API_INC_BME280_H_ */
