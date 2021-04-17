all : exec

exec : ccmpl asmcmpl linkAll

ccmpl : main.c util.c
	gcc -g -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -g -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

asmcmpl : start.s
	nasm -f elf32 start.s -o start.o

linkAll : main.o util.o start.o
	ld -m elf_i386 start.o main.o util.o -o encoder

.PHONY: clean
clean:
	rm -rf ./*.o encoder
