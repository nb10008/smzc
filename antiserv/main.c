
#include "system.h"
//#include "cmds.h"


int main(int argc, char* argv[])
{	
	server_content_t content = {
		false, 
		"AntiServer", 
		on_server_init, 
		on_server_loop, 
		on_server_cleanup, 
		0
	};
	//runcmd();
	return jam_srv_run(&content);
}

