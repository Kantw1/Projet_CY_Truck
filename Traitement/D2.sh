#Si fonctionne pas, penser à dos2unix Traitement/D2.sh
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_2.txt"
awk -F ';' '{ distances[$6] += $5 } END { for (driver in distances) print distances[driver],driver }' $fichier_trajets | sort -k2,2nr | head -n 10 | sort -k1,1n > $fichier_resultats

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 400
set output 'Image/histogramme_horizontal_2.png'
set title 'Histogramme horizontal 2 des données'
set xlabel 'Distance totale parcourue'
set ylabel 'Nom des conducteurs'
set style fill solid
set xtics nomirror
set ytics nomirror
set yrange [0:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.3):yticlabels(3) with boxxyerrorbars lc rgbcolor 'blue' ti "Histogramme Horizontal"
EOF
