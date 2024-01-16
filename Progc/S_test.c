#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct EtapeAVL {
    int id_trajet;
    int distance;
    int distance_min;
    int distance_max;
    int distance_max_min;
    int distance_moyenne;
    int hauteur;
    int nombre_etapes;
    struct EtapeAVL *gauche;
    struct EtapeAVL *droite;
    struct EtapeAVL *racine;
} EtapeAVL;

typedef struct Trajet {
 	EtapeAVL* noeud;
 	struct Trajet* next;
} Trajet;

// fonction usuelle pour avoir le max entre deux données
int max(int a, int b) {
    return (a > b) ? a : b;
}

// fonction pour calculer hauteur d'un arbre
int height(EtapeAVL *node) {
    if (node == NULL) {
        return 0; // La hauteur d'un noeud vide est 0
    } else {
        int gauche_height = height(node->gauche);
        int droite_height = height(node->droite);

        return 1 + max(gauche_height, droite_height);
    }
}

// rotation droite de l'AVL
EtapeAVL *rotateRight(EtapeAVL *y) {
    EtapeAVL *x = y->gauche;
    EtapeAVL *T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;

    return x;
}

// rotation gauche de l'AVL
EtapeAVL *rotateLeft(EtapeAVL *x) {
    EtapeAVL *y = x->droite;
    EtapeAVL *T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;

    return y;
}

// l'AVL est-il equilibre
int getBalance(EtapeAVL *node) {
    if (node == NULL) {
        return 0; // Le facteur d'equilibre d'un noeud vide est 0
    } else {
        return height(node->gauche) - height(node->droite);
    }
}

// creation d'un nouveau noeud
EtapeAVL *newEtapeAVL(int id_trajet,int distance) {
    EtapeAVL *node = (EtapeAVL *)malloc(sizeof(EtapeAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    node->distance=distance;
    node->distance_max=distance;
    node->distance_min=distance;
    node->distance_max_min=0;
    node->nombre_etapes=1;
    node->distance_moyenne=distance/node->nombre_etapes;
    node->id_trajet = id_trajet;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// modification du noeud 
Trajet* modifierTrajet(Trajet* root,EtapeAVL* nouvelle_etape){
	root->noeud->distance+=nouvelle_etape->distance;
	if (root->noeud->distance_max<nouvelle_etape->distance){
		root->noeud->distance_max=nouvelle_etape->distance;
	}
	if (root->noeud->distance_min>nouvelle_etape->distance){
		root->noeud->distance_min=nouvelle_etape->distance;
	}
	root->noeud->distance_max_min=root->noeud->distance_max-root->noeud->distance_min;
        root->noeud->nombre_etapes+=1;
        root->noeud->distance_moyenne=root->noeud->distance/root->noeud->nombre_etapes;
	return root;
}

Trajet *insertPliste(Trajet *pliste, EtapeAVL *nouvelle_etape) {
    Trajet *newNode = (Trajet *)malloc(sizeof(Trajet));
    if (newNode == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    newNode->noeud = nouvelle_etape;
    newNode->next = NULL;

    Trajet *tmp = pliste;
    while (tmp->next != NULL) {
        if (tmp->noeud->id_trajet == nouvelle_etape->id_trajet) {
            tmp = modifierTrajet(tmp, nouvelle_etape);
            free(newNode); // Libérer le nœud nouvellement alloué car il n'est pas nécessaire
            return pliste; // Pas besoin d'ajouter un nouveau nœud à la liste
        }
        tmp = tmp->next;
    }

    tmp->next = newNode;
    return pliste;
}

	
// insertion d'un nouveau noeud
EtapeAVL *insertAVL(EtapeAVL *root,EtapeAVL *nouvelle_etape) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        root=nouvelle_etape;
        return root;
    }
    if (root->distance_max_min>nouvelle_etape->distance_max_min) {
        root->gauche = insertAVL(root->gauche,nouvelle_etape);
    } else if (root->distance_max_min<nouvelle_etape->distance_max_min) {
        root->droite = insertAVL(root->droite,nouvelle_etape);
        }
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'equilibre du noeud
    int balance = getBalance(root);

    // Cas de desequilibre à gauche
    if (balance > 1) {
        if (nouvelle_etape->distance_max_min < root->gauche->distance_max_min) {
            return rotateRight(root);
        } else if (nouvelle_etape->distance_max_min > root->gauche->distance_max_min) {
            root->gauche = rotateLeft(root->gauche);
            return rotateRight(root);
        }
    }

    return root;
}




/*/ Trajet* noeud_max_parcours(EtapeAVL* arbre){
	Trajet* pliste;
	if (arbre==NULL){
	   return 0
	}
	pliste=noeud_max_parcours(arbre->droit);
	pliste=insertPliste(arbre);
	pliste=noeud_max_parcours(arbre->gauche);
	return pliste;
}
/*/

// Fonction auxiliaire pour parcourir l'arbre et remplir le tableau
void fillSortedData(struct EtapeAVL* node, struct Trajet* sortedData[50], int* currentIndex) {
    if (node != NULL && *currentIndex < 50) {
        fillSortedData(node->droite, sortedData, currentIndex);
        sortedData[*currentIndex] = (Trajet*)malloc(sizeof(Trajet));
        sortedData[*currentIndex++]->noeud = node;
        fillSortedData(node->gauche, sortedData, currentIndex);
    }
}

// Libération de la mémoire allouée pour chaque élément de sortedData
void freeSortedData(struct Trajet* sortedData[50], int currentIndex) {
    for (int i = 0; i < currentIndex; ++i) {
        free(sortedData[i]);
    }
}

// Nouvelle fonction pour le traitement statistique et la génération du graphique
void processStats(struct EtapeAVL* root) {
    // Tableau pour stocker les données triées
    struct Trajet* sortedData[50];
    int currentIndex = 0;

    // Fonction auxiliaire pour parcourir l'arbre et remplir le tableau
    fillSortedData(root, sortedData, &currentIndex);

    // Afficher les statistiques et générer les données pour le graphique
    FILE* dataFile = fopen("Temp/Resultat_s2.txt", "w");
    fprintf(dataFile, "#ID Distance_mini Distance_moyenne Distance_maxi\n");
    for (int i = 0; i < currentIndex; ++i) {
        fprintf(dataFile, "%.2lf %.2lf %.2lf %.2lf %.2lf\n", sortedData[i]->noeud->id_trajet, sortedData[i]->noeud->distance_min, sortedData[i]->noeud->distance_moyenne, sortedData[i]->noeud->distance_max, sortedData[i]->noeud->distance_max_min);
    }
    fclose(dataFile);

    // Libération de la mémoire allouée pour chaque élément de sortedData
    freeSortedData(sortedData, currentIndex);
}



int main(){
    FILE *fichier = fopen("Temp/Resultat_s.txt", "r");
    if (fichier == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    } 
    int id_trajet, distance;
    EtapeAVL *arbre = NULL;
    Trajet *pliste = NULL; // Initialisez votre liste à NULL
    Trajet *tmp = pliste;

    while (fscanf(fichier, "%d;%lf", &id_trajet, &distance) == 2 || !feof(fichier)){
    	printf("%d\n", id_trajet);
    	printf("%.3lf\n", distance);
    	EtapeAVL *nouvelle_etape = newEtapeAVL(id_trajet, distance);
    	pliste = insertPliste(pliste, nouvelle_etape);
}


    while (tmp != NULL && tmp->next != NULL) {
    arbre = insertAVL(arbre, tmp->noeud);
    tmp = tmp->next;
    }
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
