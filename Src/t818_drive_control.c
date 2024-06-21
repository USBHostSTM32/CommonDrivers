#include <t818_drive_control.h>

T818DriveControl_StatusTypeDef t818_drive_control_init(t818_drive_control_t *t818_drive_control, const t818_drive_control_config_t *t818_config){
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;
	if(	(t818_config != NULL) && (t818_config->t818_hid_state !=NULL) &&
		(t818_config->t818_host_handle != NULL) && (t818_config->wheel_range<=900) &&
		(t818_drive_control != NULL)){
		t818_drive_control->config = t818_config;
		t818_drive_control->initialized = 0;
		t818_drive_control->wheel_angle = 0.0f;
		status = T818_DC_OK;
	}
	return status;
}
