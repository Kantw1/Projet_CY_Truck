#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

//Structure AVL qui contient les ID des trajets
typedef struct conducteurAVL {
    int hauteur;
    int ID;
    struct conducteurAVL * gauche;
    struct conducteurAVL * droite;
}conducteurAVL;

typedef struct VilleAVL {
int hauteur;
int nb_passage_ville;
int nb_passage_ville_depart;
char ville[50];
struct conducteurAVL * ID;
//struct conducteurAVL * ID_depart;
struct VilleAVL *gauche;
struct VilleAVL *droite;
} VilleAVL;
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
conducteurAVL *insertAVLNode(conducteurAVL *root, int ID) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        return newconducteurAVL(ID);
    }
    if (ID< root->ID) {
        root->gauche = insertAVLNode(root->gauche, ID);
    } 
    else if (ID > root->ID) {
        root->droite = insertAVLNode(root->droite, ID);
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

VilleAVL *newVilleAVL(char ville[]) {
    VilleAVL *node = (VilleAVL *)malloc(sizeof(VilleAVL));
    if (node == NULL) {
        perror("Erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    strcpy(node->ville, ville);
    node->ID = NULL;
    node->ID = NULL;
    node->nb_passage_ville = 1; // à voir
    node->nb_passage_ville_depart = 0;
    node->gauche = NULL;
    node->droite = NULL;
    node->hauteur = 1;
    return node;
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

// Fonction pour insérer un nouveau noeud dans l'arbre AVL
VilleAVL *insertAVLNode_Ville(VilleAVL *root, char ville[],int ID, int info_depart) {
    // Effectuer l'insertion de manière normale
    if (root == NULL) {
        VilleAVL * nouvelle_etape = newVilleAVL(ville);
        nouvelle_etape->ID = insertAVLNode(nouvelle_etape->ID, ID);
        if (info_depart == 1){
            nouvelle_etape->nb_passage_ville_depart = 1;
        }
        return nouvelle_etape;
    }
    if (strcmp(ville, root->ville) == 0){
        if ((info_depart == 1)){
                    root->nb_passage_ville_depart ++;
                }
        if (conducteurExiste(root->ID, ID) == 0){
                root->nb_passage_ville ++;
                root->ID = insertAVLNode(root->ID, ID);
            }
        return root;
    }

    if (strcmp(ville,root->ville) < 0) {
        root->gauche = insertAVLNode_Ville(root->gauche, ville,ID, info_depart);
    } else if (strcmp(ville,root->ville) > 0) {
        root->droite = insertAVLNode_Ville(root->droite, ville,ID, info_depart);
    }
    // Mettre à jour la hauteur du noeud actuel
    root->hauteur = 1 + max(height_Ville(root->gauche), height_Ville(root->droite));

    // Obtenir le facteur d'équilibre du noeud
    int balance = getBalance_Ville(root);

    // Rotation gauche
    if (balance > 1) {
        if (getBalance_Ville(root->gauche) >= 0) {
            return rotateRight_Ville(root);
        } else {
            root->gauche = rotateLeft_Ville(root->gauche);
            return rotateRight_Ville(root);
        }
    }

    // Rotation droite
    if (balance < -1) {
        if (getBalance_Ville(root->droite) <= 0) {
            return rotateLeft_Ville(root);
        } else {
            root->droite = rotateRight_Ville(root->droite);
            return rotateLeft_Ville(root);
    }
    }
    return root;   
}

int main(){
    FILE *fichier = fopen("Temp/resultat_T.txt", "r");
    if (fichier == NULL) {
    	fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return 1;
    }
    int ID;
    int Step_ID;
    char ville_depart[50];
    char ville_arrivee[50];
    VilleAVL *arbre = NULL;
    VilleAVL *nouvelle_etape = NULL;
      while (fscanf(fichier, "%d;%d;%49[^;];%49[^\n]\n", &ID, &Step_ID, ville_depart, ville_arrivee) == 4 ){ 
        printf("%s : %s\n", ville_depart, ville_arrivee);
        arbre = insertAVLNode_Ville(arbre,ville_depart,ID, Step_ID);
        Step_ID = 2;
        arbre = insertAVLNode_Ville(arbre,ville_arrivee,ID, Step_ID);
      }
    fclose(fichier);
    return 0;
}