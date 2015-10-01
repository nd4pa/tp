#include <stdio.h>

int main(){

//---------------------
//Déclaration
//---------------------

int mois;
int annee;
int nbr_jours_m02;

//---------------------
//Saisie
//---------------------

printf("Entrez le mois et l'année sous le format MM/AAAA\n");
scanf("%d/%d",&mois,&annee);

//---------------------
//Test retour de saisie
//---------------------

//printf("%d/%d\n",mois,annee);

//---------------------
//Traitement
//---------------------

if( mois >= 1 && mois <= 12 && annee > 0 ){
	
	int m=mois;
	
	int bissextile = (annee % 4 == 0) && (annee % 100 != 0) || ( annee % 400 == 0);
	
	//-----------------------------------------------------------
	//Test condition bissextile + choix du nbr jours pour mois 02
	//-----------------------------------------------------------

	if ( bissextile ){
		nbr_jours_m02 = 29;
		//printf("cette année est bissextile\n");
	}else{
		nbr_jours_m02 = 28;
		//printf("cette année n'est pas bissextile\n");
	}
	//-------------------	
	//Suite du traitement
	//-------------------

	switch(m){
		case 1:case 3:case 5:case 7:case 8: case 10:case 12:
			printf("il y a 31 jours dans ce mois\n");
		break;
		case 2: 
			printf("il y a %d jours dans ce mois\n",nbr_jours_m02);
		break;
		case 4:case 6:case 9:case 11:
			printf("il y a 30 jours dans ce mois\n");
	}
}	
return 0;
}
