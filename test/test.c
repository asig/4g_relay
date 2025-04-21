#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pdu.h"
#include "settings.h"

extern Settings settings;

void test_settings_add_allowed_phone_number_empty() {
    printf("Testing settings_add_allowed_phone_number when numbers are empty... ");

    memset(&settings, 0, sizeof(settings));

    bool res = settings_add_allowed_phone_number("+41791234567");
    if (!res) {
        printf("Failed to add phone number.\n");
        return;
    }

    // Check if the phone number was added correctly
    if (strcmp(settings.allowed_phone_numbers[0], "+41791234567") != 0) {
        printf("Phone number mismatch. Expected: +41791234567, Got: %s\n", settings.allowed_phone_numbers[0]);
        return;
    }
    // Check that the other entries are still empty
    for (int i = 1; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        if (settings.allowed_phone_numbers[i][0] != '\0') {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    printf(" OK\n");
}

void test_settings_add_allowed_phone_number() {
    printf("Testing settings_add_allowed_phone_number... ");

    memset(&settings, 0, sizeof(settings));
    strcpy(settings.allowed_phone_numbers[0], "EXISTING");

    bool res = settings_add_allowed_phone_number("+41791234567");
    if (!res) {
        printf("Failed to add phone number.\n");
        return;
    }

    // Check if the phone number was added correctly
    if (strcmp(settings.allowed_phone_numbers[1], "+41791234567") != 0) {
        printf("Phone number mismatch. Expected: +41791234567, Got: %s\n", settings.allowed_phone_numbers[0]);
        return;
    }

    // Check that the first entry is still intact
    if (strcmp(settings.allowed_phone_numbers[0], "EXISTING") != 0) {
        printf("First entry mismatch. Expected: EXISTING, Got: %s\n", settings.allowed_phone_numbers[0]);
        return;
    }
   
    // Check that the other entries are still empty
    for (int i = 2; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        if (settings.allowed_phone_numbers[i][0] != '\0') {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    printf(" OK\n");
}

void test_settings_add_allowed_phone_number_full() {
    printf("Testing settings_add_allowed_phone_number when entries are full... ");

    memset(&settings, 0, sizeof(settings));
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        strcpy(settings.allowed_phone_numbers[i], "foo");
    }

    bool res = settings_add_allowed_phone_number("+41791234567");
    if (res) {
        printf("adding phone number succeeded when it should have failed.\n");
        return;
    }

    // Check that the phone numbers are untouched
    for (int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        if (strcmp(settings.allowed_phone_numbers[i], "foo") != 0) {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    printf(" OK\n");
}

void test_settings_remove_allowed_phone_number_first() {
    printf("Testing settings_remove_allowed_phone_number, removing the first entry... ");

    memset(&settings, 0, sizeof(settings));
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        sprintf(settings.allowed_phone_numbers[i], "foo%d", i);
    }

    bool res = settings_remove_allowed_phone_number("foo0");
    if (!res) {
        printf("removing phone number didn't succeeded.\n");
        return;
    }

    // Check the entries after removal
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES - 1; i++) {
        char expected[PHONE_ENTRY_SIZE];
        sprintf(expected, "foo%d", i + 1);
        if (strcmp(settings.allowed_phone_numbers[i], expected) != 0) {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }
    // Check that the last entry is empty
    if (settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1][0] != '\0') {
        printf("Last entry should be empty, but got: %s\n", settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1]);     
        return;
    }
    
    printf(" OK\n");
}

void test_settings_remove_allowed_phone_number_middle() {
    printf("Testing settings_remove_allowed_phone_number, removing a middle entry... ");

    memset(&settings, 0, sizeof(settings));
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        sprintf(settings.allowed_phone_numbers[i], "foo%d", i);
    }

    bool res = settings_remove_allowed_phone_number("foo5");
    if (!res) {
        printf("removing phone number didn't succeeded.\n");
        return;
    }

    // check entries 0 - 4
    for(int i = 0; i < 5; i++) {
        char expected[PHONE_ENTRY_SIZE];
        sprintf(expected, "foo%d", i);
        if (strcmp(settings.allowed_phone_numbers[i], expected) != 0) {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    // Check entries 6 - MAX_ALLOWED_ENTRIES - 1
    for(int i = 5; i < MAX_ALLOWED_PHONE_ENTRIES-1; i++) {
        char expected[PHONE_ENTRY_SIZE];
        sprintf(expected, "foo%d", i + 1);
        if (strcmp(settings.allowed_phone_numbers[i], expected) != 0) {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    // Check that the last entry is empty
    if (settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1][0] != '\0') {
        printf("Last entry should be empty, but got: %s\n", settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1]);     
        return;
    }
    
    printf(" OK\n");
}

void test_settings_remove_allowed_phone_number_last() {
    printf("Testing settings_remove_allowed_phone_number, removing the last entry... ");

    memset(&settings, 0, sizeof(settings));
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES; i++) {
        sprintf(settings.allowed_phone_numbers[i], "foo%d", i);
    }

    char to_remove[PHONE_ENTRY_SIZE];
    sprintf(to_remove, "foo%d", MAX_ALLOWED_PHONE_ENTRIES - 1);
    bool res = settings_remove_allowed_phone_number(to_remove);
    if (!res) {
        printf("removing phone number didn't succeeded.\n");
        return;
    }

    // check entries 0 - MAX_ALLOWED_PHONE_ENTRIES - 1
    for(int i = 0; i < MAX_ALLOWED_PHONE_ENTRIES-1; i++) {
        char expected[PHONE_ENTRY_SIZE];
        sprintf(expected, "foo%d", i);
        if (strcmp(settings.allowed_phone_numbers[i], expected) != 0) {
            printf("Unexpected phone number in entry %d: %s\n", i, settings.allowed_phone_numbers[i]);
            return;
        }
    }

    // Check that the last entry is empty
    if (settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1][0] != '\0') {
        printf("Last entry should be empty, but got: %s\n", settings.allowed_phone_numbers[MAX_ALLOWED_PHONE_ENTRIES - 1]);     
        return;
    }
    
    printf(" OK\n");
}

void test_decode_pdu() {
    printf("Testing PDU decoding... ");

    char buf[256];
    char oa[20];
    char msg[160];

    // Example PDU string
    char pdu[] = "07911487390171F9040B911497214365F700005240800042218006537A985E9F03";

    if (!pdu_receive_message(pdu, oa, sizeof(oa), msg, sizeof(msg))) {
        printf("PDU decoding test failed.\n");
        return;
    }

    char *expected_oa = "+41791234567";
    char *expected_msg = "Status";
    if (strcmp(oa, expected_oa) != 0 || strcmp(msg, expected_msg) != 0) {
        printf("PDU decoding test failed. Expected: oa=%s, msg=%s, got oa=%s, msg=%s\n", expected_oa, expected_msg, oa, msg);
        return;
    }
    printf(" OK\n");
}

void test_encode_pdu() {
    printf("Testing PDU encoding... ");

    char buf[256];
    pdu_create_message("+41791234567", "hello world", buf, sizeof(buf));

    char *expected = "0001000B911497214365F700000BE8329BFD06DDDF723619";
    if (strcmp(buf, expected) != 0) {
        printf("\nPDU encoding test failed. Expected: %s, Got: %s\n", expected, buf);
        return;
    }
    printf(" OK\n");
}

int main() {
    test_decode_pdu();
    test_encode_pdu();

    test_settings_add_allowed_phone_number_empty();
    test_settings_add_allowed_phone_number();
    test_settings_add_allowed_phone_number_full();
    test_settings_remove_allowed_phone_number_first();
    test_settings_remove_allowed_phone_number_middle();
    test_settings_remove_allowed_phone_number_last();

    return 0;
}
