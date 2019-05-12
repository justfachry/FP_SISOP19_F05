#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

static const char *dirpath = "/home/justfachry/Musik";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	(void) offset;
	(void) fi;

	DIR *dp;
	struct dirent *de;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
		if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

void listlagu(const char *path){
	DIR *dp;
	struct dirent *de;
	
	if (!(dp = opendir(path)))
        return;

	while ((de = readdir(dp)) != NULL) {
        char direct[1000];
        char dircpy[1000];
		if(de->d_type == DT_DIR){
			if(strchr(de->d_name,'.') != NULL || strcmp(de->d_name, "Musik") == 0) 
            continue;
			sprintf(direct,"%s/%s",path,de->d_name);
			listlagu(direct);
		}
		else {
			char *cek;
			sprintf(direct,"%s/%s",path,de->d_name);
			if((cek = strstr(de->d_name,".mp3")) != NULL){
					sprintf(dircpy,"cp '%s' '%s/%s'",direct,dirpath,de->d_name);
					system(dircpy);
			}
		}
	}

	closedir(dp);
}

int main(int argc, char *argv[])
{
	listlagu("/home/justfachry");
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}