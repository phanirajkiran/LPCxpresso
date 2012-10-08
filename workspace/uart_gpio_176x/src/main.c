/***********************************************************************
 * $Id::                                                               $
 *
 * Project:	uart: Simple UART echo for LPCXpresso 1700
 * File:	uarttest.c
 * Description:
 * 			LPCXpresso Baseboard uses pins mapped to UART3 for
 * 			its USB-to-UART bridge. This application simply echos
 * 			all characters received.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/*****************************************************************************
 *   History
 *   2010.07.01  ver 1.01    Added support for UART3, tested on LPCXpresso 1700
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/
#include "LPC17xx.h"
#include "type.h"
#include "uart.h"
#include "timer.h"
#include <string.h>

#define TIMER0_INTERVAL	((2 * (4 *(SystemCoreClock/10))) - 1)
//#define TIMER1_INTERVAL	((2 *(4 *(SystemCoreClock/10))/3) - 1)
//#define TIMER2_INTERVAL	((2 *(4 *(SystemCoreClock/10))/5) - 1)
//#define TIMER3_INTERVAL	((2 *(4 *(SystemCoreClock/10))/7) - 1)

extern volatile uint32_t UART3Count;
extern volatile uint8_t UART3Buffer[BUFSIZE];

/*****************************************************************************
 **   Main Function  main()
 This program has been test on LPCXpresso 1700.
 *****************************************************************************/
int main(void) {

//	init_timer(0, TIMER0_INTERVAL);
	/* Enable timer 0. Our interrupt handler will begin incrementing
	 * the TimeTick global each time timer 0 matches and resets.
	 */
//	enable_timer(0);

//	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

	LPC_PINCON->PINSEL1 &= (~(3 << 12));
	// Set GPIO - P0_22 - to be output
	LPC_GPIO0->FIODIR |= (1 << 22);

	const char* welcomeMsg = "UART3 Online:\r\n";
	//SystemInit();	//Called by startup code

	UARTInit(3, 9600); /* baud rate setting */
	UARTSend(3, (uint8_t *) welcomeMsg, strlen(welcomeMsg));

	/* Loop forever */
	while (1) {
		if (UART3Count != 0) {
			LPC_GPIO0->FIOSET = (1 << 22); // Turn LED2 on
			//  for (i = 0; i < 1000000; i++);

			LPC_UART3->IER = IER_THRE | IER_RLS; /* Disable RBR */
			UARTSend(3, (uint8_t *) UART3Buffer, UART3Count);
			UART3Count = 0;
			LPC_UART3->IER = IER_THRE | IER_RLS | IER_RBR; /* Re-enable RBR */

			LPC_GPIO0->FIOCLR = (1 << 22); // Turn LED2 off
			//  for (i = 0; i < 1000000; i++);
		}
	}
}

/*****************************************************************************
 **                            End Of File
 *****************************************************************************/