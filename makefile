CFLAGS := -g -Wall 
OUT := main
OUTDIR = build
LIBCFLAGS = -c
LINKERFLAGS = -lrocksdb

LOGC_FLAGS = -DLOG_USE_COLOR

UTILS_FILE = src/utils/req.cc src/utils/string.cc

clean: 
	rm build/*.o 
	rm build/*.a
	rm main

epoll_watcher: src/epoll_watcher.cc
	g++ $? -o ${OUTDIR}/epoll_watcher.o ${CFLAGS} ${LIBCFLAGS}

main: src/main.cc
	g++ $? -o ${OUTDIR}/main.o ${CFLAGS} ${LIBCFLAGS}

threading: src/threading.cc
	g++ $? -o ${OUTDIR}/threading.o ${CFLAGS} ${LIBCFLAGS}

server: src/server.cc
	g++ $? -o ${OUTDIR}/server.o ${CFLAGS} ${LIBCFLAGS}

config: src/config.c
	gcc $? -o ${OUTDIR}/config.o ${CFLAGS} ${LIBCFLAGS}

conn: src/connection_handler.cc
	g++ $? -o ${OUTDIR}/connection_handler.o ${CFLAGS} ${LIBCFLAGS}

logc: submodule/log.c-patched/src/log.cc
	g++ $? -o ${OUTDIR}/logc.o ${LOGC_FLAGS} ${CFLAGS} ${LIBCFLAGS}

inih: submodule/inih/ini.c
	gcc $? -o ${OUTDIR}/inih.o ${LOGC_FLAGS} ${CFLAGS} ${LIBCFLAGS}

inih_parser: src/inih_parser.c
	g++ $? -o ${OUTDIR}/inih_parser.o ${LOGC_FLAGS} ${CFLAGS} ${LIBCFLAGS}

utils: ${UTILS_FILE}
	g++ src/utils/req.cc -o ${OUTDIR}/utils-req.o ${CFLAGS} ${LIBCFLAGS}
	g++ src/utils/string.cc -o ${OUTDIR}/utils-string.o ${CFLAGS} ${LIBCFLAGS}

all: threading config server conn epoll_watcher logc inih utils inih_parser
	g++ src/main.cc ${OUTDIR}/config.o ${OUTDIR}/threading.o ${OUTDIR}/server.o \
	${OUTDIR}/connection_handler.o ${OUTDIR}/epoll_watcher.o 		\
	${OUTDIR}/logc.o ${OUTDIR}/utils-req.o ${OUTDIR}/utils-string.o		\
	${OUTDIR}/inih.o ${OUTDIR}/inih_parser.o				\
	-o main ${CFLAGS} ${LINKERFLAGS}