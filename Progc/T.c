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
    if (nouvelle_etape->conducteur < root->conducteur) {
        root->gauche = insertAVLNode(root->gauche, nouvelle_etape);
    } else if (nouvelle_etape->conducteur > root->conducteur) {
        root->droite = insertAVLNode(root->droite, nouvelle_etape);
    }

    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'équilibre du noeud
    int balance = getBalance(root);

    // Cas de déséquilibre à gauche
    if (balance > 1) {
        if (nouvelle_etape->conducteur < root->gauche->conducteur) {
            return rotateRight(root);
        } else if (nouvelle_etape->conducteur >= root->gauche->conducteur) {
            root->gauche = rotateLeft(root->gauche);
            return rotateRight(root);
        }
    }

    // Cas de déséquilibre à droite
    if (balance < -1) {
        if (nouvelle_etape->conducteur >= root->droite->conducteur ){
            return rotateLeft(root);
        } else if (nouvelle_etape->conducteur < root->droite->conducteur) {
            root->droite = rotateRight(root->droite);
            return rotateLeft(root);
        }
    }

    return root;
}


int conducteurExiste(VilleAVL *racine, char conducteur[]) {
    if (racine == NULL) {
        return 0; // Le conducteur n'existe pas dans l'AVL
    }

    int comparaison = strcmp(conducteur, racine->conducteur->conducteur);

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
    node->conducteur = insertAVLNode(node->conducteur,Newone);
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
    if (!conducteurExiste(root->noeud,nouvelle_etape->conducteur->conducteur)){
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
        if (tmp->end->noeud->ville == nouvelle_etape->ville) {
            tmp = modifierTrajet(tmp->end, nouvelle_etape);
            free(newNode); // Libérer le nœud nouvellement alloué car il n'est pas nécessaire
            return pliste; // Pas besoin d'ajouter un nouveau nœud à la liste
    }

    tmp->end->next = newNode;
    tmp->end = tmp->end->next;
    return pliste;
}

int main(){
    FILE *fichier = fopen("Temp/resultat_T2.txt", "r");
    if (fichier == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    }
    int id_trajet;
    char nom[50];
    char ville[50];
    VilleAVL *arbre = NULL;
    Trajet *pliste = NULL; // Initialisez votre liste à NULL
    Trajet *tmp = pliste;
      while (fscanf(fichier, "%49[^;];%49[^;]", ville, nom) == 2){ 
        printf("info2\n");
    	VilleAVL *nouvelle_etape = newVilleAVL(ville,nom);
    	pliste = insertPliste(pliste, nouvelle_etape);
        printf("info\n");
    }
    printf("reussi");
    //arbre = insertAVLFromList(pliste, arbre);
    fclose(fichier);
    return 0;
}