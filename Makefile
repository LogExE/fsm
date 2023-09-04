CC = gcc
CFLAGS = -g

objects = fda.o main.o
all: app

app: $(objects)
	cc -o app $(objects)

clean:
	rm -f *.o

$(objects): %.o: %.c