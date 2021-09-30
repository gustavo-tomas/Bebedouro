VPATH = ./src:./include

run: main.o student.o dead.o
	gcc main.o student.o dead.o -pthread -o bebedouro; 
	./bebedouro ;
	rm *.o ; 
	rm ./bebedouro

compile: main.o student.o dead.o 
	gcc main.o student.o dead.o -pthread -o bebedouro 

clean:
	rm *.o ; rm ./bebedouro

main.o: main.c
	gcc -c src/main.c

student.o: student.c student.h includes.h
	gcc -c src/student.c

dead.o: dead.c dead.h includes.h
	gcc -c src/dead.c