#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define SEEK_SET 0
#define O_RDRW 2
#define SYS_CLOSE 6
extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  if(argc < 3)
  {
    system_call(1,0x55,"","");
  }
  int sysCallRes  = 0;
  char* filePath = argv[1];
  char* nameToPut = argv[2];
  const int offset = 657;
  const int fileDescriptor = system_call(SYS_OPEN,filePath, O_RDRW,0777);
  if(fileDescriptor < 0)
  {
    system_call(1,0x55,"","");

  }
  sysCallRes = system_call(SYS_LSEEK,fileDescriptor,offset,SEEK_SET);
  if(sysCallRes != offset)
  {
    system_call(1,0x55,"","");

  }
  sysCallRes = system_call(SYS_WRITE,fileDescriptor,nameToPut,4);
  if(sysCallRes != 4)
  {
    system_call(1,0x55,"","");
  }
  sysCallRes = system_call(SYS_WRITE,fileDescriptor,".\n\0",3);
  if(sysCallRes != 1)
  {
    system_call(1,0x55,"","");
  }
  sysCallRes = system_call(SYS_CLOSE,fileDescriptor,"","");
  if(sysCallRes < 0)
  {
    system_call(1,0x55,"","");
  }
  return 0;
}
