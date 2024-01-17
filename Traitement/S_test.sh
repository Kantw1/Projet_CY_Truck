fichier_trajets="Data/data.csv"
# Commande pour extraire les colonnes 2 et 5 du fichier CSV et les sauvegarder dans un fichier texte temporaire
cut -d';' -f1,5 "$fichier_trajets" > Temp/Resultat_s.txt

# Commande pour exécuter le programme C
./Progc/S.c

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
plot "Temp/Resultat_s.txt" using 1:2 with lines title "Distance_mini", \
     "Temp/Resultat_s.txt" using 1:3 with lines title "Distance_moyenne", \
     "Temp/Resultat_s.txt" using 1:4 with lines title "Distance_maxi"

EOF

# Message indiquant que le script a terminé
echo "Graphique généré avec succès. Nom du fichier : histogramme_S.png"
