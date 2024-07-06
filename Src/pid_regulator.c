#include <pid_regulator.h>
#include <stdlib.h>



#ifdef USE_CLAMPING

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
 * @brief Initializes the PID regulator with clamping.
 * 
 * This function initializes the PID regulator with the specified proportional gain (kp), 
 * integral gain (ki), upper clamping limit (ukmax), and lower clamping limit (ukmin).
 * 
 * @param pid Pointer to the PID regulator structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param kd Derivative gain.
 * @param ukmax Upper clamping limit.
 * @param ukmin Lower clamping limit.
 * @return PID_StatusTypeDef PID_OK if successful, PID_ERROR otherwise.
 */
PID_StatusTypeDef pid_init(pid_t *pid, double kp, double ki,double kd, double ukmax, double ukmin){
#else
/**
 * @brief Initializes the PID regulator without clamping.
 * 
 * This function initializes the PID regulator with the specified proportional gain (kp) 
 * and integral gain (ki).
 * 
 * @param pid Pointer to the PID regulator structure.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @return PID_StatusTypeDef PID_OK if successful, PID_ERROR otherwise.
 */
PID_StatusTypeDef pid_init(pid_t *pid, double kp, double ki){
#endif
    PID_StatusTypeDef status = PID_ERROR;
    if(pid != NULL){
        pid->kp = kp;
        pid->ki = ki;
        pid->e_old = 0;
        pid->u_old = 0;

		#ifdef USE_CLAMPING
			pid->ukmax = ukmax;
			pid->ukmin = ukmin;
			pid->sk = 0;
		#endif

		status = PID_OK;
    }
    return status;
}

/**
 * @brief Changes the parameters of the PID regulator.
 * 
 * This function changes the proportional gain (kp), integral gain (ki), and derivative gain of the PID regulator.
 * If clamping is used, the function also updates the saturator gain (sk) based on the new integral gain.
 * 
 * @param pid Pointer to the PID regulator structure.
 * @param kp New proportional gain.
 * @param ki New integral gain.
 * @param kd New derivative gain.
 */
void pid_change_parameters(pid_t *pid, double kp, double ki, double kd){

	#if defined(USE_CLAMPING)

		pid->sk = pid->sk * (pid->ki/ki);

	#endif

	pid->kp = kp;
	pid->ki = ki;
    pid->kd = kd;

}


/**
 * @brief Calculates the output of the PID regulator.
 * 
 * This function calculates the output of the PID regulator based on the current error (e) and 
 * updates the output (u) accordingly.
 * If clamping is used, the function also updates the saturator gain (sk) based on the current error.
 * 
 * @param pid Pointer to the PID regulator structure.
 * @param e Current error.
 * @param u Pointer to the output variable.
 * @return PID_StatusTypeDef PID_OK if successful, PID_ERROR otherwise.
 */
PID_StatusTypeDef pid_calculate_output(pid_t *pid, double e, double *u){
    PID_StatusTypeDef status = PID_ERROR;
    if((pid != NULL) && (u != NULL)){
        #if defined(USE_NO_ANTI_WINDUP)
            *u = pid->u_old + pid->kp * e + pid->ki * pid->e_old;
        #elif defined(USE_CLAMPING)
            if (!__stop_summation(*u, e, pid->ukmax, pid->ukmin)){
                pid->sk += e;
            }
            *u = (pid->kp * e) + (pid->ki * pid->sk) + (pid->kd * (e - pid->e_old));
        #endif
            pid->e_old = e;
            pid->u_old = *u;
            status = PID_OK;
    }
    return status;
}
