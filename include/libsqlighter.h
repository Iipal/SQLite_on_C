#ifndef __LIBSQLIGHTER_H__
#define __LIBSQLIGHTER_H__

#define _GNU_SOURCE

// libc inlcludes:
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// force include above everything:
#include <input_buffer.h>
#include <pager.h>
#include <row.h>
#include <table.h>

// custom includes:
#include <command.h>
#include <cursor.h>
#include <meta_command.h>

#endif /* __LIBSQLIGHTER_H__ */
