all:clear compile run clean

clear: 
	@clear
compile:
	@gcc main.c -o main.out
d:
	@clear
	@gcc -g -o deb main.c
	@gdb deb
run:
	@./main.out 
clean:
	@rm main.out