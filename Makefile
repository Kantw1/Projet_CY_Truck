Droit :
	chmod 777 Script.sh
	chmod 777 Image/histogramme_horizontal.png
	chmod 777 Image/histogramme_horizontal_2.png
	chmod 777 Image/histogramme_horizontal_3.png
	chmod 777 Image/histogramme_horizontal_4.png
	chmod 777 Image/histogramme_horizontal_5.png

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
