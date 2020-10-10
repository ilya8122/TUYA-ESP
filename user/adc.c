// Copyright 2018-2025 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>  //malloc
#include "define.h"

/*#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_attr.h"*/

//#include "mpu_wrappers.h"
//#include "esp_err.h"

/*#include "esp_log.h"
#include "esp_phy_init.h"
#include "esp_heap_caps.h"*/

#include "adclib.h"

// static const char *TAG = "adc";
///////////////////////////////////////////////////////////
#define portMAX_DELAY 0xFFFFFFFF
#define MALLOC_CAP_8BIT (1 << 2)
typedef void *SemaphoreHandle_t;
typedef int32_t esp_err_t;
#define ESP_OK 0 /*!< esp_err_t value indicating success (no error) */
/*
#define true 1
#define false 0
*/
#define PRIVILEGED_FUNCTION
#define PRIVILEGED_DATA
#define portUSING_MPU_WRAPPERS 0

typedef struct {
  uint8_t params[128]; /*!< opaque PHY initialization parameters */
} esp_phy_init_data_t;

typedef void *xQueueHandle;
typedef void *QueueHandle_t;
#define portLONG long
typedef unsigned portLONG portTickType;
#define queueQUEUE_TYPE_MUTEX ((unsigned char)1U)
#define portBASE_TYPE long
#define pdFALSE ((portBASE_TYPE)0)

extern xQueueHandle xQueueCreateMutex(unsigned char ucQueueType)
    PRIVILEGED_FUNCTION;  // TUYA SDK
#define xSemaphoreCreateMutex() xQueueCreateMutex(queueQUEUE_TYPE_MUTEX)

#define queueSEND_TO_BACK ((portBASE_TYPE)0)
#define semGIVE_BLOCK_TIME ((portTickType)0U)
// extern  xSemaphoreGive( xSemaphore );
extern signed portBASE_TYPE xQueueGenericSend(
    xQueueHandle xQueue, const void *const pvItemToQueue,
    portTickType xTicksToWait, portBASE_TYPE xCopyPosition) PRIVILEGED_FUNCTION;
#define xSemaphoreGive(xSemaphore)                                        \
  xQueueGenericSend((xQueueHandle)(xSemaphore), NULL, semGIVE_BLOCK_TIME, \
                    queueSEND_TO_BACK)

// extern  xSemaphoreTake( xSemaphore, xBlockTime );
extern signed portBASE_TYPE xQueueGenericReceive(
    xQueueHandle xQueue, const void *const pvBuffer, portTickType xTicksToWait,
    portBASE_TYPE xJustPeek) PRIVILEGED_FUNCTION;  // TUYA SDK
#define xSemaphoreTake(xSemaphore, xBlockTime) \
  xQueueGenericReceive((xQueueHandle)(xSemaphore), NULL, (xBlockTime), pdFALSE)

//////////////////////////////////////////////////////////
#define ADC_CHECK(a, str, ret_val)                            \
  if (!(a)) {                                                 \
    ESP_LOGE(TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
    return (ret_val);                                         \
  }

extern uint16_t test_tout();
extern void phy_adc_read_fast(uint16_t *adc_addr, uint16_t adc_num,
                              uint8_t adc_clk_div);
extern uint16_t phy_get_vdd33();

typedef struct {
  adc_config_t config;
  SemaphoreHandle_t adc_mux;
} adc_handle_t;

adc_handle_t *adc_handle = NULL;

esp_err_t adc_read(uint16_t *data) {
  /*
      ADC_CHECK(data, "parameter pointer is empty", ESP_ERR_INVALID_ARG);
      ADC_CHECK(adc_handle, "ADC has not been initialized yet.", ESP_FAIL);
   */
  uint16_t ret = 0;
  xSemaphoreTake(adc_handle->adc_mux, portMAX_DELAY);

  if (adc_handle->config.mode == ADC_READ_TOUT_MODE) {
    ret = test_tout(0);

    if (ret != 0xFFFF) {
      // The working voltage of ADC is designed according to 1.1v. Later, the
      // actual working voltage of ADC is increased to 1.2v, so this scale is
      // added.
      ret = ret * 12 / 11;

      if (ret > 1023) {
        // 10-bit precision ADC
        ret = 1023;
      }
    }
  } else if (adc_handle->config.mode == ADC_READ_VDD_MODE) {
    ret = phy_get_vdd33();

    if (ret != 0xFFFF) {
      // The working voltage of ADC is designed according to 1.1v. Later, the
      // actual working voltage of ADC is increased to 1.2v, so this scale is
      // added.
      ret = ret * 12 / 11;
    }
  }

  *data = ret;
  xSemaphoreGive(adc_handle->adc_mux);
  return ESP_OK;
}

esp_err_t adc_read_fast(uint16_t *data, uint16_t len) {
  /*
      ADC_CHECK(data && len > 0, "parameter pointer is empty",
     ESP_ERR_INVALID_ARG);
      ADC_CHECK(adc_handle, "ADC has not been initialized yet.", ESP_FAIL);
      ADC_CHECK(adc_handle->config.mode == ADC_READ_TOUT_MODE, "adc_read_fast
     can only be used in ADC_READ_TOUT_MODE mode", ESP_ERR_INVALID_ARG);
      ADC_CHECK(adc_handle->config.clk_div >= 8 && adc_handle->config.clk_div <=
     32, "ADC sample collection clock=80M/clk_div, range[8, 32]", ESP_FAIL);
  */

  uint16_t i;
  uint16_t ret;

  xSemaphoreTake(adc_handle->adc_mux, portMAX_DELAY);
  phy_adc_read_fast(data, len, adc_handle->config.clk_div);

  for (i = 0; i < len; i++) {
    ret = data[i];

    if (ret != 0xFFFF) {
      // The working voltage of ADC is designed according to 1.1v. Later, the
      // actual working voltage of ADC is increased to 1.2v, so this scale is
      // added.
      ret = ret * 12 / 11;

      if (ret > 1023) {
        // 10-bit precision ADC
        ret = 1023;
      }
    }

    data[i] = ret;
  }

  xSemaphoreGive(adc_handle->adc_mux);
  return ESP_OK;
}

/*esp_err_t adc_deinit()
{
    ADC_CHECK(adc_handle, "ADC has not been initialized yet.", ESP_FAIL);

    if (adc_handle->adc_mux)
    {
        vSemaphoreDelete(adc_handle->adc_mux);
    }
    heap_caps_free(adc_handle);
    adc_handle = NULL;
    return ESP_OK;
}*/

esp_err_t adc_init(adc_config_t *config) {
  /*
      ADC_CHECK(config, "config error", ESP_ERR_INVALID_ARG);
      ADC_CHECK(NULL == adc_handle, "adc has been initialized", ESP_FAIL);
  */

  //////////////////////////////////////////////////////////////////////////////////
  /*
    uint8_t vdd33_const;
    esp_phy_init_data_t *phy_init_data;

    phy_init_data = (esp_phy_init_data_t *)esp_phy_get_init_data();
    vdd33_const = phy_init_data->params[107];
    */
  ///////////////////////////////////////////////////////////////////////////////

  /*
      ADC_CHECK((config->mode == ADC_READ_TOUT_MODE) ? (vdd33_const < 255) :
     true, "To read the external voltage on TOUT(ADC) pin, vdd33_const need less
     than 255", ESP_FAIL);
      ADC_CHECK((config->mode == ADC_READ_VDD_MODE) ? (vdd33_const == 255) :
     true, "When adc measuring system voltage, vdd33_const must be set to 255,",
     ESP_FAIL);
      ADC_CHECK(config->mode <= ADC_READ_MAX_MODE, "adc mode err", ESP_FAIL);
  */
  //   adc_handle = heap_caps_malloc(sizeof(adc_handle_t), MALLOC_CAP_8BIT);//
  //   In SDK, ``malloc(s)`` is equivalent to ``heap_caps_malloc(s,
  //   MALLOC_CAP_32BIT)``.
  adc_handle = malloc(sizeof(adc_handle_t));
  /*
      ADC_CHECK(adc_handle, "adc handle malloc error", ESP_ERR_NO_MEM);
  */
  memcpy(&adc_handle->config, config, sizeof(adc_config_t));
  adc_handle->adc_mux = xSemaphoreCreateMutex();
  /*
      if (NULL == adc_handle->adc_mux)
      {
          adc_deinit();
          ADC_CHECK(false, "Semaphore create fail", ESP_ERR_NO_MEM);
      }
  */
  return ESP_OK;
}

