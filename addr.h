/*
  addr.h

  $Id: addr.h,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#ifndef ADDR_H
#define ADDR_H

struct ip_addr {
  char  *addr;
  short len;
};

extern const int addr_len;

const char *addrtostr(const struct ip_addr *ip);
void show_addr(const struct ip_addr *ip);

#endif /* ADDR_H */

/* Eof: addr.h */
