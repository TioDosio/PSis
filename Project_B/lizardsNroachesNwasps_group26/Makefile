all:
	make lizard
	make server 
	make roach

lizard:
	make -C Lizard-client -f Makefile all
server:
	make -C lizardsNroachesNwasps-server -f Makefile all
roach:
	make -C Roach-client -f Makefile all

	
