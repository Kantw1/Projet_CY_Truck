fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt"
fichier_fusionne="Temp/fusion.txt"

awk -F ';' '!seen[$6,$1]++ { conducteurs[$6]++} END { for (diver in conducteurs) print conducteurs[diver], diver}' $fichier_trajets |sort -k1,1nr | head -n 10 | sort -k1,1n > $fichier_resultats
#cat "$fichier_trajets" | cut -d';' -f6 | sort | uniq -c | sort -nr | head -n 10 | sort -k1,1n > $fichier_resultats bon ça fonctionne pas trop quoi
#test!!! !
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 400
set output 'Image/histogramme_horizontal.png'
set title 'Histogramme horizontal des données'
set xlabel 'Nombre de trajets'
set ylabel 'Conducteurs'
set style fill solid
set xtics nomirror
set ytics nomirror
set yrange [0:10]

plot '$fichier_resultats' using (\$1*0.5):0:(\$1*0.5):(0.3):yticlabels(3) with boxxyerrorbars lc rgbcolor 'blue' ti "Histogramme Horizontal"
EOF
