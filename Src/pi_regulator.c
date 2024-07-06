#include "pi_regulator.h"
#include <stdlib.h>

#ifdef USE_CLAMPING
/**
 * @brief Initializes the PI regulator with clamping.
 * 
 * This function initializes the PI regulator with the specified proportional gain (kp), 
 * integral gain (ki), upper clamping limit (ukmax), and lower clamping limit (ukmin).
 * 
 * @param pi Pointer to the PI regulator structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param ukmax Upper clamping limit.
 * @param ukmin Lower clamping limit.
 * @return PI_StatusTypeDef PI_OK if successful, PI_ERROR otherwise.
 */
PI_StatusTypeDef init_pi(pi_t *pi, double kp, double ki,double kd, double ukmax, double ukmin){
#else
/**
 * @brief Initializes the PI regulator without clamping.
 * 
 * This function initializes the PI regulator with the specified proportional gain (kp) 
 * and integral gain (ki).
 * 
 * @param pi Pointer to the PI regulator structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @return PI_StatusTypeDef PI_OK if successful, PI_ERROR otherwise.
 */
PI_StatusTypeDef init_pi(pi_t *pi, double kp, double ki){
#endif
    PI_StatusTypeDef status = PI_ERROR;
    if(pi != NULL){
        pi->kp = kp;
        pi->ki = ki;
        pi->e_old = 0;
        pi->u_old = 0;

		#ifdef USE_CLAMPING
			pi->ukmax = ukmax;
			pi->ukmin = ukmin;
			pi->sk = 0;
		#endif

		status = PI_OK;
    }
    return status;
}

/**
 * @brief Changes the parameters of the PI regulator.
 * 
 * This function changes the proportional gain (kp) and integral gain (ki) of the PI regulator.
 * If clamping is used, the function also updates the saturator gain (sk) based on the new integral gain.
 * 
 * @param pi Pointer to the PI regulator structure.
 * @param kp New proportional gain.
 * @param ki New integral gain.
 */
void change_parameters(pi_t *pi, double kp, double ki){

	#if defined(USE_CLAMPING)

		pi->sk = pi->sk * (pi->ki/ki);

	#endif

	pi->kp = kp;
	pi->ki = ki;

}

/**
 * @brief Checks if the summation should be stopped.
 * 
 * This function checks if the summation of the integral term should be stopped based on the 
 * current output (u), error (e), upper clamping limit (ukmax), and lower clamping limit (ukmin).
 * 
 * @param u Current output.
 * @param e Current error.
 * @param ukmax Upper clamping limit.
 * @param ukmin Lower clamping limit.
 * @return uint8_t 1 if summation should be stopped, 0 otherwise.
 */
static inline uint8_t __stop_summation(double u, double e, double ukmax, double ukmin){
    return ((u > ukmax && e > 0) || (u < ukmin && e < 0));
}

/**
 * @brief Calculates the output of the PI regulator.
 * 
 * This function calculates the output of the PI regulator based on the current error (e) and 
 * updates the output (u) accordingly.
 * If clamping is used, the function also updates the saturator gain (sk) based on the current error.
 * 
 * @param pi Pointer to the PI regulator structure.
 * @param e Current error.
 * @param u Pointer to the output variable.
 * @return PI_StatusTypeDef PI_OK if successful, PI_ERROR otherwise.
 */
PI_StatusTypeDef calculate_output(pi_t *pi, double e, double *u){
    PI_StatusTypeDef status = PI_ERROR;
    if(pi != NULL){
        #if defined(USE_NO_ANTI_WINDUP)
            *u = pi->u_old + pi->kp * e + pi->ki * pi->e_old;
            pi->e_old = e; 
            pi->u_old = *u;
            status = PI_OK;
        #elif defined(USE_CLAMPING)
            if (!__stop_summation(*u, e, pi->ukmax, pi->ukmin)){
                pi->sk += e;
            }
            *u = pi->kp * e + pi->ki * pi->sk;
            status = PI_OK;
        #endif    
    }
    return status;
}
