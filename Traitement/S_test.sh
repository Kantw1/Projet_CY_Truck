#!/bin/bash

# Commande pour trier les données en utilisant le programme C
./avl_sort

# Commandes Gnuplot
gnuplot << EOF
set terminal pngcairo enhanced font 'Arial,12' size 800,600
set output "Image/histogramme_S.png"

# Configuration du graphique
set title "Graphique Min-Max-Moyenne"
set xlabel "Trajets"
set ylabel "Distances (km)"
set style data lines

# Tracer les courbes
plot "graph_data.txt" using 1:2 with lines title "Distance_mini", \
     "graph_data.txt" using 1:3 with lines title "Distance_moyenne", \
     "graph_data.txt" using 1:4 with lines title "Distance_maxi"

EOF

# Message indiquant que le script a terminé
echo "Graphique généré avec succès. Nom du fichier : graph.png"
