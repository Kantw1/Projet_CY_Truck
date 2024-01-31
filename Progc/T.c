#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

/*/
Fonctionnement du programme T.c :
-> Récupération des données du texte Résultat_2.txt dans une liste chainée
-> Insertion des données dans un AVL trié par rapport au nombre de villes traversées,et compare si l'ID existe déjà ou non (ville traversée qu'une seule fois pas trajet). Si déja 10 villes dans l'AVL, alors on compare avec le min de l'AVL, et si c'est plus grand on insère la nouvelle valeur dans l'AVL à la place.
->Récupération des données du texte Résultat_4.txt (texte contenant les villes départs et leur ID), et insertion direct en triant dans l'AVL précédent
->Ecriture des 10 villes les plus traversées avec leur nombre de départ dans un fichier résultat
/*/

//Structure AVL qui contient les ID des trajets
typedef struct conducteurAVL {
    int hauteur;
    int ID;
    struct conducteurAVL * gauche;
    struct conducteurAVL * droite;
}conducteurAVL;

//Structure AVL qui contient les noms des villes ainsi que le nombre de passage, 
//le nombre de départ, et contient 2 AVL conducteurAVL, un contient toutes les ID des trajet pour toutes les villes traversées 
//et un qui contient toutes les ID des trajet pour toutes les villes de départ traversées
typedef struct VilleAVL {
int hauteur;
int nb_passage_ville;
int nb_passage_ville_depart;
char ville[50];
struct conducteurAVL * conducteur;
struct conducteurAVL * ID;
struct VilleAVL *gauche;
struct VilleAVL *droite;
} VilleAVL;

//Structure Liste chainée qui contient les noeuds VilleAVL 
typedef struct Trajet {
 	VilleAVL* noeud;
 	struct Trajet* next;
    struct Trajet *end;
} Trajet;

//fonction retournant le max des deux valeurs en paramètres
int max(int a, int b) {
    return (a > b) ? a : b;
}

// fonction pour calculer hauteur d'un arbre
int height(conducteurAVL *node) {
    if (node == NULL) {
        return 0; // La hauteur d'un noeud vide est 0
    } else {
        int gauche_height = height(node->gauche);
        int droite_height = height(node->droite);

        return 1 + max(gauche_height, droite_height);
    }
}

// Fonction de création du noeud de l'arbre conducteurAVL
conducteurAVL * newconducteurAVL(int ID){
    conducteurAVL *node = (conducteurAVL *)malloc(sizeof(conducteurAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    node->ID = ID;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// Vérification de l'équilibre de l'arbre 
int getBalance(conducteurAVL *node) {
    if (node == NULL) {
        return 0; // Le facteur d'equilibre d'un noeud vide est 0
    } else {
        return height(node->gauche) - height(node->droite);
    }
}

// rotation droite de l'AVL conducteur
conducteurAVL *rotateRight(conducteurAVL *y) {
    if (y == NULL || y->gauche== NULL) {
        // Gestion des cas où les pointeurs sont nuls
        return y;
    }
    conducteurAVL *x = y->gauche;
    conducteurAVL *T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;

    return x;
}

// rotation gauche de l'AVL conducteur
conducteurAVL *rotateLeft(conducteurAVL *x) {
    if (x == NULL || x->droite == NULL) {
        // Gestion des cas où les pointeurs sont nuls
        return x;
    }
    conducteurAVL *y = x->droite;
    conducteurAVL *T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;

    return y;
}

// Fonction pour insérer un nouveau noeud dans l'arbre AVL
conducteurAVL *insertAVLNode(conducteurAVL *root, conducteurAVL *nouvelle_etape) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return nouvelle_etape;
    }
    if (nouvelle_etape->ID< root->ID) {
        root->gauche = insertAVLNode(root->gauche, nouvelle_etape);
    } else if (nouvelle_etape->ID > root->ID) {
        root->droite = insertAVLNode(root->droite, nouvelle_etape);
    }
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));
    // Obtenir le facteur d'équilibre du noeud
    int balance = getBalance(root);

    // Rotation gauche
    if (balance > 1) {
        if (getBalance(root->gauche) >= 0) {
            return rotateRight(root);
        } else {
            root->gauche = rotateLeft(root->gauche);
            return rotateRight(root);
        }
    }

    // Rotation droite
    if (balance < -1) {
        if (getBalance(root->droite) <= 0) {
            return rotateLeft(root);
        } else {
            root->droite = rotateRight(root->droite);
            return rotateLeft(root);
        }
    }
    return root;
}

//Fonction vérifiant si l'ID Trajet existe déjà dans l'AVL
int conducteurExiste(conducteurAVL *racine, int ID) {
    if (racine == NULL) {
        return 0; // Le conducteur n'existe pas dans l'AVL
    }
    if (ID == racine->ID) {
        return 1; // Le conducteur existe dans l'AVL
    } else if (ID < racine->ID) {
        return conducteurExiste(racine->gauche, ID);
    } else {
        return conducteurExiste(racine->droite, ID);
    }
}

//Fonction de création du noeud dans l'AVL VilleAVL
VilleAVL *newVilleAVL(char ville[],int ID) {
    VilleAVL *node = (VilleAVL *)malloc(sizeof(VilleAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    strcpy(node->ville, ville);
    conducteurAVL * Newone = newconducteurAVL(ID);
    //vérifier si le conducteur existe déjà ou pas ?
    node->conducteur = Newone;//insertAVLNode(node->conducteur,Newone);
    node->ID = NULL;
    node->nb_passage_ville = 1;
    node->nb_passage_ville_depart = 0;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

//Fonction modifiant le chainon en augmentant le nombre de passage de 1 et l'insert si l'ID n'existe pas
Trajet* modifierTrajet(Trajet* root, VilleAVL* nouvelle_etape) {
    if (root == NULL || root->noeud == NULL) {
        // Gérer le cas où root ou root->noeud est NULL
        return root;
    }
    if (conducteurExiste(root->noeud->conducteur,nouvelle_etape->conducteur->ID) == 0){
        root->noeud->nb_passage_ville ++;
        root->noeud->conducteur = insertAVLNode(root->noeud->conducteur,nouvelle_etape->conducteur);
    }

    return root;
}

//Fonction permettant l'insertion dans la liste chainée du noeud VilleAVL en paramètre dans un chainon
Trajet *insertPliste(Trajet *pliste, VilleAVL *nouvelle_etape) {
    Trajet *newNode = (Trajet *)malloc(sizeof(Trajet));
    //Vérification pour éviter la segmentation fault
    if (newNode == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    newNode->noeud = nouvelle_etape;
    newNode->next = NULL;

    if (pliste == NULL) {
        // Si la liste est vide, le nouveau noeud devient la tête de la liste
        Trajet * inter = newNode;
        inter->end = newNode;
        return inter;
    }
    
    Trajet *tmp = pliste;
    //Condition qui vérifie si la dernière ville de la liste est la même que la ville de nouvelle étape,et si c'est le cas, on ajoute nouvelle étape au chainon trajet
        if (strcmp(tmp->end->noeud->ville, nouvelle_etape->ville) == 0) {
            tmp = modifierTrajet(tmp->end, nouvelle_etape);
            free(nouvelle_etape);
            free(newNode); // Libérer le noeud nouvellement alloué car il n'est pas nécessaire
            return pliste; // Pas besoin d'ajouter un nouveau noeud à la liste
    }

    tmp->end->next = newNode;
    tmp->end = tmp->end->next;
    return pliste;
}

//Fonction qui recherche la ville traversée par le moins de trajet de l'AVL et la return 
VilleAVL *rechercherPlusPetit(VilleAVL *racine) {
    // Parcours vers le plus à gauche
    while (racine != NULL && racine->gauche != NULL) {
        racine = racine->gauche;
    }
    return racine;
}

int height_Ville(VilleAVL *node) {
    if (node == NULL) {
        return 0; // La hauteur d'un noeud vide est 0
    } else {
        int gauche_height = height_Ville(node->gauche);
        int droite_height = height_Ville(node->droite);

        return 1 + max(gauche_height, droite_height);
    }
}

int getBalance_Ville(VilleAVL *node) {
    if (node == NULL) {
        return 0; // Le facteur d'equilibre d'un noeud vide est 0
    } else {
        return height_Ville(node->gauche) - height_Ville(node->droite);
    }
}

// rotation droite de l'AVL
VilleAVL*rotateRight_Ville(VilleAVL*y) {
    VilleAVL*x = y->gauche;
    VilleAVL*T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = max(height_Ville(y->gauche), height_Ville(y->droite)) + 1;
    x->hauteur = max(height_Ville(x->gauche), height_Ville(x->droite)) + 1;

    return x;
}

// rotation gauche de l'AVL
VilleAVL*rotateLeft_Ville(VilleAVL*x) {
    VilleAVL*y = x->droite;
    VilleAVL*T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = max(height_Ville(x->gauche), height_Ville(x->droite)) + 1;
    y->hauteur = max(height_Ville(y->gauche), height_Ville(y->droite)) + 1;

    return y;
}

VilleAVL *mettreAJourHauteurEquilibre_Ville(VilleAVL *racine) {
    if (racine == NULL) {
        return NULL;
    }

    // Mettre à jour la hauteur du nœud actuel
    racine->hauteur = 1 + max(height_Ville(racine->gauche), height_Ville(racine->droite));

    // Calculer le facteur d'équilibre
    int balance = getBalance_Ville(racine);

    // Cas de déséquilibre à gauche
    if (balance > 1) {
        if (getBalance_Ville(racine->gauche) >= 0) {
            // Cas de la rotation simple à droite
            return rotateRight_Ville(racine);
        } else {
            // Cas de la double rotation gauche-droite
            racine->gauche = rotateLeft_Ville(racine->gauche);
            return rotateRight_Ville(racine);
        }
    }

    // Cas de déséquilibre à droite
    if (balance < -1) {
        if (getBalance_Ville(racine->droite) <= 0) {
            // Cas de la rotation simple à gauche
            return rotateLeft_Ville(racine);
        } else {
            // Cas de la double rotation droite-gauche
            racine->droite = rotateRight_Ville(racine->droite);
            return rotateLeft_Ville(racine);
        }
    }

    return racine;
}

// Fonction pour insérer un nouveau noeud dans l'arbre AVL
VilleAVL *insertAVLNode_Ville(VilleAVL *root, VilleAVL *nouvelle_etape) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return nouvelle_etape;
    }
    if (nouvelle_etape->nb_passage_ville< root->nb_passage_ville) {
        root->gauche = insertAVLNode_Ville(root->gauche, nouvelle_etape);
    } else if (nouvelle_etape->nb_passage_ville >= root->nb_passage_ville) {
        root->droite = insertAVLNode_Ville(root->droite, nouvelle_etape);
    }

    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height_Ville(root->gauche), height_Ville(root->droite));

    // Obtenir le facteur d'équilibre du noeud
    int balance = getBalance_Ville(root);

    // Cas de déséquilibre à gauche
    if (balance > 1) {
        if (nouvelle_etape->nb_passage_ville < root->gauche->nb_passage_ville) {
            return rotateRight_Ville(root);
        } else if (nouvelle_etape->nb_passage_ville >= root->gauche->nb_passage_ville) {
            root->gauche = rotateLeft_Ville(root->gauche);
            return rotateRight_Ville(root);
        }
    }

    // Cas de déséquilibre à droite
    if (balance < -1) {
        if (nouvelle_etape->nb_passage_ville >= root->droite->nb_passage_ville) {
            return rotateLeft_Ville(root);
        } else if (nouvelle_etape->nb_passage_ville < root->droite->nb_passage_ville) {
            root->droite = rotateRight_Ville(root->droite);
            return rotateLeft_Ville(root);
        }
    }

    return root;
}

VilleAVL *supprimerPlusPetit(VilleAVL *racine) {
    // Cas de base : arbre vide
    if (racine == NULL) {
        return NULL;
    }

    // Trouver le plus petit nœud (le plus à gauche)
    if (racine->gauche != NULL) {
        racine->gauche = supprimerPlusPetit(racine->gauche);
    } else {
        // Le plus petit nœud est trouvé, le supprimer
        VilleAVL *temp = racine->droite;
        free(racine);
        return temp;
    }
    return racine;
}

VilleAVL *insertAVLFromList(Trajet *pliste, VilleAVL *arbre) {
    Trajet *tmp = pliste;
    int compter = 0;
    while (tmp != NULL) {
        if ( compter >= 10){
            //si la distance max min supérieur au plus petit de l'arbre, on l'intégre_depart
            if(rechercherPlusPetit(arbre)->nb_passage_ville< tmp->noeud->nb_passage_ville){
                arbre = supprimerPlusPetit(arbre);
                arbre = mettreAJourHauteurEquilibre_Ville(arbre);
                arbre = insertAVLNode_Ville(arbre, tmp->noeud);
            }
        }
        else{
            arbre = insertAVLNode_Ville(arbre, tmp->noeud);
        }
        compter ++;
        tmp = tmp->next;
    }
    return arbre;
}

// Fonction auxiliaire pour parcourir l'arbre et remplir le tableau
void fillSortedDataDecreasing(struct VilleAVL* node, struct VilleAVL* sortedData[10], int* currentIndex) {
    if (node != NULL && *currentIndex < 10) {
        fillSortedDataDecreasing(node->droite, sortedData, currentIndex);

        // Comparaison avec le pointeur node
        if (*currentIndex < 10) {
            sortedData[*currentIndex] = node;
            *currentIndex = *currentIndex + 1;
        }

        fillSortedDataDecreasing(node->gauche, sortedData, currentIndex);
    }
}



// Fonction pour libérer la mémoire allouée pour chaque élément de sortedData
void freeSortedData(struct VilleAVL* sortedData[10], int currentIndex) {
    for (int i = 0; i < currentIndex; ++i) {
        free(sortedData[i]);
    }
}

// Fonction pour traiter les statistiques et générer le fichier de sortie
void processStats(struct VilleAVL* root) {
    // Tableau pour stocker les données triées
    struct VilleAVL* sortedData[10];
    int currentIndex = 0;

    // Fonction auxiliaire pour parcourir l'arbre et remplir le tableau
    fillSortedDataDecreasing(root, sortedData, &currentIndex);

    // Afficher les statistiques et générer les données pour le graphique
    FILE* dataFile = fopen("Temp/Resultat_T3.txt", "w");
    for (int i = 0; i < currentIndex; ++i) {
        fprintf(dataFile, "%s;%d;%d\n", sortedData[i]->ville, sortedData[i]->nb_passage_ville, sortedData[i]->nb_passage_ville_depart);
    }
    fclose(dataFile);

    // Libération de la mémoire allouée pour chaque élément de sortedData
    freeSortedData(sortedData, currentIndex);
}

VilleAVL *insertAVLNode_Ville_trie(VilleAVL *root, VilleAVL *nouvelle_etape) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return nouvelle_etape;
    }
    if (strcmp(nouvelle_etape->ville,root->ville) < 0) {
        root->gauche = insertAVLNode_Ville_trie(root->gauche, nouvelle_etape);
    } else if (strcmp(nouvelle_etape->ville,root->ville) > 0) {
        root->droite = insertAVLNode_Ville_trie(root->droite, nouvelle_etape);
    }

    return root;
}

void trieVille(VilleAVL * root, VilleAVL * nvRoot){
    if(root != NULL){
        nvRoot = insertAVLNode_Ville_trie(nvRoot,root);
        trieVille(root->gauche,nvRoot);
        trieVille(root->droite,nvRoot);
    }
}

void VilleExiste(VilleAVL *racine, char ville[], int ID) {
    if (racine == NULL) {
        return; // Le conducteur n'existe pas dans l'AVL
    }
    if (strcmp(ville, racine->ville) == 0) {
        if (conducteurExiste(racine->ID,ID) == 0){
            racine->nb_passage_ville_depart ++;
            racine->ID = insertAVLNode(racine->ID,newconducteurAVL(ID));
        }
        return; // Le conducteur existe dans l'AVL
    } else if (strcmp(ville, racine->ville) < 0) {
        VilleExiste(racine->gauche, ville, ID);
    } else {
        VilleExiste(racine->droite, ville,ID);
    }
}


int main(){
    FILE *fichier = fopen("Temp/resultat_T2.txt", "r");
    if (fichier == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    }
    int id_trajet;
    int ID;
    char ville[50];
    VilleAVL *arbre = NULL;
    Trajet *pliste = NULL; // Initialisez votre liste à NULL
    Trajet *tmp = pliste;
    VilleAVL *nouvelle_etape = NULL;
      while (fscanf(fichier, "%d;%49[^\n]\n", &ID, ville) == 2 ){ 
        nouvelle_etape = newVilleAVL(ville,ID);
    	pliste = insertPliste(pliste, nouvelle_etape);
      }
    fclose(fichier);
    arbre = insertAVLFromList(pliste, arbre);
    VilleAVL * Ville_trie = NULL;
    trieVille(arbre,Ville_trie);
    FILE *fichier2 = fopen("Temp/resultat_T4.txt", "r");
    if (fichier2 == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    }
    while (fscanf(fichier2, "%d;%49[^\n]\n", &ID, ville) == 2 ){
        VilleExiste(arbre,ville,ID);
      }
    pliste=NULL;
    processStats(arbre);
    fclose(fichier2);
    while (pliste != NULL) {
        Trajet *temp = pliste;
        pliste = pliste->next;
        free(temp);
	}
    return 0;
}
