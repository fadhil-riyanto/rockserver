#ifndef ROCKSDB_CTX
#define ROCKSDB_CTX

#include "rocksdb/c.h"
#include "rocksdb/db.h"

struct rocksdb_ctx {
        rocksdb::DB* db;
        rocksdb::Options options;
};

#endif