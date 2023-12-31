#création de deux fichiers resultats et trajets
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_2.txt"

#calcul des 10 plus grandes distance totale
awk -F ';' '{ distances[$6] += $5 } END { for (driver in distances) print distances[driver], driver }' $fichier_trajets | sort -k1,1nr | head -n 10 | sort -k1,1n > $fichier_resultats

#utilisation de gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_2.png'
set title 'Option -d2 : Distance = f(Driver)'
set xlabel 'DISTANCE (Km)'
set ylabel 'DRIVER NAMES'
set style fill solid
#set xtics nomirror #je crois qu'il y a pas besoin de nomirror
#set ytics nomirror
set yrange [-1:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF
