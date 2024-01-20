#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

typedef struct conducteurAVL {
    int hauteur;
    char conducteur[50];
    struct conducteurAVL * gauche;
    struct conducteurAVL * droite;
}conducteurAVL;

typedef struct VilleAVL {
int hauteur;
int nb_conducteurs;
int nb_passage_ville;
char ville[50];
struct conducteurAVL * conducteur;
struct VilleAVL *gauche;
struct VilleAVL *droite;
} VilleAVL;


typedef struct Trajet {
 	VilleAVL* noeud;
 	struct Trajet* next;
    struct Trajet *end;
} Trajet;


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

conducteurAVL * newconducteurAVL(char nom[]){
    conducteurAVL *node = (conducteurAVL *)malloc(sizeof(conducteurAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    strcpy(node->conducteur, nom);
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}
// l'AVL est-il equilibre
int getBalance(conducteurAVL *node) {
    if (node == NULL) {
        return 0; // Le facteur d'equilibre d'un noeud vide est 0
    } else {
        return height(node->gauche) - height(node->droite);
    }
}

// rotation droite de l'AVL conducteur
conducteurAVL *rotateRight(conducteurAVL *y) {
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
    if (strcmp(nouvelle_etape->conducteur, root->conducteur) < 0) {
        root->gauche = insertAVLNode(root->gauche, nouvelle_etape);
    } else if (strcmp(nouvelle_etape->conducteur, root->conducteur) > 0) {
        root->droite = insertAVLNode(root->droite, nouvelle_etape);
    }
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'équilibre du noeud
    int balance = getBalance(root);
    // Cas de déséquilibre à gauche
    if (balance > 1) {
        if (strcmp(nouvelle_etape->conducteur, root->gauche->conducteur) > 0) {
            return rotateRight(root);
        } else if (strcmp(nouvelle_etape->conducteur, root->gauche->conducteur) > 0) {
            root->gauche = rotateLeft(root->gauche);
            return rotateRight(root);
        }
    }

    // Cas de déséquilibre à droite
    if (balance < -1) {
        if (strcmp(nouvelle_etape->conducteur, root->droite->conducteur) > 0){
            return rotateLeft(root);
        } else if (strcmp(nouvelle_etape->conducteur, root->droite->conducteur) < 0 ){
            root->droite = rotateRight(root->droite);
            return rotateLeft(root);
        }
    }

    return root;
}


int conducteurExiste(conducteurAVL *racine, char conducteur[]) {
    if (racine == NULL) {
        return 0; // Le conducteur n'existe pas dans l'AVL
    }
    int comparaison = strcmp(conducteur, racine->conducteur);
    if (comparaison == 0) {
        return 1; // Le conducteur existe dans l'AVL
    } else if (comparaison < 0) {
        return conducteurExiste(racine->gauche, conducteur);
    } else {
        return conducteurExiste(racine->droite, conducteur);
    }
}

VilleAVL *newVilleAVL(char ville[],char nom[]) {
    VilleAVL *node = (VilleAVL *)malloc(sizeof(VilleAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    strcpy(node->ville, ville);
    conducteurAVL * Newone = newconducteurAVL(nom);
    //vérifier si le conducteur existe déjà ou pas ?
    node->conducteur = Newone;//insertAVLNode(node->conducteur,Newone);
    node->nb_conducteurs = 1;
    node->nb_passage_ville = 1;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

Trajet* modifierTrajet(Trajet* root, VilleAVL* nouvelle_etape) {
    if (root == NULL || root->noeud == NULL) {
        // Gérer le cas où root ou root->noeud est NULL
        return root;
    }
    root->noeud->nb_passage_ville ++;
    if (conducteurExiste(root->noeud->conducteur,nouvelle_etape->conducteur->conducteur) == 0){
        root->noeud->nb_conducteurs ++;
        root->noeud->conducteur = insertAVLNode(root->noeud->conducteur,nouvelle_etape->conducteur);
    }

    return root;
}

Trajet *insertPliste(Trajet *pliste, VilleAVL *nouvelle_etape) {
    Trajet *newNode = (Trajet *)malloc(sizeof(Trajet));
    if (newNode == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    newNode->noeud = nouvelle_etape;
    newNode->next = NULL;

    if (pliste == NULL) {
        // Si la liste est vide, le nouveau nœud devient la tête de la liste
        Trajet * inter = newNode;
        inter->end = newNode;
        return inter;
    }

    Trajet *tmp = pliste;
        if (strcmp(tmp->end->noeud->ville, nouvelle_etape->ville) == 0) {
            tmp = modifierTrajet(tmp->end, nouvelle_etape);
            free(nouvelle_etape);
            free(newNode); // Libérer le nœud nouvellement alloué car il n'est pas nécessaire
            return pliste; // Pas besoin d'ajouter un nouveau nœud à la liste
    }

    tmp->end->next = newNode;
    tmp->end = tmp->end->next;
    return pliste;
}

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
            //si la distance max min supérieur au plus petit de l'arbre, on l'intégre
            if(rechercherPlusPetit(arbre)->nb_passage_ville< tmp->noeud->nb_passage_ville){
                arbre = supprimerPlusPetit(arbre);
                arbre = mettreAJourHauteurEquilibre_Ville(arbre);
                arbre = insertAVLNode_Ville(arbre, tmp->noeud);
            }
            //arbre = mettreAJourHauteurEquilibre(arbre);
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
        fprintf(dataFile, "%s;%d;%d\n", sortedData[i]->ville, sortedData[i]->nb_passage_ville, sortedData[i]->nb_conducteurs);
    }
    fclose(dataFile);

    // Libération de la mémoire allouée pour chaque élément de sortedData
    freeSortedData(sortedData, currentIndex);
}


int main(){
    FILE *fichier = fopen("Temp/resultat_T2.txt", "r");
    //FILE *fichier2 = fopen("Temp/resultat_T2.txt", "r");
    if (fichier == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    }
    int id_trajet;
    char nom[50];
    char ville[50];
    //char ville_B[50];
    VilleAVL *arbre = NULL;
    Trajet *pliste = NULL; // Initialisez votre liste à NULL
    Trajet *tmp = pliste;
    int compteur = 0;
    VilleAVL *nouvelle_etape = NULL;
      while (fscanf(fichier, "%49[^;];%49[^\n]\n", ville, nom) == 2 ){ 
        printf("Ville: %s, Nom : %s\n", ville, nom);
        nouvelle_etape = newVilleAVL(ville,nom);
    	pliste = insertPliste(pliste, nouvelle_etape);
        compteur ++;
        printf("%d\n",compteur);
      }
    arbre = insertAVLFromList(pliste, arbre);
    pliste=NULL;
    processStats(arbre);
    fclose(fichier);
    while (pliste != NULL) {
        Trajet *temp = pliste;
        pliste = pliste->next;
        free(temp);
	}
    return 0;
}
