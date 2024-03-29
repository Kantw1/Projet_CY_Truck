#!/bin/bash
# Création des fichiers nécessaires ! 
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_2.txt"

# Tri des données utilisant awk pour additionner les distances faites par routier, et renvoie des 10 plus grandes distances par routier effectuées
awk -F ';' '{ distances[$6] += $5 } END { for (driver in distances) print distances[driver], driver }' $fichier_trajets | sort -k1,1nr | head -n 10 | sort -k1,1n > $fichier_resultats

# Utilisation de gnuplot pour la création du graphe
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_2.png'
set title 'Option -d2 : Distance = f(Driver)'
set xlabel 'DISTANCE (Km)'
set ylabel 'DRIVER NAMES'
set style fill solid
# Mise en place de la plage de yrange sur 10 en plage maximal
set yrange [-1:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF
# Utilisation de chmod pour donner les droits absolues à l'accès au graphe de sortie en png
chmod 777 Image/histogramme_horizontal_2.png
xdg-open Image/histogramme_horizontal_2.png

# Afficher le répertoire où les images ont été sauvegardées
echo "Les images ont été sauvegardées dans le répertoire : /Image"
