/**
 * @file pid_regulator.h
 * @brief Header file for PID (Proportional-Integral-Derivative) Regulator
 *
 * This file contains the definition of the PID_Regulator structure and the function prototypes for initializing the regulator, calculating the output, and changing the regulator parameters.
 */

#ifndef INC_PID_REGULATOR_H_
#define INC_PID_REGULATOR_H_

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
	 * It provides functionality for clamping the output of the PID regulator.
	 */
	#define USE_CLAMPING
#endif

typedef uint8_t PID_StatusTypeDef;

#define PID_OK       	((PID_StatusTypeDef) 0)
#define PID_ERROR    	((PID_StatusTypeDef) 1)



#define PID_KP			((double)5.50)
#define PID_KI			((double)0.00)
#define PID_KD			((double)4000)
#define PID_MAX_U		((double)32766.0)
#define PID_MIN_U		((double)-32767.0)

/**
 * @struct pid_t
 * @brief Structure for PID regulator parameters
 *
 * This structure holds the parameters and variables required for the PID regulator.
 */
typedef struct {

	double ki; 		/**< Integral gain */
	double kp; 		/**< Proportional gain */
	double kd; 		/**< Derivative gain */
	double e_old; 	/**< Previous error value */
	double u_old; 	/**< Previous control output value */

	#ifdef USE_CLAMPING
		double	ukmax; 	/**< Maximum control output value */
		double	ukmin; 	/**< Minimum control output value */
		double sk; 		/**< Saturator gain */
	#endif

} pid_t;

#if defined(USE_NO_ANTI_WINDUP)
	/**
	 * @brief Initialize the PID regulator without anti-windup
	 *
	 * This function initializes the PID regulator with the specified proportional and integral gains.
	 *
	 * @param pid Pointer to the pid_t structure
	 * @param kp Proportional gain
	 * @param ki Integral gain
	 * @param kd Derivative gain
	 * @return PID_OK if successful, PID_ERROR otherwise
	 */
	PID_StatusTypeDef pid_init(pid_t *pid, double kp, double ki, double kd);
#elif defined(USE_CLAMPING)
	/**
	 * @brief Initialize the PID regulator with clamping
	 *
	 * This function initializes the PID regulator with the specified proportional and integral gains, as well as the maximum and minimum control output values.
	 *
	 * @param pid Pointer to the pid_t structure
	 * @param kp Proportional gain
	 * @param ki Integral gain
	 * @param kd Derivative gain
	 * @param ukmax Maximum control output value
	 * @param ukmin Minimum control output value
	 * @return PID_OK if successful, PID_ERROR otherwise
	 */
	PID_StatusTypeDef pid_init(pid_t *pid, double kp, double ki, double kd, double ukmax, double ukmin);
#endif

/**
 * @brief Calculate the output of the PID regulator
 *
 * This function calculates the control output of the PID regulator based on the current error value.
 *
 * @param pid Pointer to the pid_t structure
 * @param e Current error value
 * @param u Pointer to store the control output value
 * @return PID_OK if successful, PID_ERROR otherwise
 */
PID_StatusTypeDef pid_calculate_output(pid_t *pid, double e, double *u);

/**
 * @brief Change the parameters of the PID regulator
 *
 * This function changes the proportional and integral gains of the PID regulator.
 *
 * @param pid Pointer to the pid_t structure
 * @param kp New proportional gain
 * @param ki New integral gain
 * @param kd New derivative gain
 */
void pid_change_parameters(pid_t *pid, double kp, double ki, double kd);

#endif /* INC_PID_REGULATOR_H_ */
