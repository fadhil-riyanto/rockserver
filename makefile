CFLAGS := -g -Wall 
OUT := main
OUTDIR = build

LOGC_FLAGS = -DLOG_USE_COLOR

clean: 
	rm build/*.o 
	rm build/*.a
	rm main

epoll_watcher: src/epoll_watcher.cc
	g++ $? -o ${OUTDIR}/epoll_watcher.o ${CFLAGS} -c 

main: src/main.cc
	g++ $? -o ${OUTDIR}/main.o ${CFLAGS} -c 

threading: src/threading.cc
	g++ $? -o ${OUTDIR}/threading.o ${CFLAGS} -c 

server: src/server.cc
	g++ $? -o ${OUTDIR}/server.o ${CFLAGS} -c 

config: src/config.c
	gcc $? -o ${OUTDIR}/config.o ${CFLAGS} -c 

conn: src/connection_handler.cc
	g++ $? -o ${OUTDIR}/connection_handler.o ${CFLAGS} -c 

logc: submodule/log.c-patched/src/log.cc
	g++ $? -o ${OUTDIR}/logc.o ${LOGC_FLAGS} -c

all: threading config server conn epoll_watcher logc
	g++ src/main.cc ${OUTDIR}/config.o ${OUTDIR}/threading.o ${OUTDIR}/server.o \
	${OUTDIR}/connection_handler.o ${OUTDIR}/epoll_watcher.o \
	${OUTDIR}/logc.o \
	-o main