/*
  forward.c

  $Id: forward.cc,v 1.20 2019/04/07 21:08:57 cvs Exp $
 */

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>
#include "portfwd.h"
#include "forward.h"
#include "util.h"
#include "solve.h"
#include "host_map.hpp"
#include "iterator.hpp"


static int isProbablePrime(long oddNumber) {
    long i;

    for (i=3; i<51; i+=2)
        if (oddNumber == i)
            return 1;
        else if (oddNumber%i == 0)
            return 0;

    return 1; /* maybe */
}

static long calculateIdealNumOfBuckets(HashTable *hashTable) {
    long idealNumOfBuckets = hashTable->numOfElements / hashTable->idealRatio;
    if (idealNumOfBuckets < 5)
        idealNumOfBuckets = 5;
    else
        idealNumOfBuckets |= 0x01; /* make it an odd number */
    while (!isProbablePrime(idealNumOfBuckets))
        idealNumOfBuckets += 2;

    return idealNumOfBuckets;
}

static int pointercmp(const void *pointer1, const void *pointer2) {
    return (pointer1 != pointer2);
}

static unsigned long pointerHashFunction(const void *pointer) {
    return ((unsigned long) pointer) >> 4;
}

void HashTableRehash(HashTable *hashTable, long numOfBuckets) {
    KeyValuePair **newBucketArray;
    int i;

    assert(numOfBuckets >= 0);
    if (numOfBuckets == 0)
        numOfBuckets = calculateIdealNumOfBuckets(hashTable);

    if (numOfBuckets == hashTable->numOfBuckets)
        return; /* already the right size! */

    newBucketArray = (KeyValuePair **)
                                malloc(numOfBuckets * sizeof(KeyValuePair *));
    if (newBucketArray == NULL) {
        /* Couldn't allocate memory for the new array.  This isn't a fatal
         * error; we just can't perform the rehash. */
        return;
    }

    for (i=0; i<numOfBuckets; i++)
        newBucketArray[i] = NULL;

    for (i=0; i<hashTable->numOfBuckets; i++) {
        KeyValuePair *pair = hashTable->bucketArray[i];
        while (pair != NULL) {
            KeyValuePair *nextPair = pair->next;
            long hashValue = hashTable->hashFunction(pair->key) % numOfBuckets;
            pair->next = newBucketArray[hashValue];
            newBucketArray[hashValue] = pair;
            pair = nextPair;
        }
    }

    free(hashTable->bucketArray);
    hashTable->bucketArray = newBucketArray;
    hashTable->numOfBuckets = numOfBuckets;
}


HashTable *HashTableCreate(long numOfBuckets) {
    HashTable *hashTable;
    int i;

    assert(numOfBuckets > 0);

    hashTable = (HashTable *) malloc(sizeof(HashTable));
    if (hashTable == NULL)
        return NULL;

    hashTable->bucketArray = (KeyValuePair **)
                        malloc(numOfBuckets * sizeof(KeyValuePair *));
    if (hashTable->bucketArray == NULL) {
        free(hashTable);
        return NULL;
    }
    
    hashTable->numOfBuckets = numOfBuckets;
    hashTable->numOfElements = 0;

    for (i=0; i<numOfBuckets; i++)
        hashTable->bucketArray[i] = NULL;

    hashTable->idealRatio = 3.0;
    hashTable->lowerRehashThreshold = 0.0;
    hashTable->upperRehashThreshold = 15.0;

    hashTable->keycmp = pointercmp;
    hashTable->valuecmp = pointercmp;
    hashTable->hashFunction = pointerHashFunction;
    hashTable->keyDeallocator = NULL;
    hashTable->valueDeallocator = NULL;

    return hashTable;
}


int HashTablePut(HashTable *hashTable, const void *key, void *value) {
    long hashValue;
    KeyValuePair *pair;

    assert(key != NULL);
    assert(value != NULL);

    hashValue = hashTable->hashFunction(key) % hashTable->numOfBuckets;
    pair = hashTable->bucketArray[hashValue];

    while (pair != NULL && hashTable->keycmp(key, pair->key) != 0)
        pair = pair->next;

    if (pair) {
        if (pair->key != key) {
            if (hashTable->keyDeallocator != NULL)
                hashTable->keyDeallocator((void *) pair->key);
            pair->key = key;
        }
        if (pair->value != value) {
            if (hashTable->valueDeallocator != NULL)
                hashTable->valueDeallocator(pair->value);
            pair->value = value;
        }
    }
    else {
        KeyValuePair *newPair = (KeyValuePair *) malloc(sizeof(KeyValuePair));
        if (newPair == NULL) {
            return -1;
        }
        else {
            newPair->key = key;
            newPair->value = value;
            newPair->next = hashTable->bucketArray[hashValue];
            hashTable->bucketArray[hashValue] = newPair;
            hashTable->numOfElements++;

            if (hashTable->upperRehashThreshold > hashTable->idealRatio) {
                float elementToBucketRatio = (float) hashTable->numOfElements /
                                             (float) hashTable->numOfBuckets;
                if (elementToBucketRatio > hashTable->upperRehashThreshold)
                    HashTableRehash(hashTable, 0);
            }
        }
    }

    return 0;
}


void *HashTableGet(const HashTable *hashTable, const void *key) {
    long hashValue = hashTable->hashFunction(key) % hashTable->numOfBuckets;
    KeyValuePair *pair = hashTable->bucketArray[hashValue];

    while (pair != NULL && hashTable->keycmp(key, pair->key) != 0)
        pair = pair->next;

    return (pair == NULL)? NULL : pair->value;
}


void HashTableRemove(HashTable *hashTable, const void *key) {
    long hashValue = hashTable->hashFunction(key) % hashTable->numOfBuckets;
    KeyValuePair *pair = hashTable->bucketArray[hashValue];
    KeyValuePair *previousPair = NULL;

    while (pair != NULL && hashTable->keycmp(key, pair->key) != 0) {
        previousPair = pair;
        pair = pair->next;
    }

    if (pair != NULL) {
        if (hashTable->keyDeallocator != NULL)
            hashTable->keyDeallocator((void *) pair->key);
        if (hashTable->valueDeallocator != NULL)
            hashTable->valueDeallocator(pair->value);
        if (previousPair != NULL)
            previousPair->next = pair->next;
        else
            hashTable->bucketArray[hashValue] = pair->next;
        free(pair);
        hashTable->numOfElements--;

        if (hashTable->lowerRehashThreshold > 0.0) {
            float elementToBucketRatio = (float) hashTable->numOfElements /
                                         (float) hashTable->numOfBuckets;
            if (elementToBucketRatio < hashTable->lowerRehashThreshold)
                HashTableRehash(hashTable, 0);
        }
    }
}


int grandchild_pid[PORTFWD_MAX_FD];

/*
 * Returns -1 on failure; file descriptor on success.
 */
int tcp_connect(const struct ip_addr *ip, int port)
{
  int rsd = socket(PF_INET, SOCK_STREAM, get_protonumber(P_TCP));
  if (rsd == -1) {
    syslog(LOG_ERR, "tcp_connect: Can't create TCP socket: %m");
    return -1;
  }
  DEBUGFD(syslog(LOG_DEBUG, "socket open: FD %d", rsd));

  struct sockaddr_in sa;
  sa.sin_family      = PF_INET;
  sa.sin_port        = htons(port);
  sa.sin_addr.s_addr = *((unsigned int *) ip->addr);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));

  if (connect(rsd, (struct sockaddr *) &sa, sizeof(sa))) {
    syslog(LOG_ERR, "tcp_connect: Can't connect to %s:%d: %m", inet_ntoa(sa.sin_addr), port);
    socket_close(rsd);
    return -1;
  }

  return rsd;
}

/*
 * Returns host_map success; NULL on failure.
 */
host_map *tcp_match(const vector<host_map*> *map_list, const struct ip_addr *ip, int port)
{
  host_map *hm;
  iterator<vector<host_map*>,host_map*> it(*map_list);
  for (it.start(); it.cont(); it.next()) {
    hm = it.get();
    if (hm->tcp_match(ip, port))
      return hm;
  }
  return 0;
}

void quit_handler(int sig)
{
  ONVERBOSE(syslog(LOG_DEBUG, "child: quit_handler: Grandchild with PID %d exiting under request", getpid()));

  exit(0);
}

int simple_buf_copy(int src_fd, int trg_fd)
{
  char buf[BUF_SZ];
  int rd = read(src_fd, buf, BUF_SZ);
  if (!rd)
    return -1;
  if (rd < 0) {
    syslog(LOG_ERR, "simple_copy: Failure reading from socket: %m");
    return -1;
  }

  int wr = write(trg_fd, buf, rd);
  if (wr == -1) {
    if (errno == EPIPE)
      ONVERBOSE2(syslog(LOG_DEBUG, "simple_copy: Broken pipe: %m"));
    return -1;
  }
  if (wr < rd) {
    ONVERBOSE(syslog(LOG_WARNING, "simple_copy: Partial write to socket: %m"));
    return -1;
  }

  return 0;
}

void simple_tcp_forward(int sd, struct ip_addr *remote_ip, int remote_port)
{
  int dest_fd[PORTFWD_MAX_FD];

  fd_set fds, tmp_fds;
  int fd_set_len = sizeof(fd_set);
  FD_ZERO(&fds);
  int maxfd = 0;
  fdset(sd, &fds, &maxfd);
  int nd;

  for (;;) { /* forever */
    /*
     * Restores tmp_fds from fds.
     */
    memcpy(&tmp_fds, &fds, fd_set_len);

    /*
     * Wait for event: connection on sd or data on anyone else.
     */
    DEBUGFD(syslog(LOG_DEBUG, "simple_forward: TCP select(): %d FDs", maxfd));
    nd = select(maxfd, &tmp_fds, 0, 0, 0);
    if (nd == -1) {
      syslog(LOG_ERR, "simple_forward: TCP select() failed: %m");
      continue;
    }

    /*
     * Let's handle sd here (mother socket).
     */
    if (FD_ISSET(sd, &tmp_fds)) {

      --nd;

      /*
       * Clear sd so it's ignored below in the
       * loop for the client sockets.
       */
      FD_CLR(sd, &tmp_fds); 

      struct sockaddr_in cli_sa;
      socklen_t cli_sa_len = sizeof(cli_sa);
      int csd = accept(sd, (struct sockaddr *) &cli_sa, &cli_sa_len);
      if (csd < 0)
	syslog(LOG_ERR, "simple_forward: Can't accept TCP socket: %m");
      else {
	int cli_port = ntohs(cli_sa.sin_port);
	ONVERBOSE(syslog(LOG_DEBUG, "simple_forward: TCP connection from %s:%d", inet_ntoa(cli_sa.sin_addr), cli_port));

	/*
	 * Connect to destination.
	 */
	  
	int rsd = tcp_connect(remote_ip, remote_port);
	if (rsd != -1) {
	  if ((csd >= PORTFWD_MAX_FD) || (rsd >= PORTFWD_MAX_FD)) {
	    syslog(LOG_ERR, "simple_forward: Destination socket descriptors overflow");
	    socket_close(csd);
	    socket_close(rsd);
	  }
	  else {
	    /*
	     * Add pair of communicating sockets.
	     */
	    fdset(csd, &fds, &maxfd);
	    fdset(rsd, &fds, &maxfd);
	  
	    /*
	     * Save peers so they can be remembered later.
	     */
	    dest_fd[csd] = rsd;
	    dest_fd[rsd] = csd;
	  }
	}
      }

    } /* sd (mother socket) handled */

    /*
     * Now all other sockets.
     */
    for (int src_fd = 0; nd; ++src_fd)
      if (FD_ISSET(src_fd, &tmp_fds)) {
	--nd;
	int trg_fd = dest_fd[src_fd];
	/*
	 * Copy data.
	 */
	int fail = simple_buf_copy(src_fd, trg_fd);
	if (fail) {
	  /*
	   * Remove pair of communicating sockets.
	   */
	  DEBUGFD(syslog(LOG_DEBUG, "simple_forward: closed socket (FD %d or %d)", src_fd, trg_fd));

	  fdclear(src_fd, &fds, &maxfd);
	  fdclear(trg_fd, &fds, &maxfd);
	  socket_close(src_fd);
	  socket_close(trg_fd);
	}
      }

  } /* main loop */

}

int ftp_spawn(struct ip_addr *local_ip, int *local_port, struct ip_addr *remote_ip, int remote_port)
{
  int sd = tcp_listen(local_ip, local_port, 3);
  if (sd == -1) {
    syslog(LOG_ERR, "FTP spawn: Can't listen: %m");
    return -1;
  }

  pid_t pid = fork();

  /* Fork failed? */
  if (pid < 0) {

    /* Fork failed */

    syslog(LOG_ERR, "FTP spawn: fork() failed: %m");

    socket_close(sd); /* the socket won't be used at all */
    return pid;
  }

  if (pid) {

    /* Child */

    socket_close(sd); /* the child won't use the socket */
    return pid;
  }

  /* Grandchild */

  void (*prev_handler)(int);
  prev_handler = signal(SIGCHLD, SIG_IGN);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed on ignore: %m");
    exit(1);
  }
  prev_handler = signal(SIGUSR1, quit_handler);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed for grandchild's quit handler: %m");
    exit(1);
  }

  simple_tcp_forward(sd, remote_ip, remote_port);

  syslog(LOG_ERR, "Grandchild exiting (!)");
  exit(1);
}

void gc_clean(int fd)
{
  pid_t pid = grandchild_pid[fd];
  if (pid != -1) {
    ONVERBOSE(syslog(LOG_DEBUG, "Requesting termination of PID %d for FD %d", pid, fd));

    if (kill(pid, SIGUSR1))
      syslog(LOG_ERR, "Can't request grandchild termination for PID: %d: %m", pid);
    grandchild_pid[fd] = -1;
  }
}

void gc_fill(int fd, pid_t pid)
{
  gc_clean(fd);

  grandchild_pid[fd] = pid;

  ONVERBOSE(syslog(LOG_DEBUG, "PID %d stored for termination request on FD %d", pid, fd));
}

int ftp_active(const struct ip_addr *actv_ip, char *buf, int *rd, int src_fd, int trg_fd)
{
  if (strncasecmp(buf, "port", 4))
    return 0;

  ONVERBOSE(syslog(LOG_DEBUG, "Active FTP request detected: %s", buf));

  char *i = strchr(buf, ' ');
  if (!i) {
    syslog(LOG_ERR, "Missing remote address in active FTP request");
    return -1;
  }
  ++i;

  int ad[4];
  int port[2];
  
  if (sscanf(i, "%d,%d,%d,%d,%d,%d", 
	    &ad[0], &ad[1], &ad[2], &ad[3],
	    &port[0], &port[1]) < 6) {
    syslog(LOG_ERR, "Active FTP request mismatch");
    return -1;
  }

  /*
   * Remote address.
   */

  char addr[4];
  addr[0] = ad[0];
  addr[1] = ad[1];
  addr[2] = ad[2];
  addr[3] = ad[3];

  struct ip_addr remote_ip;
  remote_ip.len = addr_len;
  remote_ip.addr = addr;
  int remote_port = (port[0] << 8) | port[1];

  ONVERBOSE(syslog(LOG_DEBUG, "Remote address extracted: %s:%d", addrtostr(&remote_ip), remote_port));

  /*
   * Local address (0.0.0.0).
   */

  int laddr = INADDR_ANY;
  struct ip_addr local_ip;
  local_ip.len = addr_len;
  local_ip.addr = (char *) &laddr;

  /*
   * FTP forwarder.
   */
    
  int local_port = 0;

  pid_t pid = ftp_spawn(&local_ip, &local_port, &remote_ip, remote_port);
  if (pid == -1) {
    syslog(LOG_ERR, "Can't spawn FTP forwarder");
    /* free(local_ip.addr); */
    return -1;
  }

  /*
   * Address rewriting (IP given by ftp-active-mode-on).
   */
  const char *local_addr = actv_ip->addr;
  if (snprintf(buf, BUF_SZ, "PORT %u,%u,%u,%u,%u,%u\n", 
	       (unsigned char) local_addr[0], 
	       (unsigned char) local_addr[1], 
	       (unsigned char) local_addr[2], 
	       (unsigned char) local_addr[3],
	       (local_port >> 8) & 0xFF, local_port & 0xFF) == -1)
    syslog(LOG_WARNING, "Active FTP request truncated");

  *rd = strlen(buf);

  /*
   * Store forwarder's PID so it can be terminated.
   */
  gc_fill(src_fd, pid);
  gc_fill(trg_fd, pid);

  return 0;
}

int ftp_passive(const struct ip_addr *pasv_ip, char *buf, int *rd, int src_fd, int trg_fd)
{
  if (memcmp(buf, "227", 3))
    return 0;

  ONVERBOSE(syslog(LOG_DEBUG, "Passive FTP reply detected: %s", buf));

  char *i = strchr(buf, '(');
  if (!i) {
    syslog(LOG_ERR, "Missing remote address in passive FTP reply");
    return -1;
  }
  ++i;

  int ad[4];
  int port[2];
  
  if (sscanf(i, "%d,%d,%d,%d,%d,%d", 
	    &ad[0], &ad[1], &ad[2], &ad[3],
	    &port[0], &port[1]) < 6) {
    syslog(LOG_ERR, "Passive FTP reply mismatch");
    return -1;
  }

  /*
   * Remote address.
   */

  char addr[4];
  addr[0] = ad[0];
  addr[1] = ad[1];
  addr[2] = ad[2];
  addr[3] = ad[3];

  struct ip_addr remote_ip;
  remote_ip.len = addr_len;
  remote_ip.addr = addr;
  int remote_port = (port[0] << 8) | port[1];

  ONVERBOSE(syslog(LOG_DEBUG, "Remote address extracted: %s:%d", addrtostr(&remote_ip), remote_port));

  /*
   * Local address (0.0.0.0).
   */

  int laddr = INADDR_ANY;
  struct ip_addr local_ip;
  local_ip.len = addr_len;
  local_ip.addr = (char *) &laddr;

  /*
   * FTP forwarder.
   */
    
  int local_port = 0;

  pid_t pid = ftp_spawn(&local_ip, &local_port, &remote_ip, remote_port);
  if (pid == -1) {
    syslog(LOG_ERR, "Can't spawn FTP forwarder");
    return -1;
  }

  /*
   * Address rewriting (IP given by ftp-passive-mode-on).
   */
  const char *local_addr = pasv_ip->addr;
  if (snprintf(buf, BUF_SZ, "227 Portfwd Passive Mode (%u,%u,%u,%u,%u,%u)\n", 
	       (unsigned char) local_addr[0], 
	       (unsigned char) local_addr[1], 
	       (unsigned char) local_addr[2], 
	       (unsigned char) local_addr[3],
	       (local_port >> 8) & 0xFF, local_port & 0xFF) == -1)
    syslog(LOG_WARNING, "Passive FTP reply truncated");

  *rd = strlen(buf);

  /*
   * Store forwarder's PID so it can be terminated.
   */
  gc_fill(src_fd, pid);
  gc_fill(trg_fd, pid);

  return 0;
}


typedef struct{
  unsigned long long in_offset;
  unsigned long long out_offset;
  unsigned long long session_key;
  int inflate_when_copying;
} clientSock_session_t;

HashTable * fd_offset_table = NULL;
void remove_fd_offset(int fd){
  HashTableRemove(fd_offset_table,  (void *)(1llu+fd));
}

int pseudo_rand_key_offset(long long key, long long offset, long long session_key){
<<<<<<< HEAD
  return (int)( ( key ^ session_key ^ ((offset + key % 39191llu) << 30) ^(((offset+key+session_key)% 6373737llu)<<15) ^ ((offset+key+526231)<<3)) % 41621llu);
=======
  return (int)( (key ^(((offset ^ session_key) % 2038074739ll) << 27) ^ (((offset ^ key) % 3603804991llu)<<3)) % 41621llu);
>>>>>>> 8c360db
}

int is_insertion_byte(long long key, long long inflated_offset, long long  session_key){
  if(key==0) return 0;
  int vr =  pseudo_rand_key_offset(key, inflated_offset, session_key);

  long long vcut = 10000 + ( vr<<14 )  % 65103llu;
<<<<<<< HEAD
  if(vcut > inflated_offset) return vr > (vr<<14 + inflated_offset) % 30857 + 2000;
  else return vr < 1500;
=======
  if(vcut > inflated_offset) return vr > (vr<<14 + inflated_offset) % 30857 + 10000;
  else return vr < 1000;
>>>>>>> 8c360db
}

int apply_XOR_buf(long long XOR_key, long long confusing_key, char *buf, char ** buf2, int * rd, int sock_fd, int need_session){
  clientSock_session_t * session = NULL;

  if( need_session ) session = (clientSock_session_t *)HashTableGet(fd_offset_table, (void *)(1llu+sock_fd));
  int xx;

  if(session && session -> inflate_when_copying){
    for(xx = 0; xx<*rd; xx++){
      unsigned long long offs = session->in_offset+xx;
      buf[xx] ^= (pseudo_rand_key_offset(XOR_key, offs,  session -> session_key)&0xff);
    }
    session->in_offset+=(*rd);
    int oxx = 0;
    xx = 0;

    int buf2_size = (*rd)*1.2 + 3;
    (*buf2) = (char*)malloc(buf2_size);
    while(xx <  * rd){
      if(buf2_size <= oxx){
         buf2_size = buf2_size*1.5 +1;
         *buf2 = (char*)realloc(*buf2, buf2_size);
      }

      if(is_insertion_byte(confusing_key, session->out_offset, session -> session_key)) (*buf2)[oxx] = (char)(pseudo_rand_key_offset(confusing_key, session->out_offset+33378787llu, session -> session_key)&0xff);
      else (*buf2)[oxx] = buf[xx++];
      oxx++;
      session->out_offset++;
    }
    *rd = oxx;
    return 1;
  }else if(session && !session -> inflate_when_copying){
    int wxx = 0;
    for(xx=0; xx<*rd; xx++){
       if(!is_insertion_byte(confusing_key, session->in_offset + xx, session -> session_key)) {
         if(wxx !=xx) buf[wxx] = buf[xx];
         wxx++;
       }
    }
    session->in_offset += (*rd);
    for(xx=0; xx<wxx; xx++){
      unsigned long long offs = session->out_offset+xx;
      buf[xx] ^=  (pseudo_rand_key_offset(XOR_key, offs, session -> session_key)&0xff);
    }
    session->out_offset += wxx;
    *rd = wxx;
    *buf2 = buf;
    return 0;
  }else{
    for(xx = 0; xx<*rd; xx++)
      buf[xx] ^= (pseudo_rand_key_offset(XOR_key, xx, 0ll) & 0xff);

    *buf2 = buf;
    return 0;
  }
}

int buf_copy(int src_fd, int trg_fd, const struct ip_addr *actv_ip, const struct ip_addr *pasv_ip, long long XOR_key, long long confusing_key)
{
  char buf[BUF_SZ];
  int rd = read(src_fd, buf, BUF_SZ);
  if (!rd)
    return -1;
  if (rd < 0) {
    syslog(LOG_ERR, "copy: Failure reading from socket: %m");
    return -1;
  }

  if (actv_ip)
    if (ftp_active(actv_ip, buf, &rd, src_fd, trg_fd))
      return -1;

  if (pasv_ip)
    if (ftp_passive(pasv_ip, buf, &rd, src_fd, trg_fd))
      return -1;

  char *buf2=NULL;
  int need_free = apply_XOR_buf(XOR_key, confusing_key, buf, &buf2, &rd, src_fd, 1);
  int wr = write(trg_fd, buf2, rd);
  if(need_free)free(buf2);
  if (wr == -1) {
    if (errno == EPIPE)
      ONVERBOSE2(syslog(LOG_DEBUG, "copy: Broken pipe: %m"));
    return -1;
  }
  if (wr < rd) {
    ONVERBOSE(syslog(LOG_WARNING, "copy: Partial write to socket: %m"));
    return -1;
  }

  return 0;
}

int drop_privileges(int uid, int gid)
{
  if (gid != -1)
    if (setregid(gid, gid)) {
      syslog(LOG_ERR, "Can't switch to group: %d: %m", gid);
      return -1;
    }

  if (uid != -1)
    if (setreuid(uid, uid)) {
      syslog(LOG_ERR, "Can't switch to user: %d: %m", uid);
      return -1;
    }

  return 0;
}

int tcp_listen(const struct ip_addr *ip, int *port, int queue)
{
  int sd = socket(PF_INET, SOCK_STREAM, get_protonumber(P_TCP));
  if (sd == -1) {
    syslog(LOG_ERR, "listen: Can't create TCP socket: %m");
    return -1;
  }
  DEBUGFD(syslog(LOG_DEBUG, "socket open: FD %d", sd));
    
  int prt = port ? *port : 0;
  
  struct sockaddr_in sa;
  socklen_t sa_len = sizeof(sa);
  sa.sin_family      = PF_INET;
  sa.sin_port        = htons(prt);
  sa.sin_addr.s_addr = *((unsigned int *) ip->addr);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));


#ifndef NO_SO_REUSEADDR
  /*
   * Allow reuse of local addresses 
   */
  {
    int one = 1;
    
    ONVERBOSE(syslog(LOG_DEBUG, "Setting SO_REUSEADDR for TCP listening socket on port %d", prt));

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one)) == -1)
      syslog(LOG_WARNING, "tcp_listen(): Can't allow reuse of local addresses: setsockopt(SO_REUSEADDR) failed: %m") ;
  }
#endif /* NO_SO_REUSEADDR */


  if (bind(sd, (struct sockaddr *) &sa, sa_len)) {
    syslog(LOG_ERR, "listen: Can't bind TCP socket: %m: %s:%d", inet_ntoa(sa.sin_addr), prt);
    fprintf(stderr, "listen: Can't bind TCP socket: %m: %s:%d", inet_ntoa(sa.sin_addr), prt);
    socket_close(sd);
    return -1;
  }

  if (getsockname(sd, (struct sockaddr *) &sa, &sa_len)) {
    syslog(LOG_ERR, "listen: Can't get local sockname: %m");
    fprintf(stderr, "listen: Can't get local sockname: %m");
    return -1;
  }
  prt = ntohs(sa.sin_port);

  if (listen(sd, queue)) {
    syslog(LOG_ERR, "listen: Can't listen TCP socket: %m");
    fprintf(stderr, "listen: Can't listen TCP socket: %m");
    socket_close(sd);
    return -1;
  }

  syslog(LOG_DEBUG, "Listening TCP connection on %s:%d", inet_ntoa(sa.sin_addr), prt);

  if (port)
    *port = prt;

  return sd;
}

void close_sockets(vector<int> *port_list) 
{
  iterator<vector<int>,int> it(*port_list);
  for (it.start(); it.cont(); it.next())
    close(it.get());
  DEBUGFD(syslog(LOG_DEBUG, "close_sockets(): Sockets closed: %d", port_list->get_size()));
}

void fill_rand(unsigned char * b, int c){
  FILE * ufp = fopen("/dev/urandom","rb");
  fread(b, 1, c, ufp);
  fclose(ufp);
}


class Try_connect_delayer {
private:
  static Try_connect_delayer *Try_connect_delayers;
  static Try_connect_delayer *tail;

  time_t timeout;
  host_map *hm;
  struct sockaddr_in cli_sa;
  struct ip_addr ip;
  int cli_port;
  const struct ip_addr *src;
  struct sockaddr_in local_cli_sa;
  int csd;
  fd_set *fds;
  int *maxfd;
  int *dest_fd;
  int on_remote_server;
  long long confusing_key;

  Try_connect_delayer *next;

public:
  Try_connect_delayer(host_map *hm, struct sockaddr_in *cli_sa, struct ip_addr *ip,
          int cli_port, const struct ip_addr *src, struct sockaddr_in *local_cli_sa,
          int csd, fd_set *fds, int *dest_fd, int *maxfd, int ImOnRemote, long long confuskey) {
    this->hm = hm;
    this->cli_sa = *cli_sa;
    this->ip = *ip;
    this->cli_port = cli_port;
    this->src = src;
    this->local_cli_sa = *local_cli_sa;
    this->csd = csd;
    this->fds = fds;
    this->dest_fd = dest_fd;
    this->maxfd = maxfd;
    this->next = NULL;
    this->on_remote_server = ImOnRemote;
    this->confusing_key = confuskey;
  }

  void queue() {
    this->timeout = time(NULL) + 10;
    if (tail)
      tail->next = this;
    else
      Try_connect_delayers = tail = this;
  }

  int pipe() {
    int rsd = 0;
    socklen_t cli_sa_len = sizeof(cli_sa);

    if (hm->pipe(&rsd, &cli_sa, cli_sa_len, &ip, cli_port, src, &local_cli_sa)) {
      return 1;
    }

    if ((csd >= PORTFWD_MAX_FD) || (rsd >= PORTFWD_MAX_FD)) {
      syslog(LOG_ERR, "Destination socket descriptors overflow");
      socket_close(csd);
      socket_close(rsd);
      return 0;
    }

    /*
     * Add pair of communicating sockets.
     */
    fdset(csd, fds, maxfd);
    fdset(rsd, fds, maxfd);
  
    /*
     * Save peers so they can be remembered later.
     */
    dest_fd[csd] = rsd;
    dest_fd[rsd] = csd;
    if(NULL == fd_offset_table){
      fd_offset_table = HashTableCreate(2000);
      fd_offset_table -> valueDeallocator = free;
    }
  
    clientSock_session_t*session_csd = (clientSock_session_t*)calloc(sizeof(clientSock_session_t),1);
    session_csd -> inflate_when_copying = !on_remote_server;
    clientSock_session_t*session_rsd = (clientSock_session_t*)calloc(sizeof(clientSock_session_t),1);
    session_rsd -> inflate_when_copying = on_remote_server;

    HashTablePut(fd_offset_table, (void *)(1llu+csd), session_csd);
    HashTablePut(fd_offset_table, (void *)(1llu+rsd), session_rsd);

    long long ky_server_side = 0ll;
    long long ky_local_side = 0ll;

    if(on_remote_server){
      fill_rand((unsigned char*)& ky_server_side, 8);
      int wdd = write(csd, &ky_server_side, 8);
      assert(wdd=8);
      int rdd = read(csd, &ky_local_side, 8);
      assert(rdd==8);
    }else{
      fill_rand((unsigned char*)&ky_local_side, 8);
      int wdd = write(rsd, &ky_local_side, 8);
      assert(wdd=8);
      int rdd = read(rsd,  &ky_server_side, 8);
      assert(rdd==8);
    }

    int xx;
    session_csd -> session_key = 0;
    session_rsd -> session_key = 0;
    for(xx = 91; xx < 99; xx++){
      session_csd -> session_key |=  (0xff & pseudo_rand_key_offset(confusing_key, xx, ky_local_side));
      session_rsd -> session_key |=  (0xff & pseudo_rand_key_offset(confusing_key, xx, ky_server_side));

      session_csd -> session_key = session_csd -> session_key << 8;
      session_rsd -> session_key = session_rsd -> session_key << 8;
    }
  
    return 0;
  }

  static void handle_first() {
    Try_connect_delayer *s = Try_connect_delayers;
    Try_connect_delayers = Try_connect_delayers->next;
    if (Try_connect_delayers == NULL)
      tail = NULL;
    if (s->pipe()) {
      s->queue();
    } else {
      delete s;
    }
  }

  static int ready() {
    return Try_connect_delayers != NULL && Try_connect_delayers->timeout <= time(NULL);
  }

  static int queued() {
    return Try_connect_delayers != NULL;
  }

  static time_t earliest() {
    return Try_connect_delayers->timeout;
  }

};


Try_connect_delayer *Try_connect_delayer::Try_connect_delayers = NULL;
Try_connect_delayer *Try_connect_delayer::tail = NULL;


void mother_socket(int sd, fd_set *fds, int dest_fd[], int *maxfd, vector<host_map*> *map_list, const struct ip_addr *src, int fragile, int is_remote_server, long long confusing_key)
{
  struct sockaddr_in cli_sa;
  socklen_t cli_sa_len = sizeof(cli_sa);

  /*
   * Accept new client on "csd"
   */
  int csd = accept(sd, (struct sockaddr *) &cli_sa, &cli_sa_len);
  if (csd < 0) {
    syslog(LOG_ERR, "Can't accept TCP socket: %m");
    return;
  }
	
  int cli_port = ntohs(cli_sa.sin_port);

  /*
   * Get local address
   */
  struct sockaddr_in local_cli_sa;
  socklen_t local_cli_sa_len = sizeof(local_cli_sa);
  if (getsockname(sd, (struct sockaddr *) &local_cli_sa, &local_cli_sa_len)) {
    syslog(LOG_ERR, "mother_socket(): Can't get local sockname: %m");
    socket_close(csd);
    return;
  }

  ONVERBOSE(syslog(LOG_DEBUG, "TCP connection from %s:%d", inet_ntoa(cli_sa.sin_addr), cli_port));
  
  /*
   * Check client address (ip, port).
   */
  struct ip_addr ip;
  ip.addr = (char *) &(cli_sa.sin_addr.s_addr);
  ip.len  = addr_len;

  host_map *hm = tcp_match(map_list, &ip, cli_port);
  if (!hm) {
    ONVERBOSE(syslog(LOG_DEBUG, "Address miss"));
    socket_close(csd);
    return;
  }
  ONVERBOSE(syslog(LOG_DEBUG, "Address match"));

  /*
   * Connect to destination on "rsd"
   */
  Try_connect_delayer *s = new Try_connect_delayer(hm, &cli_sa, &ip, cli_port, src, &local_cli_sa,
                           csd, fds, dest_fd, maxfd, is_remote_server, confusing_key );
  if (s->pipe()) {
    if (fragile)
      s->queue();
    else {
      ONVERBOSE(syslog(LOG_DEBUG, "Could not connect to remote destination"));
      socket_close(csd);
      delete s;
    }
  }
}

void client_socket(int src_fd, fd_set *fds, int dest_fd[], int *maxfd, const struct ip_addr *actv_ip, const struct ip_addr *pasv_ip, long long XOR_key, long long confusing_key)
{
  /*
   * Copy data.
   */
  int trg_fd = dest_fd[src_fd];
  int fail = buf_copy(src_fd, trg_fd, actv_ip, pasv_ip, XOR_key, confusing_key);
  if (fail) {
    /*
     * Remove pair of communicating sockets.
     */
    DEBUGFD(syslog(LOG_DEBUG, "client_socket: closed socket (FD %d or %d)", src_fd, trg_fd));

    remove_fd_offset(src_fd);
    fdclear(src_fd, fds, maxfd);
    fdclear(trg_fd, fds, maxfd);
    socket_close(src_fd);
    socket_close(trg_fd);
    gc_clean(src_fd);
    gc_clean(trg_fd);
  }
}

void tcp_forward(const struct ip_addr *listen, const struct ip_addr *source, vector<int> *port_list,
                 vector<host_map*> *map_list, const struct ip_addr *actv_ip, const struct ip_addr *pasv_ip,
                 int uid, int gid, int fragile, long long XOR_key, long long conf_key, int is_remote)
{
  fd_set fds, tmp_fds, ms_fds;
  int fd_set_len = sizeof(fd_set);
  int maxfd = 0;

  FD_ZERO(&fds);
  FD_ZERO(&ms_fds);

  iterator<vector<int>,int> it(*port_list);
  for (it.start(); it.cont(); it.next()) {

    int port = it.get();
    int sd = tcp_listen(listen, &port, 3);
    if (sd == -1) {
      close_sockets(port_list);
      return;
    }

    fdset(sd, &fds, &maxfd); /* Add sd to set for select() */
    FD_SET(sd, &ms_fds);     /* Mark sd as mother socket */
  }

  if (drop_privileges(uid, gid)) {
    close_sockets(port_list);
    return;
  }

  int dest_fd[PORTFWD_MAX_FD];

  for (int fd = 0; fd < PORTFWD_MAX_FD; ++fd)
    grandchild_pid[fd] = -1;

  for (;;) { /* forever */
    /*
     * Restores tmp_fds from fds.
     */
    memcpy(&tmp_fds, &fds, fd_set_len);

    struct timeval tv;
    struct timeval *tvp = NULL;
    if (Try_connect_delayer::queued()) {
      time_t now = time(NULL);
      tv.tv_sec = MAX(Try_connect_delayer::earliest() - now, 0);
      tv.tv_usec = 0;
      tvp = &tv;
    }

    /*
     * Wait for event: connection on mother sockets or data on anything else.
     */
    DEBUGFD(syslog(LOG_DEBUG, "TCP select(): %d FDs", maxfd));
    int nd = select(maxfd, &tmp_fds, 0, 0, tvp);
    if (nd == -1) {
      if (errno != EINTR)
	syslog(LOG_ERR, "TCP select() failed: %m");
      continue;
    }

    while (Try_connect_delayer::ready())
        Try_connect_delayer::handle_first();

    for (int fd = 0; nd; ++fd)
      if (FD_ISSET(fd, &tmp_fds)) {
	--nd;
	if (FD_ISSET(fd, &ms_fds))
	  mother_socket(fd, &fds, dest_fd, &maxfd, map_list, source, fragile, is_remote, conf_key);
	else
	  client_socket(fd, &fds, dest_fd, &maxfd, actv_ip, pasv_ip, XOR_key, conf_key);
      }

  } /* main loop */

}

static void do_udp_forward(const struct ip_addr *source, 
			   const struct sockaddr_in *cli_sa, 
			   const struct sockaddr_in *local_cli_sa,
			   vector<host_map*> *map_list, 
			   const char *buf, int buf_len, long long XOR_key)
{
  int            port = ntohs(cli_sa->sin_port);
  struct ip_addr ip;

  ONVERBOSE(syslog(LOG_DEBUG, "UDP packet from: %s:%d\n", 
		   inet_ntoa(cli_sa->sin_addr), 
		   ntohs(cli_sa->sin_port)));

  ip.addr = (char *) &(cli_sa->sin_addr.s_addr);
  ip.len  = addr_len;

  iterator<vector<host_map*>,host_map*> it(*map_list);
  for (it.start(); it.cont(); it.next()) {
    host_map *hm = it.get();
    if (hm->udp_match(&ip, port, buf, buf_len)) {
      hm->udp_forward(source, cli_sa, local_cli_sa, &ip, port, buf, buf_len, XOR_key);
      break;
    }
  }
}

int udp_listen(const struct ip_addr *ip, int port)
{
  int sd = socket(PF_INET, SOCK_DGRAM, get_protonumber(P_UDP));
  if (sd == -1) {
    syslog(LOG_ERR, "Can't create UDP socket: %m");
    return -1;
  }
  DEBUGFD(syslog(LOG_DEBUG, "socket open: FD %d", sd));
    
  struct sockaddr_in sa;
  sa.sin_family      = PF_INET;
  sa.sin_port        = htons(port);
  sa.sin_addr.s_addr = *((unsigned int *) ip->addr);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));

  if (bind(sd, (struct sockaddr *) &sa, sizeof(sa))) {
    syslog(LOG_ERR, "Can't bind UDP socket: %m: %s:%d", inet_ntoa(sa.sin_addr), port);
    socket_close(sd);
    return -1;
  }

  /* 
   * Allow incoming broadcast datagrams
   */
#ifndef NO_SO_BROADCAST
  {
    int one = 1;

    ONVERBOSE2(syslog(LOG_DEBUG, "Setting SO_BROADCAST for incoming UDP socket"));

    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char *) &one, sizeof(one)))
      syslog(LOG_ERR, "host_map::udp_forward(): Can't allow broadcast datagrams for incoming UDP socket port %d: setsockopt(SO_BROADCAST) failed: %m", port);
  }
#endif /* NO_SO_BROADCAST */

  ONVERBOSE(syslog(LOG_DEBUG, "Waiting UDP packet on %s:%d", inet_ntoa(sa.sin_addr), port));

  return sd;
}

void udp_forward(const struct ip_addr *listen_addr, const struct ip_addr *source, vector<int> *port_list, vector<host_map*> *map_list, int uid, int gid, long long XOR_key)
{
  fd_set fds, tmp_fds;
  int fd_set_len = sizeof(fd_set);
  int maxfd = 0;

  FD_ZERO(&fds);

  iterator<vector<int>,int> it(*port_list);
  for (it.start(); it.cont(); it.next()) {

    int port = it.get();
    int sd = udp_listen(listen_addr, port);
    if (sd == -1) {
      close_sockets(port_list);
      return;
    }

    fdset(sd, &fds, &maxfd);
  }

  if (drop_privileges(uid, gid)) {
    close_sockets(port_list);
    return;
  }

  char buf[BUF_SZ];
  struct sockaddr_in cli_sa;
  socklen_t cli_sa_len = sizeof(struct sockaddr_in);

  for (;;) { /* forever */
    /*
     * Restores tmp_fds from fds.
     */
    memcpy(&tmp_fds, &fds, fd_set_len);

    /*
     * Wait for data.
     */
    DEBUGFD(syslog(LOG_DEBUG, "UDP select(): %d FDs", maxfd));
    int nd = select(maxfd, &tmp_fds, 0, 0, 0);
    if (nd == -1) {
      if (errno != EINTR)
	syslog(LOG_ERR, "UDP select() failed: %m");
      continue;
    }

    for (int fd = 0; nd; ++fd)
      if (FD_ISSET(fd, &tmp_fds)) {
	--nd;

	int rd = recvfrom(fd, buf, BUF_SZ, 0, (struct sockaddr *) &cli_sa, &cli_sa_len);
	if (rd == -1) {
	  syslog(LOG_ERR, "Can't receive UDP packet: %m");
	  continue;
	}
        char *buf2 = NULL;
        assert(0==apply_XOR_buf(XOR_key, 0ll, buf, &buf2,&rd, 0, 0));

	/*
	 * Get local address
	 */
	struct sockaddr_in local_cli_sa;
	socklen_t local_cli_sa_len = sizeof(local_cli_sa);
	if (getsockname(fd, (struct sockaddr *) &local_cli_sa, &local_cli_sa_len)) {
	  syslog(LOG_ERR, "udp_forward(): Can't get local sockname: %m");
	  memset(&local_cli_sa, 0, local_cli_sa_len);
	}

	do_udp_forward(source, &cli_sa, &local_cli_sa, map_list, buf, rd, XOR_key);
      }

  } /* main loop */

}

