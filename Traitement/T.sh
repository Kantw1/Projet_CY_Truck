fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_T.txt"
fichier_resultats_test="Data/data_t.txt"

#sed -i 's/;/ /g' $fichier_resultats_test
#./Progc/prog_t t Data/data.csv

gnuplot -persist <<EOF
set datafile separator ";"
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_4.png'
set title 'Option -t : NB routes = t(Towns)'
set ylabel 'NB ROUTES'
set xlabel 'TOWN NAMES'
set style fill solid 
set yrange [0:4000]
set boxwidth 0.5
set xtics rotate by 45 right font ",8"
set bmargin 5
set style histogram clustered gap 1
set style fill solid 0.8 border

plot '$fichier_resultats_test' using (\$0):2:xticlabel(1) with boxes lc rgbcolor 'spring-green' notitle,\
    '' using (\$0+0.2):(\$3) with boxes lc rgbcolor 'dark-green' notitle
#plot for [COL=2:3] '$fichier_resultats_test' using COL:xticlabels(1) with boxes 
EOF

chmod 777 Image/histogramme_horizontal_4.png
xdg-open Image/histogramme_horizontal_4.png