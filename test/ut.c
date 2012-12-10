#include <stdio.h>
#include "prinfo.h"
#include <string.h>
//#include <linux/types.h>
typedef long pid_t;
void copyLine(struct prinfo p, char * line);
char getStateChar(long i);
#define TASK_RUNNING            0
#define TASK_INTERRUPTIBLE      1
#define TASK_UNINTERRUPTIBLE    2
#define __TASK_STOPPED          4
#define __TASK_TRACED           8
#define EXIT_ZOMBIE             16
#define EXIT_DEAD               32

int main()
{
 struct prinfo procInfo[3];

 procInfo[0].pid = 1;
 procInfo[0].state = 2;
 procInfo[0].parent_pid = 1; 
 procInfo[0].first_child_pid = 1;
 procInfo[0].next_sibling_pid = 1;
 procInfo[0].uid = 1; 
 procInfo[0].comm[0] = 'i';
 procInfo[0].comm[1] = 'n';
 procInfo[0].comm[2] = 'i';
 procInfo[0].comm[3] = 't';
 procInfo[0].comm[4] = '\0';

 procInfo[1].pid = 27;
 procInfo[1].state = 2;
 procInfo[1].parent_pid = 1; 
 procInfo[1].first_child_pid = 0;
 procInfo[1].next_sibling_pid = 28;
 procInfo[1].uid = 0; 
 procInfo[1].comm[0] = 'n';
 procInfo[1].comm[1] = 'e';
 procInfo[1].comm[2] = 'x';
 procInfo[1].comm[3] = 't';
 procInfo[1].comm[4] = '\0';

 
 procInfo[2].pid = 33;
 procInfo[2].state = 2;
 procInfo[2].parent_pid = 1; 
 procInfo[2].first_child_pid = 66;
 procInfo[2].next_sibling_pid = 77;
 procInfo[2].uid = 88; 
 procInfo[2].comm[0] = 'z';
 procInfo[2].comm[1] = 'y';
 procInfo[2].comm[2] = 'g';
 procInfo[2].comm[3] = 'o';
 procInfo[2].comm[4] = 't';
 procInfo[2].comm[5] = 'e';
 procInfo[2].comm[6] = '\0';

 int len = sizeof(procInfo);
 char result[len];
 int i;

	for (i=0;i<3;i++){
 		copyLine(procInfo[i], result);
	}
 	printf("  %s\n\n",result);

 	return 0;
}
void copyLine(struct prinfo procInfo, char * line)
{
	char num[50];
	char s;

	strcat(line, procInfo.comm); //max 64 char
	strcat(line, " ["); 
	sprintf (num, "%lu" , procInfo.pid); 
	strcat(line, num); 
	strcat(line, "] "); 
	s = getStateChar( procInfo.state); 
	strcat(line, &s); 
	strcat(line, ", "); 
	sprintf (num, "%lu" , procInfo.parent_pid); 
	strcat(line, num); 
	strcat(line, ", "); 
	sprintf (num, "%lu" , procInfo.first_child_pid);
	strcat(line, num); 
	strcat(line, ", "); 
	sprintf (num, "%lu" , procInfo.next_sibling_pid);
	strcat(line, num); 
	strcat(line, ", "); 
	sprintf (num, "%lu" , procInfo.uid);
	strcat(line, num); 
	strcat(line, " \n");
}
char getStateChar(long i)
{
	char ans;

	switch (i) {
	case TASK_RUNNING:
		ans = 'R';
		break;
	case TASK_INTERRUPTIBLE:  // sleeping
		ans = 'S';
		break;
	case TASK_UNINTERRUPTIBLE:  // uniterruptible sleep
		ans = 'S';
		break;
	case __TASK_STOPPED:  // stopped 
		ans = 'S';
		break;
	case __TASK_TRACED:  // being traced 
		ans = 'S';
		break;
	case EXIT_ZOMBIE:  
		ans = 'Z';
		break;
	case EXIT_DEAD:  
		ans = 'Z';
		break;
	default:
		ans = '?';
		break;
	}
	return ans;
}


