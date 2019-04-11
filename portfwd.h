/*
  portfwd.h

  $Id: portfwd.h,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#ifndef PORTFWD_H
#define PORTFWD_H

#include "config.h"

extern const int BUF_SZ;
extern const char * const portfwd_version;

#ifdef HAVE_MSG_PROXY
extern int transparent_proxy;
#endif

extern int on_the_fly_dns;
extern int foreground;

#endif /* PORTFWD_H */

/* portfwd.h */
