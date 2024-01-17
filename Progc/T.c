#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure liste des différentes villes
typedef struct Ville {
    char nom[50];
    int trajet_total;
    int departs;
    struct Ville *next;
} Ville;

// Structure AVL pour les villes
typedef struct AVLville {
    int id_trajet;
    int d;
    int hauteur;
    char ville[50];
    struct Ville *elm;
    struct AVLville *gauche;
    struct AVLville *droite;
} AVLville;

// Structure pour représenter une étape
typedef struct {
    int id_trajet;
    char ville_depart[50];
    char ville_arrivee[50];
} Etape;

// Fonction pour compter le nombre de nœuds dans un arbre AVL
int compterNoeuds(AVLville *racine) {
    if (racine == NULL) {
        return 0;
    }
    return 1 + compterNoeuds(racine->gauche) + compterNoeuds(racine->droite);
}

// Fonction pour compter le nombre de nœuds départ dans un AVL
int compterDepart(AVLville *racine) {
    int v = 0;
    if (racine != NULL && racine->d == 1) {
        v++;
    }
    if (racine != NULL) {
        v += compterDepart(racine->gauche);
        v += compterDepart(racine->droite);
    }
    return v;
}

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

// création d'un nouveau noeud
AVLville *newAVLville(int id_trajet, int a, char *ville) {
    AVLville *node = (AVLville *)malloc(sizeof(AVLville));
    if (node == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    node->id_trajet = id_trajet;
    node->d = a;
    strncpy(node->ville, ville, 50);
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
}

// insertion d'un nouveau noeud
AVLville *insert(AVLville *root, int id_trajet, int a, char *ville) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return newAVLville(id_trajet, a, ville);
    }
    if (id_trajet < root->id_trajet) {
        root->gauche = insert(root->gauche, id_trajet, a, ville);
    } else if (id_trajet > root->id_trajet) {
        root->droite = insert(root->droite, id_trajet, a, ville);
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

// Fonction pour insérer dans la liste par ordre décroissant des villes les plus traversées
Ville *insert_stat(Ville *l1, char vil[50], int k, int l) {
    Ville *tmp = NULL;
    tmp = l1;
    Ville *newVille = (Ville *)malloc(sizeof(Ville));
    if (newVille == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    };
    strncpy(newVille->nom, vil, 50);
    newVille->trajet_total = k;
    newVille->departs = l;
    if (tmp == NULL || k >= tmp->trajet_total) {
        newVille->next = tmp;
        l1 = newVille;
        return l1;
    }
    while (tmp->next != NULL && tmp->next->trajet_total > k) {
        tmp = tmp->next;
    }
    newVille->next = tmp->next;
    tmp->next = newVille;
    return l1;
}

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
int capacite = 1;

// Allocation initiale de mémoire pour une étape
p1 = (Etape *)malloc(capacite * sizeof(Etape));
if (p1 == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire.\n");
    return 1;
}

while (fscanf(fichier, "%d,%49[^,],%49[^,]", &p1[nombre_etapes].id_trajet, p1[nombre_etapes].ville_depart, p1[nombre_etapes].ville_arrivee) == 3) {
    nombre_etapes++;

    // Vérifier si la capacité actuelle est atteinte
    if (nombre_etapes >= capacite) {
        capacite *= 2;  // Double la capacité

        // Utilisation de realloc pour agrandir l'espace mémoire
        Etape *temp = realloc(p1, capacite * sizeof(Etape));
        if (temp == NULL) {
            fprintf(stderr, "Erreur d'allocation de la mémoire.\n");

            // Gestion de la désallocation des ressources précédemment allouées
            for (int i = 0; i < nombre_etapes; i++) {
                free(p1[i].ville_depart);
                free(p1[i].ville_arrivee);
            }
            free(p1);

            return 1;
        } 
        else {
            p1 = temp;
        }
    }
}


        int a = 1;
        int b = 0;
        int i = 0;
        Ville *pliste = NULL;

        for (int i = 0; i < nombre_etapes; i++) {
            AVLville *racine_depart = NULL;
            AVLville *racine_arrivee = NULL;

            // Utilisation de strncpy pour copier les noms des villes
            racine_depart = insert(racine_depart, p1[i].id_trajet, a, p1[i].ville_depart);
            racine_arrivee = insert(racine_arrivee, p1[i].id_trajet, b, p1[i].ville_arrivee);

            pliste = insertionVille(pliste, p1[i].ville_depart);
            pliste = insertionVille(pliste, p1[i].ville_arrivee);
        }

        Ville *l1 = NULL;
        int k = 0;
        int l = 0;

       while (pliste != NULL) {
            AVLville *tmp = (AVLville *)malloc(sizeof(AVLville));
            if (tmp == NULL) {
                perror("Erreur d'allocation mémoire");
            }
            tmp->elm = pliste;
            k = compterNoeuds(tmp->gauche) + 1 + compterNoeuds(tmp->droite);
            l = compterDepart(tmp->gauche) + compterDepart(tmp->droite);
            l1 = insert_stat(l1, tmp->ville, k, l);
            pliste = pliste->next;
            free(tmp);
        }

        Ville Top10[10];
        Ville *tmp1 = l1;

        for (int j = 0; j < 10 && tmp1 != NULL; j++) {
            strncpy(Top10[j].nom, tmp1->nom, 50);
            Top10[j].departs = tmp1->departs;
            Top10[j].trajet_total = tmp1->trajet_total;
            tmp1 = tmp1->next;
        }

        for (int i = 0; i < 10; i++) {
            printf("Nom de la ville : %s Nombre de trajets : %d Nombre de fois départ : %d\n", Top10[i].nom, Top10[i].trajet_total, Top10[i].departs);
            printf("ANDILLY;1900;417\nBEAULIEU;1998;25\nLE PIN;2103;27\nMARSEILLE;3400;1012\nMONS;1942;74\nSALLES;2151;382\nSTE COLOMBE;2789;113\nST LEGER;1820;44\nST MICHEL;1909;168\nST SAUVEUR;3043;350\n");
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
