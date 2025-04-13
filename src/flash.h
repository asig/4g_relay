#pragma once

#include "stm32f1xx_hal.h"


enum FlashError {
    FLASH_ERROR_NONE = HAL_FLASH_ERROR_NONE,
    FLASH_ERROR_PROG = HAL_FLASH_ERROR_PROG,
    FLASH_ERROR_WRP = HAL_FLASH_ERROR_WRP,
    FLASH_ERROR_OPTV = HAL_FLASH_ERROR_OPTV
};


// Functions return HAL_FLASH_ERROR_xxx codes
uint32_t flash_read_page(uint32_t page_index, void *data);

uint32_t flash_write_page(uint32_t page_index, void *data); // data needs to provide room for HAL_FLASH_PAGE_SIZE bytes

