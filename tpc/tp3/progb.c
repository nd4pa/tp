#include <stdio.h>
#define MAJORITE 18
int main(){	

int age=0; 		    // Déclarations
int majeur=0;

printf("Tapez votre age:"); //Lecture
scanf("%d",&age);

majeur=age>=MAJORITE;

if ( majeur ){		    // Boucle IF
	printf("Vous êtes majeur\n");
}else{
	printf("Vous êtes mineur\n");
}

return 0;
}
