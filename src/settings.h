#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "stm32f1xx_hal.h" // for  FLASH_PAGE_SIZE


enum {
    PHONE_ENTRY_SIZE = 24,
    MAX_ALLOWED_PHONE_ENTRIES = 10,
    MAX_ALERT_PHONE_ENTRIES = 5,

    MAGIC1 = 's'<<24 | 'e'<<16 | 't'<<8 | 't',
    MAGIC2 = 'i'<<24 | 'n'<<16 | 'g'<<8 | 's',
};

typedef char SettingsPhoneEntry[PHONE_ENTRY_SIZE];

typedef struct {
/* ofs (dec) */
/* 0000 */    uint32_t magic0; // 'sett'
/* 0004 */    uint32_t magic1; // 'ings'
/* 0008 */    uint32_t pin;
/* 0012 */    SettingsPhoneEntry allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES];
/* 0252 */    SettingsPhoneEntry alert_phone_numbers[MAX_ALERT_PHONE_ENTRIES];
/* 0372 */    uint8_t padding[1020-372];
/* 1020 */    uint32_t crc32;
} Settings;

#ifndef size_assert
#define size_assert( what, howmuch ) \
  typedef char what##_size_wrong_[( !!(sizeof(what) == howmuch) )*2-1 ]
#endif

size_assert( Settings, FLASH_PAGE_SIZE);

void settings_init();

bool settings_add_allowed_phone_number(const char *phone);
bool settings_remove_allowed_phone_number(const char *phone);
bool settings_remove_all_allowed_phone_numbers();

bool settings_add_alert_phone_number(const char *phone);
bool settings_remove_alert_phone_number(const char *phone);
bool settings_remove_all_alert_phone_numbers();

uint32_t settings_get_pin();
void settings_set_pin(uint32_t pin);

void settings_write();