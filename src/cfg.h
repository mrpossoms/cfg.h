#ifndef _CFGFS_H
#define _CFGFS_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

const char* BASE_PATH;


void  cfg_base(const char* path)
{
	BASE_PATH = path;	
}

void _cfg_mkdir(const char *dir) {
	char tmp[256];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);

	if(tmp[len - 1] == '/') tmp[len - 1] = 0;

	for(p = tmp + 1; *p; p++)
	{
		if(*p == '/')
		{
			*p = 0;
			mkdir(tmp, S_IRWXU);
			*p = '/';
		}
	}

	mkdir(tmp, S_IRWXU);
}


int _cfg_open_or_create(const char* name, FILE** fd)
{
	if (!name || !BASE_PATH) { return -1; }

	int res = 0;
	size_t path_str_len = strlen(name) + strlen(BASE_PATH) + 2;
	struct stat s = {};
	char path[path_str_len];
	
	snprintf(path, sizeof(path), "%s/%s", BASE_PATH, name);
	res = stat(path, &s);
	
	if (res != 0)
	switch (errno)
	{
		case 0: // everything is ok
			break;
		case EACCES:
		case ENOENT:
		{
			_cfg_mkdir(BASE_PATH);
			res = chmod(BASE_PATH, 0755);
			*fd = fopen(path, "w+");
			res = chmod(path, 0666);
			if (*fd < 0) return -ENOENT;
			return 1; // file creation occurred
		}
		default: // another unhandled error occurred
			return -4;
	}

	*fd = fopen(path, "r");
	
	if (fd < 0) return -5;

	return 2; // file existed and opened		
}


float cfg_float(const char* name, float def_val)
{
	FILE* f = NULL;
	float value = def_val;

	switch (_cfg_open_or_create(name, &f))
	{
		case -EACCES:	 
		case -ENOENT:
		case -4:
		case -5:
			value = def_val;
			break;
		case 1: // file created, defaults must be written
			fprintf(f, "%f", value);
			break;
		case 2: // file exists, read
			fscanf(f, "%f", &value);
			break;
	}

	if (f) fclose(f);
	return value;
}


int cfg_int(const char* name, int def_val)
{
	FILE* f = NULL;
	int value = def_val;

	switch (_cfg_open_or_create(name, &f))
	{
		case -EACCES:	 
		case -ENOENT:
		case -4:
		case -5:
			value = def_val;
			break;
		case 1: // file created, defaults must be written
			fprintf(f, "%d", value);
			break;
		case 2: // file exists, read
			fscanf(f, "%d", &value);
			break;
	}

	if (f) fclose(f);
	return value;
}


char* cfg_str(const char* name, char* def_val)
{
	FILE* f = NULL;
	char* value = def_val;

	switch (_cfg_open_or_create(name, &f))
	{
		case -EACCES:	 
		case -ENOENT:
		case -4:
		case -5:
			value = def_val;
			break;
		case 1: // file created, defaults must be written
			fprintf(f, "%s", value);
			break;
		case 2: // file exists, read
		{
			static char buf[256];
			bzero(buf, sizeof(buf)); 
			fread(buf, 1, sizeof(buf), f);
			for (int i = strnlen(buf, 256); i--;)
			{
				if (isspace(buf[i])) { buf[i] = '\0'; }
				else { break; }
			}
			value = buf;
		}
		break;
	}

	if (f) fclose(f);
	return value;
}

#endif
