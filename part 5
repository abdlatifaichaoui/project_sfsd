#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BLOCS 100

typedef struct MetaDonnees{
	char nomFichier[50];
	int tailleFichier;
	int nbBlocsUtilises;
	int nbEnregistrements;
	int adressePremierBloc;
} MetaDonnees;
typedef  struct Bloc{
	int id;
	char donnees[256];
	int suivant;
} Bloc;

//Lire les metadonnes
void lireMetaDonnees(FILE*f, MetaDonnees*meta){
	rewind(f);
	fread(meta,sizeof(MetaDonnees),1,f);
}
//mettre a jour les metadonnees
void majMetaDonnees(FILE*f,MetaDonnees*meta){
	rewind(f);
	fwrite(meta,sizeof(MetaDonnees),1,f);
}
//creation d'un fichier de metadonnees
void creeMetaDonnees(FILE*f,const char *nomFichier,int tailleFichier,int nbBlocs,int adressePremierBloc){
	MetaDonnees meta;
	strcpy(meta.nomFichier, nomFichier);
	meta.tailleFichier=tailleFichier;
	meta.nbBlocsUtilises=nbBlocs;
	meta.nbEnregistrements=0;
	meta.adressePremierBloc=adressePremierBloc;
	
	majMetaDonnees(f,&meta);
	}
	
	//affichage des metadonnees
	void afficherMetaDonnees(const MetaDonnees*meta){
	
	
	printf("Nom du fichier :%s\n",meta ->nomFichier);
	printf("Taille totale du fichier:%d octets\n",meta->tailleFichier);
	printf("Nombre de blocs utilisee:%d\n",meta->nbEnregistrements);
	printf("Adresse du premier bloc :%d\n",meta->adressePremierBloc);}
	
	void ajouterEnregistrement(FILE*fMeta,FILE*fBlocs,const char *donnee,int tailleDonnee){
		MetaDonnees meta;
		lireMetaDonnees(fMeta,&meta);
		Bloc bloc;
		fseek(fBlocs,meta.adressePremierBloc*sizeof(Bloc),SEEK_SET);
		fread(&bloc,sizeof(Bloc),1,fBlocs);
		//si le bloc courant est plein ajouter un nv
		while(strlen(bloc.donnees)+tailleDonnee>sizeof(bloc.donnees)){
			if(bloc.suivant==-1){
				bloc.suivant==meta.nbBlocsUtilises;
				fseek(fBlocs,meta.adressePremierBloc*sizeof(Bloc),SEEK_SET);
				fwrite(&bloc,sizeof(Bloc),1,fBlocs);
				
				Bloc nouveauBloc={meta.nbBlocsUtilises, "",-1};
				meta.nbBlocsUtilises++;
				fseek(fBlocs, nouveauBloc.id * sizeof(Bloc),SEEK_SET);
				fwrite(&nouveauBloc,sizeof(Bloc),1,fBlocs);
				bloc = nouveauBloc;
			} else{
				fseek(fBlocs,bloc.suivant*sizeof(Bloc),SEEK_SET);
				fread(&bloc,sizeof(Bloc),1,fBlocs);
			}
			
		}
		
		strcat(bloc.donnees,donnee);
		strcat(bloc.donnees, "\n");
		fseek(fBlocs,bloc.id*sizeof(Bloc),SEEK_SET);
		fwrite(&bloc,sizeof(Bloc),1,fBlocs);
		meta.nbEnregistrements++;
		majMetaDonnees(fMeta,&meta);
		}
		int main(){
			FILE*fMeta=fopen("MetaDonnees.bin","wb+");
			FILE*fBlocs=fopen("Bloc.bin","wb+");
			if(!fMeta|| !fBlocs){
				printf("erreur");
				return 1;
			}
			creeMetaDonnees(fMeta,"FichierDonnees.bin",1024,1,0);
			MetaDonnees meta;
			lireMetaDonnees(fMeta,&meta);
			afficherMetaDonnees(&meta);
			ajouterEnregistrement(fMeta,fBlocs,"Produit A",9);
			ajouterEnregistrement(fMeta,fBlocs,"Produit B",9);
			lireMetaDonnees(fMeta,&meta);
			afficherMetaDonnees(&meta);
			fclose(fMeta);
			fclose(fBlocs);
			return 0;
			
			
		}
	
	
	
