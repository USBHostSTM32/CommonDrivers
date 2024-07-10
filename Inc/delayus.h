
#ifndef DELAYUS_H_
#define DELAYUS_H_



/**
 * @brief Generates a delay of a specified number of microseconds.
 *
 * This macro creates a delay loop that runs for the specified number of microseconds.
 * It is implemented in inline assembly and directly manipulates CPU registers to create
 * the delay. The macro assumes that the system clock is configured to provide a clock
 * frequency of 64 MHz to the CPU, as per the associated clock tree configuration.
 *
 * @note The actual delay is contingent upon the CPU's clock frequency being precisely
 *       64 MHz. If the clock frequency deviates from this value, the delay will not be accurate.
 *
 * @param us The number of microseconds to delay. The input parameter is scaled by a factor
 *           calculated based on the clock frequency (currently fixed at 64 MHz).
 *
 * Usage example:
 * @code
 *     delay_us(10); // Delays for 10 microseconds
 * @endcode
 */

#define delay_us(us) do { 							\
asm volatile (	"MOV R0,%[loops]\n 					\
			1: \n 									\
			SUB R0, #1\n 							\
			CMP R0, #0\n 							\
			BNE 1b \t" 								\
			: : [loops] "r" (13*us) : "memory" 		\
			); 										\
} while(0)

#endif
