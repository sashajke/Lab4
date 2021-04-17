#include "util.h"

#define SYS_WRITE 4
#define SYS_READ 3
#define STDERR 2
#define STDOUT 1
#define STDIN 0

extern int system_call();

char convertChar(char ch)
{
  if(ch >= 'A' && ch <= 'Z')
    ch = ch + 32;
  return ch;
}

int processInputDebugMode()
{
  char ch;
  int sysCallReadRes = 0,sysCallWriteRes = 0;
  char* message = "insert sentence\n";
  system_call(SYS_WRITE,STDOUT,message,strlen(message));
  while(1)
  {
    sysCallReadRes = system_call(SYS_READ,STDIN,&ch,1);

    system_call(SYS_WRITE,STDERR,itoa(SYS_READ),strlen(itoa(SYS_READ)));
    system_call(SYS_WRITE,STDERR,"  ",2);
    system_call(SYS_WRITE,STDERR,itoa(sysCallReadRes),strlen(itoa(sysCallReadRes)));
    system_call(SYS_WRITE,STDERR,"\n",1);

    if(sysCallReadRes < 0)
    {
      system_call(1,0x55,"","");
    }
    else if(sysCallReadRes == 0)
    {
      break;
    }
    if(ch == '\n')
    {
      sysCallWriteRes = system_call(SYS_WRITE,STDOUT,&ch,1);
      system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
      system_call(SYS_WRITE,STDERR,"  ",2);
      system_call(SYS_WRITE,STDERR,itoa(sysCallWriteRes),strlen(itoa(sysCallWriteRes)));
      system_call(SYS_WRITE,STDERR,"\n",1);

    }
    else
    {
      ch = convertChar(ch);
      sysCallWriteRes = system_call(SYS_WRITE,STDOUT,&ch,1);
      system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
      system_call(SYS_WRITE,STDERR,"  ",2);
      system_call(SYS_WRITE,STDERR,itoa(sysCallWriteRes),strlen(itoa(sysCallWriteRes)));
      system_call(SYS_WRITE,STDERR,"\n",1);

    }
    if(sysCallWriteRes < 0)
    {
      system_call(1,0x55,"","");
    }
  }
  return 0;

}

int processInputRegularMode()
{
  char ch;
  int sysCallReadRes = 0,sysCallWriteRes=0;
  char* message = "insert sentence\n";
  system_call(SYS_WRITE,STDOUT,message,strlen(message));

  while(1)
  {
    sysCallReadRes = system_call(SYS_READ,STDIN,&ch,1);

    if(sysCallReadRes == 0)
    {
      break;
    }
    if(sysCallReadRes < 0)
    {
      system_call(1,0x55,"","");
    }
    if(ch == '\n')
    {
      sysCallWriteRes = system_call(SYS_WRITE,STDOUT,&ch,1);
    }
    else
    {
      ch = convertChar(ch);
      sysCallWriteRes = system_call(SYS_WRITE,STDOUT,&ch,1);
    }
    if(sysCallWriteRes < 0)
    {
      system_call(1,0x55,"","");
    }
  }
  return 0;
}

int main (int argc , char* argv[], char* envp[])
{
  if(argc > 1)
  {
    return processInputDebugMode();
  }
  return processInputRegularMode();
}
