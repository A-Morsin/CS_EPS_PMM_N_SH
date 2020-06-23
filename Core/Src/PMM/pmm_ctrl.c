
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "INA231.h"
#include "pmm_struct.h"
#include "pmm_config.h"

#include "pmm_ctrl.h"

/**********************TO DO ***********************************/
/*1. Change PMM_Set_MUX_CAN_CPUm_CPUb after 08.06
 *
 *
 *
 */


/** @brief  Set state (enable/disable) PMM power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_channel - number of channel PMM :
								PMM_PWR_Ch_CANmain
								PMM_PWR_Ch_CANbackup
								PMM_PWR_Ch_VBAT1_eF1 
								PMM_PWR_Ch_VBAT1_eF2 
								PMM_PWR_Ch_VBAT2_eF1 
								PMM_PWR_Ch_VBAT2_eF2 
								PMM_PWR_Ch_PBMs_Logic
								PMM_PWR_Ch_Deploy_Logic
								PMM_PWR_Ch_Deploy_Power
								PMM_PWR_Ch_5V_Bus				
								PMM_PWR_Ch_3_3V_Bus			
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	
	_PMM_table pmm_table;

	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

 	pmm_table = PMM__Table( num_pwr_channel );

	if(num_pwr_channel == PMM_PWR_Ch_CANmain ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_CANmain = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_CANmain = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_CANbackup = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_CANbackup = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat1_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat1_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat1_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat1_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat2_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat2_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat2_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat2_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_PBMs_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_PBMs_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Deploy_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Deploy_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Deploy_Power = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Deploy_Power = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_5V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_5V_Bus = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_3_3V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_3_3V_Bus = DISABLE;
		}

	}else{
		return ERROR_N;
	}

	pmm_ptr->PMM_save_conf_flag = 1;//Need save configure in FRAM.

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( state_channel == ENABLE ){
			error_I2C = TCA9539_Set_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );

		}else if( state_channel == DISABLE ){
			error_I2C = TCA9539_Reset_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
		}

		if( error_I2C == SUCCESS ){
			error_I2C = TCA9539_conf_IO_dir_output( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	LL_mDelay(20); //Delay for startup power supply
	error_I2C = PMM_Check_state_PWR_CH( pmm_ptr, num_pwr_channel );
	
	#ifdef DEBUGprintf
		if (error_I2C == ERROR){
			Error_Handler();
		}
	#endif

	return error_I2C;
}



/** @brief  Checking the state of the PMM power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_channel - number of channel PMM :
								PMM_PWR_Ch_CANmain
								PMM_PWR_Ch_CANbackup
								PMM_PWR_Ch_VBAT1_eF1 
								PMM_PWR_Ch_VBAT1_eF2 
								PMM_PWR_Ch_VBAT2_eF1 
								PMM_PWR_Ch_VBAT2_eF2
								PMM_PWR_Ch_PBMs_Logic
								PMM_PWR_Ch_Deploy_Logic
								PMM_PWR_Ch_Deploy_Power
								PMM_PWR_Ch_5V_Bus				
								PMM_PWR_Ch_3_3V_Bus	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	uint8_t read_val_pin_EN;
	_PMM_table pmm_table;

	pmm_table = PMM__Table(  num_pwr_channel );

	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9539_read_input_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext, &read_val_pin_EN);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}
	

	if( error_I2C == SUCCESS ){

		pmm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		if(num_pwr_channel == PMM_PWR_Ch_CANmain ){

			if( pmm_ptr->PWR_Ch_State_CANmain == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_CANmain = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_CANmain = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){

			if( pmm_ptr->PWR_Ch_State_CANbackup == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_CANbackup = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_CANbackup = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){

			if( pmm_ptr->PWR_Ch_State_Vbat1_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){

			if( pmm_ptr->PWR_Ch_State_Vbat1_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){

			if( pmm_ptr->PWR_Ch_State_Vbat2_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){

			if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){

			if( pmm_ptr->PWR_Ch_State_PBMs_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){

			if( pmm_ptr->PWR_Ch_State_Deploy_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){

			if( pmm_ptr->PWR_Ch_State_Deploy_Power == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Power = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Deploy_Power = ERROR; ///0-ERROR
			}
		
		}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){

			if( pmm_ptr->PWR_Ch_State_5V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_5V_Bus = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_5V_Bus = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){

			if( pmm_ptr->PWR_Ch_State_3_3V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = ERROR; ///0-ERROR
			}

		}

	}else{
		pmm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		if(num_pwr_channel == PMM_PWR_Ch_CANmain ){
			pmm_ptr->Error_PWR_Ch_State_CANmain = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){
			pmm_ptr->Error_PWR_Ch_State_CANbackup = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = ERROR; ///0-ERROR
		
		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = ERROR; ///0-ERROR
	
		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){
				pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Power = ERROR; ///0-ERROR
		
		}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){
				pmm_ptr->Error_PWR_Ch_State_5V_Bus = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){
				pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = ERROR; ///0-ERROR
		}
	}
	
	return error_I2C;
}


/** @brief  Get Power Good all power channel status om PMM.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_PG_all_PWR_CH( _PMM *pmm_ptr ){



	//return error_I2C;
}


/** @brief  Get temperature from TMP1075 sensor.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_Temperature( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr ){

	int8_t temp_value;
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = TMP1075_read_int8_temperature( I2Cx,tmp1075_addr, &temp_value);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}


	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Temp_sensor = 0x7F;
		pmm_ptr->Error_TMP1075_sensor = ERROR;
	}else{
		pmm_ptr->Error_TMP1075_sensor = SUCCESS;
		pmm_ptr->Temp_sensor = temp_value;
	}

	return error_I2C;
}



/** @brief  Get value current, voltage and power of VBAT Power channel
	@param  *pdm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_ch - number power channel.
							PMM_PWR_Ch_VBAT1_eF1
							PMM_PWR_Ch_VBAT1_eF2
							PMM_PWR_Ch_VBAT2_eF1
							PMM_PWR_Ch_VBAT2_eF2
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_PWR_CH_VBAT_I_V_P( _PMM *pmm_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PMM_table pmm_table;
	
	
	// Switch MUX to PDM I2C bus on PMM 
	SW_TMUX1209_I2C_main_PMM(); 

	pmm_table = PMM__Table(num_pwr_ch);

	//Read INA231
	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = INA231_Get_I_V_P_int16( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if(val_bus_voltage < 5 ){ //If power less than 5mV equate to zero.
		val_bus_voltage = 0;
	}

	if(val_power < 5 ){ //If power less than 5mW equate to zero.
		val_power = 0;
	}


	if( error_I2C == ERROR_N ){//Error I2C INA231 
		val_current = 0;
		val_bus_voltage = 0;
		val_power = 0;
	}

	if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF1 ){
		pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat1_eF1_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = ERROR; 
		}
		
	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF2 ){
		pmm_ptr->PWR_Ch_Vbat1_eF2_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat1_eF2_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = ERROR; 
		}
	
	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF1 ){
		pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat2_eF1_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = ERROR; 
		}

	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF2 ){
		pmm_ptr->PWR_Ch_Vbat2_eF2_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat2_eF2_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = ERROR; 
		}

	}

	return error_I2C;
}


//********************change FN PMM_Set_MUX_CAN_CPUm_CPUb after 08.06 *********************//
/** @brief  Setup multiplexor. CAN bus switching between CPUm and CPUb.
	@param  num_CAN_pwr_channel - number of channel :
								CPUmain
								CPUbackup
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( uint8_t active_CPU ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;


	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( active_CPU == CPUmain ){
			error_I2C = TCA9539_conf_IO_dir_input( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );

		}else if(  active_CPU == CPUbackup ){
			error_I2C = TCA9539_Set_output_pin( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );
			error_I2C = TCA9539_conf_IO_dir_output( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );

		}else{
			error_I2C = ERROR_N;
		}


		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	return error_I2C;
}



