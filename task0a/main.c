#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define STDOUT 1
#define SEEK_SET 0
#define O_RDRW 2

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  char* myString = "hello world\n";
  int length = strlen(myString);
  system_call(SYS_WRITE,STDOUT, myString,length);
  return 0;
  
}
