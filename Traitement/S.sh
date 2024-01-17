#création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats_test="Data/data_s.dat"

#lancer le fichier .c

# utilisation de gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal.png'
set title 'Option -d1 : Nbroutes = f(Driver)'
set xlabel 'NB ROUTES'
set ylabel 'DRIVER NAMES'
set style fill solid
#set xtics nomirror #je crois qu'il y a pas besoin de nomirror
#set ytics nomirror
set yrange [-1:10]

plot '$fichier_resultats_test' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF
