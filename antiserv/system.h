
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "head.h"


bool on_server_init(server_content_t* srv);


void on_server_loop(server_content_t* srv);


void on_server_cleanup(server_content_t* srv);


#endif
