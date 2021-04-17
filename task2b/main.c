#include "util.h"
#include "/usr/include/dirent.h"



enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };

#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define STDERR 2
#define STDOUT 1
#define STDIN 0
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64


extern int system_call();

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char d_name[];  /* Filename (null-terminated) */
};

char* getType(char d_type)
{
  char* type = (d_type == DT_REG) ? "regular" :
  (d_type == DT_DIR) ?   "directory" :
  (d_type == DT_FIFO) ?  "FIFO" :
  (d_type == DT_SOCK) ?  "socket" :
  (d_type == DT_LNK) ?   "symlink" :
  (d_type == DT_BLK) ?   "block dev" :
  (d_type == DT_CHR) ?   "char dev" :
   "???";
  return type;
}

void printWithPrefix(char prefix)
{
  const int BUF_SIZE = 8192;
  long bpos;
  int folderDescriptor,writeSucces;
  long nread;
  char buf[BUF_SIZE];
  struct linux_dirent *d;
  char d_type;


 folderDescriptor = system_call(SYS_OPEN,".", O_RDONLY,0777);
 if (folderDescriptor < 0)
 {
   system_call(1,0x55,"","");
 }
 nread = system_call(SYS_GETDENTS, folderDescriptor, buf, BUF_SIZE);
 if(nread < 0)
 {
   system_call(1,0x55,"","");
 }
 for (bpos = 0; bpos < nread;) {
     d = (struct linux_dirent *) (buf + bpos);
     if(d->d_name[0] == prefix)
     {
       writeSucces = system_call(SYS_WRITE,STDOUT,d->d_name,strlen(d->d_name));
       if(writeSucces != strlen(d->d_name)){
         system_call(SYS_CLOSE,folderDescriptor,"","");
         system_call(1,0x55,"","");
       }
       system_call(SYS_WRITE,STDOUT,"  ",2);
       d_type = *(buf + bpos + d->d_reclen - 1);
       char* type = getType(d_type);
       writeSucces = system_call(SYS_WRITE,STDOUT,type,strlen(type));
       if(writeSucces < 0)
       {
         system_call(SYS_CLOSE,folderDescriptor,"","");
         system_call(1,0x55,"","");
       }
       system_call(SYS_WRITE,STDOUT,"\n",1);
     }
     bpos += d->d_reclen;
  }
 system_call(SYS_CLOSE,folderDescriptor,"","");
}

void printWithoutPrefix()
{
  const int BUF_SIZE = 8192;
  long bpos;
  int folderDescriptor,writeSucces;
  long nread;
  char buf[BUF_SIZE];
  struct linux_dirent *d;
  char d_type;

 folderDescriptor = system_call(SYS_OPEN,".", O_RDONLY,0777);
 if (folderDescriptor < 0)
 {
   system_call(1,0x55,"","");
 }
 nread = system_call(SYS_GETDENTS, folderDescriptor, buf, BUF_SIZE);
 if(nread < 0)
 {
   system_call(1,0x55,"","");

 }
 for (bpos = 0; bpos < nread;) {
     d = (struct linux_dirent *) (buf + bpos);
     writeSucces = system_call(SYS_WRITE,STDOUT,d->d_name,strlen(d->d_name));
     if(writeSucces != strlen(d->d_name)){
       system_call(SYS_CLOSE,folderDescriptor,"","");
       system_call(1,0x55,"","");
     }
     system_call(SYS_WRITE,STDOUT,"  ",2);
     d_type = *(buf + bpos + d->d_reclen - 1);
     char* type = getType(d_type);
     writeSucces = system_call(SYS_WRITE,STDOUT,type,strlen(type));
     if(writeSucces < 0)
     {
       system_call(SYS_CLOSE,folderDescriptor,"","");
       system_call(1,0x55,"","");
     }
     system_call(SYS_WRITE,STDOUT,"\n",1);
     bpos += d->d_reclen;
  }
 system_call(SYS_CLOSE,folderDescriptor,"","");
}

int main (int argc , char* argv[], char* envp[])
{
  if(argc > 1 && argv[1][1] == 'p')
  {
    printWithPrefix(argv[1][2]);
  }
  else
  {
    printWithoutPrefix();
  }
  return 0;
}
