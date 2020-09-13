
#include <stdio.h>
#include <string.h>
#include "jam_private_server.inl"



typedef struct server_content_t				server_content_t;

typedef bool (*on_srv_init_t)(server_content_t* srv);
typedef void (*on_srv_loop_t)(server_content_t* srv);
typedef void (*on_srv_cleanup_t)(server_content_t* srv);


struct server_content_t
{
	bool volatile			exit;
	char					name[128];
	on_srv_init_t			on_init;
	on_srv_loop_t			on_loop;
	on_srv_cleanup_t		on_cleanup;
	void*					user_data;
};


/** 开始运行一个服务器程序 */
static int jam_srv_run(server_content_t* srv)
{
	bool ret = true;
	int32_t quit_seconds = JAM_SRV_QUIT_WAIT;
	char srv_name[JAM_MAX_PATH] = {0};

	if(!(*srv->name))
	{
		strncpy(srv->name, "JamServer", 128);
	}

	sprintf(srv_name, "%s    Powerby: %s", srv->name, JAM_VERSION);

	jam_console_no_close();
	jam_console_no_ctrl_c();
	jam_console_set_title(srv_name);

	jam_lg_init("log", srv->name, JAM_SRV_LOG_SIZE);
	jam_lg_enable_print(true, true);

	/* system init */
	jam_network_init();
	jam_svc_init();


	if(srv->on_init)
	{
		jam_lg(JAM_SRV_LG_SYS, "init server...");
		ret = srv->on_init(srv);
	}
	if(ret)
	{
		while(!srv->exit)
		{
			if(srv->on_loop)
			{
				srv->on_loop(srv);
			}
		}
		jam_lg(JAM_SRV_LG_SYS, "closing server.");
		if(srv->on_cleanup)
		{
			jam_lg(JAM_SRV_LG_SYS, "fini server...");
			srv->on_cleanup(srv);
		}
	}

	jam_lg(JAM_SRV_LG_SYS, "terminating process.");

	/* system cleanup */
	jam_svc_cleanup();
	jam_network_cleanup();
	jam_lg_cleanup();

	printf("\n");
	printf("server will exit after %i seconds.\n", JAM_SRV_QUIT_WAIT);
	while(quit_seconds--)
	{
		__jam_show_process(JAM_SRV_QUIT_WAIT - quit_seconds, JAM_SRV_QUIT_WAIT);
		jam_sleep(1000);
	}
	printf("\n");

	return 0;
}


