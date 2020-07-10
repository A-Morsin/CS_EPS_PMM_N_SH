#include "main.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "PCA9534.h"
#include "PBM_config.h"
#include "PBM_init_IC.h"

/** @brief	Initialize single TMP1075 sensor.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	AddrTMP1075 - 7-bit device address.
 @retval 0-OK, ERROR_N-Error
 */
ErrorStatus TMP1075_InitState(I2C_TypeDef *I2Cx, uint8_t AddrTMP1075) {

	int8_t Error = -1;
	uint8_t count = 0;

	SW_TMUX1209_I2C_main_PBM();

	while ((Error != 0) && (count < 3)) {
		if (TMP1075_set_mode(I2Cx, AddrTMP1075, TMP1075_CONTINUOS_CONV) == SUCCESS) {
			if (TMP1075_ALERT_active_level(I2Cx, AddrTMP1075, TMP1075_ACTIVE_HIGH) == SUCCESS) {
				if (TMP1075_set_mode_ALERT_pin(I2Cx, AddrTMP1075, TMP1075_COMPARATOR_MODE) == SUCCESS) {
					if (TMP1075_set_time_conversion(I2Cx, AddrTMP1075, TMP1075_CR_MEDIUM) == SUCCESS) {
						if (TMP1075_ALERT_sensitivity(I2Cx, AddrTMP1075, TMP1075_TWO_FAULT) == SUCCESS) {
							if (TMP1075_set_low_limit(I2Cx, AddrTMP1075, PBM_TMP1075_TEMP_LO) == SUCCESS) {
								Error = TMP1075_set_high_limit(I2Cx, AddrTMP1075, PBM_TMP1075_TEMP_HI);
							}
						}
					}
				}
			}
		}
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
	}
	if (Error != 0) {
		return ERROR_N;
	}
	return SUCCESS;
}

