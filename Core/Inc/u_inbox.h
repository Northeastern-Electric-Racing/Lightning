#ifndef __U_INBOX_H
#define __U_INBOX_H

#include "fdcan.h"

/**
 * @brief Processes the given CAN message
 */
void inbox_can(can_msg_t *message);

#endif /* u_inbox.h */