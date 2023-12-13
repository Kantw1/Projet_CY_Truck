#script schell

#chmod 777 Script.sh

fichier1="Data/data.csv"

# Fonction pour afficher l'aide
afficher_aide() {
    echo "  -h, : En lancant ce programme vous avez le choix entre différents arguments 
    liés à différents traitements"
}

# Vérifier l'existence du fichier
if [ ! -f "$fichier1" ]; then
    echo "Erreur : Le fichier $fichier1 n'existe pas."
fi

executable="Progc/prog"  # Remplacez "prog" par le nom de votre exécutable

#vérifier si l'executable c existe
if command -v "$executable" >/dev/null 2>&1; then
    echo "L'exécutable $executable existe."
else
    echo "Erreur : L'exécutable $executable n'existe pas."
    cd Progc
    make build
    cd ..
fi
#vérifier si le dossier Temp existe
if [ ! -d "Temp" ]; then
    mkdir "Temp"
else
    rm -f Temp/*  #Supprimer tous les fichiers dans le dossier Temp si il existe
fi
#vérifier si le dossier Image existe
if [ ! -d "Image" ]; then
    echo "creation du dossier image"
    mkdir "Image"
fi
# Vérifier si l'argument -h est présent
if [ "$1" == "-h" ]; then
    afficher_aide
    exit 0
fi
# Mesurer le temps de début
start_time=$(date +%s)

# Exécuter le programme avec les arguments appropriés
"$executable" "$@"

# Mesurer le temps de fin
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))

echo "Le traitement a pris $elapsed_time secondes."
#test
# IL reste ca : le programme GnuPlot et choisir le traitement
