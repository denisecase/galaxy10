//#include <stdio.h>
//#include <linux/unistd.h>
//#include <sys/syscall.h>
//#include <linux/prinfo.h>

#include <stdio.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include "prinfo.h"

int main()
{
   // int entries;
   // printfo * buf;
   // int * nr;
   // entries = syscall(sys_ptree, buf, nr);

   struct prinfo procInfo[3];
   int n = 3;
   int p;

   p = syscall(__NR_getpid);
   printf("pid = %d\n", p);
   printf("__NR_getpid = %d\n", __NR_getpid);

   p = syscall(223,procInfo,&n);

  // ptree(procInfo,&n);

   printf("procInfo[0].pid = %d\n", procInfo[0].pid);
   printf("procInfo[0].uid = %ld\n", procInfo[0].uid);
  
   printf("p= %d\n", p);

   return 0;
}