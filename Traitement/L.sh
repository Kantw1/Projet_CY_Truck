fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt_3"
fichier_fusionne="Temp/fusion.txt_3"

awk -F ';' '{ distances[$1] += $5 } END { for (route_id in distances) print distances[route_id], route_id }' "$fichier_trajets" | sort -k1,1nr | head -n 10 > "$fichier_resultats"

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 400
set output 'Image/histogramme_horizontal_3.png'
set title 'Histogramme horizontal 2 des données'
set xlabel 'Distance du trajet'
set ylabel 'Route ID'
set style fill solid
set xtics nomirror
set ytics nomirror
set yrange [0:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.3):yticlabels(3) with boxxyerrorbars lc rgbcolor 'blue' ti "Histogramme Horizontal"
EOF
