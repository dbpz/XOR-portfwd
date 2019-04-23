/*
  $Id: fd_set.h,v 1.1.1.1 2019/04/06 10:36:07 cvs Exp $
 */


#ifndef FD_SET_H
#define FD_SET_H


/*
 * FD_SETSIZE is usually defined as 1024.
 * You may want to force a greater value here.
 */

#ifndef FD_SETSIZE
#define FD_SETSIZE 8988
#endif

#define PORTFWD_MAX_FD FD_SETSIZE


void close_fds(int first_fds);
void fdset(int fd, fd_set *fds, int *maxfd);
void fdclear(int fd, fd_set *fds, int *maxfd);


#endif /* FD_SET_H */


/* eof */
