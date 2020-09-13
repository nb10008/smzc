
#include "cmds.h"
#include "head.h"
#include "to_listen.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_LEN 20
#define MAX_COMMANDS 20
#define MAX_MSG 1000

void trimLeft(char* input);
void trimRight(char* input);
int countParam(const char* input);
char* getParam(const char* const input, int index);
char* getNBreak(const char* pInput);

char* Help(char*);
char* Disconnect(char*);
char* PrintInfo(char*);
char* PrintUserInfo(char* pParam);
char* SendAntiState(char* pParam);
char* SetUserState(char* pParam);
char* CountUser(char* pParam);
char* DisconnectAll(char* pParam);
#if (VERSION_DEFINE==1)
char* SetCheckPointTime(char* pParam);
#endif
typedef struct {
	char pCmdText[MAX_COMMAND_LEN];
	CmdFunc func;
	int nParamCount;
} Command, *PCommand;

Command _commands[MAX_COMMANDS];
static int cmds = 0;

void trimRight(char* input)
{
	char* p = input + strlen(input) -1;
	while((*p==' ' || *p=='\t') && p>=input)
	{
		*p=0;
		p--;
	}
}

void trimLeft(char* input)
{
	int nCount=0;
	char* p = input;
	while(*p!=0 && (*p==' ' || *p=='\t'))
	{
		nCount++;
		p++;
	}
	if(nCount>0)
	{
		int i=0;
		int nLen = strlen(input)-nCount;
		for(i=0;i<=nLen; i++)
		{
			input[i] = input[i+nCount];
		}
	}
}

int countParam(const char* input)
{
	int nCount=1;
	char *p = input;
	int i=0;
	while(*p != 0)
	{
		p++;
		if(*p==' ' || *p=='\t')
			nCount++;
		while((*p==' ' || *p=='\t') && *p!=0)
		{
			p++;
		}
	}
	return nCount;
}

char* getNBreak(const char* pInput)
{
	char* p = pInput;
	while(*p!=0 && (*p==' ' || *p=='\t'))
		p++;
	while(*p!=0 && *p!=' '&& *p!='\t')
		p++;
	while(*p!=0 && (*p==' '|| *p=='\t'))
		p++;
	if(*p == 0) return NULL;
	return p;
}

char* getParam(const char* const input, int index)
{
	int totalParam = countParam(input);
	char *pStart = input,*pEnd = getNBreak(input);
	int nCount=0;

	if(index<0)
	{
		index = totalParam + index;
	}
	if(index <0 || index> totalParam) return NULL;
	while(pStart!=NULL && *pStart != 0)
	{		
		if(pEnd == NULL) pEnd = pStart + strlen(pStart);
		if(nCount == index)
		{			
			char* res = (char*)malloc(pEnd - pStart + 1);
			strncpy(res, pStart, pEnd-pStart);
			res[pEnd - pStart] = 0;
			trimLeft(res);
			trimRight(res);
			return res;
		} 
		pStart = getNBreak(pStart);
		pEnd = getNBreak(pEnd);
		nCount++;		
	}
	return NULL;
}


void initCommands()
{
	int i;
	for(i=0; i< MAX_COMMANDS; i++)
	{
		_commands[i].pCmdText[0]=0;
	}
	
	addCommand("HELP", Help,1); //HELP
	addCommand("DISCONNECT", Disconnect,2); //DISCONNECT
	addCommand("PRINTINFO", PrintInfo, 1); //PRINTINFO	
	addCommand("USER", PrintUserInfo, 2); //USER
	addCommand("ANTI", SendAntiState, 2);//ANTI
	addCommand("SET", SetUserState, 3);//SET
	addCommand("COUNT", CountUser, 1);//COUNT
	addCommand("DISALL", DisconnectAll, 1);//DisconnectAll
#if (VERSION_DEFINE==1)
	addCommand("CHECKPOINTTIME", SetCheckPointTime, 2);
#endif
}

void addCommand(const char* pCmd, CmdFunc func, int paramLen)
{
	strcpy(_commands[cmds].pCmdText, pCmd);
	_commands[cmds].func = func;
	_commands[cmds].nParamCount = paramLen;
	cmds++;
}

int runCommand(const char* pCmd)
{
	char* cmd;
	strupr(pCmd);
	trimLeft(pCmd);
	trimRight(pCmd);

	cmd = getParam(pCmd, 0);
	trimLeft(cmd);
	trimRight(cmd);
	if(strlen(cmd) >0 ) 
	{
		int index;
		for(index=0; index< MAX_COMMANDS && index<cmds; index++)
		{
			if(strcmp(cmd, _commands[index].pCmdText) == 0)
			{
				char* pRes = NULL;
				if(countParam(pCmd) == _commands[index].nParamCount)
				{
					pRes = _commands[index].func(pCmd);
				}
				else
				{
					pRes = "Wrong paramaters";
				}
				printf("%s\n", pRes);
				free(cmd);
				return 1;
			}
		}
	}	
	free(cmd);
	return 0;
}

char* Help(char* pParam)
{
	static char prompt[MAX_MSG];
	static int bInited = 0;
	if(!bInited)
	{
		int index = 0;
		int nCount = 1;
		char* p = prompt;
		strcpy(p, "Usage:");
		p+=strlen(p);
		for(index=0;index < MAX_COMMANDS; index++)
		{
			if(_commands[index].pCmdText[0] != 0)
			{
				sprintf(p, "\n%d. %s", nCount++, _commands[index].pCmdText);
				p += strlen(p);
			}
		}
		bInited = 1;
	}
	return prompt;
}

char* Disconnect(char* pParam)
{
	static char prompt[MAX_MSG];	
	char* param = getParam(pParam, 1);
	int section_id = atoi(param);
	prompt[0] = 0;
	if(section_id>0)
	{
		serv_t* serv = serv_list_get_by_sectionid(section_id);
		if(serv != NULL) 
		{
			close_connection(serv->fd);
			sprintf(prompt, "server %d disconnected.", section_id);
		}
		else
		{
			sprintf(prompt, "cannot find server %d", section_id);
		}
	}
	free(param);
	return prompt;
}

static char serverInfo[MAX_MSG];
void getServerList(serv_t* serv)
{
	static char ip[288];
	char* p=serverInfo;
	jam_ip_to_host(ip, jam_socket_addr(serv->fd));
	p+=strlen(serverInfo);
	sprintf(p, "%d\t%s\n", serv->section_id, ip);
}

char* PrintInfo(char* pParam)
{
	memset(serverInfo, 0, MAX_MSG);
	enum_section(getServerList);
	return serverInfo;
}

void disconectAll(serv_t* serv)
{
	close_connection(serv->fd);
}

char* DisconnectAll(char* pParam)
{
	enum_section(disconectAll);
	strcpy(serverInfo, "All client disconnected.");
	return serverInfo;
}

static char userInfo[MAX_MSG];

void getUserInfo(online_info_t* oi, void* param)
{
	char* p = (char*) param;
	char* pMsg = userInfo + strlen(userInfo);
	if(stricmp(oi->name, p) == 0)
	{
		sprintf(pMsg, "name(%s) state(%u) onlinetime(%d) offlinetime(%d)\n", oi->name, oi->notify_color, oi->online_period/1000, oi->offline_period/1000);
	}
}

char* PrintUserInfo(char* pParam)
{
	char*p = getParam(pParam, 1);

	trimLeft(p);
	trimRight(p);
	memset(userInfo, 0, MAX_MSG);
	enum_user(getUserInfo,p);
	if(strlen(userInfo)==0) strcpy(userInfo, "user not found");
	free(p);
	return userInfo;
}

void sendAnti(online_info_t* oi, void* param)
{
	char* p = (char*) param;	
	if(stricmp(oi->name, p) == 0)
	{
		uint32_t plen = 29, mlen = 21;
		uint32_t cmd = CMD_NS_ANTIENTHRALL;
		byte_t buf[33] = {0};
		serv_t* s;
		uint32_t online_time = 3600*3;
		byte_t color = COLOR_ORANGE;
		int i=0;
		char* pMsg = userInfo + strlen(userInfo);

		memcpy(buf, &plen, sizeof(uint32_t));
		memcpy(buf + 4, &cmd, sizeof(uint32_t));
		memcpy(buf + 8, &mlen, sizeof(uint32_t));
		strncpy(buf + 12, oi->name, 16);
		memcpy(buf + 28, &color, sizeof(int8_t));
		memcpy(buf + 29, &online_time, sizeof(uint32_t));

		/* 通知所有登录服务器，玩家在线情况 */
		for(i = 0; i < MAX_SID; ++i)
		{
			if(oi->serv[i].sid && (s = serv_list_get_by_sid(oi->serv[i].sid)))
			{							
				jam_socket_send(s->fd, buf, plen+4, 0);
				sprintf(pMsg, "send user(%s) color(%u) onlinetime(%d) to server(%d)\n",oi->name,color, online_time, s->section_id);
				pMsg += strlen(pMsg);
			}
		}
	}
}

char* SendAntiState(char* pParam)
{
	char*p = getParam(pParam, 1);

	trimLeft(p);
	trimRight(p);
	memset(userInfo, 0, MAX_MSG);
	enum_user(sendAnti,p);
	if(strlen(userInfo)==0) strcpy(userInfo, "user not found");
	free(p);
	return userInfo;
}

void setUserState(online_info_t* oi, void* param)
{
	char* p = (char*) param;
	char*pName = getParam(p, 1);
	
	if(stricmp(oi->name, pName) == 0)
	{
		char*pTime = getParam(p, 2);
		char* pMsg = userInfo + strlen(userInfo);

		oi->online_period = atoi(pTime) * 1000;

		if(oi->online_period < GREEN_TIME)
		{
			/* 在线时间小于 3 小时, 1 小时通知一次"绿色"时间 */				
			oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(GREEN_NOTIFY_PERIOD))) * GREEN_NOTIFY_PERIOD*1000 ;
			oi->notify_color = COLOR_GREEN;
		}
		else if(oi->online_period < RED_TIME)
		{
			/* 在线时间在 3 - 5 小时之间, 30 分钟通知一次"橙色"时间 */
			oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(ORANGE_NOTIFY_PERIOD))) * ORANGE_NOTIFY_PERIOD*1000 ;
			oi->notify_color = COLOR_ORANGE;
		}
		else
		{
			/* 在线时间超过 5 小时, 15 分钟通知一次"红色"时间 */
			oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(REDNOTIFY_PERIOD))) * REDNOTIFY_PERIOD*1000 ;
			oi->notify_color = COLOR_RED;
		}					

		free(pTime);
		sprintf(pMsg, "set user(%s) state(%u) onlinetime(%d) nextnotify(%d)", oi->name, oi->notify_color, oi->online_period, oi->next_notify);
	}
	free(pName);	
}

char* SetUserState(char* pParam)
{
	//char*p = getParam(pParam, 1);

	//trimLeft(p);
	//trimRight(p);
	memset(userInfo, 0, MAX_MSG);
	enum_user(setUserState,pParam);
	if(strlen(userInfo)==0) strcpy(userInfo, "user not found");
	//free(p);
	return userInfo;
}

int total, online;

void countUser(online_info_t* oi, void* param)
{	
	int32_t i;
	int32_t c = 0;
	total++;
	for(i = 0; i < MAX_SID; ++i)
	{
		if(oi->serv[i].sid)
		{
			online++;
			break;
		}
	}
}

char* CountUser(char* pParam)
{
	total = 0;
	online = 0;
	enum_user(countUser,NULL);
	sprintf(userInfo, "Total %d, Online %d",total, online);
	return userInfo;
}

#if (VERSION_DEFINE==1)
char* SetCheckPointTime(char* pParam)
{
	char* param = getParam(pParam, 1);
	int check_point_time = atoi(param);
	static char prompt[MAX_MSG];	

	if (check_point_time < 0 || check_point_time > 23)
	{
		sprintf(prompt, "Check point time invalid!!");
	}
	else
	{
		CHECK_POINT_TIME = check_point_time;
		sprintf(prompt, "<check point time> Set Success!!\n<check point time> Current time:%d", CHECK_POINT_TIME);
	}
	return prompt;
}
#endif