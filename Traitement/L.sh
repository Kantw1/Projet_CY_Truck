fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_3.txt"

awk -F ';' '{ distances[$1] += $5 } END { for (route_id in distances) print distances[route_id], route_id }' "$fichier_trajets" | sort -k1,1nr | head -n 10 | sort -k2,2nr> "$fichier_resultats"

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_3.png'
set title 'Option -l : Distance = f(Route)'
set ylabel 'DISTANCE (Km)'
set xlabel 'ROUTE ID'
set style fill solid
#set xtics nomirror #je crois qu'il y a pas besoin de nomirror
#set ytics nomirror
set yrange [0:3000]
set boxwidth 0.7

plot '$fichier_resultats' using 1:xtic(2) with boxes lc rgbcolor 'spring-green' notitle
EOF
xdg-open Image/histogramme_horizontal_3.png