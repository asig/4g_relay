#include <stdint.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_crc.h"

uint32_t flash_read_page(uint32_t page_index, void *data) {
    abort();
}

uint32_t flash_write_page(uint32_t page_index, void *data) {
    abort();
};


HAL_StatusTypeDef HAL_CRC_Init(CRC_HandleTypeDef *hcrc) {
    abort();
}

HAL_StatusTypeDef HAL_CRC_DeInit(CRC_HandleTypeDef *hcrc) {
    abort();
}

uint32_t HAL_CRC_Accumulate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength){
    abort();
}

