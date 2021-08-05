/*
 * ee24_driver.c
 *
 *  Created on: Jun 25, 2021
 *      Author: fil
 */

#include "main.h"

ProgramTypeDef	Program;
ProgramTypeDef	CurrentProgram;

uint8_t werr,rerr;
void ee24_write(uint16_t address, uint8_t *data, size_t len, uint32_t timeout)
{
HAL_StatusTypeDef ret;
	__disable_irq();
	ret = HAL_I2C_Mem_Write(&EEPROM_I2C_HANDLE, EEPROM_ADDRESS, address,2, data, len, timeout);
	__enable_irq();
	if ( ret != HAL_OK )
		rerr++;
}

void ee24_read(uint16_t address, uint8_t *data, size_t len, uint32_t timeout)
{
HAL_StatusTypeDef ret;

	__disable_irq();
	ret = HAL_I2C_Mem_Read(&EEPROM_I2C_HANDLE, EEPROM_ADDRESS, address,2, data, len, timeout);
	__enable_irq();
	if ( ret != HAL_OK )
		rerr++;
}

void ee24_read_program(uint16_t program_number)
{
	ee24_read((program_number+PROGRAM_OFFSET)*PROGRAM_SIZE,(uint8_t *)&Program,PROGRAM_SIZE,100);
	if (( Program.program_flags0 != BB_EE_MACHINE_FAMILY) || ( Program.program_flags1 != BB_EE_MACHINE_MODEL))
	{
		Program.program_flags0 = BB_EE_MACHINE_FAMILY;
		Program.program_flags1 = BB_EE_MACHINE_MODEL;
	}
}

void ee24_write_program(uint16_t program_number)
{
	ee24_write((program_number+PROGRAM_OFFSET)*PROGRAM_SIZE,(uint8_t *)&Program,PROGRAM_SIZE,100);
}

uint16_t ee24_load_program(uint16_t program_number)
{
uint32_t	i;

	ee24_read_program(program_number);

	if (( Program.program_flags0 == BB_EE_MACHINE_FAMILY) && ( Program.program_flags1 == BB_EE_MACHINE_MODEL))
	{
		SystemFlags.oscillator_flags 	= ( Program.oscillator_flagsh << 4 ) | Program.oscillator_flagsl;
		SystemFlags.control_flags 		= ( Program.control_flagsh << 4 ) | Program.control_flagsl;
		SystemFlags.vcf_flags 			= ( Program.vcf_flagsh << 4 ) | Program.vcf_flagsl;
		SystemFlags.effect_flags 		= ( Program.effect_flagsh << 4 ) | Program.effect_flagsl;
		SystemFlags.delay_flags 		= ( Program.delay_flagsh << 4 ) | Program.delay_flagsl;
		SystemFlags.osc_waves[0] 		= Program.osc_wavesh  & 0x03;
		SystemFlags.osc_waves[1] 		= (Program.osc_wavesh  & 0x0c) >> 2;
		SystemFlags.osc_waves[2] 		= Program.osc_wavesl  & 0x03;
		SystemFlags.osc_waves[3] 		= (Program.osc_wavesl  & 0x0c) >> 2;
		SystemFlags.delay_value 		= Program.delay_value*10;
		for(i=0;i<4;i++)
		{
			SystemFlags.osc_duty_percent[i] = Program.osc_duty_percent[i];
			SystemFlags.osc_detune[i] = Program.osc_detune[i];
			SystemFlags.osc_duty[i] = (uint8_t )((float )SystemFlags.osc_duty_percent[i] * 2.55F );
			SystemFlags.osc_volume[i] = Program.osc_volume[i];
		}
		SystemFlags.Atime = Program.Atime;
		SystemFlags.Dtime = Program.Dtime;
		SystemFlags.Sval = Program.Sval;
		SystemFlags.Rtime = Program.Rtime;

		MenusDrawEmptyStatus();
		MenusDrawStatus();
		DisplayADSR();
		return 0;
	}
	else
		return 1;
}

void ee24_store_program(uint16_t program_number)
{
uint32_t	i;

/* MIDI compatible program architecture */

	Program.program_flags0 = BB_EE_MACHINE_FAMILY;
	Program.program_flags1 = BB_EE_MACHINE_MODEL;
	Program.oscillator_flagsh = SystemFlags.oscillator_flags >> 4;
	Program.oscillator_flagsl = SystemFlags.oscillator_flags & 0x0f;
	Program.control_flagsh = SystemFlags.control_flags >> 4;
	Program.control_flagsl = SystemFlags.control_flags & 0x0f;
	Program.vcf_flagsh = SystemFlags.vcf_flags >> 4;
	Program.vcf_flagsl = SystemFlags.vcf_flags & 0x0f;
	Program.effect_flagsh = SystemFlags.effect_flags >> 4;
	Program.effect_flagsl = SystemFlags.effect_flags & 0x0f;
	Program.delay_flagsh = SystemFlags.delay_flags >> 4;
	Program.delay_flagsl = SystemFlags.delay_flags & 0x0f;
	Program.delay_value = SystemFlags.delay_value/10;
	Program.osc_wavesh = SystemFlags.osc_waves[0];
	Program.osc_wavesh |= (SystemFlags.osc_waves[1] << 2);
	Program.osc_wavesl = SystemFlags.osc_waves[2];
	Program.osc_wavesl |= (SystemFlags.osc_waves[3] << 2);

	for(i=0;i<4;i++)
	{
		Program.osc_duty_percent[0] = SystemFlags.osc_duty_percent[0];
		Program.osc_detune[0] = SystemFlags.osc_detune[0];
		Program.osc_volume[0] = SystemFlags.osc_volume[0];
	}
	Program.Atime = SystemFlags.Atime;
	Program.Dtime = SystemFlags.Dtime;
	Program.Sval = SystemFlags.Sval;
	Program.Rtime = SystemFlags.Rtime;
	ee24_write_program(program_number);
	ee24_read_program(program_number);
}

void ee24_get_current_program(void)
{
uint32_t	i;

/* MIDI compatible program architecture */
	CurrentProgram.program_flags0 = BB_EE_MACHINE_FAMILY;
	CurrentProgram.program_flags1 = BB_EE_MACHINE_MODEL;
	CurrentProgram.oscillator_flagsh = SystemFlags.oscillator_flags >> 4;
	CurrentProgram.oscillator_flagsl = SystemFlags.oscillator_flags & 0x0f;
	CurrentProgram.control_flagsh = SystemFlags.control_flags >> 4;
	CurrentProgram.control_flagsl = SystemFlags.control_flags & 0x0f;
	CurrentProgram.vcf_flagsh = SystemFlags.vcf_flags >> 4;
	CurrentProgram.vcf_flagsl = SystemFlags.vcf_flags & 0x0f;
	CurrentProgram.effect_flagsh = SystemFlags.effect_flags >> 4;
	CurrentProgram.effect_flagsl = SystemFlags.effect_flags & 0x0f;
	CurrentProgram.delay_flagsh = SystemFlags.delay_flags >> 4;
	CurrentProgram.delay_flagsl = SystemFlags.delay_flags & 0x0f;
	CurrentProgram.delay_value = SystemFlags.delay_value/10;
	CurrentProgram.osc_wavesh = SystemFlags.osc_waves[0];
	CurrentProgram.osc_wavesh |= (SystemFlags.osc_waves[1] << 2);
	CurrentProgram.osc_wavesl = SystemFlags.osc_waves[2];
	CurrentProgram.osc_wavesl |= (SystemFlags.osc_waves[3] << 2);

	for(i=0;i<4;i++)
	{
		CurrentProgram.osc_duty_percent[0] = SystemFlags.osc_duty_percent[0];
		CurrentProgram.osc_detune[0] = SystemFlags.osc_detune[0];
		CurrentProgram.osc_volume[0] = SystemFlags.osc_volume[0];
	}
	CurrentProgram.Atime = SystemFlags.Atime;
	CurrentProgram.Dtime = SystemFlags.Dtime;
	CurrentProgram.Sval = SystemFlags.Sval;
	CurrentProgram.Rtime = SystemFlags.Rtime;
}

