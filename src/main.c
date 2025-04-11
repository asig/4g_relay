#include "bsp_timer.h"
#include "bsp_uart_fifo.h"
#include <stdint.h>

#include "relay.h"
#include "pdu.h"

#define VERSION "0.1"
#define YEAR "2025"

enum State {
    WAITING_FOR_EVENT,
    CMT_RECEIVED,
} state;

#define RELAY 0

// Generic buffers
enum { bufsize = 512 };
static uint8_t buf[bufsize];
static uint8_t buf2[bufsize];

// Line buffer
enum { maxline = 511 }; 
char line[maxline+1];

void send(char *msg) {
    comSendBuf(COM2, (uint8_t*)msg, strlen(msg));
    bsp_DelayMS(100);
}

void recv() {
    uint8_t read;
    uint32_t pos = 0;
    bsp_DelayMS(100);
    while(comGetChar(COM2,&read)) {
        if (pos < bufsize - 1) {
            buf[pos++] = read;
        }
    }
    buf[pos] = 0;
}

void send_sms(char *da, char *msg) {
    pdu_create_message(da, msg, buf, bufsize);

    printf("Sending: \"%s\" to %s\r\n",msg, da);
    printf("(PDU: %s)\r\n", buf);

    // Don't send both lines in one go, otherwise you'll get ERROR...
    snprintf((char*)buf2, sizeof(buf2), "AT+CMGS=%d\r", strlen(buf)/2); 
    send(buf2);
    snprintf((char*)buf2, sizeof(buf2), "%s\x1a", buf); 
    send(buf2);
    recv();
}


void send_status(char *oa) {    
    char msg[100];
    snprintf(msg, sizeof(msg), "Das Ger{t ist %s", relay_state(RELAY) ? "eingeschaltet." : "ausgeschaltet."); // Too stupid to get the encoding right...
    send_sms(oa, msg);
}

void handle_message(char *oa, char *msg) {
    printf("Message from %s: %s\r\n", oa, msg);
    bsp_DelayMS(100);

    if (strcasecmp(msg, "ein") == 0) {
        relay_switch(RELAY, true);
        send_status(oa);
    } else if (strcasecmp(msg, "aus") == 0) {
        relay_switch(RELAY, false);
        send_status(oa);
    } else if (strcasecmp(msg, "status") == 0) {
        send_status(oa);
    } else {
        send_sms(oa, "Unbekannter Befehl. Befehle sind: ein, aus, status");
    }
}

void process_line() {
    printf("line received: %s\r\n", line);
    bsp_DelayMS(100);
    printf("---\r\n");
    bsp_DelayMS(100);

    switch(state) {
        case WAITING_FOR_EVENT:        
            if (strncmp(line, "+CMT:", 5) == 0) {                
                state = CMT_RECEIVED;
            }
            break;
        case CMT_RECEIVED: {
            char oa[20];
            printf("PDU received: %s\r\n", line);            
            pdu_receive_message(line, oa, sizeof(oa), buf, sizeof(buf));
            handle_message(oa, (char*)buf);
            state = WAITING_FOR_EVENT;
            break;    
        }
    }
}

int main(void)
{

    state = WAITING_FOR_EVENT;

    bsp_Init();		/* Hardware initialization */
    relay_init();

    bsp_DelayMS(100);

    printf("Welcome to 4g_relay %s, built on %s %s\r\n", VERSION, __DATE__, __TIME__);
    printf("Copyright (c) " YEAR " Andreas Signer <asigner@gmail.com>\r\n");

    send("AT+SLEDS=?\r");  // Echo mode off

    send("ATE0\r");  // Echo mode off
    send("AT*I\r");  // Dump info about the module
    send("AT+CMGF=0\r"); // Set PDU mode
    
    bsp_StartAutoTimer(0, 1000); /* Start a 1-second auto-reloading timer */

    /* Enter the main program loop */
    int pos = 0;
    while (1)
    {
        bsp_Idle();		/* This function is in bsp.c file. Users can modify this function to implement CPU sleep and watchdog feeding */

        /* Check if the timer timeout has occurred */
        if (bsp_CheckTimer(0)) {
            // Maybe at some point we will want to do something here?
        }        
       
        uint8_t buf;
        // if(	comGetChar(COM2,&buf)) {
        //     comSendChar(COM1, buf);            
        // }
        // continue;

        if(	comGetChar(COM2,&buf)) {
            switch(buf) {                
            case '\r':
                // ignore
                break;
            case '\n':
                // Line finished!
                line[pos] = 0;
                process_line();
                pos = 0;
                break;
            default:
                if (pos < maxline) {
                    line[pos++] = buf;
                }
            }
        }

    }
}

