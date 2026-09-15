/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    main_app.c
 * @author  ST67 Application Team
 * @brief   main_app program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Application */
#include "main.h"
#include "main_app.h"
#include "app_config.h"

#if (LOW_POWER_MODE > LOW_POWER_DISABLE)
#include "utilities_conf.h"
#include "stm32_lpm.h"
#endif /* LOW_POWER_MODE */

#include "w6x_api.h"
#include "spi_iface.h" /* SPI falling/rising_callback */
#include "logging.h"
#include "shell.h"
#include "logshell_ctrl.h"
#if (LOW_POWER_MODE == LOW_POWER_STDBY_ENABLE)
#error "low power standby mode not supported"
#endif /* LOW_POWER_MODE */

/* USER CODE BEGIN Includes */
#include <imu.h>

/* USER CODE END Includes */

/* Global variables ----------------------------------------------------------*/
/* USER CODE BEGIN GV */

/* USER CODE END GV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
;

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TaskHandle_t imuTaskHandle;

MDI_input_t data_in;
MDI_output_t data_out;

struct sockaddr_in addr_t = {0};
int32_t sock;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/**
 * @brief  Wi-Fi event callback
 * @param  event_id: Event ID
 * @param  event_args: Event arguments
 */
static void APP_wifi_cb(W6X_event_id_t event_id, void *event_args);

/**
 * @brief  Network event callback
 * @param  event_id: Event ID
 * @param  event_args: Event arguments
 */
static void APP_net_cb(W6X_event_id_t event_id, void *event_args);

/**
 * @brief  MQTT event callback
 * @param  event_id: Event ID
 * @param  event_args: Event arguments
 */
static void APP_mqtt_cb(W6X_event_id_t event_id, void *event_args);

/**
 * @brief  BLE event callback
 * @param  event_id: Event ID
 * @param  event_args: Event arguments
 */
static void APP_ble_cb(W6X_event_id_t event_id, void *event_args);

/**
 * @brief  W6X error callback
 * @param  ret_w6x: W6X status
 * @param  func_name: function name
 */
static void APP_error_cb(W6X_Status_t ret_w6x, char const *func_name);

/* USER CODE BEGIN PFP */

void APP_init_socket();

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void main_app(void)
{
  W6X_Status_t ret;

  /* USER CODE BEGIN main_app_1 */

  static IMUContext_t imuContext;

  imuContext.data_in = &data_in;
  imuContext.data_out = &data_out;
  // Create IMU task
  xTaskCreate(
      IMUTask,
      "IMU",
      IMU_TASK_STACK_SIZE,
      &imuContext,
      5,
      &imuTaskHandle);
  /* USER CODE END main_app_1 */

  /* Initialize the logging utilities */
  LoggingInit();

  /* USER CODE BEGIN main_app_2 */

  /* USER CODE END main_app_2 */

  /* Register the application callback to received events from ST67W6X Driver */
  W6X_App_Cb_t App_cb = {0};
  App_cb.APP_wifi_cb = APP_wifi_cb;
  App_cb.APP_net_cb = APP_net_cb;
  App_cb.APP_ble_cb = APP_ble_cb;
  App_cb.APP_mqtt_cb = APP_mqtt_cb;
  App_cb.APP_error_cb = APP_error_cb;
  (void)W6X_RegisterAppCb(&App_cb);

  /* Initialize the ST67W6X Driver */
  ret = W6X_Init();
  if (ret != W6X_STATUS_OK)
  {
    LogError("Failed to initialize ST67W6X Driver, %" PRIi32 "\n", ret);
    goto _err;
  }

  /* USER CODE BEGIN main_app_3 */
  APP_init_socket(); // connec
  /* USER CODE END main_app_3 */

  LogInfo("##### Quitting the application\n");

  /* USER CODE BEGIN main_app_Last */

  /* USER CODE END main_app_Last */

_err:
  /* USER CODE BEGIN main_app_Err_1 */

  /* USER CODE END main_app_Err_1 */
  /* De-initialize the ST67W6X Driver */
  W6X_DeInit();

  /* USER CODE BEGIN main_app_Err_2 */

  /* USER CODE END main_app_Err_2 */
  LogInfo("##### Application end\n");
}

/* coverity[misra_c_2012_rule_5_8_violation : FALSE] */
/* coverity[misra_c_2012_rule_8_6_violation : FALSE] */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* USER CODE BEGIN HAL_GPIO_EXTI_Callback_1 */

  /* USER CODE END HAL_GPIO_EXTI_Callback_1 */
  /* Callback when data is available in Network CoProcessor to enable SPI Clock */
  if (GPIO_Pin == SPI_RDY_Pin)
  {
    if (HAL_GPIO_ReadPin(SPI_RDY_GPIO_Port, SPI_RDY_Pin) == GPIO_PIN_SET)
    {
      (void)spi_on_txn_data_ready();
    }
    else
    {
      (void)spi_on_header_ack();
    }
  }
  /* USER CODE BEGIN HAL_GPIO_EXTI_Callback_End */
  if (GPIO_Pin == GPIO_PIN_9)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        imuTaskHandle,
        &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  /* USER CODE END HAL_GPIO_EXTI_Callback_End */
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/* Private Functions Definition ----------------------------------------------*/
static void APP_wifi_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_wifi_cb_1 */

  /* USER CODE END APP_wifi_cb_1 */
}

static void APP_net_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_net_cb_1 */
  if (event_id == W6X_NET_EVT_SOCK_DATA_ID)
  {
    char data[5] = "Hallo";
    W6X_Net_Sendto(
        sock,
        data,
        sizeof(data),
        0,
        (struct sockaddr *)&addr_t,
        sizeof(addr_t));
    // Notify UDP task
    // Don't do heavy packet processing here
  }
  /* USER CODE END APP_net_cb_1 */
}

static void APP_mqtt_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_mqtt_cb_1 */

  /* USER CODE END APP_mqtt_cb_1 */
}

static void APP_ble_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_ble_cb_1 */

  /* USER CODE END APP_ble_cb_1 */
}

static void APP_error_cb(W6X_Status_t ret_w6x, char const *func_name)
{
  /* USER CODE BEGIN APP_error_cb_1 */

  /* USER CODE END APP_error_cb_1 */
  LogError("[%s] in %s API\n", W6X_StatusToStr(ret_w6x), func_name);
  /* USER CODE BEGIN APP_error_cb_2 */

  /* USER CODE END APP_error_cb_2 */
}

/* USER CODE BEGIN PFD */
void APP_init_socket()
{

  uint8_t remote_app_server_addr[4] = {192, 168, 0, 30};

  uint16_t remote_app_port = REMTOE_APP_PORT;

  int32_t net_ret = 0;
  int32_t ret_code = -1;
  int32_t sock = -1;

  // Create a UDP socket
  LogInfo("\nCreate a new socket\n");
  sock = W6X_Net_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0)
  {
    LogInfo("Socket creation failed\n");
    goto end;
  }
  LogInfo("Socket creation done\n");

  addr_t.sin_family = AF_INET;
  addr_t.sin_port = PP_HTONS(remote_app_port);
  addr_t.sin_addr.s_addr = ATON(remote_app_server_addr);

end:
  if (sock >= 0)
  {
    net_ret = W6X_Net_Close(sock); /* Close the TCP socket */
    if (net_ret != 0)
    {
      LogError("Socket close failed\n");
    }
    else
    {
      LogInfo("Socket closed\n");
    }
  }
}
/* USER CODE END PFD */
