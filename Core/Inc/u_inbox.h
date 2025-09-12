#ifndef __U_INBOX_H
#define __U_INBOX_H

#include "fdcan.h"

void inbox_can(can_msg_t *message); /* Function to process received CAN messages. */

#endif /* u_inbox.h */