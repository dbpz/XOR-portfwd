/*
  from_addr.hpp
  
  $Id: from_addr.hpp,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#ifndef FROM_ADDR_HPP
#define FROM_ADDR_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "port_pair.h"
#include "net_portion.h"

class from_addr
{
private:
  net_portion *np;
  port_pair   *pp;

  void pipe(const struct ip_addr *ip, int port) const;

public:
  from_addr(net_portion *portion, port_pair *pair)
    {
      np = portion;
      pp = pair;
    }

  void show() const;

  int match(const struct ip_addr *ip, int port) const;
};

#endif /* FROM_ADDR_HPP */

/* Eof: from_addr.hpp */
