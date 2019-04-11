/*
  to_addr.hpp
  
  $Id: to_addr.hpp,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#ifndef TO_ADDR_HPP
#define TO_ADDR_HPP

#include "addr.h"

class to_addr
{
public:
  virtual void show() const = 0;
  virtual int get_addr(const char *protoname, 
	const struct sockaddr_in *cli_sa, 
	const struct sockaddr_in *local_cli_sa, 
	const struct ip_addr **addr, int *prt) = 0;
};

#endif /* TO_ADDR_HPP */

/* Eof: to_addr.hpp */
