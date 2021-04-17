#include "util.h"

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

int main (int argc , char* argv[], char* envp[])
{
  const int BUF_SIZE = 8192;
  long bpos;
  int folderDescriptor,writeSucces;
  long nread;
  char buf[BUF_SIZE];
  struct linux_dirent *d;

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
     system_call(SYS_WRITE,STDOUT,"\n",1);
     bpos += d->d_reclen;
  }
 system_call(SYS_CLOSE,folderDescriptor,"","");

 return 0;
}
