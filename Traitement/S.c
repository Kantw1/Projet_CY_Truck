#include <stdio.h>
#include <stdlib.h>

typedef struct EtapeAVL {
    int id_trajet;
    int hauteur;
    struct EtapeAVL *gauche;
    struct EtapeAVL *droite;
} EtapeAVL;

typedef struct Trajet{
    int id_trajet;
    int distance_min;
    int distance_max;
    int distance_totale;
    struct Trajet *next;
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
EtapeAVL *newEtapeAVL(int id_trajet) {
    EtapeAVL *node = (EtapeAVL *)malloc(sizeof(EtapeAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    node->id_trajet = id_trajet;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// insertion d'un nouveau noeud
EtapeAVL *insert(EtapeAVL *root, int id_trajet) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return newEtapeAVL(id_trajet, a);
    }
    if (id_trajet < root->id_trajet) {
        root->gauche = insert(root->gauche, id_trajet, a);
    } else if (id_trajet > root->id_trajet) {
        root->droite = insert(root->droite, id_trajet, a);
    } else {
        return root;
    }
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'equilibre du noeud
    int balance = getBalance(root);

    // Cas de desequilibre à gauche
    if (balance > 1) {
        if (id_trajet < root->gauche->id_trajet) {
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
    int id_trajet, distance_min, distance_max, distance_totale;
    EtapeAVL *arbre = NULL;

    //  Utilisation de malloc pour allouer de l'espace pour une etape
    EtapeAVL = (EtapeAVL *)malloc(sizeof(EtapeAVL));

     while (fscanf(fichier, "%d,%d,%d,%d", &id_trajet, &distance_min, &distance_max, &distance_totale) == 4) {
            // Utilisation de malloc pour allouer de l'espace pour un nouveau noeud
            EtapeAVL *nouvelle_etape = (EtapeAVL *)malloc(sizeof(EtapeAVL));
            if (nouvelle_etape == NULL) {
                perror("Erreur d'allocation memoire");
                exit(EXIT_FAILURE);
            }

            // Initialisation du nouveau noeud avec les donnees lues depuis le fichier
            nouvelle_etape->id_trajet = id_trajet;
            nouvelle_etape->hauteur = 1;  // Nouveau noeud, hauteur initiale est 1
            nouvelle_etape->gauche = NULL;
            nouvelle_etape->droite = NULL;

            // Insertion de la nouvelle etape dans l'arbre AVL
            arbre = insert(arbre, nouvelle_etape);

            // Liberation de l'espace memoire utilise par la structure temporaire
            free(nouvelle_etape);
        }

        fclose(fichier);
    
