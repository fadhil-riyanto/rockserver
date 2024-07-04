CFLAGS := -g -Wall
OUT := main
OUTDIR = build

clean: 
	rm main

main: main.cc
	g++ $? -o ${OUTDIR}/main.o ${CFLAGS} -c 

threading: threading.cc
	g++ $? -o ${OUTDIR}/threading.o ${CFLAGS} -c 

server: server.cc
	g++ $? -o ${OUTDIR}/server.o ${CFLAGS} -c 

config: config.c
	gcc $? -o ${OUTDIR}/config.o ${CFLAGS} -c 

all: main threading config server
	g++ ${OUTDIR}/config.o ${OUTDIR}/main.o ${OUTDIR}/threading.o ${OUTDIR}/server.o -o main