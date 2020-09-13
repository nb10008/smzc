
#ifndef JAM_SRV_LOG_SIZE
	#define JAM_SRV_LOG_SIZE				(40 * 1024 * 1024)
#endif

#ifndef JAM_SRV_QUIT_WAIT
	#define JAM_SRV_QUIT_WAIT				5
#endif

#define JAM_SRV_LG_SYS						"<SYS>"



static void __jam_show_process(int32_t cur, int32_t all)
{
	const int32_t show_len = 30;
	int32_t proc = 0;
	int32_t blank = 0;

	proc = (int32_t)jam_lerp(0, show_len, (float)cur / (float)all);
	blank = show_len - proc;

	printf("\r");
	printf("[");
	while(proc--)
	{
		printf("#");
	}
	while(blank--)
	{
		printf(" ");
	}
	printf("] %i second.", all - cur);
}


