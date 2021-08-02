/*
 * pushbutton.h
 *
 *  Created on: Jul 5, 2021
 *      Author: fil
 */

#ifndef INC_BB101_VCO_DRIVERS_BUTTONS_PUSHBUTTON_H_
#define INC_BB101_VCO_DRIVERS_BUTTONS_PUSHBUTTON_H_

extern	uint32_t poll_pushbtn(void);
extern	void tact0_button(void);
extern	void tact1_button(void);
extern	void tact2_button(void);
extern	void EmergencyReset(void);


#endif /* INC_BB101_VCO_DRIVERS_BUTTONS_PUSHBUTTON_H_ */
