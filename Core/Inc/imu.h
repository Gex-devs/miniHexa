#ifndef _IMU_H
#define _IMU_H

#include <motion_di.h>
#include <custom_bus.h>
#include <lsm6dsl_reg.h>
#include "cmsis_os.h"
#include "stdbool.h"

#define IMU_TASK_STACK_SIZE 256

typedef struct
{
  MDI_input_t *data_in;
  MDI_output_t *data_out;
} IMUContext_t;


void IMUInit();
void IMUTask(void *args);

#endif /* _IMU_H */