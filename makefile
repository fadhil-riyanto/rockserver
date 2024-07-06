CFLAGS := -g -Wall 
OUT := main
OUTDIR = build

LOGC_FLAGS = 

clean: 
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

logc: submodule/log.c/src/log.c
	gcc submodule/log.c/src/log.c -o build/logc.o ${LOGC_FLAGS} -c

all: main threading config server conn epoll_watcher logc
	g++ ${OUTDIR}/config.o ${OUTDIR}/main.o ${OUTDIR}/threading.o ${OUTDIR}/server.o \
	${OUTDIR}/connection_handler.o ${OUTDIR}/epoll_watcher.o ${OUTDIR}/logc.o \
	-o main