#ifndef SERVER_H
#define SERVER_H

#include <signal.h>
#include "rocksdb/db.h"

struct server_ctx {
        int *sockfd;
        int *epfd;
};

/**
 * all initialization from server carried by this struct
 */
typedef struct server_state {
        volatile sig_atomic_t *exit_now;

        void *threading_ch_ctx;
        struct rocksdb_ctx *rocksdb_ctx;

        rocksdb::DB* db;

} server_state_t;

int sock_init(int port);
void sock_cleanup(int *sockfd);

#endif

