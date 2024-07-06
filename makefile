CFLAGS := -g -Wall 
OUT := main
OUTDIR = build

LOGC_FLAGS = -DLOG_USE_COLOR

clean: 
	rm build/*.o 
	rm build/*.a
	rm main

epoll_watcher: epoll_watcher.cc
	g++ $? -o ${OUTDIR}/epoll_watcher.o ${CFLAGS} -c 

main: main.cc
	g++ $? -o ${OUTDIR}/main.o ${CFLAGS} -c 

threading: threading.cc
	g++ $? -o ${OUTDIR}/threading.o ${CFLAGS} -c 

server: server.cc
	g++ $? -o ${OUTDIR}/server.o ${CFLAGS} -c 

config: config.c
	gcc $? -o ${OUTDIR}/config.o ${CFLAGS} -c 

conn: connection_handler.cc
	g++ $? -o ${OUTDIR}/connection_handler.o ${CFLAGS} -c 

logc: submodule/log.c-patched/src/log.cc
	g++ $? -o ${OUTDIR}/logc.o ${LOGC_FLAGS} -c

all: threading config server conn epoll_watcher logc
	g++ main.cc ${OUTDIR}/config.o ${OUTDIR}/threading.o ${OUTDIR}/server.o \
	${OUTDIR}/connection_handler.o ${OUTDIR}/epoll_watcher.o \
	${OUTDIR}/logc.o \
	-o main