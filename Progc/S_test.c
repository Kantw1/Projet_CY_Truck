// avl_sort.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour les données de trajet
struct Trajet {
    char id[50];
    double distance_mini;
    double distance_maxi;
    double distance_moyenne;
    struct Trajet* left;
    struct Trajet* right;
    int height;
};

// Fonction utilitaire pour calculer le maximum de deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction utilitaire pour obtenir la hauteur d'un nœud
int height(struct Trajet* N) {
    if (N == NULL) {
        return 0;
    }
    return N->height;
}

// Fonction pour créer un nouveau nœud
struct Trajet* newNode(char id[], double distance_mini, double distance_maxi, double distance_moyenne) {
    struct Trajet* node = (struct Trajet*)malloc(sizeof(struct Trajet));
    strcpy(node->id, id);
    node->distance_mini = distance_mini;
    node->distance_maxi = distance_maxi;
    node->distance_moyenne = distance_moyenne;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Rotation à droite
struct Trajet* rightRotate(struct Trajet* y) {
    struct Trajet* x = y->left;
    struct Trajet* T2 = x->right;

    // Effectuer la rotation
    x->right = y;
    y->left = T2;

    // Mettre à jour les hauteurs
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retourner le nouveau nœud racine
    return x;
}

// Rotation à gauche
struct Trajet* leftRotate(struct Trajet* x) {
    struct Trajet* y = x->right;
    struct Trajet* T2 = y->left;

    // Effectuer la rotation
    y->left = x;
    x->right = T2;

    // Mettre à jour les hauteurs
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retourner le nouveau nœud racine
    return y;
}

// Obtenir le facteur d'équilibre d'un nœud N
int getBalance(struct Trajet* N) {
    if (N == NULL) {
        return 0;
    }
    return height(N->left) - height(N->right);
}

// Insérer un nouveau nœud dans l'arbre AVL
struct Trajet* insert(struct Trajet* node, char id[], double distance_mini, double distance_maxi, double distance_moyenne) {
    // Effectuer l'insertion normale de l'arbre binaire de recherche
    if (node == NULL) {
        return newNode(id, distance_mini, distance_maxi, distance_moyenne);
    }

    if (strcmp(id, node->id) < 0) {
        node->left = insert(node->left, id, distance_mini, distance_maxi, distance_moyenne);
    } else if (strcmp(id, node->id) > 0) {
        node->right = insert(node->right, id, distance_mini, distance_maxi, distance_moyenne);
    } else {
        // Ignorer les entrées en double
        return node;
    }

    // Mettre à jour la hauteur du nœud actuel
    node->height = 1 + max(height(node->left), height(node->right));

    // Obtenir le facteur d'équilibre de ce nœud pour vérifier l'équilibre
    int balance = getBalance(node);

    // Cas de déséquilibre à gauche
    if (balance > 1) {
        if (strcmp(id, node->left->id) < 0) {
            return rightRotate(node);
        } else if (strcmp(id, node->left->id) > 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }

    // Cas de déséquilibre à droite
    if (balance < -1) {
        if (strcmp(id, node->right->id) > 0) {
            return leftRotate(node);
        } else if (strcmp(id, node->right->id) < 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    // Le nœud est équilibré
    return node;
}

// Parcours préfixe pour afficher les données triées
void preOrder(struct Trajet* root) {
    if (root != NULL) {
        preOrder(root->left);
        printf("%s %.2lf %.2lf %.2lf\n", root->id, root->distance_mini, root->distance_maxi, root->distance_moyenne);
        preOrder(root->right);
    }
}


// Nouvelle fonction pour le traitement statistique et la génération du graphique
void processStats(struct Trajet* root) {
    // Tableau pour stocker les données triées
    struct Trajet* sortedData[50];
    int currentIndex = 0;

    // Fonction auxiliaire pour parcourir l'arbre et remplir le tableau
    void fillSortedData(struct Trajet* node) {
        if (node != NULL && currentIndex < 50) {
            fillSortedData(node->right);
            sortedData[currentIndex++] = node;
            fillSortedData(node->left);
        }
    }

    // Remplir le tableau trié
    fillSortedData(root);

    // Afficher les statistiques et générer les données pour le graphique
    FILE* dataFile = fopen("graph_data.txt", "w");
    fprintf(dataFile, "#ID Distance_mini Distance_moyenne Distance_maxi\n");
    for (int i = 0; i < currentIndex; ++i) {
        fprintf(dataFile, "%s %.2lf %.2lf %.2lf\n", sortedData[i]->id, sortedData[i]->distance_mini, sortedData[i]->distance_moyenne, sortedData[i]->distance_maxi);
    }
    fclose(dataFile);
}

int main() {
    struct Trajet* root = NULL;
    char id[50];
    double distance_mini, distance_maxi, distance_moyenne;

    // Lire les données depuis l'entrée standard
    while (scanf("%49s %lf %lf %lf", id, &distance_mini, &distance_maxi, &distance_moyenne) == 4) {
        root = insert(root, id, distance_mini, distance_maxi, distance_moyenne);
    }

    // Appeler la fonction pour le traitement statistique et la génération du graphique
    processStats(root);

    return 0;
}
