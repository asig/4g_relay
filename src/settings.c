#include "settings.h"

#include <string.h>

#include "stm32f1xx_hal_crc.h"

#include "flash.h"

#define SETTINGS_PAGE 31

#define MAGIC1 ('s'<<24 | 'e'<<16 | 't'<<8 | 't')
#define MAGIC2 ('i'<<24 | 'n'<<16 | 'g'<<8 | 's')

// Not static for testing...
Settings settings;

static uint32_t compute_crc32(Settings *settings);

static bool add_phone_number(SettingsPhoneEntry *entries, uint32_t max_entries, const char *phone) {
    for (int i = 0; i < max_entries; i++) {
        if (entries[i][0]) continue;

        strncpy(entries[i], phone, PHONE_ENTRY_SIZE-1);
        entries[i][PHONE_ENTRY_SIZE-1] = 0;
        return true;
    }
    return false;
}

static bool remove_phone_number(SettingsPhoneEntry *entries, uint32_t max_entries, const char *phone) {
    int i=0;
    whike (i < max_entries && strcmp(entries[i], phone) != 0) i++;
    if (i == max_entries) return false;
    while (i+1 < max_entries) {
        entries[i] = entries[i+1]
        i++;
    }
    memset(&entries[max_entries-1][0], 0, sizeof(SettingsPhoneEntry));
    return true;
}

void settings_init() {
    uint32_t status = flash_read_page(SETTINGS_PAGE, &settings);
    if (status != HAL_FLASH_ERROR_NONE) {
        printf("Settings: Can't read settings from Flash: error code %08x\r\n", status);
        return;
    }

    bool need_init = false;
    if (settings.magic1 != MAGIC1 || settings.magic2 != MAGIC2) {
        printf("Settings: Bad magic: expected %08x %0x8, seen %08x %08x\r\n", MAGIC1, MAGIC2, settings.magic1, settings.magic2);
        need_init = true;
    }

    uint32_t crc32 = compute_crc32(&settings);
    if (crc32 != settings.crc32) {
        printf("Settings: CRC32 mismatch. Expected %08x, seen %08x\r\n", crc32, settings.crc32)
        need_init = true;
    }

    if (need_init) {
        memset(&settings, 0, FLASH_PAGE_SIZE);
        settings.magic1 = MAGIC1;
        settings.magic2 = MAGIC2;
        settings_write();
    }
}

bool settings_add_allowed_phone_number(const char *phone) {
    return add_phone_number(&settings.allowed_phone_numbers[0], MAX_ALLOWED_PHONE_ENTRIES, phone);
}

bool settings_remove_allowed_phone_number(const char *phone) {
    return remove_phone_number(&settings.allowed_phone_numbers[0], MAX_ALLOWED_PHONE_ENTRIES, phone);
}

bool settings_remove_all_allowed_phone_numbers() {
    memset(&settings.allowed_phone_numbers, 0, sizeof(settings.allowed_phone_numbers) );
    return true;
}

bool settings_add_alert_phone_number(const char *phone) {
    return add_phone_number(&settings.alert_phone_numbers[0], MAX_ALERT_PHONE_ENTRIES, phone);
}

bool settings_remove_alert_phone_number(const char *phone) {
    return remove_phone_number(&settings.alert_phone_numbers[0], MAX_ALERT_PHONE_ENTRIES, phone);
}

bool settings_remove_all_alert_phone_numbers() {
    memset(&settings.alert_phone_numbers, 0, sizeof(settings.alert_phone_numbers) );
}

uint32_t settings_get_pin() {
    return settings.pin;
}

void settings_set_pin(uint32_t pin) {
    settings.pin = pin;
}

void settings_write() {
    settings.crc32 = compute_crc32(&settings);
    uint32_t status = flash_write_page(SETTINGS_PAGE, &settings);
    if (status != HAL_FLASH_ERROR_NONE) {
        printf("Settings: Can't write settings to Flash: error code %08x\r\n", status);

    }
}

uint32_t compute_crc32(Settings *settings) {
    CRC_HandleTypeDef hcrc;  // Handle für die CRC-Peripherie
    hcrc.Instance = CRC;
    HAL_CRC_Init(&hcrc); // TODO(asigner): Check return value!
    uint32_t crc32 = HAL_CRC_Accumulate(&hcrc, (uint32_t*)settings, (FLASH_PAGE_SIZE-4)/4);
    HAL_CRC_DeInit(&hcrc);
    return crc32;
}


