all:
	make project
	make lab5

project:
	make clean
	make -C Project -f Makefile all

lab5: 
	make -C Lab5/Lab5files/lab3-exercise-3-solution -f Makefile all

clean:
	make -C Lab5/Lab5files/lab3-exercise-3-solution -f Makefile all clean
	make -C Project -f Makefile clean

