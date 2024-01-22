#création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt"

# Tri des données utilisant awk pour additionner le nombre de trajet faits par conducteur, et renvoie des 10 conducteurs avec le plus de trajets
awk -F ';' '!seen[$6,$1]++ { conducteurs[$6]++} END { for (diver in conducteurs) print conducteurs[diver], diver}' $fichier_trajets |sort -k1,1nr | head -n 10 | sort -k1,1n > $fichier_resultats

# Utilisation de gnuplot pour la création d'un graphe
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_1.png'
set title 'Option -d1 : Nbroutes = f(Driver)'
set xlabel 'NB ROUTES'
set ylabel 'DRIVER NAMES'
set style fill solid
set yrange [-1:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF
# Utilisation de chmod pour donner les droits absolues à l'accès au graphe de sortie en png
chmod 777 Image/histogramme_horizontal_1.png
xdg-open Image/histogramme_horizontal_1.png
