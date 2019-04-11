/*
  port_pair.h

  $Id: port_pair.h,v 1.1.1.1 2019/04/06 10:36:05 cvs Exp $
 */

#ifndef PORT_PAIR_H
#define PORT_PAIR_H

#include <stdio.h>

class port_pair {
 private:
  int first;
  int last;

 public:
  port_pair(int p1, int p2);

  void show() const;

  int match(int port) const;
};

#endif /* PORT_PAIR_H */

/* Eof: port_pair.h */
