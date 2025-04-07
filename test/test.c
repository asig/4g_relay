#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pdu.h"


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
    return 0;
}
