Droit :
	chmod 777 Script.sh
	chmod 777 Traitement/D1.sh
	chmod 777 Traitement/D2.sh
	chmod 777 Traitement/L.sh
	chmod 777 Traitement/S.sh
	chmod 777 Traitement/T.sh

Clean :
	rm -f Temp/*

D1:
	./Script.sh D1

D2: 
	./Script.sh D2

T:
	./Script.sh T

S:
	./Script.sh S

L:
	./Script.sh L

Help:	
	./Script.sh -h
