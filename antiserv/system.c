
#include "system.h"
#include "event.h"
#include "serv_list.h"
#include "user_list.h"
#include "to_listen.h"
#include "getini.h"
#include "cmds.h"


uint32_t OFFLINE_TIMEOUT;				//(120 * 1000)				/* 下线超时检测时间 */
uint32_t OFFLINE_CLEAR;				//(5 * 60 * 60 * 1000)		/* 恢复健康游戏所需的时间 */
uint32_t GREEN_TIME;					//(3 * 60 * 60 * 1000)		/* 健康游戏的时间 */
uint32_t RED_TIME;					//(5 * 60 * 60 * 1000)		/* 非健康游戏的时间 */
uint32_t GREEN_NOTIFY_PERIOD;
uint32_t ORANGE_NOTIFY_PERIOD;
uint32_t REDNOTIFY_PERIOD;
#if (VERSION_DEFINE==1)
uint32_t CHECK_POINT_TIME;			/* 取值范围0-23，重置账号在线时间的时间点 */
#endif
static server_content_t* s_srv = NULL;

void cmd_exit(const char* p)
{
	if(s_srv != NULL) s_srv->exit = true;
}

bool on_server_init(server_content_t* srv)
{
	OFFLINE_TIMEOUT = 1000*(uint32_t)cfg_get_long(CONFIG_FILE, "Config","OFFLINE_TIMEOUT");
	OFFLINE_CLEAR = 1000*(uint32_t)cfg_get_long(CONFIG_FILE, "Config","OFFLINE_CLEAR");
	GREEN_TIME = 1000*(uint32_t)cfg_get_long(CONFIG_FILE, "Config","GREEN_TIME");
	RED_TIME = 1000*(uint32_t)cfg_get_long(CONFIG_FILE, "Config","RED_TIME");
	GREEN_NOTIFY_PERIOD = (uint32_t)cfg_get_long(CONFIG_FILE, "Config","GREEN_NOTIFY_PERIOD");
	ORANGE_NOTIFY_PERIOD = (uint32_t)cfg_get_long(CONFIG_FILE, "Config","ORANGE_NOTIFY_PERIOD");
	REDNOTIFY_PERIOD = (uint32_t)cfg_get_long(CONFIG_FILE, "Config","REDNOTIFY_PERIOD");
#if (VERSION_DEFINE==1)
	CHECK_POINT_TIME = (uint32_t)cfg_get_long(CONFIG_FILE, "Config","CHECK_POINT_TIME");
#endif
	event_init();

	if(!serv_list_init())
	{
		return false;
	}
	if(!user_list_init())
	{
		return false;
	}
	if(!listen_init())
	{
		return false;
	}

	initCommands();
	addCommand("EXIT", cmd_exit, 1);
	s_srv = srv;
	return true;
}


void on_server_loop(server_content_t* srv)
{
	char cmdline[64] = {0};
	size_t len = jam_console_line(cmdline, 64, true);

	if(len)
	{
		if(!runCommand(cmdline))
		{
			if(!runCommand("HELP"))
				printf(">>> unknown command(%u).\n", len);
		}	
	}
}

void on_server_cleanup(server_content_t* srv)
{
	listen_cleanup();
	user_list_cleanup();
	serv_list_cleanup();
	event_cleanup();
}

