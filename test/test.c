#include <stdio.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <linux/prinfo.h>

int main(void)
{
   int entries;
   printfo * buf;
   int * nr;
   entries = syscall(sys_ptree, buf, nr);
}