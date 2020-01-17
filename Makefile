all: clean proc

proc: main
	gcc -O0 -g2 main.o -o proc -L. -lwiringPi -lcs5463 -lipc -lcjson

main: cs5463 ipc
	gcc -I. -O0 -g2  main.c -c -o main.o -L. #-L. -lcs5463

cs5463:
	gcc -shared -I. -O0 -g2 -fpic cs5463.c -o libcs5463.so

ipc:
	gcc -shared -I. -O0 -g2 -fpic ipc.c -o libipc.so

clean:
	rm -f *.o
	rm -f test
	rm -f *.so
