#include "imu.h"

void IMUInit()
{
  float odr_freq = 416.0f; // khz
  MotionDI_Initialize(&odr_freq);

  // Enable Interrupt pin 1 on Gryo data ready
  uint8_t INT1_GYRO_MASK = 0x2;
  BSP_I2C1_WriteReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_INT1_CTRL, &INT1_GYRO_MASK, sizeof(INT1_GYRO_MASK));

  // enable GYRO and ACC at high performance
  uint8_t HIGH_PERF_GYRO_ACC = 0x60;
  BSP_I2C1_WriteReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G, &HIGH_PERF_GYRO_ACC, sizeof(HIGH_PERF_GYRO_ACC));
  BSP_I2C1_WriteReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, &HIGH_PERF_GYRO_ACC, sizeof(HIGH_PERF_GYRO_ACC));

  // WARNING: Value recieved from AI, might be incorrect.
  uint8_t FS_G_MASK = 0b11110000; // Set Full-scale factor to +-250dps
  uint8_t CTRL2_G_VALUE = 0;
  BSP_I2C1_ReadReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G, &CTRL2_G_VALUE, sizeof(CTRL2_G_VALUE));
  CTRL2_G_VALUE &= FS_G_MASK;
  BSP_I2C1_WriteReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL2_G, &CTRL2_G_VALUE, sizeof(int8_t));

  uint8_t FS_XL_MASK = 0b11111100; // Set Full-scale to +-2g
  uint8_t CTRL1_XL_VALUE = 0;
  BSP_I2C1_ReadReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, &CTRL1_XL_VALUE, sizeof(CTRL1_XL_VALUE));
  CTRL1_XL_VALUE &= FS_XL_MASK;
  BSP_I2C1_WriteReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, &CTRL1_XL_VALUE, sizeof(int8_t));
}


void IMUTask(void *args)
{
  for (;;)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    IMUContext_t *imu = (IMUContext_t *)args;

    MDI_input_t *data_in = imu->data_in;
    MDI_output_t *data_out = imu->data_out;

    float SenestivityMultiplierGyro = 0.008750f;
    float SenestivityMultiplierAcc = 0.000061f;
    // GYRO data ready interrupt

    uint8_t status_reg_value = 0;
    uint8_t GDA_BIT_MASK = 0x02; // bit mask for GDA (Gyroscope Data Available) in STATUS_REG
    BSP_I2C1_ReadReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_STATUS_REG, &status_reg_value, sizeof(status_reg_value));

    bool newData = (status_reg_value & GDA_BIT_MASK) >> 1;

    if (newData)
    {
      uint8_t gyro_raw[6] = {0};

      BSP_I2C1_ReadReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,
                       LSM6DSL_ACC_GYRO_OUTX_L_G,
                       &gyro_raw[0],
                       sizeof(gyro_raw));

      data_in->Gyro[0] = ((int16_t)((gyro_raw[1] << 8) | gyro_raw[0])) * SenestivityMultiplierGyro;
      data_in->Gyro[1] = ((int16_t)((gyro_raw[3] << 8) | gyro_raw[2])) * SenestivityMultiplierGyro;
      data_in->Gyro[2] = ((int16_t)((gyro_raw[5] << 8) | gyro_raw[4])) * SenestivityMultiplierGyro;

      uint8_t acc_raw[6] = {0};
      BSP_I2C1_ReadReg(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,
                       LSM6DSL_ACC_GYRO_OUTX_L_XL,
                       &acc_raw[0],
                       sizeof(acc_raw));

      data_in->Acc[0] = ((int16_t)((acc_raw[1] << 8) | acc_raw[0])) * SenestivityMultiplierAcc;
      data_in->Acc[1] = ((int16_t)((acc_raw[3] << 8) | acc_raw[2])) * SenestivityMultiplierAcc;
      data_in->Acc[2] = ((int16_t)((acc_raw[5] << 8) | acc_raw[4])) * SenestivityMultiplierAcc;

      MotionDI_update(data_out, data_in);
    }
  }
}