
#ifndef __SERV_LIST_H__
#define __SERV_LIST_H__

#include "head.h"


/* 服务器队列初始化 */
bool serv_list_init();


/* 服务器队列销毁 */
bool serv_list_cleanup();


/* 添加一个服务器连接 */
serv_t* serv_list_put(socket_t fd, uint32_t section_id);


/* 删除一个服务器连接 */
void serv_list_erase(socket_t fd);


/* 根据套接字查询一个服务器连接 */
serv_t* serv_list_get(socket_t fd);


/* 根据编号查询一个服务器 */
serv_t* serv_list_get_by_sid(int32_t sid);

/*根据大区查询一个服务器连接*/
serv_t* serv_list_get_by_sectionid(uint32_t section_id);

/*枚举服务器*/
typedef void (*eFunc)(serv_t*);
void enum_section(eFunc);
#endif
