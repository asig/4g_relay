#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// size_t parse_address(uint8_t *buf, char *address, size_t address_len);
// void decode_7bit_pdu(const uint8_t *input, size_t input_len, char *output);
bool pdu_receive_message(char *pdu, char *oa, size_t oa_len, char *message, size_t message_len);
bool pdu_create_message(char *da, char *message, uint8_t *pdu, size_t pdu_len);
