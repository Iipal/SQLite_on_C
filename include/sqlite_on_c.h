#ifndef __SQLITE_ON_C_H__
#define __SQLITE_ON_C_H__

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

// okay, comments just to avoid stupid auto-sort of includes by clang-format
#include <cursor.h>

// custom includes:
#include <btree.h>
#include <command.h>
#include <meta_command.h>

#endif /* __SQLITE_ON_C_H__ */
