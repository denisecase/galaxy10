====================================================
Building a new syscall for the android kernel
====================================================
Team Red Shirt - CIS 520 Fall 2012
	Denise Case
	Emily Jordan
	Heath Yates

====================================================
Source Code
====================================================
The following kernel files were added or modified:

 kernel/arch/arm/kernel/calls.S 
 kernel/include/asm-generic/unistd.h 
 kernel/include/linux/prinfo.h  
 kernel/include/linux/syscalls.h 
 kernel/kernel/sys.c 

The following test programs were created:

 test/testright.c - used to call the system call from android
 test/methodtest.c  - used to test method calls offline

The first test program was helpful for developing as there
where some difficulties encountered getting a true
android test environment established. 

After several days, neither the emulator or the 
actual tablet were able to correctly open a new 
kernel built from the original source; we have
been unable to field test our updated source code. The 
new kernel compiles well with no errors or warnings 
(related to our content). 

The new prinfo.h file describes the given structure and
the majority of the coding was done in sys.c - specific
code changes are described in sections below. 

====================================================
Two Versions
====================================================
Modifications were made to the original source 
provided and we also worked with the new Jellybean
source. We were able to successfully install a
new signed kernel zipfile to the tablet's sdcard, but
the boot was not fully successful.

Notes on the process to develop and attempt to 
test our new system call are included below. 

============================================
Extract JB
============================================
original source for Jelly Bean 
/pub/CIS520/android_jellybean.tgz
tar xvzf android_jellybean.tgz
cd cis520/galaxy10/android_kernel_samsung_p4-jellybean

============================================
Handy install utility
============================================

We used the provided P7510 folder in JB that contains 
the script that can be used to generate a 
signed zip file that can be installed on 
the tablet.

============================================
Set up Makefile and get ready to build
============================================
To create the original image, you should 
just go to the top-level folder 
(after extracting the files) and edit 
the Makefile to specify location of cross-compiler
and remove errant tags:

 #CROSS_COMPILE ?= /pub/CIS520/usr/arm/bin/arm-angstrom-linux-gnueabi-
 CROSS_COMPILE	?= /pub/CIS520/usr/arm-2009q1/bin/arm-none-linux-gnueabi-

 #CFLAGS_KERNEL  = -mtune=cortex-a9 -ftree-vectorize -ffast-math -fsingle-precision-constant
 #AFLAGS_KERNEL  = -mtune=cortex-a9 -ftree-vectorize -ffast-math -fsingle-precision-constant
 CFLAGS_KERNEL	= -ftree-vectorize -ffast-math -fsingle-precision-constant
 AFLAGS_KERNEL	= -ftree-vectorize -ffast-math -fsingle-precision-constant

Edit and resave:
 drivers/net/wireless/bcmdhd/src/dhd/sys/dhd_sec_feature.h 

============================================
Make zImage
============================================

Then, skip the "make samsung_p4wifi_defconfig" 
step and just type 

> make 

To create /kernel/arch/arm/boot/zImage

............... old notes......................
Build correct config file in galaxy10/kernel:
make samsung_p4wifi_defconfig (if using tablet)
make versatile_defconfig (if using emulator)

Build zImage on cis linux for ARM platform
From galaxy10/kernel:
make clean kernel
make -j4 ARCH=arm 

(I already grabbed the configuration off of 
the tablet using adb put /proc/config.gz, 
and gunzipped it in the P7510 subfolder 
(this is the configuration file that 
was used to generate the image currently 
on the tablet). I also copied that config 
file to ".config" in the top-level source 
folder. This is the file that is created 
when you perform the step 
"make samsung_p4wifi_defconfig".)

============================================
Correct wifi error
============================================

Fix wifi error:
in P7510/tmp/META-INF/com/google/android/updater-script

assert(getprop("ro.product.device") == "p4wifi" || getprop("ro.build.product") == "p4wifi");
mount("ext4", "EMMC", "/dev/block/platform/sdhci-tegra.3/by-num/p4", "/system");
package_extract_file("system/bin/backuptool.sh", "/tmp/backuptool.sh");
package_extract_file("system/bin/backuptool.functions", "/tmp/backuptool.functions");

Add a # to comment out the first line.

============================================
Prepare kernel zipfile
============================================

In android_kernel_samsung_p4-jellybean/P7510:

> ./doit 

or 

>cp ../arch/arm/boot/zImage tmp
>cp ../drivers/net/wireless/bcmdhd/dhd.ko tmp/system/lib/modules
>cd tmp
>./abootimg -u boot.img -k zImage
>zip -u p4wifi-boot-120612.zip
>cd ..
>java -classpath testsign.jar testsign tmp/p4wifi-boot-120612.zip tmp/p4wifi-boot-120612-signed.zip
>java -classpath testsign.jar testsign tmp/p4wifi-boot-120612.zip tmp/dmc-signed.zip

Will create a signed zip file ready to be 
installed on the tablet:
P7510/tmp/p4wifi-boot-120612-signed.zip 

============================================
Move kernel zipfile to device (Samsung 10.1 tablet)
============================================

Copy signed zipfile to Windows platform.

Open Command Window at C:\adt-bundle-windows\sdk\tools
Verify device is connected and use adb to push it to the tablet. 

adb devices
adb push D:/_KSU/KSU-CIS520/p3/TAKE2/p4wifi-boot-120612-signed.zip /sdcard/Download

Shutdown tablet and boot into recovery mode,
by holding power and pushing volume down.
 
select "install zip from sdcard", 
then "choose zip from sdcard", 
browse to the file, and select 
Yes - Install p4wifi-boot-120612-signed.zip. 
Finally, go back and reboot the system. 
This got us to a permanent splash screen, but no further.

Additional notes not used:
You have to just raw write the data from that block device to create a boot.img (boot image) file; e.g.,
dd if=/dev/block/mmcblk0p3  of=/sdcard/boot.img
So, the boot.img doesn't exist as a file on the tablet originally.


============================================
Files added to source control (ref & modified)
============================================
from jellybean

/net/avenger.cis.ksu.edu/exports/home/grads/dmcase/cis520/galaxy10/android_kernel_samsung_p4-jellybean

git add -f android_kernel_samsung_p4-jellybean/arch/arm/kernel/calls.S 
git add android_kernel_samsung_p4-jellybean//include/linux/syscalls.h
git add android_kernel_samsung_p4-jellybean/kernel/sys.c
git add submittal/*.*
git add android_kernel_samsung_p4-jellybean/P7510/abootimg
git add android_kernel_samsung_p4-jellybean/P7510/abootimg-pack-initrd
git add android_kernel_samsung_p4-jellybean/P7510/abootimg-unpack-initrd
git add android_kernel_samsung_p4-jellybean/P7510/config
git add android_kernel_samsung_p4-jellybean/P7510/doit
git add android_kernel_samsung_p4-jellybean/P7510/tmp/boot.img
git add android_kernel_samsung_p4-jellybean/P7510/tmp/initrd.img
git add android_kernel_samsung_p4-jellybean/P7510/tmp/*.zip

cp kernel/arch/arm/kernel/calls.S submittal
cp kernel/include/asm-generic/unistd.h submittal
cp kernel/include/linux/prinfo.h  submittal
cp kernel/include/linux/syscalls.h submittal
cp kernel/kernel/sys.c submittal

from galaxy10:

git add test/test.c
git add test/ut.c
git add test/testcall.c
git add test/testright.c
git add docs/notes.txt

git add kernel/include/linux/prinfo.h 
git add kernel/arch/x86/kernel/syscall_table_32.S
git add kernel/include/linux/syscalls.h
git add kernel/include/asm-generic/errno-base.h
git add kernel/include/asm-generic/unistd.h  << right
git add kernel/arch/x86/include/asm/unistd_32.h << wrong
git add kernel/arch/arm/kernel/calls.S (used num 13)

git add kernel/kernel/sys.c
git add kernel/kernel/sched.c
git add kernel/kernel/timer.c
git add kernel/include/linux/list.h
git add kernel/include/linux/sched.h
git add kernel/include/linux/timer.h

Output:

git add -f kernel/arch/arm/boot/zImage

============================================
Defined syscall:
============================================
We originally used 347 for our syscall, but 
moved it to 13 after Dr. Neilsen recommended.

Defined new syscall ptree in 
   galaxy10/kernel/arch/x86/kernel/syscall_table_32.S (old)
   NOT .long sys_ptree  /* 347 our new call */
   .long sys_ptree   /* OUR NEW CALL NOW 13 */

Defined our new syscall number and updated total number of calls in  
   galaxy10/kernel/include/asm/unistd_32.h
   #define __NR_ptree		 13  /*OUR NEW CALL NOW 13*/
   NOT #define __NR_ptree		347    /* added - red shirt team */
   NOT #define NR_syscalls   348            /* modified - red shirt team (old total was 347) */

UPDATED: Defined our new syscall number and updated total number of calls in  
   galaxy10/kernel/include/asm-generic/unistd.h
  NOT  /*  our new system call */
  NOT  #define __NR_ptree 245
  NOT  __SYSCALL(__NR_ptree, sys_ptree)
//#define __NR_flistxattr 13
//__SYSCALL(__NR_flistxattr, sys_flistxattr)
/*  our new system call */
#define __NR_ptree 13
__SYSCALL(__NR_ptree, sys_ptree)

UPDATED: used obsolete number 13
galaxy10/kernel/arch/arm/kernel/calls.S:
CALL(sys_ptree) /* OUR NEW CALL not CALL(OBSOLETE(sys_time))used by libc4 */

Created new structure prinfo in 
   galaxy10/kernel/include/linux/prinfo.h

Added our new syscall signature to 
   galaxy10/kernel/include/linux/syscalls.h
   struct prinfo;              /* added - red shirt team */
   #include <linux/prinfo.h>   /* added - red shirt team */
   asmlinkage long sys_pinfo(struct prinfo *buf,int *nr);   /* added - red shirt team */

Added our new system call method to 
   galaxy10/kernel/kernel/sys.c
#include <linux/prinfo.h>   /* added - red shirt team */
/*
 * Our new ptree system call........................................................................... 
 */
SYSCALL_DEFINE2(ptree, struct prinfo __user *, buf, int *, nr)
{
	long retval = -EINVAL;
	if (!buf || !nr )
		return -EINVAL;
	if (!access_ok(VERIFY_READ, buf, sizeof(struct prinfo)))
		return -EFAULT;

	rcu_read_lock();
	read_lock(&tasklist_lock);

	// code goes here
	retval = 0;

	read_unlock(&tasklist_lock);
	rcu_read_unlock();
	return retval;
}

============================================
Process States
============================================

Need to map to S (sleeping) r(running) z(zombie)

Process states (in UNIX) are: 

R - runnable which means the process has done a context switch and has the kernel.
S - sleeping which means the process is waiting on I/O completion (blocked), a pipe, memory, etc.
T - process has been stopped - sent a SIGSTOP usually with ctrl/z
Z - zombie - a process that has a process image in memory but no context, ie., not swappable.

============================================
Process State Mapping
============================================

We mapped all states as shown below.  Only running 
mapped exactly.  Anything that could reawaken was 
considered as  S for sleeping and anything done 
(dead or zombie) was considered Z for zombie.

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
============================================
Coverting the procInfo struct to string
============================================
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

============================================
Test harness
============================================

#include <stdio.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "prinfo.h"

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

============================================
Compile test program:
============================================
In galaxy10/test:
/pub/CIS520/usr/arm/bin/arm-angstrom-linux-gnueabi-gcc -static -o testright testright.c

For reference, view 
pstree -G

============================================
Testing: 
From ADB Command Window at D:\KSU\KSU-CIS520\p3
============================================

Android Debug Bridge (to actual & virtual devices)  
http://developer.android.com/tools/help/adb.html
Push and run program on emulator:
       (start emulator)
	adb devices
	adb push Take2/testright /data/misc
	adb shell
	#su
	#cd data/misc
	#ls -all
	#chmod 755 testcall
	# ./testcall
	#ps (to check)

New kernel:
   (start emulator)
	adb devices
	#cp Take2/zImage  tmp
	adb push Take2/zImage /data/misc
	adb shell
	#su
	#cd data/misc
	#ls -all
	#chmod 755 testcall
	# ./testcall
	#ps (to check)


Remove from emulator:
	adb pull /path/in/emulator /local/path
	adb pull /sdcard/download/boot.img

Debug:
arm-angstrom-linux-gnueabi-gdb exe_name
(gdb) target remote localhost:1234
>

==================================================
Using the Emulator 
==================================================
From Command Window at C:\adt-bundle-windows\sdk\tools
Create avds in the associated eclipse 
android list targets
android create avd -n my31 -t 7
android create avd -n my412 -t 12 (must specify --abi x86 vs arm)
android create avd -n my42 -t 14 (def is armeabi-v7a)
emulator -avd 10tab
emulator -avd p3H
emulator -avd p3 

Various attempts to load a new kernel in an android virtual device (avd):

emulator-arm -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -show-kernel -verbose @my42S 
emulator -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -show-kernel -verbose -gpu on @my42 
emulator-arm -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -show-kernel -verbose -noaudio -gpu off @my42 
emulator-arm -kernel D:/_KSU/KSU-CIS520/p3/zImage -show-kernel -verbose -noaudio -gpu off @my31 
emulator-arm -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -show-kernel -verbose -noaudio -gpu off @my41S 
emulator-arm -kernel D:/_KSU/KSU-CIS520/p3/zImage -show-kernel -verbose -noaudio -gpu off @my31S 
emulator  -avd 10tab -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -ramdisk ramdisk.img -show-kernel -verbose
emulator-arm  -avd 10tab -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage   -verbose -show-kernel
emulator  -avd 10tab -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage   -verbose
emulator  -avd p3H -kernel D:/_KSU/KSU-CIS520/p3/zImage 
emulator  -avd p3H -kernel D:/_KSU/KSU-CIS520/p3/TAKE1/zImage 
emulator  -avd p3 -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -ramdisk ramdisk.img -show-kernel
emulator-arm  -avd p3 -kernel D:/_KSU/KSU-CIS520/p3/TAKE2/zImage -verbose -show-kernel

GOOGLE FIX for OpenGLES error:
I fired up process explorer on this and found that it was looking for the openGL stuff in the wrong folder:
\tools instead of \tools\lib.  I copied the following files to the tools folder:
libEGL_translator.dll
libGLES_CM_translator.dll
libGLES_V2_translator.dll
libOpenglRender.dll

GOOGLE FIX for offline:
Manually delete these folders:
C:\Users\%UserName%.android\avd\AVD2.1.avd\cache.img.lock
C:\Users\%UserName%.android\avd\AVD2.1.avd\userdata-qemu.img.lock

Emulator suggestions:
set the resolution to 1280x800. 
Name: GalaxyTab10.1wJB (for JellyBean), 
Target: Android 4.1 - APILevel 16, 
CPU/ABI: ARM (armeabi-v7a), 
SD Card: Size: 128 MiB, 
Resolution: 1280x800. 
And it works fine, just kind of slow.
Ours never worked with a newly-built (unmodified) kernel.



============================================
Additional Notes:
============================================
The zImage has to be packaged inside of the boot.img, 
and the boot.img has to be flashed to 
/dev/block/mmcblk0p3. (that's a zero p three). 
You can grab the existing boot.img on the tablet, 
and then update the zImage using the command: 
abootimg -u boot.img -k zImage, and you can 
extract the existing boot.img using: 
abootimg -x boot.img. 
What you'll see as output will be a configuration file, 
bootimg.cfg, the initial ramdisk image, 
initrd.img, and zImage. 
If you have your tablet rooted 
(e.g., added the command su = superuser), 
then you can fire up a shell on the tablet 
and extract the existing boot image using the command: 
dd if=/dev/block/mmcblk0p3 of=/sdcard/Download/boot.img, 
then use adb to copy the file to your pc using: 
adb pull /sdcard/Download/boot.img. 
Fix up a new boot image, and push it to the tablet using: 
adb push boot.img /sdcard/Download/boot.img, 
then fire up a su shell on the tablet, 
fire up a shell and type the command: 
su - to become "root" or "superuser", 
and issue the command: 
dd if=/sdcard/Download/boot.img of=/dev/block/mmcblk0p3. 
When you reboot and check the "About" information, 
you should see that the operating system has been modified. 
Final caveat, for some reason, you have to also update the 
wireless module (dhd.ko) or your wireless access will be disabled -- 
I think the wireless module depends on the kernel code directly, 
but I haven't investigated that too closely yet. Hopefully, 
I didn't totally confuse you, but it's a bit more 
involved than Linux where you can just replace the 
vmlinux image and LILO will boot the new kernel -- 
Android uses a multi-step booting process.


On the last page of Project 3, the adb command 
is used to push the executable to the /data folder. 
This works fine on the emulator, but not directly 
on the Galaxy Tab -- you can't push to 
the /data folder directly.
============================================
END
============================================