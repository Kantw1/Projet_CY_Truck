#!/bin/bash
# Création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats2="Temp/Resultat_s2.txt"
fichier_resultats="Temp/Resultat_s.txt"

cut -d';' -f1,5 "$fichier_trajets" | tail -n +2  | sort -t';' -k1,1n | sed 's/;/ /g'> $fichier_resultats
./Progc/prog_s
sed -i '1d' $fichier_resultats2
#max_value=$(awk -F' ' 'NR==1 {max=$4} $4>max {max=$4} END {print max}' "$fichier_resultats2")
#max_value2=$(awk 'BEGIN {rounded = int((ENVIRON["max_value"] + 99) / 100) * 100; printf "%.0f\n", rounded}')


# Utilisation de gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 1200, 600
set output 'Image/histogramme_horizontal_5.png'
set title "Option -S : Distance = f(Route)"
set ylabel 'DISTANCE (km)'
set yrange [0:]
set xtics rotate by 45 right #offset -3,-1.5
set bmargin 3

plot '$fichier_resultats2' using 0:(\$2):(\$4) with filledcurve lc rgbcolor 'spring-green' title "Distance Max/Min (Km)",\
    ''using 3:xtic(1) smooth mcspline lc rgbcolor 'dark-green' title "Distance average (Km)"
EOF

chmod 777 Image/histogramme_horizontal_5.png
xdg-open Image/histogramme_horizontal_5.png
