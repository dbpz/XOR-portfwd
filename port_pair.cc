/*
  port_pair.cc

  $Id: port_pair.cc,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#include <syslog.h>
#include "port_pair.h"

port_pair::port_pair(int p1, int p2)
{
  first = p1;
  last  = p2;
}

void port_pair::show() const
{
  syslog(LOG_INFO, "%d+%d", first, last);
}

int port_pair::match(int port) const
{
  return (port >= first) && (port <= last);
}

/* Eof: port_pair.cc */
