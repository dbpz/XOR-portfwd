/*
  from_addr.c

  $Id: from_addr.cc,v 1.1.1.1 2019/04/06 10:36:08 cvs Exp $
 */

#include <syslog.h>
#include "from_addr.hpp"

void from_addr::show() const
{
  np->show();
  syslog(LOG_INFO, ":");
  pp->show();
}

/*
 * Returns 0 on failure.
 */
int from_addr::match(const struct ip_addr *ip, int port) const
{
  /*
  syslog(LOG_DEBUG, "? %s:%d", addrtostr(ip), port);
  np->show();
  pp->show();
  syslog(LOG_DEBUG, "> %d %d", np->owns(ip), pp->match(port));
  */

  return pp->match(port) && np->owns(ip);
}

/* Eof: from_addr.c */
