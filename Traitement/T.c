#include <stdio.h>
#include <stdlib.h>

// Structure AVL pour les villes
typedef struct AVLville {
    int id_trajet;
    int d;
    int hauteur;
    char ville[50];
    struct AVLville *gauche;
    struct AVLville *droite;
} AVLville;

// fonction usuelle pour avoir le max entre deux données
int max(int a, int b) {
    return (a > b) ? a : b;
}

// fonction pour calculer hauteur d'un arbre
int height(AVLville *node) {
    if (node == NULL) {
        return 0; // La hauteur d'un nœud vide est 0
    } else {
        int gauche_height = height(node->gauche);
        int droite_height = height(node->droite);

        return 1 + max(gauche_height, droite_height);
    }
}

// création d'un nouveau noeud
AVLville *newAVLville(int id_trajet, int a) {
    AVLville *node = (AVLville *)malloc(sizeof(AVLville));
    if (node == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    node->id_trajet = id_trajet;
    node->d = a;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// insertion d'un nouveau noeud
AVLville *insert(AVLville *root, int id_trajet, int a) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return newAVLville(id_trajet, a);
    }
    if (id_trajet < root->id_trajet) {
        root->gauche = insert(root->gauche, id_trajet, a);
    } else if (id_trajet > root->id_trajet) {
        root->droite = insert(root->droite, id_trajet, a);
    } else {
        return root;
    }
    // Mettre à jour la hauteur du nœud actuel
    root->hauteur = 1 + max(height(root->gauche), height(root->droite));

    // Obtenir le facteur d'équilibre du nœud
    int balance = getBalance(root);

    // Cas de déséquilibre à gauche
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
AVLville *rotateRight(AVLville *y) {
    AVLville *x = y->gauche;
    AVLville *T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;

    return x;
}

// rotation gauche de l'AVL
AVLville *rotateLeft(AVLville *x) {
    AVLville *y = x->droite;
    AVLville *T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = max(height(x->gauche), height(x->droite)) + 1;
    y->hauteur = max(height(y->gauche), height(y->droite)) + 1;

    return y;
}

// l'AVL est-il équilibré
int getBalance(AVLville *node) {
    if (node == NULL) {
        return 0; // Le facteur d'équilibre d'un nœud vide est 0
    } else {
        return height(node->gauche) - height(node->droite);
    }
}

// Structure liste des différentes villes
typedef struct Ville {
    char nom[50];
    int trajet_total;
    int departs;
    struct Ville *next;
} Ville;

// Structure pour représenter une étape
typedef struct {
    int id_trajet;
    char ville_depart[50];
    char ville_arrivee[50];
} Etape;

// Structure pour représenter une distance d'une étape
typedef struct {
    int id_trajet;
    int id_etape;
    int distance;
} Distance;

// Fonction pour insérer une ville dans la liste des villes
Ville *insertionVille(Ville *pliste, char vil[50]) {
    Ville *nouvelleVille = (Ville *)malloc(sizeof(Ville));
    if (nouvelleVille == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    strncpy(nouvelleVille->nom, vil, 50);
    nouvelleVille->next = pliste;
    return nouvelleVille;
}

// Fonction pour insérer dans la liste par ordre décroissant des villes les plus traversées
Ville *insert_stat(Ville *l1, char vil[50], int k, int l) {
    Ville *tmp = NULL;
    tmp = l1;
    Ville *newVille = (Ville *)malloc(sizeof(Ville));
    if (newVille == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    };
    strcpy(newVille->nom, vil);
    newVille->trajet_total = k;
    newVille->departs = l;
    if (tmp == NULL) {
        newVille->next = NULL;
        l1 = newVille;
        return l1;
    }
    if (l1 != NULL && k >= l1->trajet_total) {
        newVille->next = l1->next;
        l1->next = newVille;
        return l1;
    }
    while (tmp->next != NULL && tmp->next->trajet_total > k) {
        tmp = tmp->next;
    }
    newVille->next = tmp->next;
    tmp->next = newVille;
    return l1;
}

// Fonction pour compter le nombre de villes différents dans une liste
int nombre_chainon(Ville *pliste) {
    int v = 0;
    while (pliste != NULL) {
        v++;
        pliste = pliste->next;
    }
    return v;
}

// Fonction pour compter le nombre de nœuds départ dans un AVL
int compterDépart(AVLville *racine) {
    int v = 0;
    if (racine != NULL && racine->d == 1) {
        v++;
        v += compterDépart(racine->gauche);
        v += compterDépart(racine->droite);
    } else {
        v += compterDépart(racine->gauche);
        v += compterDépart(racine->droite);
    }
    return v;
}

// Fonction pour compter le nombre de nœuds dans un arbre AVL
int compterNoeuds(AVLville *racine) {
    if (racine == NULL) {
        return 0;
    }
    return 1 + compterNoeuds(racine->gauche) + compterNoeuds(racine->droite);
}

// Fonction pour désallouer la liste des villes
void desalouerVille(Ville *l1) {
    while (l1 != NULL) {
        Ville *aLiberer = l1;
        l1 = l1->next;
        free(aLiberer);
    }
}

// Fonction pour désallouer la liste des étapes
void desalouerEtapes(Etape *p1) {
    free(p1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <option> <fichier.csv>\n", argv[0]);
        return 1;
    }

    char option = argv[1][0];
    char *fichier_csv = argv[2];

    if (option == 't') {
        FILE *fichier = fopen(fichier_csv, "r");
        if (fichier == NULL) {
            fprintf(stderr, "Erreur d'ouverture du fichier.\n");
            return 1;
        }

        Etape *p1 = NULL;
        int nombre_etapes = 0;

        //  Utilisation de malloc pour allouer de l'espace pour une étape
        p1 = (Etape *)malloc(sizeof(Etape));

        while (fscanf(fichier, "%d,%49[^,],%49[^,]", &p1[nombre_etapes].id_trajet, p1[nombre_etapes].ville_depart, p1[nombre_etapes].ville_arrivee) != EOF) {
            nombre_etapes++;

            // Utilisation de realloc pour agrandir l'espace mémoire
            p1 = realloc(p1, (nombre_etapes + 1) * sizeof(Etape));
            if (p1 == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire.\n");
                return 1;
            }
        }

        int a = 1;
        int b = 0;
        int i = 0;
        int j = 0;
        Ville *pliste = NULL;

        for (int i = 0; i < nombre_etapes; i++) {
            AVLville *racine_depart = NULL;
            AVLville *racine_arrivee = NULL;

            // Utilisation de strncpy pour copier les noms des villes
            strncpy(racine_depart->ville, p1[i].ville_depart, 50);
            strncpy(racine_arrivee->ville, p1[i].ville_arrivee, 50);

            racine_depart = insert(racine_depart, p1[i].id_trajet, a);
            racine_arrivee = insert(racine_arrivee, p1[i].id_trajet, b);

            pliste = insertionVille(pliste, racine_depart->ville);
            pliste = insertionVille(pliste, racine_arrivee->ville);
        }

        Ville *l1 = NULL;
        int k = 0;
        int l = 0;

        while (pliste != NULL) {
            Ville *tmp = pliste;
            k = compterNoeuds(tmp->ville);
            l = compterDépart(tmp->ville);
            l1 = insert_stat(l1, tmp->nom, k, l);
            tmp = tmp->next;
        }

        Ville Top10[10];
        Ville *tmp1 = l1;

        for (int j = 0; j < 10 && tmp1 != NULL; j++) {
            strcpy(Top10[j].nom, tmp1->nom);
            Top10[j].departs = tmp1->departs;
            Top10[j].trajet_total = tmp1->trajet_total;
            tmp1 = tmp1->next;
        }

        for (int i = 0; i < 10; i++) {
            printf("nom de la ville : %s nombre de trajets : %d nombre de fois départ : %d\n", Top10[i].nom, Top10[i].trajet_total, Top10[i].departs);
        }

        // Désallocation des ressources
        desalouerVille(pliste);
        desalouerVille(l1);
        desalouerEtapes(p1);

        fclose(fichier);
        return 0;
    }
    return 0;
}
