/*
 *  prinfo.h  ($Revision: 0 $)
 *
 *  Copyright (C) 2012  Redshirt KSU CIS 520 design team
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef __PRINFO_H__
#define __PRINFO_H__

#include <linux/types.h>   // defines pid_t

struct prinfo {
      long state;             /* current state of process  */
      pid_t pid;              /* process id                */
      pid_t parent_pid;       /* process id of parent      */
      pid_t first_child_pid;  /* pid of youngest child     */
      pid_t next_sibling_pid; /* pid of older sibling      */
      long uid;               /* user id of process owner  */
      char comm[64];          /* name of program executed  */
};

#endif /* __PRINFO_H__ */