

static void __jam_on_download_lost(socket_t sock, void* param)
{
	download_t* dl = (download_t*)param;
	download_result_t dr = JAM_DOWNLOAD_UNKNOWN;

	if(dl->ftp_fd)
	{
		jam_socket_close(dl->ftp_fd);
		dl->ftp_fd = 0;
	}
	if(dl->fp)
	{
		fclose(dl->fp);
		dl->fp = 0;
	}
	if(!dl->total_size)
	{
		switch(dl->type)
		{
		case JAM_DOWNLOAD_HTTP:
			{
				if(strlen(dl->redirect))
				{
					dr = JAM_DOWNLOAD_REDIRECT;
				}
				else
				{
					dr = JAM_DOWNLOAD_NOT_FOUND;
				}
			}	break;
		case JAM_DOWNLOAD_FTP:
			{
				switch(dl->ftp_step)
				{
				case 1:
					{
						dr = JAM_DOWNLOAD_INVALID_UID;
					}	break;
				case 2:
					{
						dr = JAM_DOWNLOAD_INVALID_PWD;
					}	break;
				default:
					{
						dr = JAM_DOWNLOAD_NOT_FOUND;
					}	break;
				}
			}	break;
		}
	}
	else if(!(dl->total_size - dl->current_size))
	{
		dr = JAM_DOWNLOAD_COMPLETE;
	}
	else
	{
		dr = JAM_DOWNLOAD_LOST_CONN;
	}
	dl->on_end(dl, dr);

	jam_free(dl);
}


static void __jam_on_download_data(socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param)
{
	download_t* dl = (download_t*)param;

	switch(dl->type)
	{
	case JAM_DOWNLOAD_HTTP:
		{
			__jam_download_parse_http(dl, buffer, len);
		}	break;
	case JAM_DOWNLOAD_FTP:
		{
			__jam_download_parse_ftp(dl, buffer, len);
		}	break;
	}
}

