
#include "serv_list.h"
#include "getini.h"


typedef struct
{
	arrayset_t			list;
}serv_list_t;


static serv_list_t		g_serv_list = {0};


bool serv_list_init()
{
	g_serv_list.list = jam_arrayset_alloc();
	return !!g_serv_list.list;
}


bool serv_list_cleanup()
{
	if(g_serv_list.list)
	{
		jam_arrayset_free(g_serv_list.list);
		g_serv_list.list = 0;
	}
	return true;
}


serv_t* serv_list_put(socket_t fd, uint32_t section_id)
{
	if(g_serv_list.list)
	{
		if(!serv_list_get(fd))
		{
			/* 创建服务器连接对象 */
			serv_t* s = (serv_t*)jam_malloc(sizeof(serv_t));
			s->fd = fd;						/* 套接字 */
			s->section_id = section_id;		/* 服务器大区编号 */
			s->sid = jam_socket_fd(fd);		/* 系统底层套接字作为连接编号 */
			jam_arrayset_put(g_serv_list.list, s->sid, s);
			return s;
		}
	}
	return 0;
}


void serv_list_erase(socket_t fd)
{
	if(g_serv_list.list)
	{
		int32_t sid = jam_socket_fd(fd);
		serv_t* s = jam_arrayset_get(g_serv_list.list, sid);
		if(s)
		{
			jam_arrayset_erase(g_serv_list.list, sid);
			jam_free(s);
		}
	}
}


serv_t* serv_list_get(socket_t fd)
{
	return serv_list_get_by_sid(jam_socket_fd(fd));
}


serv_t* serv_list_get_by_sid(int32_t sid)
{
	if(g_serv_list.list)
	{
		return jam_arrayset_get(g_serv_list.list, sid);
	}
	return 0;
}

serv_t* serv_list_get_by_sectionid(uint32_t section_id)
{
	uint32_t sid = 0;
	if(jam_arrayset_first(g_serv_list.list, &sid))
	{
		do
		{
			serv_t*  serv = jam_arrayset_get(g_serv_list.list, sid);
			if(serv->section_id == section_id) return serv;			
		}
		while(jam_arrayset_next(g_serv_list.list, &sid));		
	}
	return NULL;
}

void enum_section(eFunc pFunc)
{
	uint32_t sid = 0;
	if(jam_arrayset_first(g_serv_list.list, &sid))
	{
		do
		{
			serv_t*  serv = jam_arrayset_get(g_serv_list.list, sid);
			pFunc(serv);		
		}
		while(jam_arrayset_next(g_serv_list.list, &sid));		
	}
}

