cp1 : cp1.o
	gcc -o cp1 cp1.o

who3 : who3.o utmplib.o
	gcc -o who3 who3.o utmplib.o

mywho : mywho.o utmplib.o
	gcc -o mywho mywho.o utmplib.o

utmplib : utmplib.o utmplib.h
	gcc -o utmplib utmplib.o utmplib.h

cp1.o : cp1.c
	gcc -c cp1.c

who3.o : who3.c
	gcc -c who3.c

utmplib.o: utmplib.c
	gcc -c utmplib.c

mywho.o : mywho.c
	gcc -c mywho.c
