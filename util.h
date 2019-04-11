/*
  util.h

  $Id: util.h,v 1.1.1.1 2019/04/06 10:36:05 cvs Exp $
 */

#ifndef UTIL_H
#define UTIL_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"

extern int verbose_mode;

#define ONVERBOSE(proc) if (verbose_mode) (proc);
#define ONVERBOSE2(proc) if (verbose_mode > 1) (proc);
#define ONVERBOSE3(proc) if (verbose_mode > 2) (proc);
#define DEBUGFD(proc) ONVERBOSE3(proc)

void set_prog_name(const char *name);
const char *get_prog_name();

char* safe_strdup(const char* str);
void safe_strcpy(char *trg, const char *src, int trg_buf_size);

int cd_root();
int std_to_null();
int daemonize();

void socket_close(int fd); 

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#endif /* UTIL_H */

/* eof: util.h */
