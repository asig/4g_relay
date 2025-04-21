#pragma once

#include "stm32f1xx_hal.h"

#define FLASH_BASE_ADDRESS 0x08000000

#define FLASH_PAGE_ADDR(idx) (FLASH_BASE_ADDRESS + idx*FLASH_PAGE_SIZE)

// Functions return HAL_FLASH_ERROR_xxx codes
uint32_t flash_read_page(uint32_t page_index, void *data);

uint32_t flash_write_page(uint32_t page_index, void *data); // data needs to provide room for FLASH_PAGE_SIZE bytes

