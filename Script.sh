#script schell

chmod 777 Script.sh
chmod 777 Traitement/D1.sh Traitement/D2.sh Traitement/T.sh Progc/T.c Progc/S.c

fichier1="Data/data.csv"

# Fonction pour afficher l'aide
afficher_aide() {
    echo "  -h, : En lancant ce programme vous avez le choix entre différents arguments 
    liés à différents traitements :
    - Le traitement D1 permet de voir les conducteurs avec le plus de trajet
    - Le traitement D2 permet de voir les conducteurs et la plus grande distance
    - Le traitement T permet de voir les 10 trajets les plus longs
    - Le traitement S permet de voir les 10 villes les plus traversées
    - Le traitement L permet de voir les statistiques sur les étapes
    "
}

# Fonction pour le traitement D1
traitement_D1() {
    echo "Traitement D1"
    #lancer les verif et le traitement demandé
    verifier_tout;
    ./Traitement/D1.sh
}

# Fonction pour le traitement D2
traitement_D2() {
    echo "Traitement D2"
    # lancer les verif et le traitement demandé
    verifier_tout;
    ./Traitement/D2.sh
}

# Fonction pour le traitement L
traitement_L() {
    echo "Traitement L"
    # lancer les verif et le traitement demandé
    verifier_tout;
    ./Traitement/L.sh
}

# Fonction pour le traitement T
traitement_T() {
    echo "Traitement T"
    # lancer les verif et le traitement demandé
    verifier_tout;
    ./Progc/prog_t t Data/data.csv
}

# Fonction pour le traitement S
traitement_S() {
    echo "Traitement S"
    # lancer les verif et le traitement demandé
    verifier_tout;
    gcc Progc/S.c -o Progc/prog_s
    ./Progc/prog_s s Data/data.csv
}

#fonction de vérification
verifier_tout() {
# Vérifier l'existence du fichier
if [ ! -f "$fichier1" ]; then
    echo "Erreur : Le fichier $fichier1 n'existe pas."
fi

executable1="Progc/prog_t"  # executable du traitement T
executable2="Progc/prog_s"  # executable du traitement S

#vérifier si l'executable c existe
if command -v "$executable1" >/dev/null 2>&1; then
    echo "L'exécutable $executable1 existe."
else
    echo "Erreur : Les exécutables $executable1"
    cd Progc
    gcc Progc/T.c -o Progc/prog_t
    cd ..
fi

if command -v "$executable2" >/dev/null 2>&1; then
    echo "L'exécutable $executable2 existe."
else
    echo "Erreur : Les exécutables $executable2"
    cd Progc
    #gcc Progc/T.c -o Progc/prog_s
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

}

# Mesurer le temps de début
start_time=$(date +%s)

# Vérifier et exécuter le traitement en fonction de l'argument fourni
case "$1" in
    "D1") traitement_D1 ;;
    "D2") traitement_D2 ;;
    "L")  traitement_L ;;
    "T")  traitement_T ;;
    "S")  traitement_S ;;
    "-h")  afficher_aide ;;
    *)    echo "Argument invalide. Utilisez l'option -h pour afficher l'aide." ;;
esac

# Mesurer le temps de fin
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))

echo "Le traitement a pris $elapsed_time secondes."
