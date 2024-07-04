CFLAGS := -g -Wall
OUT := main
OUTDIR = build

clean: 
	rm main

main: main.cc
	g++ $? -o ${OUTDIR}/main.o -c 
threading: threading.cc
	g++ $? -o ${OUTDIR}/threading.o -c 

config: config.c
	gcc $? -o ${OUTDIR}/config.o -c 

all: main threading config
	g++ ${OUTDIR}/config.o ${OUTDIR}/main.o ${OUTDIR}/threading.o -o main