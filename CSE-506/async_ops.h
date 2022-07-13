// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 1998-2020 Erez Zadok
 * Copyright (c) 2009	   Shrikar Archak
 * Copyright (c) 2003-2020 Stony Brook University
 * Copyright (c) 2003-2020 The Research Foundation of SUNY
 */

#ifndef _ASYNCOPS_H_
#define _ASYNCOPS_H_

#include <linux/fs.h>
#include <linux/magic.h>
#include <linux/sched.h>
#include <linux/xattr.h>


/* useful for tracking code reachability */
#define UDBG printk(KERN_DEFAULT "DBG:%s:%s:%d\n", __FILE__, __func__, __LINE__)

#define IOCTL_DELETE _IOR('a', 'a', char *)

struct var_args{
    int count;
    char *filenames[6];
};

#endif