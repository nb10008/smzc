#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "getini.h"


#define MAX_BUFFER_SIZE 60000
#define MAX_VALUE_BUFFER_SIZE 128

static char s_szbuffer[MAX_BUFFER_SIZE];   /*存放文件内容*/
static int i_buffer_len=0;                 /*存放文件大小*/
static int i_buffer_changed=0;             /*存放改动标志*/
static FILE *fp=NULL;

/********************************************************************
  Function:       OpnIniFile       // 函数名称
  Description:    打开要操作的文件   // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:      p_File -- 文件名(相对,绝对路径)
  Output:                          // 对输出参数的说明。
  Return:                          // 函数返回值的说明
              0 -- 打开文件成功
             -1 -- 打开文件错误
             -2 -- 读取缓冲区错误
             -3 -- 文件太大
  Others:                          // 其它说明
**********************************************************************/
int open_ini_file(const char *pFile)
{
    /*
    stat():返回文件的状态(属性). 
    需要两个参数:文件路径名和一个结构指针,指向状态信息的存放的位置. 
    结构定义如下: 
    st_mode: 文件类型和存取许可方式 
    st_ino: I节点号 
    st_dev: 文件所在设备的ID 
    st_rdev: 特别文件的ID 
    st_nlink: 文件链接数 
    st_uid: 文件所有者的UID 
    st_gid: 文件小组的GID 
    st_size: 按字节计数的文件大小 
    st_atime: 最后存取时间(读) 
    st_mtime: 最后修改时间(写)和最后状态的改变 
    st_ctime: 最后的状态修改时间 
    返回值: 0:成功 1:失败 
    */

    struct stat statbuf; 

    stat(pFile,&statbuf);
    if ( statbuf.st_size > MAX_BUFFER_SIZE )
    {
       fprintf( stderr,"配置文件太大，请缩小配置文件。\n");
       fflush(stderr);
       return -3;
    }

    i_buffer_len = 0; 
    i_buffer_changed = 0; 
    if ( NULL == (fp=fopen(pFile, "rt")) ) 
    {
        printf("文件不存在或打开文件出错!\n");
        return -1; 
    }

    /*读到指定长度的位组或到文件尾 EOF*/
    /*int fread(void *ptr,int size,int nitems,FILE *stream)*/ 
    /*从流stream中读入nitems个长度为size的字符串存入ptr中*/
	//i_buffer_len = fread(s_szbuffer,statbuf.st_size,1,fp);
    if ( 0 != fread(s_szbuffer,statbuf.st_size,1,fp) )
    { 
        if (fp != NULL) 
        {
            fclose(fp); 
        }

        fp = NULL; 
        return -2; 
    } 

    i_buffer_len = statbuf.st_size; 
    return 0; 
}

/********************************************************************
  Function:       CloseIniFile     // 函数名称
  Description:    关闭文件         // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
  Output:                          // 对输出参数的说明
  Return:                          // 函数返回值的说明
              0 -- 关闭文件成功
             -1 -- 关闭已打开的文件错误
  Others:                          // 其它说明
**********************************************************************/
int close_ini_file(void) 
{ 
    if ( fp != NULL)
    {   /*如果缓冲区里的内容被改变,则写入文件再关闭*/
        if (i_buffer_changed) 
        { 
            /*将当前文件指针fp移到文件开头*/
            rewind(fp); 
            /*int fwrite(void *ptr,int size,int nitems,FILE *stream)*/
            /*向流stream中写入nitems个长度为size的字符串,字符串在ptr中*/
            fwrite(s_szbuffer, i_buffer_len, 1, fp); 
        } 

        if (!fclose(fp)) 
        {
            return 0; 
        }
        else 
        {
            return -1; 
        }

    } 
    else
    {
        return 0; 
    }
} 

/********************************************************************
  Function:       GetIniString     // 函数名称
  Description:    读取字符型数据   // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
    char *s_section : Section Name  // 段名
    char *s_key: Identity Name    // 关键字名
  Output:                          // 对输出参数的说明
    char *s_result: Returned string // 返回值
  Return:                          // 函数返回值的说明
              0 -- 读取相应值成功
             -1 -- 读取相应值失败
  Others:                          // 其它说明
**********************************************************************/
int get_ini_string(const char *s_section,const char *s_key, char *s_result)
{
    int i=0;                    /*用于控制读取字符*/
    int j=0;                    /*用于找到值后的指针赋值*/
    int min;                    /*用于存放读取值的位数*/
    int i_key_found=-1;         /*是否找到段名的标志*/
    
    /*如果段名或关键字或文件为空,返回-1*/
    if (!strlen(s_section) || !strlen(s_key) || (fp == NULL))
    {
        return -1; 
    }

    while ( i < i_buffer_len ) 
    { 
        /*读取开头为' '或'\t')有多少位,指针向后移动i位*/
        while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t')))
        {
            i++; 
        }

        /*如果i大于了文件总长度,则返回-1*/
        if (i>=i_buffer_len) 
        {
            return -1; 
        }

        if ( '#' == s_szbuffer[i]) 
        {    /*忽略以'#'开头的语句,直到\n*/
            while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')) 
            {
                i++; 
            }

            if (i>=i_buffer_len) 
            {
                return -1; 
            }

            /*如果是\n,则移向下一行*/
            i++; 
        } 
        else 
        { 
            if ('[' == s_szbuffer[i]) 
            { 
                i++; 
                /*如果[]中前面包括''或\t则往后移*/
                while ((i<i_buffer_len) &&((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                {
                    i++; 
                }

                if (i>=i_buffer_len) 
                {
                    return -1; 
                }

                if (0 == strncmp(s_szbuffer+i, s_section, strlen(s_section))) 
                { 
                    /*从文件内容的i位开始和输入的段名比较,如果相同*/
                    i += strlen(s_section); 
                    /*如果[]中后面包括''或\t则往后移*/
                    while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len)
                    {
                        return -1; 
                    }

                    /*如果读取到了']',则改变i_key_found=0*/
                    if (']' == s_szbuffer[i]) 
                    {    
                        i_key_found=0; 
                    }

                    i++;
                    
                    /*移动指针直到读取到'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n'))
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return -1; 
                    }
                    
                    /*到下一行*/
                    i++; 
                } 
                else 
                {   /*如果没有找到和段名匹配,则一直移动指针到'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return -1; 
                    }

                    /*到下一行*/
                    i++; 
                } 
            } 
            else 
            { 
                if (0 != i_key_found) 
                {   
                    /*如果i_key_found为0则说明没有找到匹配的段名,则往后移到'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {    
                        return -1; 
                    }

                    /*到下一行*/
                    i++; 
                } 
                else 
                {   /*如果找了段名匹配,则比较关键字*/
                    if (0 == strncmp(s_szbuffer+i, s_key, strlen(s_key)))
                    { 
                        i += strlen(s_key); 
                        if (i>=i_buffer_len)
                        {
                            return -1; 
                        }
                        while ((i<i_buffer_len) && ((s_szbuffer[i]=='\t') || (s_szbuffer[i]==' '))) 
                        {
                            i++; 
                        }

                        if ( '=' == s_szbuffer[i]) 
                        {   /*读取=则说明找到了关键字了*/
                            i++; 

                            /*把要获取的值前面的''和'\t'都过滤掉*/
                            while ((i<i_buffer_len) && 
                                   ((s_szbuffer[i]=='\t') || 
                                   (s_szbuffer[i]==' '))) 
                            {
                                i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                                return -1; 
                            }

                            j=i; 
                            /*下一指针不为'\n'并且不为'#',则往下读取*/
                            while ((j<i_buffer_len) && 
                                   (s_szbuffer[j]!='\n') && 
                                   (s_szbuffer[j]!='#'))
                            {
                                j++; 
                            }

                            j--; 
                            while ((s_szbuffer[j]==' ') || 
                                   (s_szbuffer[j]=='\t')) 
                            {
                                 j--; 
                            }
                            /*计算获取到的值得总长度*/
                            min = j-i+1; 
                            /*将结果copy到s_result中,加上末尾的'\0',返回0,成功*/
                            strncpy(s_result, s_szbuffer+i, min); 
                            *(s_result+min) = '\0'; 
                            return 0; 
                        } 
                        else 
                        {   /*如果没有找到=则往下移指针*/
                            while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')) 
                            {
                               i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                               return -1; 
                            }

                            /*到下一行*/
                            i++; 
                        }    /*没有找到'='号*/ 
                    }
                    else 
                    {    /*没有找到关键字,往下移指针*/
                        while ((i<i_buffer_len) && (s_szbuffer[i]!='\n'))
                        {
                            i++; 
                        }
                        
                        if (i>=i_buffer_len) 
                        {
                            return -1; 
                        }

                        /*移到下一行*/
                        i++; 
                    } 
                } 
            } 
        } 
    } /* while (i<i_buffer_len)  */
    /*如果没有return0则说明找到,返回-1*/
    return -1; 
} 

/********************************************************************
  Function:       CFG_get_key      // 函数名称
  Description:    读取字符型数据   // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
    char *pFile: File Name         // 文件路径名
    char *section : Section Name   // 段名
    char *key: Identity Name       // 关键字名
  Output:                          // 对输出参数的说明
    char *val: Returned string     // 返回值
  Return:                          // 函数返回值的说明
              0 -- 读取相应值成功
             -1 -- 读取相应值失败
  Others:                          // 其它说明
**********************************************************************/
int cfg_get_key(const char *pFile, const char *s_section, 
                const char *s_key, char *s_val)
{
    int ret_code;
    ret_code = open_ini_file(pFile);

    if ( 0 == ret_code)
    {
        ret_code = get_ini_string(s_section,s_key,s_val);
        if (0 == ret_code)
        {
            /*找到值了*/
            close_ini_file();
            return 0;
        }
        else
        {
            close_ini_file();
            printf("在%s中的[%s]段没有找到%s的值.\n",pFile,s_section,s_key);
            return -1;
        }
    }
    close_ini_file();
    return -1;
}

/********************************************************************
  Function:       Get_get_ini      // 函数名称
  Description:    读取整型数据     // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
    char *pFile: File Name         // 文件路径名
    char *section : Section Name   // 段名
    char *key: Identity Name       // 关键字名
    int iDefVal                    // 默认返回整型值
  Output:                          // 对输出参数的说明
  Return:                          // 函数返回值的说明
    s_temp                         // 返回获取值
    iDefVal                        // 返回默认值
  Others:                          // 其它说明
**********************************************************************/
int cfg_get_int(const char *pFile, const char *s_section, const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 ==  cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        /*如果获取的是'0x'或'0X'则为16进制数,转换成十进制数*/
        if (strlen(s_temp)>2)
        {
            if ((s_temp[0]=='0') && ((s_temp[1]=='x') || (s_temp[1]=='X')))
            {
                /*将字符型通过16进制转换成整型*/
                return (int)(strtol(s_temp,(char **)NULL,16));
            }
        }
        return atoi(s_temp);
    }
    return 0;
}

/********************************************************************
  Function:       Get_get_long     // 函数名称
  Description:    读取长整型数据   // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
    pFile     -- 文件路径名
    section   -- 段名
    key       -- 关键字名
  Output:                          // 对输出参数的说明
  Return:                          // 函数返回值的说明
                                // 返回获取值
  Others:                          // 其它说明
**********************************************************************/
long cfg_get_long(const char *pFile, const char *s_section, const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 == cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        /*如果获取的是'0x'或'0X'则为16进制数,转换成十进制数*/
        if (strlen(s_temp)>2)
        {
            if ((s_temp[0]=='0') && ((s_temp[1]=='x') || (s_temp[1]=='X')))
            {
                /*将字符型通过16进制转换成长整型*/
                return (long)(strtol(s_temp,(char **)NULL,16));
            }
        }
        return atol(s_temp);
    }
    return 0;
}

/********************************************************************
  Function:       Get_get_double   // 函数名称
  Description:    读取实型数据     // 函数功能、性能等的描述
  Calls:                           // 被本函数调用的函数清单
  Input:                           // 对输入参数的说明
    char *pFile: File Name         // 文件路径名
    char *section : Section Name   // 段名
    char *key: Identity Name       // 关键字名
  Output:                          // 对输出参数的说明
  Return:                          // 函数返回值的说明
    s_temp                         // 返回获取值
  Others:                          // 其它说明
**********************************************************************/
double cfg_get_double(const char *pFile,const char *s_section,const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 ==  cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        return atof(s_temp);   /*将获取的字符型用atof装换成double型*/
    }

    return 0.0;
}
