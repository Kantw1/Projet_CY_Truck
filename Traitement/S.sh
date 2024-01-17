#création des fichiers nécessaires !
fichier_trajets="Data/data.csv"
fichier_resultats_test="Data/data_s.txt"

#lancer le fichier .c

# utilisation de gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal.png'
set title "Error on y represented by filledcurve shaded region"
set xlabel "Time (sec)" 
set ylabel "Rate" 
set grid xtics mxtics ytics mytics
set log y
Shadecolor = "#80E0A080"

# Utilisez le bon nom de fichier 'Data/data_s.txt'
plot $fichier_resultats_test using (\$2):2:(\$3) with filledcurve fc rgb Shadecolor title "Shaded error region", using 2:1 smooth mcspline lw 2 title "Monotonic spline through data"
EOF