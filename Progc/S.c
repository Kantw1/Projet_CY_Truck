#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct EtapeAVL {
    int id_trajet;
    int distance_min;
    int distance_max;
    int distance_max_min;
    int hauteur;
    struct EtapeAVL *gauche;
    struct EtapeAVL *droite;
    struct EtapeAVL *racine;
} EtapeAVL;

typedef struct Trajet {
 	EtapeAVL* noeud;
 	struct* Trajet next;
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
    node->id_trajet = id_trajet;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// modification du noeud 
EtapeAVL *modifierTrajet(EtapeAVL* root,EtapeAVL* nouvelle_etape){
	root->distance+=nouvelle_etape->distance
	if (root->distance_max<nouvelle_etape->distance){
		root->distance_max=nouvelle_etape->distance;
	}
	if (root->distance_min>nouvelle_etape->distance){
		root->distance_min=distance;
	}
	root->distance_max_min=root->distance_max-root->distance_min;
	return root;
}

Trajet *insertPliste(Trajet *pliste,EtapeAVL *nouvelle_etape){
	Trajet* tmp=pliste;
	while (tmp->next!=NULL){
		if (tmp->noeud->id_trajet==nouvelle_etape->id_trajet){
			tmp->noeud=modifierTrajet(Trajet->noeud,nouvelle_etape);
		}
		tmp=tmp->next;
	}
	tmp->next->noeud=nouvelle_etape;
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
        root->gauche = insert(root->gauche,nouvelle_etape,id_trajet,distance);
    } else if (root->distance_max_min<nouvelle_etape->distance_max_min) {
        root->droite = insert(root->droite,nouvelle_etape,id_trajet,distance);
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'equilibre du noeud
    int balance = getBalance(root);

    // Cas de desequilibre à gauche
    if (balance > 1) {
        if (nouvelle_etape->distance < root->gauche->id_trajet) {
            return rotateRight(root);
        } else if (id_trajet > root->gauche->id_trajet) {
            root->gauche = rotateLeft(root->gauche);
            return rotateRight(root);
        }
    }

    return root;
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

void noeud_max_parcours(arbre){
	EtapeAVL* parcours=arbre;
	while (parcours->droite!=NULL){
		parcours=parcours->droite;
	}
	Trajet* max_50;
	max_50 = remplir_tab(parcours);
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <option> <fichier.csv>\n", argv[0]);
        return 1;
    }

    char option = argv[1][0];
    char *fichier_csv = argv[2];

    if (option == 's') {
        FILE *fichier = fopen(fichier_csv, "r");
        if (fichier == NULL) {
            fprintf(stderr, "Erreur d'ouverture du fichier.\n");
            return 1;
        }
    int id_trajet, distance;
    EtapeAVL *arbre = NULL;
    Trajet* pliste= NULL;
    pliste->next=NULL;
    //  Utilisation de malloc pour allouer de l'espace pour une etape
    EtapeAVL = (EtapeAVL *)malloc(sizeof(EtapeAVL));
    
    while (fscanf(fichier, "%d,%d", &id_trajet, &distance) == 2) {
            EtapeAVL *nouvelle_etape=newEtapeAVL(id_trajet,distance);
            pliste=insertPliste(pliste,nouvelle_etape);
       }
    while (tmp->next!=NULL){
    	arbre=insertAVL(arbre,tmp->noeud);
    }
    
    fclose(fichier);
    }
    return 0;
}
