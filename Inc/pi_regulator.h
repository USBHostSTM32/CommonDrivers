/**
 * @file pi_regulator.h
 * @brief Header file for PI (Proportional-Integral) Regulator
 *
 * This file contains the definition of the PI_Regulator structure and the function prototypes for initializing the regulator, calculating the output, and changing the regulator parameters.
 */

#ifndef INC_PI_REGULATOR_H_
#define INC_PI_REGULATOR_H_

#include <stdint.h>


/**
 * @def USE_NO_ANTI_WINDUP
 * @brief Macro to enable/disable anti-windup feature.
 *
 * This macro can be defined as (1U) to enable the anti-windup feature.
 * If not defined, the macro USE_CLAMPING will be enabled by default.
 */

#ifndef USE_NO_ANTI_WINDUP
	/**
	 * @def USE_CLAMPING
	 * @brief Macro to enable clamping feature.
	 *
	 * This macro is enabled by default if USE_NO_ANTI_WINDUP is not defined.
	 * It provides functionality for clamping the output of the PI regulator.
	 */
	#define USE_CLAMPING
#endif

typedef uint8_t PI_StatusTypeDef;

#define PI_OK       	((PI_StatusTypeDef) 0)
#define PI_ERROR    	((PI_StatusTypeDef) 1)



#define PI_KP			((double)5.50)
#define PI_KI			((double)0.00)
#define PI_KD			((double)4000)
#define PI_MAX_U		((double)32766.0)
#define PI_MIN_U		((double)-32767.0)

/**
 * @struct pi_t
 * @brief Structure for PI regulator parameters
 *
 * This structure holds the parameters and variables required for the PI regulator.
 */
typedef struct {

	double ki; /**< Integral gain */
	double kp; /**< Proportional gain */
	double kd;
	double e_old; /**< Previous error value */
	double u_old; /**< Previous control output value */

	#ifdef USE_CLAMPING
		double	ukmax; /**< Maximum control output value */
		double	ukmin; /**< Minimum control output value */
		double sk; /**< Saturator gain */
	#endif

} pi_t;

#if defined(USE_NO_ANTI_WINDUP)
	/**
	 * @brief Initialize the PI regulator without anti-windup
	 *
	 * This function initializes the PI regulator with the specified proportional and integral gains.
	 *
	 * @param pi Pointer to the pi_t structure
	 * @param kp Proportional gain
	 * @param ki Integral gain
	 * @return PI_OK if successful, PI_ERROR otherwise
	 */
	PI_StatusTypeDef init_pi(pi_t *pi, double kp, double ki);
#elif defined(USE_CLAMPING)
	/**
	 * @brief Initialize the PI regulator with clamping
	 *
	 * This function initializes the PI regulator with the specified proportional and integral gains, as well as the maximum and minimum control output values.
	 *
	 * @param pi Pointer to the pi_t structure
	 * @param kp Proportional gain
	 * @param ki Integral gain
	 * @param ukmax Maximum control output value
	 * @param ukmin Minimum control output value
	 * @return PI_OK if successful, PI_ERROR otherwise
	 */
	PI_StatusTypeDef init_pi(pi_t *pi, double kp, double ki, double kd, double ukmax, double ukmin);
#endif

/**
 * @brief Calculate the output of the PI regulator
 *
 * This function calculates the control output of the PI regulator based on the current error value.
 *
 * @param pi Pointer to the pi_t structure
 * @param e Current error value
 * @param u Pointer to store the control output value
 * @return PI_OK if successful, PI_ERROR otherwise
 */
PI_StatusTypeDef calculate_output(pi_t *pi, double e, double *u);

/**
 * @brief Change the parameters of the PI regulator
 *
 * This function changes the proportional and integral gains of the PI regulator.
 *
 * @param pi Pointer to the pi_t structure
 * @param kp New proportional gain
 * @param ki New integral gain
 */
void change_parameters(pi_t *pi, double kp, double ki);

#endif /* INC_PI_REGULATOR_H_ */
