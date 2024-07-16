
# introduction
<a href="https://github.com/facebook/rocksdb">RocksDB</a> is an embedded database. This program is a server ported version using the RocksDB Facebook backend.

## build
1. get the source code
```sh
git clone https://github.com/fadhil-riyanto/rockserver.git
```
2. clone & build facebook/rocksdb tree
```sh
git clone https://github.com/facebook/rocksdb.git
cd rocksdb
make all
```

note: This will compile RocksDB in debug mode. Don't use binaries compiled by make all in production.

3. back to rockserver source, run
```sh
make all
```

## Default
default port are: `8998` on tcp
listen: `127.0.0.1`

## Operating system compatibility
not yet tested on windows. tested on Archlinux, untested on other distros

## maintainer
<a href="https://github.com/fadhil-riyanto">@fadhil-riyanto</a>