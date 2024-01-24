fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_T.txt"
fichier_resultats2="Temp/resultat_T2.txt"
fichier_resultats3="Temp/Resultat_T3.txt"
fichier_resultats4="Temp/resultat_T4.txt"
fichier_resultats5="Temp/resultat_T5.txt"
fichier_resultats_test="Data/data_t.txt"

cut -d';' -f1,3,4 "$fichier_trajets" | tail -n +2 > $fichier_resultats #head -n 6000 à enlever
awk -F';' '{print $1 FS $2; print $1 FS $3}' $fichier_resultats | sort -t';' -k2,2 > $fichier_resultats2
awk -F';' '{print $1 FS $2}' $fichier_resultats > $fichier_resultats4 #| sort -t';' -k2,2
#sed -i 's/$/;/g' $fichier_resultats2
./Progc/prog_t
sort -t';' -k1,1 "$fichier_resultats3" > $fichier_resultats5


gnuplot -persist <<EOF
set datafile separator ";"
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'Image/histogramme_horizontal_4.png'
set title 'Option -t : NB routes = t(Towns)'
set ylabel 'NB ROUTES'
set xlabel 'TOWN NAMES'
set style fill solid 
set yrange [0:]
set boxwidth 0.5
set xtics rotate by 45 right font ",8"
set bmargin 5
set style histogram clustered gap 1
set style fill solid 0.8 border

plot '$fichier_resultats5' using (\$0):2:xticlabel(1) with boxes lc rgbcolor 'spring-green' notitle,\
    '' using (\$0+0.2):(\$3) with boxes lc rgbcolor 'dark-green' notitle
EOF

chmod 777 Image/histogramme_horizontal_4.png
xdg-open Image/histogramme_horizontal_4.png
