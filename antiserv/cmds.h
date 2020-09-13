#ifndef _CMDS_H_
#define _CMDS_H_

typedef char* (*CmdFunc)(char*);

void initCommands();

int runCommand(const char* pCmd);

void addCommand(const char* pCmd, CmdFunc func, int paramLen);
#endif