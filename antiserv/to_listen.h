
#ifndef __TO_LISTEN_H__
#define __TO_LISTEN_H__

#include "head.h"


bool listen_init();


bool listen_cleanup();

bool close_connection(socket_t sock);

int32_t send_user_state(socket_t socket, char* name, uint32_t online_time);
#endif
