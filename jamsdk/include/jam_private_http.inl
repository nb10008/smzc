

static void __jam_download_parse_http(download_t* dl, const void* buffer, size_t len)
{
	if(!dl->total_size)
	{
		char* http_head = (char*)buffer;
		char* http_end = strstr(http_head, "\r\n\r\n");
		if(http_end)
		{
			char* p;
			int32_t code;

			http_end += strlen("\r\n\r\n") - 1;
			*http_end = 0;

			p = strchr(http_head, ' ');
			code = atoi(p);

			if((code >= 200) && (code < 300))
			{
				/* HTTP OK */
				char* length = strstr(http_head, "Content-Length:");
				length += strlen("Content-Length:");
				dl->total_size = atol(length);

				__jam_download_local_path(dl->save_file);

				dl->fp = fopen(dl->save_file, "w+b");
				dl->begin_time = jam_time_now();
				dl->last_time = dl->begin_time;
				dl->current_size = len - (http_end - http_head + 1);
				fwrite(http_end + 1, dl->current_size, 1, dl->fp);
			}
			else if((code >= 300) && (code < 400))
			{
				/* HTTP Redirect */
				char* location = strstr(http_head, "Location:");
				p = strstr(location, "\r\n");
				location += strlen("Location:");

				strncpy(dl->redirect, location, p - location);
				jam_trim(dl->redirect, ' ');

				jam_socket_close(dl->fd);
			}
			else
			{
				/* HTTP Error */
				jam_socket_close(dl->fd);
			}
		}
		else
		{
			jam_socket_close(dl->fd);
		}
	}
	else
	{
		if(dl->fp)
		{
			fwrite(buffer, len, 1, dl->fp);
			dl->current_size += len;
			dl->last_time = jam_time_now();

			if(dl->current_size == dl->total_size)
			{
				jam_socket_close(dl->fd);
			}
		}
	}
}

