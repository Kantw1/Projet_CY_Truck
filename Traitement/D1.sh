#création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt"

#calcul du nombre de trajets de chaque conducteur
awk -F ';' '!seen[$6,$1]++ { conducteurs[$6]++} END { for (diver in conducteurs) print conducteurs[diver], diver}' $fichier_trajets |sort -k1,1nr | head -n 10 | sort -k1,1n > $fichier_resultats

# utilisation de gnuplot
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
chmod 777 Image/histogramme_horizontal_1.png
xdg-open Image/histogramme_horizontal_1.png
