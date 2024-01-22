fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_3.txt"

# Tri effectuant le traitement L, utilisant awk pour faire une somme des distances des id trajets.
awk -F ';' '{ distances[$1] += $5 } END { for (route_id in distances) print distances[route_id], route_id }' "$fichier_trajets" | sort -k1,1nr | head -n 10 | sort -k2,2nr> "$fichier_resultats"

# Utilisation de gnuplot pour la création du graphe
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_3.png'
set title 'Option -l : Distance = f(Route)'
set ylabel 'DISTANCE (Km)'
set xlabel 'ROUTE ID'
set style fill solid
# La plage maximale de yrange est ici indéfinie, elle ajustera en fonction des données
set yrange [0:]
set boxwidth 0.7

plot '$fichier_resultats' using 1:xtic(2) with boxes lc rgbcolor 'spring-green' notitle
EOF
chmod 777 Image/histogramme_horizontal_3.png
xdg-open Image/histogramme_horizontal_3.png
