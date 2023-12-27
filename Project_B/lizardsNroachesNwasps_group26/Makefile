all:
	make clean 
	make lizard
	make server 
	make roach 

lizard:
	make -C Lizard-client -f Makefile all
server:
	make -C lizardsNroachesNwasps-server -f Makefile all
roach:
	make -C Roach-client -f Makefile all

clean:
	make -C lizard -f Makefile clean
	make -C server -f Makefile clean
	make -C roach -f Makefile clean
	
