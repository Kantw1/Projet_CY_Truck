#!/bin/bash
fichier_trajets="Data/data.csv"
fichier_resultats="Temp/resultat_T.txt"
fichier_resultats2="Temp/resultat_T2.txt"

cut -d';' -f1,2,3,4 "$fichier_trajets" | tail -n +2 | head -n 60 > $fichier_resultats #head -n 6000 à enlever
./Progc/prog_t_test