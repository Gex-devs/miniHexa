#include <main.h>
#include <imu.h>

typedef enum
{
    CTRL1_XL = 0x10,
    CTRL2_G = 0x11,
    CTRL3_C = 0x12,
    OUT_TEMP_L = 0x20,
    OUT_TEMP_H = 0x21,

}imu_registers_t;





void IMU_Init(void) {
    /* USER CODE BEGIN IMU_Init */
    /* USER CODE END IMU_Init */
}



void IMU_setPowerMode(imu_handle_t *imu_handle, imu_devices_t device, imu_powermode_t mode) {

    HAL_I2C_Mem_Write(imu_handle->hi2c, imu_handle->address, 0x10, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&mode, 1, HAL_MAX_DELAY);
}