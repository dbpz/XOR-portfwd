/*
  solve.h

  $Id: solve.h,v 1.1.1.1 2019/04/06 10:36:06 cvs Exp $
 */

#ifndef SOLVE_H
#define SOLVE_H

#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include "addr.h"

enum proto_t { P_TCP, P_UDP };

extern const int FIRST_PORT;
extern const int LAST_PORT;

const char *get_protoname(proto_t proto);
void solve_protonumbers();
int get_protonumber(proto_t proto);
int solve_portnumber(const char *portname, const char *protoname);
int solve_port(const char *portname, const char *protoname);
int solve_hostname_addr(char *buf, size_t *buf_len, const char *hostname);
struct ip_addr solve_hostname(const char *hostname);
uid_t solve_user(const char *username);
gid_t solve_group(const char *groupname);

#endif /* SOLVE_H */

/* Eof: solve.h */

