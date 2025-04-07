#include "pdu.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint8_t buf[512];

int hexdigit(char c) {    
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return -1; // Invalid hex digit
    }
}

size_t encode_address(const char *address, uint8_t *buf) {
    int outpos = 0;

    if (address[0] == '+') {
        address++;
        // Remove the '+' sign from the address
    }

    // Encode the length of the address
    size_t len = strlen(address);
    buf[outpos++] = len;

    // Encode the type of address
    buf[outpos++] = 0x91; // International number

    // Encode the address itself
    for (size_t i = 0; i < len; i += 2) {        
        uint8_t lo = (address[i] - '0') & 0x0F;
        uint8_t hi = (i + 1 < len) ? (address[i + 1] - '0') & 0x0F : 0xF;
        buf[outpos++] = (hi << 4) | lo;
    }
    return outpos;
}

// Function to parse the address from the PDU buffer
size_t decode_address(uint8_t *buf, char *address, size_t address_len) {
    int outpos = 0;

    uint8_t len = buf[0];

    if (address_len < (len+1)/2 + 2) {
        return 0; // Not enough space
    }
    
    uint8_t t = buf[1];
    if (t == 0x91) {
        // International number
        address[outpos++] = '+';
    }    
    int i = 0;
    uint8_t *p = (uint8_t*)&buf[2];
    while (i < len) {
        uint8_t b = *(p++);
        uint8_t hi = b >> 4;
        uint8_t lo = b & 0x0F;
        if (i < len) {
            address[outpos++] = lo + '0';
            i++;
        }
        if (i < len) {
            address[outpos++] = hi + '0';
            i++;
        }
    }
    address[outpos] = 0;
    return (len+1)/2 + 2;

}

size_t encode_7bit_pdu(const char *input, size_t input_len, uint8_t *output) {
    size_t output_index = 0;
    uint8_t shift_cur = 0;
    uint8_t shift_next = 7;
    uint8_t mask = 0x1;

    for (size_t i = 0; i < input_len; i++) {
        uint8_t cur = input[i] & 0x7F; // Mask to 7 bits
        uint8_t next = (i < input_len - 1 ? input[i+1] : 0) & 0x7f;
        uint8_t out = (cur >> shift_cur) | ((next & mask) << shift_next);
        output[output_index++] = out;

        shift_cur++;
        shift_next--;
        mask = mask << 1 | 1;
        
        if (shift_cur == 7) {
            // Full next byte is already shifted in
            shift_cur = 0;
            shift_next = 7;
            mask = 0x1;
            i++;
        }
    }
    return output_index;
}

// Function to decode a 7-bit packed PDU
void decode_7bit_pdu(const uint8_t *input, size_t input_len, char *output) {
    size_t output_index = 0;
    uint8_t carry_bits = 0;
    int carry_count = 0;

    for (size_t i = 0; i < input_len; i++) {
        uint8_t current_byte = input[i];

        // Extract the 7 bits from the current byte and carry bits
        output[output_index++] = ((current_byte << carry_count) | carry_bits) & 0x7F;

        // Update carry bits and carry count
        carry_bits = current_byte >> (7 - carry_count);
        carry_count++;

        // If carry count reaches 7, reset it and output the carry bits
        if (carry_count == 7) {
            output[output_index++] = carry_bits & 0x7F;
            carry_bits = 0;
            carry_count = 0;
        }
    }

    // Null-terminate the output string
    output[output_index] = '\0';
}


bool pdu_receive_message(char *pdu, char *oa, size_t oa_len, char *message, size_t message_len) {
    int i = 0;
    int j = 0;
    while (i < strlen(pdu)) {
        uint8_t hi = hexdigit(pdu[i++]);
        uint8_t lo = hexdigit(pdu[i++]);
        buf[j++] = hi << 4 | lo;
    }

    // parse PDU
    // https://www.gsmfavorites.com/documents/sms/packetformat/ 

    int pos = 0;

    // skip SCA information, we don't care
    pos += buf[pos] + 1; // skip SCA information (+1 for length byte)
    uint8_t pdu_type = buf[pos++];

    pos += decode_address(&buf[pos], oa, oa_len);
    uint8_t pid = buf[pos++];
    uint8_t dcs = buf[pos++];
    pos += 7; // skip SCTS
    uint8_t udl = buf[pos++];


    memset(message, 0, message_len);
    decode_7bit_pdu(&buf[pos], udl, (char*)message);
    return true;
}

bool pdu_create_message(char *da, char *message, uint8_t *pdu, size_t pdu_len) {
    char *hex = "0123456789ABCDEF";

    int pos = 0;

    buf[pos++] = 0x00; // SCA length 0 --> no SCA
    buf[pos++] = 0x01; // PDU type: no validity period, SMS-SUBMIT
    buf[pos++] = 0x00; // Message reference
    pos += encode_address(da, &buf[pos]); // DA
    buf[pos++] = 0x00; // Protocol identifier: SMS

    buf[pos++] = 0x00; // Data coding scheme: 7-bit encoding
    buf[pos++] = strlen(message); 
    pos += encode_7bit_pdu(message, strlen(message), &buf[pos]);

    // Now, convert buf[0..pos] to hex
    for (int i = 0; i < pos; i++) {
        uint8_t b = buf[i];
        uint8_t hi = b >> 4;
        uint8_t lo = b & 0x0F;
        pdu[i * 2] = hex[hi];
        pdu[i * 2 + 1] = hex[lo];
    }
    pdu[pos * 2] = '\0'; // Null-terminate the hex string

    return true;
}
