#include <stdio.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <syscall.h>  // added s? kernel/include/asm-generic has h
#include <sys/types.h>
#include "prinfo.h"


/** Display process information for up to three entries
in depth-first search order.  Display information about
process name, pid, parent pid, two siblings, and process
owner.  If a role is empty (e.g. there is no child), use 0.
*/

int main()
{
        struct prinfo procInfo[3];   
        int n = 3;
        int p;

        p = syscall(__NR_getpid);
        printf("pid = %d\n", p);
        printf("__NR_getpid = %d\n", __NR_getpid);

        p = syscall(245,procInfo,&n);

//      ptree(procInfo,&n);

        printf("procInfo[0].pid = %d\n", procInfo[0].pid);
        printf("procInfo[0].uid = %ld\n", procInfo[0].uid);

        printf("p= %d\n", p);

        return 0;
};