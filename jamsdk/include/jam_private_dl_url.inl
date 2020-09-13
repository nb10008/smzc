

static bool __jam_download_parse_url(download_t* dl, const char* url)
{
	static const char http_scheme[] = {"http://"};
	static const char ftp_scheme[] = {"ftp://"};

	char url_copy[JAM_MAX_PATH] = {0};

	char* ptr = url_copy;
	char* p = 0;
	char* tmp;

	strncpy(url_copy, url, JAM_MAX_PATH);

	if(!strncasecmp(ptr, http_scheme, sizeof(http_scheme) - 1))
	{
		ptr += sizeof(http_scheme) - 1;
		tmp = ptr;
		while(*tmp)
		{
			if(':' == (*tmp))
			{
				*tmp = 0;
				p = tmp + 1;
			}
			if('/' == (*tmp))
			{
				*tmp++ = 0;
				break;
			}
			++tmp;
		}
		dl->type = JAM_DOWNLOAD_HTTP;
		strncpy(dl->domain, ptr, 128);
		strncpy(dl->path, tmp, 128);
		if(p)
		{
			dl->port = (uint16_t)atoi(p);
		}
		if(!dl->port)
		{
			dl->port = 80;
		}
		return true;
	}
	if(!strncasecmp(ptr, ftp_scheme, sizeof(ftp_scheme) - 1))
	{
		char* uid = 0;
		char* pwd = 0;
		bool first = true;
		bool have_at = false;

		ptr += sizeof(ftp_scheme) - 1;
		tmp = ptr;
		while(*tmp)
		{
			if(':' == (*tmp))
			{
				if(first)
				{
					*tmp = 0;
					uid = ptr;
					pwd = tmp + 1;
					p = pwd;
					first = false;
				}
				else
				{
					*tmp = 0;
					p = tmp + 1;
				}
			}
			if('@' == (*tmp))
			{
				*tmp = 0;
				ptr = tmp + 1;
				have_at = true;
			}
			if('/' == (*tmp))
			{
				*tmp++ = 0;
				break;
			}
			++tmp;
		}
		if(!have_at)
		{
			uid = 0;
			pwd = 0;
		}
		dl->type = JAM_DOWNLOAD_FTP;
		strncpy(dl->domain, ptr, 128);
		strncpy(dl->path, tmp, 128);
		if(uid)
		{
			strncpy(dl->ftp_uid, uid, 128);
		}
		if(pwd)
		{
			strncpy(dl->ftp_pwd, pwd, 128);
		}
		if(p)
		{
			dl->port = (uint16_t)atoi(p);
		}
		if(!strlen(dl->ftp_uid))
		{
			strcpy(dl->ftp_uid, "anonymous");
		}
		if(!strlen(dl->ftp_pwd))
		{
			strcpy(dl->ftp_pwd, "anonymous@example.com");
		}
		if(!dl->port)
		{
			dl->port = 21;
		}
		return true;
	}
	return false;
}


static void __jam_download_local_path(const char* filename)
{
	char path[JAM_MAX_PATH] = {0};
	char* ptr = path;

	strncpy(path, filename, JAM_MAX_PATH);
	while(*ptr)
	{
		if('\\' == (*ptr))
		{
			*ptr = '/';
		}
		++ptr;
	}
	jam_trim(path, '/');
	ptr = strrchr(path, '/');
	if(ptr)
	{
		*ptr = 0;
		jam_path_make(path);
		*ptr = '/';
	}
}

