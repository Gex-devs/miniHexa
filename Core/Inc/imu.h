
//
// Created by Gedewon Jerene on 29/08/2026.
// IMU handler for LSM6DS3TR-C
#include <stdint.h>
#include "main.h"

#ifndef IMU_H
#define IMU_H


#define LSM6DS_DEFAULT_ADDR 101010b

// Imu handler
typedef struct
{
    I2C_HandleTypeDef *hi2c;

    uint8_t address;

} imu_handle_t;

// Enums
typedef enum
{
    accelerometer,
    gyroscope,
    acc_gyro
} imu_powermode_t;

typedef enum
{

    power_down,
    low_power,
    normal_mode,
    highPer_mode

} imu_powermode;

typedef enum
{
    accelerometer,

    gyroscope

} imu_devices_t;






void IMU_Init(imu_handle_t *imu_handle);
void IMU_setPowerMode(imu_handle_t *imu_handle, imu_devices_t device, imu_powermode_t mode);
void IMU_getTemperature(imu_handle_t *imu_handle, uint16_t *temp);
void IMU_getAcceleration(imu_handle_t *imu_handle, int16_t *accel_data);
void IMU_getGyroscope(imu_handle_t *imu_handle, int16_t *gyro);


#endif