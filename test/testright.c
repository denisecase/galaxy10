#include <stdio.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "prinfo.h"

// The working test program provided by Dr. Neilsen.
// compile with 
// /pub/CIS520/usr/arm/bin/arm-angstrom-linux-gnueabi-gcc -static -o testright testright.c 
//
int main()
{
        static struct prinfo procInfo[20];
        static int n = 20;
        int p;
        int i;

        n=20;
        printf("Trying 13\n");
        p = syscall(13,procInfo,&n);
        printf("p= %d, n= %d\n", p, n);
        for (i=0; i<20; i++)
          printf("procInfo[%d].pid = %d, parent_pid = %d, %s\n",i, procInfo[i].pid,procInfo[i].parent_pid,procInfo[i].comm);

        return 0;
};