#include <stdio.h>
#include <stdling.h>
#include <string.h>

#define MAX_FICHIERS 10
#define TAILLE_MAX 10

//representer un enregistrement:
typedef struct{
    int id;
    char nom[50];
}Enregistrement ;

//  BLOC:
//Pour l'organisation chainee :
typedef struct Bloc{
    Enregistrement enregistrement;
    struct Bloc*suivant;
} Bloc;
//organisation contigue:
typedef struct {
    Enregistrement enregistrements[10]; 
    int taille_actuelle;                
}FichierContigue;

//FICHIER:
//chainee:
typedef struct {
    Bloc *tete; 
}   FichierChaine;
//contigue:
typedef struct {
    Enregistrement enregistrements[10]; 
    int taille_max;                      
    int taille_actuelle;                
} FichierContigue;

//MS:
typedef struct {
    FichierChaine fichiers_chaînes[MAX_FICHIERS]; 
    FichierContigue fichiers_contigus[MAX_FICHIERS]; 
    int nombre_fichiers;
} MemoireSecondaire;

//organisastion contigue:
void initialiserFichierContigu(FichierContigue*fichier){
    fichier->taille_actuelle = 0;
}
int ajouter_enregistrement_contigu(FichierContigue*fichier, int id, const char *nom){
     if (fichier->taille_actuelle >= TAILLE_MAX) {
        printf("Erreur : Espace insuffisant dans le fichier contigu.\n");
        return 0; // Échec
    }
    fichier->enregistrements[fichier->taille_actuelle].id = id;
    strcpy(fichier->enregistrements[fichier->taille_actuelle].nom, nom);
    fichier->taille_actuelle++;
    return 1; 
}
void liberer_fichier_contigu(FichierContigue *fichier) {
    fichier->taille_actuelle = 0;
}

void afficher_fichier_contigu(const FichierContigue *fichier) {
    if (fichier->taille_actuelle == 0) {
        printf("Le fichier est vide.\n");
    }
    for (int i = 0; i < fichier->taille_actuelle; i++) {
        printf("Enregistrement %d: ID=%d, Nom=%s\n", i + 1, fichier->enregistrements[i].id, fichier->enregistrements[i].nom);
    }
}
void compactage_fichier_contigu(FichierContigue *fichier) {
    int i, j = 0;
    for (i = 0; i < fichier->taille_actuelle; i++) {
        if (fichier->enregistrements[i].id != 0) { 
            fichier->enregistrements[j] = fichier->enregistrements[i];
            j++;
        }
    }
    fichier->taille_actuelle = j; 
}
void defragmentation_fichier_contigu(FichierContigue *fichier) {
    int j = 0;

    for (int i = 0; i < fichier->taille_actuelle; i++) {
        if (fichier->enregistrements[i].id != 0) { 
            fichier->enregistrements[j] = fichier->enregistrements[i];
            j++;
        }
    }
    fichier->taille_actuelle = j; 
}


//0rganisation chainee:
void initialiser_fichier_chaine(FichierChaine *fichier) {
    fichier->tete = NULL;
}
void ajouter_enregistrement_chaine(FichierChaine *fichier, int id, const char *nom) {
    Bloc *nouveau_bloc = (Bloc *)malloc(sizeof(Bloc));
    if (nouveau_bloc == NULL) {
        printf("Erreur : Espace memoire insuffisant pour allouer un nouveau bloc.\n");
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
void liberer_fichier_chaine(FichierChaine *fichier) {
    Bloc *courant = fichier->tete;
    while (courant != NULL) {
        Bloc *temp = courant;
        courant = courant->suivant;
        free(temp); 
    }
    fichier->tete = NULL; 
}
void afficher_fichier_chaine(const FichierChaine *fichier) {
     if (fichier->tete == NULL) {
        printf("Le fichier chaîné est vide.\n");
        return;
    }
    Bloc *courant = fichier->tete;
    while (courant != NULL) {
        printf("Enregistrement: ID=%d, Nom=%s\n", courant->enregistrement.id, courant->enregistrement.nom);
        courant = courant->suivant; 
    }
}
void compactage_fichier_chaine(FichierChaine *fichier) {
    Bloc *courant = fichier->tete;
    Bloc *nouvelle_tete = NULL;
    Bloc *dernier_bloc = NULL;   

    while (courant != NULL) {
        Bloc *nouveau_bloc = (Bloc *)malloc(sizeof(Bloc));
        if (nouveau_bloc == NULL) {
            printf("Erreur : Espace mémoire insuffisant pour allouer un nouveau bloc lors du compactage.\n");
            return; 
        }
        nouveau_bloc->enregistrement = courant->enregistrement;
        nouveau_bloc->suivant = NULL;

        if (nouvelle_tete == NULL) {
            nouvelle_tete = nouveau_bloc;
        } else {
            dernier_bloc->suivant = nouveau_bloc; 
        }
        dernier_bloc = nouveau_bloc; 
        courant = courant->suivant;  
    }
    liberer_fichier_chaine(fichier);
    fichier->tete = nouvelle_tete;
}
void defragmentation_fichier_chaine(FichierChaine *fichier) {
    Bloc *courant = fichier->tete;
    Bloc *nouvelle_tete = NULL; 
    Bloc *dernier_bloc = NULL;  

    while (courant != NULL) {
       
        Bloc *nouveau_bloc = (Bloc *)malloc(sizeof(Bloc));
        if (nouveau_bloc == NULL) {
            printf("Erreur : Espace mémoire insuffisant pour allouer un nouveau bloc lors de la défragmentation.\n");
            return; 
        }
        nouveau_bloc->enregistrement = courant->enregistrement;
        nouveau_bloc->suivant = NULL;

        if (nouvelle_tete == NULL) {
            nouvelle_tete = nouveau_bloc; 
        } else {
            dernier_bloc->suivant = nouveau_bloc; 
        }
        dernier_bloc = nouveau_bloc; 
        courant = courant->suivant;
    } 
    liberer_fichier_chaine(fichier);
    fichier ->tete=nouvelle_tete;
}

void initialiser_fichier_contigu(FichierContigue *fichier);
void initialiser_fichier_chaine(FichierChaine *fichier);
int ajouter_enregistrement_contigu(FichierContigue *fichier, int id, const char *nom);
void liberer_fichier_contigu(FichierContigue *fichier);
void afficher_fichier_contigu(const FichierContigue *fichier);
void defragmentation_fichier_contigu(FichierContigue *fichier);
void ajouter_enregistrement_chaine(FichierChaine *fichier, int id, const char *nom);
void liberer_fichier_chaine(FichierChaine *fichier);
void afficher_fichier_chaine(const FichierChaine *fichier);
void defragmentation_fichier_chaine(FichierChaine *fichier);
void afficher_etat_memoire(FichierContigue *fichier_contigu, FichierChaine *fichier_chaine);
    













