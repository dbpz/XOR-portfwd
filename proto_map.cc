/*
  proto_map.cc

  $Id: proto_map.cc,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#include <syslog.h>
#include <errno.h>
#include "proto_map.hpp"
#include "forward.h"
#include "portfwd.h"

proto_map::proto_map(vector<int> *port_l, vector<host_map*> *map_l, struct ip_addr *actv, struct ip_addr *pasv, int user, int group, struct ip_addr listen, struct ip_addr *source, long long XORkey, long long confuskey, int isRemServer)
{
  port_list  = port_l;
  map_list   = map_l;
  confusing_key = confuskey;
  fragile    = 0; /* false */
  XOR_key = XORkey;
  is_remote_server = isRemServer;

  ftp_actv   = actv != NULL;
  if (ftp_actv)
    actv_ip = *actv;

  ftp_pasv   = pasv != NULL;
  if (ftp_pasv)
    pasv_ip = *pasv;

  uid        = user;
  gid        = group;
  local_listen = listen;

  if (source) {
    local_source = *source;
    local_src = &local_source;
  }
  else
    local_src = 0;
}

void proto_map::set_fragile(bool b)
{
  this->fragile = b;
}

void proto_map::show() const
{
  iterator<vector<int>,int> it(*port_list);
  it.start();
  if (it.cont()) {
    syslog(LOG_INFO, "%d", it.get());
    it.next();
  }
  for (; it.cont(); it.next())
    syslog(LOG_INFO, ", %d", it.get());

  syslog(LOG_INFO, " /* uid: %d, gid: %d */", uid, gid);
  syslog(LOG_INFO, " /* listen: %s */", addrtostr(&local_listen));

  if (local_src)
    syslog(LOG_INFO, " /* source: %s */", addrtostr(&local_source));

  if (ftp_actv)
    syslog(LOG_INFO, " ftp-active-mode-on %s", addrtostr(&actv_ip));

  if (ftp_pasv)
    syslog(LOG_INFO, " ftp-passive-mode-on %s", addrtostr(&pasv_ip));

  syslog(LOG_INFO, "\n{");
  iterator<vector<host_map*>,host_map*> it2(*map_list);
  it2.start();
  if (it2.cont()) {
    it2.get()->show();
    it2.next();
  }
  for (; it2.cont(); it2.next()) {
    syslog(LOG_INFO, ";\n");
    it2.get()->show();
  }
  syslog(LOG_INFO, "\n}\n");
}

void proto_map::serve(proto_t proto) const
{
  if (!port_list) {
    syslog(LOG_WARNING, "Missing port list");
    return;
  }

  const struct ip_addr *listen = &local_listen;

  switch (proto) {
  case P_TCP:
    tcp_forward(listen, local_src, port_list, map_list, ftp_actv ? &actv_ip : 0, ftp_pasv ? &pasv_ip : 0, uid, gid, fragile, XOR_key, confusing_key, is_remote_server);
    break;

  case P_UDP:
    udp_forward(listen, local_src, port_list, map_list, uid, gid, XOR_key);
    break;

  default:
    syslog(LOG_ERR, "Unknown protocol identifier: %d\n", proto);
  }

}

/* Eof: proto_map.cc */
