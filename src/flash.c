#include "flash.h"

#include <stdint.h>
#include <string.h>

#include "stm32f1xx_hal_flash_ex.h"

static uint32_t buf[FLASH_PAGE_SIZE/sizeof(uint32_t)];

uint32_t flash_write_page(uint32_t page_index, void *data) {

	// copy data to a buffer so that we don't have to worry about alignment. Also, we don't care about speed that much :-)
	memcpy(buf, data, FLASH_PAGE_SIZE);

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;

	/* Unlock the flash to enable the flash control register access */
	HAL_FLASH_Unlock();

	uint32_t page_address = FLASH_BASE_ADDRESS + page_index * FLASH_PAGE_SIZE;

	/* Fill Erase Structure */
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = page_address;
	EraseInitStruct.NbPages     = 1;

	if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
		/*Error occurred while page erase.*/
		 return HAL_FLASH_GetError();
	}

	/* Program the user Flash area word by word*/
	for (uint32_t i = 0; i < FLASH_PAGE_SIZE/sizeof(uint32_t); i++) {
		/* Specifies word-level programming mode
		 * Increase by 4 to point to the next word address << assuming that word size if 4 bytes >>
		 */
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, page_address, buf[i]) == HAL_OK){
			page_address += 4;
		}else{
			/*  Error occurred while writing data in Flash memory */
			return HAL_FLASH_GetError();
		}
	}

	/* Lock the Flash to disable the flash control register access
	 * Recommended to protect the FLASH MEMORY against possible unwanted operation
	 */
	HAL_FLASH_Lock();

	return 0;
}

uint32_t flash_read_page(uint32_t page_index, void *data) {
	uint32_t page_address = FLASH_BASE_ADDRESS + page_index * FLASH_PAGE_SIZE;
	for(uint32_t i = 0; i < FLASH_PAGE_SIZE/sizeof(uint32_t); i++) {
		buf[i] = *(__IO uint32_t *)page_address;
		page_address += 4;
	}
	memcpy(data, buf, FLASH_PAGE_SIZE);
	return 0;
}
