/*
 * mac.h
 *
 *  Created on: 31 maj 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#ifndef MAC_H_
#define MAC_H_

#define TRUE 1
#define FALSE 0

#define LED1_PIN PB1
#define LED1_PORT B

#define LED2_PIN PB2
#define LED2_PORT B

#define OSC_TEST_PIN PB5
#define OSC_TEST_PORT B

#define LED1_TOG PORT(LED1_PORT) ^= (1<<LED1_PIN)
#define LED2_TOG PORT(LED2_PORT) ^= (1<<LED2_PIN)
#define OSC_TEST_TOG PORT(OSC_TEST_PORT) ^= (1<<OSC_TEST_PIN)

// *** PORT
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
// *** PIN
#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)
// *** DDR
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)




#endif /* MAC_H_ */
