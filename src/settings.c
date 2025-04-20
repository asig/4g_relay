#include "settings.h"

#include <string.h>

#include "stm32f1xx_hal_crc.h"

#include "flash.h"

#define SETTINGS_PAGE 31

static Settings settings;

static uint32_t compute_crc32(Settings *settings);

static bool add_phone_number(SettingsPhoneEntry *entries, uint32_t max_entries, const char *phone) {
    return false;
}

static bool remove_phone_number(SettingsPhoneEntry *entries, uint32_t max_entries, const char *phone) {
    return false;
}

void settings_init() {
    // TODO(asigner): implement me!
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
    // TODO(asigner): implement write!
}

uint32_t compute_crc32(Settings *settings) {
    CRC_HandleTypeDef hcrc;  // Handle für die CRC-Peripherie
    hcrc.Instance = CRC;
    HAL_CRC_Init(&hcrc); // TODO(asigner): Check return value!
    uint32_t crc32 = HAL_CRC_Accumulate(&hcrc, (uint32_t*)settings, (FLASH_PAGE_SIZE-4)/4);
    HAL_CRC_DeInit(&hcrc);
    return crc32;
}


