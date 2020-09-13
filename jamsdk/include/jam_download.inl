
#include <stdlib.h>
#include <string.h>


typedef struct download_t		download_t;


typedef enum
{
	JAM_DOWNLOAD_HTTP,
	JAM_DOWNLOAD_FTP
}download_type_t;

typedef enum
{
	JAM_DOWNLOAD_UNKNOWN,
	JAM_DOWNLOAD_COMPLETE,
	JAM_DOWNLOAD_REDIRECT,
	JAM_DOWNLOAD_NOT_FOUND,
	JAM_DOWNLOAD_INVALID_UID,
	JAM_DOWNLOAD_INVALID_PWD,
	JAM_DOWNLOAD_LOST_CONN
}download_result_t;


typedef void (*on_download_end_t)(download_t* dl, download_result_t complete);


struct download_t
{
	download_type_t		type;
	socket_t			fd;
	socket_t			ftp_fd;
	FILE*				fp;
	size_t				total_size;
	size_t				current_size;
	datetime_t			begin_time;
	datetime_t			last_time;
	uint16_t			port;
	char				save_file[JAM_MAX_PATH];
	char				redirect[JAM_MAX_PATH];
	char				url[JAM_MAX_PATH];
	char				domain[128];
	char				path[128];
	char				ftp_uid[128];
	char				ftp_pwd[128];
	bool				ftp_broken;
	int32_t				ftp_step;
	on_download_end_t	on_end;
};


#include "jam_private_dl_url.inl"
#include "jam_private_http.inl"
#include "jam_private_ftp.inl"
#include "jam_private_dl_net.inl"



/**
	添加一个下载任务(支持 http, ftp 下载)
	url 格式:
				http://domain:port/path/file
				ftp://username:password@domain:port/path/file
  */
static download_t* jam_download_task(const char* url, const char* save_file, on_download_end_t on_end)
{
	if(jam_path_valid(save_file))
	{
		char cmd[JAM_MAX_PATH << 1] = {0};
		download_t* dl = (download_t*)jam_malloc(sizeof(download_t));

		memset(dl, 0, sizeof(download_t));

		if(!__jam_download_parse_url(dl, url))
		{
			jam_free(dl);
			return 0;
		}
		if(!(dl->fd = jam_client_tcp(jam_host_to_ip(dl->domain), dl->port, dl, __jam_on_download_lost, __jam_on_download_data)))
		{
			jam_free(dl);
			return 0;
		}

		strncpy(dl->url, url, JAM_MAX_PATH);
		strncpy(dl->save_file, save_file, JAM_MAX_PATH);
		dl->on_end = on_end;

		if(JAM_DOWNLOAD_HTTP == dl->type)
		{
			sprintf(cmd, "GET /%s HTTP/1.1\r\nHost: %s:%d\r\nReferer: %s\r\n\r\n", dl->path, dl->domain, dl->port, dl->url);
			jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
		}
		return dl;
	}
	return 0;
}


