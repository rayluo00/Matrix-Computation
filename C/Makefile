
LIBS=-lpthread

matrix: main.o matrix.o rdtsc.o
	$(CC) -o matrix  main.o matrix.o rdtsc.o $(LIBS)

clean:
	-rm matrix main.o matrix.o rdtsc.o *~
