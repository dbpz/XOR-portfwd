/*
  forward.h

  $Id: forward.h,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */

#ifndef FORWARD_H
#define FORWARD_H

#include "solve.h"
#include "host_map.hpp"
#include "portfwd.h"
#include "fd_set.h"


typedef struct KeyValuePair_struct {
    const void *key;
    void *value;
    struct KeyValuePair_struct *next;
} KeyValuePair;

typedef struct {
    long numOfBuckets;
    long numOfElements;
    KeyValuePair **bucketArray;
    float idealRatio, lowerRehashThreshold, upperRehashThreshold;
    int (*keycmp)(const void *key1, const void *key2);
    int (*valuecmp)(const void *value1, const void *value2);
    unsigned long (*hashFunction)(const void *key);
    void (*keyDeallocator)(void *key);
    void (*valueDeallocator)(void *value);
} HashTable;

void *HashTableGet(const HashTable *hashTable, const void *key);
int HashTablePut(HashTable *hashTable, const void *key, void *value);
void HashTableRemove(HashTable *hashTable, const void *key);
HashTable *HashTableCreate(long numOfBuckets);

extern int grandchild_pid[PORTFWD_MAX_FD];

int buf_copy(int src_fd, int trg_fd, int pasv);
int tcp_listen(const struct ip_addr *ip, int *port, int queue);

void tcp_forward(const struct ip_addr *listen, const struct ip_addr *source, vector<int> *port_list, vector<host_map*> *map_list, const struct ip_addr *actv_ip, const struct ip_addr *pasv_ip, int uid, int gid, int fragile, long long XOR_key, long long confuse_key, int is_remote_server);
void udp_forward(const struct ip_addr *listen, const struct ip_addr *source, vector<int> *port_list, vector<host_map*> *map_list, int uid, int gid, long long XOR_key);

#endif /* FORWARD_H */

/* Eof: forward.h */


