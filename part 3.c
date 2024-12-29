#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FICHIERS 10
#define TAILLE_MAX 10

// Structures
typedef struct {
    int id;
    char nom[50];
} Enregistrement;

// Organisation contiguë
typedef struct {
    Enregistrement enregistrements[TAILLE_MAX];
    int taille_actuelle;
} FichierContigue;

// Organisation chaînée
typedef struct Bloc {
    Enregistrement enregistrement;
    struct Bloc *suivant;
} Bloc;

typedef struct {
    Bloc *tete;
} FichierChaine;

// Mémoire secondaire
typedef struct {
    FichierContigue fichiers_contigus[MAX_FICHIERS];
    FichierChaine fichiers_chaînes[MAX_FICHIERS];
    int nombre_fichiers;
} MemoireSecondaire;

// Fonctions pour organisation contiguë
void initialiser_fichier_contigu(FichierContigue *fichier) {
    fichier->taille_actuelle = 0;
}

int ajouter_enregistrement_contigu(FichierContigue *fichier, int id, const char *nom) {
    if (fichier->taille_actuelle >= TAILLE_MAX) {
        printf("Erreur : Espace insuffisant dans le fichier contigu.\n");
        return 0;
    }
    fichier->enregistrements[fichier->taille_actuelle].id = id;
    strcpy(fichier->enregistrements[fichier->taille_actuelle].nom, nom);
    fichier->taille_actuelle++;
    return 1;
}

void afficher_fichier_contigu(const FichierContigue *fichier) {
    if (fichier->taille_actuelle == 0) {
        printf("Le fichier contigu est vide.\n");
    } else {
        for (int i = 0; i < fichier->taille_actuelle; i++) {
            printf("Enregistrement %d: ID=%d, Nom=%s\n", i + 1, fichier->enregistrements[i].id, fichier->enregistrements[i].nom);
        }
    }
}

// Fonctions pour organisation chaînée
void initialiser_fichier_chaine(FichierChaine *fichier) {
    fichier->tete = NULL;
}

void ajouter_enregistrement_chaine(FichierChaine *fichier, int id, const char *nom) {
    Bloc *nouveau_bloc = (Bloc *)malloc(sizeof(Bloc));
    if (nouveau_bloc == NULL) {
        printf("Erreur : Espace mémoire insuffisant.\n");
        return;
    }
    nouveau_bloc->enregistrement.id = id;
    strcpy(nouveau_bloc->enregistrement.nom, nom);
    nouveau_bloc->suivant = NULL;

    if (fichier->tete == NULL) {
        fichier->tete = nouveau_bloc;
    } else {
        Bloc *courant = fichier->tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau_bloc;
    }
}

void afficher_fichier_chaine(const FichierChaine *fichier) {
    if (fichier->tete == NULL) {
        printf("Le fichier chaîné est vide.\n");
    } else {
        Bloc *courant = fichier->tete;
        while (courant != NULL) {
            printf("Enregistrement: ID=%d, Nom=%s\n", courant->enregistrement.id, courant->enregistrement.nom);
            courant = courant->suivant;
        }
    }
}

// Création des fichiers
void creer_fichier(MemoireSecondaire *memoire) {
    if (memoire->nombre_fichiers >= MAX_FICHIERS) {
        printf("Erreur : Nombre maximal de fichiers atteint.\n");
        return;
    }

    char type[10];
    printf("Type d'organisation (contigue/chainee) : ");
    scanf("%s", type);

    if (strcmp(type, "contigue") == 0) {
        initialiser_fichier_contigu(&memoire->fichiers_contigus[memoire->nombre_fichiers]);
        printf("Fichier contigu créé avec succès.\n");
    } else if (strcmp(type, "chainee") == 0) {
        initialiser_fichier_chaine(&memoire->fichiers_chaînes[memoire->nombre_fichiers]);
        printf("Fichier chaîné créé avec succès.\n");
    } else {
        printf("Type d'organisation invalide.\n");
        return;
    }

    memoire->nombre_fichiers++;
}

// Insertion d'enregistrements
void inserer_enregistrement(MemoireSecondaire *memoire, int fichier_index, const char *type, int id, const char *nom) {
    if (fichier_index >= memoire->nombre_fichiers) {
        printf("Erreur : Index de fichier invalide.\n");
        return;
    }

    if (strcmp(type, "contigue") == 0) {
        ajouter_enregistrement_contigu(&memoire->fichiers_contigus[fichier_index], id, nom);
    } else if (strcmp(type, "chainee") == 0) {
        ajouter_enregistrement_chaine(&memoire->fichiers_chaînes[fichier_index], id, nom);
    } else {
        printf("Type d'organisation invalide.\n");
    }
}

// Fonction principale
int main() {
    MemoireSecondaire memoire = {0};

    int choix;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Créer un fichier\n");
        printf("2. Insérer un enregistrement\n");
        printf("3. Afficher les fichiers\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                creer_fichier(&memoire);
                break;
            case 2: {
                int fichier_index, id;
                char type[10], nom[50];
                printf("Index du fichier : ");
                scanf("%d", &fichier_index);
                printf("Type d'organisation (contigue/chainee) : ");
                scanf("%s", type);
                printf("ID : ");
                scanf("%d", &id);
                printf("Nom : ");
                scanf("%s", nom);
                inserer_enregistrement(&memoire, fichier_index, type, id, nom);
                break;
            }
            case 3: {
                for (int i = 0; i < memoire.nombre_fichiers; i++) {
                    printf("\nFichier %d :\n", i);
                    printf("Organisation contiguë :\n");
                    afficher_fichier_contigu(&memoire.fichiers_contigus[i]);
                    printf("Organisation chaînée :\n");
                    afficher_fichier_chaine(&memoire.fichiers_chaînes[i]);
                }
                break;
            }
            case 4:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 4);

    return 0;