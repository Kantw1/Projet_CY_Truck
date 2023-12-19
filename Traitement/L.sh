fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt"
fichier_fusionne="Temp/fusion.txt"

cat "$fichier_trajets" | cut -d';' -f1,5 | sort | awk -F ';' 'NR>1 { sum[$1]+=$2 } END { for (RouteID in sum) print RouteID " " sum[RouteID] }' | sort -nr | head -n 10 | sort -k2,1n > "$fichier_resultats"

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 400
set output 'Image/histogramme_horizontal.png'
set title 'Histogramme horizontal des données'
set xlabel 'Distance du trajet'
set ylabel 'ID Trajet'
set style fill solid
set xtics nomirror
set ytics nomirror
set yrange [0:10]

plot '$fichier_resultats' using 2:0:yticlabels(1) with boxxyerrorbars lc rgbcolor 'blue' ti "Histogramme Horizontal"
EOF
