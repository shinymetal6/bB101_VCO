/*
 * ee24_driver.h
 *
 *  Created on: Jun 25, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_DRIVERS_EEPROM_EE24_DRIVER_H_
#define INC_BB101_VCO_DRIVERS_EEPROM_EE24_DRIVER_H_

#define		EEPROM_ADDRESS		0xA0
#define		EEPROM_PAGE_SIZE	64
#define		EEPROM_I2C_HANDLE	hi2c1

#ifdef OLD
typedef struct _ProgramTypeDef
{									// Flash offset		MIDI Packet offset
	uint8_t 	program_flags0;		//	0				1
	uint8_t 	program_flags1;		//	1				2
	uint8_t 	oscillator_flagsh;	//	2				3
	uint8_t		oscillator_flagsl;	//	3				4
	uint8_t 	control_flagsh;		//	4				5
	uint8_t		control_flagsl;		//	5				6
	uint8_t 	vcf_flagsh;			//	6				7
	uint8_t		vcf_flagsl;			//	7				8
	uint8_t 	effect_flagsh;		//	8				9
	uint8_t		effect_flagsl;		//	9				10
	uint8_t 	delay_flagsh;		//	10				11
	uint8_t		delay_flagsl;		//	11				12
	uint8_t 	delay_value;		//	12				13
	uint8_t		osc_wavesh;			//	13				14
	uint8_t		osc_wavesl;			//	14				15
	uint8_t 	osc_duty_percent[4];//	15,16,17,18		16:19
	uint8_t 	osc_detune[4];		//	19,20,21,22		20:23
	uint8_t 	osc_volume[4];		//  23,24,25,26		24:27
	uint8_t 	Atime,Dtime,Sval,Rtime;	// 27,28,29,30	28:31
}ProgramTypeDef;
#else
typedef struct _ProgramTypeDef
{									// Flash offset		MIDI Packet offset
	uint8_t 	program_flags0;		//	0				1
	uint8_t 	program_flags1;		//	1				2
	uint8_t 	oscillator_flags;	//	2				3
	uint8_t 	control_flags;		//	3				4
	uint8_t 	vcf_flags;			//	4				5
	uint8_t 	afx_flags;		//	5				6
	uint8_t 	delay_flags;		//	6 				7
	uint8_t 	delay_value;		//	7 				8
	uint8_t		osc_wavesh;			//	8 				9
	uint8_t		osc_wavesl;			//	9 				10
	uint8_t 	osc_duty_percent[4];//	10,11,12,13		11:14
	uint8_t 	osc_detune[4];		//	14,15,16,17		15:18
	uint8_t 	osc_volume[4];		//  18,19,20,21		19:22
	uint8_t 	Atime,Dtime,Sval,Rtime;	// 22,23,24,25	23:26
}ProgramTypeDef;
#endif

extern	ProgramTypeDef	Program;
extern	ProgramTypeDef	CurrentProgram;
#define		PROGRAM_SIZE		EEPROM_PAGE_SIZE
#define		PROGRAM_OFFSET		4
#define		NUM_PROGRAMS_MAX	64

extern	void ee24_write(uint16_t address, uint8_t *data, size_t len, uint32_t timeout);
extern	void ee24_read(uint16_t address, uint8_t *data, size_t len, uint32_t timeout);
extern	void ee24_eraseChip(void);
extern	void ee24_read_program(uint16_t program_number);
extern	uint16_t ee24_load_program(uint16_t program_number);
extern	void ee24_store_program(uint16_t program_number);
extern	void ee24_remove_program(uint16_t program_number);
extern	void ee24_get_current_program(void);

#endif /* INC_BB101_VCO_DRIVERS_EEPROM_EE24_DRIVER_H_ */
