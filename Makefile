all:
	make project-b 

project-a:
	make clean
	make -C Project_A -f Makefile all

project-b:
	make -C Project_B/lizardsNroachesNwasps_group26 -f Makefile all

clean:
	make -C Project_B/lizardsNroachesNwasps_group26 -f Makefile clean

