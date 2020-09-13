

static char* __jam_download_next_line(const char* buffer)
{
	char* p = (char*)strstr(buffer, "\r\n");
	if(p)
	{
		p += strlen("\r\n");
	}
	return p;
}


static char* __jam_download_ftp_cmd(download_t* dl, int32_t* cmd, const char* buffer)
{
	char* p = (char*)buffer;

	if('-' == p[3])
	{
		char match[5] = {0};
		strncpy(match, p, 3);
		match[3] = ' ';
		do
		{
			if(!(p = __jam_download_next_line(p)))
			{
				size_t len = strlen(buffer);
				if('\n' != buffer[len - 1])
				{
					dl->ftp_broken = true;
				}
				break;
			}
		}while(strncmp(match, p, 4));
	}
	if(p)
	{
		*cmd = atoi(p);
	}
	return p;
}


static void __jam_on_download_ftp_data(socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param)
{
	download_t* dl = (download_t*)param;

	if(dl->fp)
	{
		fwrite(buffer, len, 1, dl->fp);
		dl->current_size += len;
		dl->last_time = jam_time_now();

		if(dl->current_size == dl->total_size)
		{
			jam_socket_close(dl->ftp_fd);
			jam_socket_close(dl->fd);
			dl->ftp_fd = 0;
		}
	}
}


static void __jam_download_parse_ftp(download_t* dl, const void* buffer, size_t len)
{
	int32_t code;
	char cmd[128] = {0};
	const char* p = (char*)buffer;
	if(dl->ftp_broken)
	{
		p = __jam_download_next_line(p);
		dl->ftp_broken = false;
	}
	if(p && (p = __jam_download_ftp_cmd(dl, &code, p)))
	{
		bool have_error = false;

		switch(dl->ftp_step)
		{
		case 0:
			{
				if((code >= 200) && (code < 300))
				{
					sprintf(cmd, "user %s\r\n", dl->ftp_uid);
					jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 1:
			{
				if((code >= 300) && (code < 400))
				{
					sprintf(cmd, "pass %s\r\n", dl->ftp_pwd);
					jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 2:
			{
				if((code >= 200) && (code < 300))
				{
					strcpy(cmd, "type i\r\n");
					jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 3:
			{
				if((code >= 200) && (code < 300))
				{
					sprintf(cmd, "pasv\r\n");
					jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 4:
			{
				if((code >= 200) && (code < 300))
				{
					uint16_t port = 0;
					char serv[64] = {0};
					char* pp = serv;
					char* p1;
					char* p2;
					const char* sb = strchr(p, '(');
					const char* se = strchr(p, ')');
					strncpy(serv, sb + 1, se - sb - 1);
					p1 = (char*)strrchr(serv, ',');
					*p1++ = 0;
					p2 = (char*)strrchr(serv, ',');
					*p2++ = 0;
					while(*pp)
					{
						if(',' == (*pp))
						{
							*pp = '.';
						}
						++pp;
					}
					port = atoi(p2) * 256 + atoi(p1);

					if(dl->ftp_fd = jam_client_tcp(jam_host_to_ip(serv), port, dl, 0, __jam_on_download_ftp_data))
					{
						__jam_download_local_path(dl->save_file);
						dl->fp = fopen(dl->save_file, "w+b");
						dl->begin_time = jam_time_now();
						dl->last_time = dl->begin_time;

						sprintf(cmd, "retr /%s\r\n", dl->path);
						jam_socket_send(dl->fd, cmd, strlen(cmd), 0);
					}
					else
					{
						have_error = true;
					}
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 5:
			{
				if((code >= 100) && (code < 200))
				{
					const char* l = strchr(p, '(');
					dl->total_size = atol(l + 1);
				}
				else
				{
					have_error = true;
				}
				++dl->ftp_step;
			}	break;
		case 6:
			{
				if((code < 200) || (code >= 300))
				{
					have_error = true;
				}
			}	break;
		}
		if(have_error)
		{
			if(dl->ftp_fd)
			{
				jam_socket_close(dl->ftp_fd);
				dl->ftp_fd = 0;
			}
			jam_socket_close(dl->fd);
		}
	}
}

