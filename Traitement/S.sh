#!/bin/bash
# Création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats_test="Data/data_s.txt"

# Lancer le fichier .c

# Utilisation de gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_5.png'
set title "Option -S : Distance = f(Route)"
set ylabel 'DISTANCE (km)'
#set xtics nomirror #je crois qu'il y a pas besoin de nomirror
#set ytics nomirror
set yrange [0:1000]

# Utilisez le bon nom de fichier 'Data/data_s.txt'
plot '$fichier_resultats_test' using 2:xtic(1) lc rgbcolor 'spring-green' with boxes notitle
EOF