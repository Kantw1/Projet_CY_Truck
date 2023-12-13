fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat.txt"

#cat "$fichier_trajets" | cut -d';' -f6 | sort | uniq -c | sort -nr | head -n 10 > $fichier_resultats

gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 400
set output 'Image/histogramme_horizontal.png'  # Chemin complet vers le dossier "Image"
set title 'Histogramme horizontal des données'
set xlabel 'Nombre de trajets'
set ylabel 'Conducteurs'
set style fill solid
set boxwidth 0.5
set xtics nomirror
set ytics nomirror
set yrange [0:*]
set yrange [0:10] reverse

plot '$fichier_resultats' using 1:0:ytic(2) with boxes lc rgbcolor 'blue' notitle
EOF

