#include "util.h"

#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_OPEN 5
#define SYS_CLOSE 6

#define STDERR 2
#define STDOUT 1
#define STDIN 0
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64


extern int system_call();

char convertChar(char ch)
{
  if(ch >= 'A' && ch <= 'Z')
    ch = ch + 32;
  return ch;
}



int readFromStdinWriteToStdout()
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


void closeFiles(int readFileDescriptor,int writeToFileDescriptor)
{
  if(readFileDescriptor != STDIN)
  {
    system_call(SYS_CLOSE,readFileDescriptor,"","");
  }
  if(writeToFileDescriptor != STDOUT)
  {
    system_call(SYS_CLOSE,writeToFileDescriptor,"","");
  }
}


int processInputDebugMode(int readFileDescriptor,int writeToFileDescriptor)
{
  char ch;
  int sysCallReadRes = 0,sysCallWriteRes = 0;
  while(1)
  {
    sysCallReadRes = system_call(SYS_READ,readFileDescriptor,&ch,1);

    system_call(SYS_WRITE,STDERR,itoa(SYS_READ),strlen(itoa(SYS_READ)));
    system_call(SYS_WRITE,STDERR,"  ",2);
    system_call(SYS_WRITE,STDERR,itoa(sysCallReadRes),strlen(itoa(sysCallReadRes)));
    system_call(SYS_WRITE,STDERR,"\n",1);

    if(sysCallReadRes < 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);
      system_call(1,0x55,"","");
    }
    else if(sysCallReadRes == 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);

      break;
    }
    if(ch == '\n')
    {
      sysCallWriteRes = system_call(SYS_WRITE,writeToFileDescriptor,&ch,1);
      system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
      system_call(SYS_WRITE,STDERR,"  ",2);
      system_call(SYS_WRITE,STDERR,itoa(sysCallWriteRes),strlen(itoa(sysCallWriteRes)));
      system_call(SYS_WRITE,STDERR,"\n",1);

    }
    else
    {
      ch = convertChar(ch);
      sysCallWriteRes = system_call(SYS_WRITE,writeToFileDescriptor,&ch,1);
      system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
      system_call(SYS_WRITE,STDERR,"  ",2);
      system_call(SYS_WRITE,STDERR,itoa(sysCallWriteRes),strlen(itoa(sysCallWriteRes)));
      system_call(SYS_WRITE,STDERR,"\n",1);

    }
    if(sysCallWriteRes < 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);

      system_call(1,0x55,"","");
    }
  }
  return 0;

}


int processInput(int readFileDescriptor,int writeToFileDescriptor)
{
  char ch;
  int sysCallReadRes = 0,sysCallWriteRes=0;



  while(1)
  {
    sysCallReadRes = system_call(SYS_READ,readFileDescriptor,&ch,1);

    if(sysCallReadRes == 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);
      break;
    }
    if(sysCallReadRes < 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);
      system_call(1,0x55,"","");
    }
    if(ch == '\n')
    {
      sysCallWriteRes = system_call(SYS_WRITE,writeToFileDescriptor,&ch,1);
    }
    else
    {
      ch = convertChar(ch);
      sysCallWriteRes = system_call(SYS_WRITE,writeToFileDescriptor,&ch,1);
    }
    if(sysCallWriteRes < 0)
    {
      closeFiles(readFileDescriptor,writeToFileDescriptor);
      system_call(1,0x55,"","");
    }
  }
  return 0;
}



int main (int argc , char* argv[], char* envp[])
{
  int debug = 0;
  int readFileDescriptor = STDIN,writeToFileDescriptor = STDOUT;
  int i;
  char action;
  for(i = 1;i < argc; i++)
  {
    action = argv[i][1];
    if(action == 'D')
    {
      debug = 1;
    }
    else if(action == 'i')
    {
      readFileDescriptor = system_call(SYS_OPEN,argv[i]+2,O_RDONLY,0777);
      if(readFileDescriptor < 0)
      {
        system_call(1,0x55,"","");
      }
    }
    else
    {
      writeToFileDescriptor = system_call(SYS_OPEN,argv[i]+2, O_CREAT|O_WRONLY,0777);
      if(writeToFileDescriptor < 0)
      {
        system_call(1,0x55,"","");
      }
    }
  }
  if(debug == 1)
    return processInputDebugMode(readFileDescriptor,writeToFileDescriptor);
  return processInput(readFileDescriptor,writeToFileDescriptor);
}
