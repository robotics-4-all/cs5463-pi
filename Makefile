test: main.o
	gcc -O0 -g2  main.o -o test -L. -lwiringPi -lcs5463

main.o: main.c
	gcc -I. -O0 -g2  main.c -c -o main.o #-L. -lcs5463

cs5463:
	gcc -shared -I. -O0 -g2 -fpic cs5463.c -o libcs5463.so

clean:
	rm -f *.o
	rm -f test
	rm -f *.so
