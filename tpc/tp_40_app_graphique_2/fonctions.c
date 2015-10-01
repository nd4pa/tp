#include "header.h"

SDL_Surface *init(char * imgFond_filename, SDL_Surface **pFond)
{
	//Prologue
	SDL_Surface *fond = *pFond;
	
	//	Initialisation de la SDL (Avec test)
	if( SDL_Init ( SDL_INIT_VIDEO ) == -1 )
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//	Chargement de l'image de fond(récupération
	//	dimension fenêtre)
	fond= SDL_LoadBMP(imgFond_filename);
	if ( fond == NULL )
	{
		fprintf(stderr, "Impossible de charger le fichier bmp : %s\n", SDL_GetError());
	}


	//	Création de la fenêtre mère (Avec test)
	SDL_Surface *ecran = NULL;
	ecran = SDL_SetVideoMode(fond->w,fond->h,32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("XRoach",NULL);
	if(ecran == NULL)
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

		SDL_Rect rect_fond;
		rect_fond.x = 0;
		rect_fond.y = 0;
		SDL_BlitSurface(fond, NULL, ecran, &rect_fond);
		SDL_Flip(ecran);

	//Epilogue
	*pFond = fond;

	return ecran;
}

SDL_Surface *LoadSprites(char * sprites_filename)
{
	SDL_Surface *sprites= SDL_LoadBMP(sprites_filename);
	if ( sprites == NULL )
	{
		fprintf(stderr, "Impossible de charger le fichier bmp : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_SetColorKey( sprites, SDL_SRCCOLORKEY, SDL_MapRGB( sprites->format,255,255,255 ));

	return sprites;
}

void DrawRoach ( Roach roach, SDL_Surface *ecran)
{
	SDL_Rect rect_src; // Rectangle source
	SDL_Rect rect_dest; // Rectangle destination

	rect_src.x = ( roach.dir % NB_SPRITES_P_LINE ) * ROACH_WIDTH;
	rect_src.y = ( roach.dir / NB_SPRITES_P_LINE ) * ROACH_HEIGHT;
	rect_src.w = ROACH_WIDTH;
	rect_src.h = ROACH_HEIGHT;

	rect_dest.x = roach.x;
	rect_dest.y = roach.y;

	SDL_BlitSurface(roach.sprites, &rect_src, ecran, &rect_dest);
}

/**
 * Génére un entier aléatoire entre 0 et maxVal-1
 */
int RandInt(int maxVal)
{
	return rand() % maxVal;
}

/**
 * Création d'un cafard
 */
Roach CreateRoach( SDL_Surface *ecran, SDL_Surface *sprites)
{
	Roach roach;
	roach.sprites = sprites; // surface correspondant au orientations
	roach.dir=RandInt(ROACH_ORIENTATIONS);
	roach.x = RandInt(ecran->w - ROACH_WIDTH);
	roach.y = RandInt(ecran->h- ROACH_HEIGHT);
	roach.hidden = 0;
	roach.steps = RandInt(MAX_STEPS);
	roach.angle = roach.dir * ROACH_ANGLE / 180.0 *M_PI; // radians
	roach.turnLeft = RandInt(2);

	return roach;
}

/** 
 * Test la position du cafard
 */
int RoachInRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight)
{
    if (x < rectx) return 0;
    if ((x + ROACH_WIDTH) > (rectx + rectwidth)) return 0;
    if (y < recty) return 0;
    if ((y + ROACH_HEIGHT) > (recty + rectheight)) return 0;
    return 1;
}

/* Changement de direction */
void TurnRoach(Roach *roach)
{
    if (roach->turnLeft) 
    {
	   roach->dir += RandInt(3) + 1; // +1 à 3
	   if (roach->dir >= ROACH_ORIENTATIONS)
	    {
	    	roach->dir -= ROACH_ORIENTATIONS;
    	}
    }
    else
    {
	   roach->dir -= RandInt(3) + 1; // -1 à 3
	   if (roach->dir < 0)
		{
	       roach->dir += ROACH_ORIENTATIONS;
		}
    }
    roach->angle = roach->dir * ROACH_ANGLE / 180.0 * M_PI; //radians
}

/*   Déplacement d'un cafard   */
void MoveRoach(Roach *roaches, int nbRoaches, int index, float roachSpeed, SDL_Surface *ecran)
{
     Roach *roach = &roaches[index];
    int newX, newY;
    int i;

    newX = roach->x + (int)(roachSpeed * cos (roach->angle) );
    newY = roach->y - (int)(roachSpeed * sin (roach->angle) );
    
    // Si dans la fenetre
    if (RoachInRect(newX, newY, 0, 0, ecran->w, ecran->h)) {
	
       // Gestion des collisions
       for ( i = 0; i < index; i++ ) {

	   		if (RoachIntersectRect(newX,newY,roaches[i].x, roaches[i].y,ROACH_WIDTH, ROACH_HEIGHT))
	    	{	
	        	TurnRoach(roach);
	        	break;
	    	}
	  	}
	}

    /* Si dans la fenetre */
    if (RoachInRect(newX, newY, 0, 0, ecran->w, ecran->h)) {
	
        roach->x = newX;
        roach->y = newY;

        if (roach->steps-- <= 0) {
	    	TurnRoach(roach);
	    	roach->steps = RandInt(MAX_STEPS);
        }
    } else {
        TurnRoach(roach);
    }
}

void DrawRoaches(Roach *roaches, int nbRoach, SDL_Surface *ecran)
{
    int i;

    for ( i = 0; i < nbRoach; i++ ) {
        DrawRoach( roaches[i], ecran);
    }
}

Roach *CreateRoaches(SDL_Surface *sprites, int nbRoach, SDL_Surface *ecran)
{
    int i;
   	Roach *roaches = malloc(sizeof(Roach)*nbRoach);
    for ( i = 0; i < nbRoach; i++ ) 
    {
        roaches[i] = CreateRoach(ecran, sprites);
    }
    return roaches;
}

void MoveRoaches(Roach *roaches, int nbRoach, float roachSpeed, SDL_Surface *ecran)
{
    int i;

    for ( i = 0; i < nbRoach; i++ ) {
    	if (!(roaches[i].hidden))
    	{
	    	MoveRoach(roaches,nbRoach,i,roachSpeed,ecran);
	    }
    }
}

/*   Teste si le cafard est en intersection avec le rectangle spécifié   */
int RoachIntersectRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight)
{
    if (x >= (rectx + rectwidth)) return 0;
    if ((x + ROACH_WIDTH) <= rectx) return 0;
    if (y >= (recty + rectheight)) return 0;
    if ((y + ROACH_HEIGHT) <= recty) return 0;
    
    return 1;
}

SDL_Surface *LoadImage ( char * img_filename, int x, int y )
{
	SDL_Surface *img= SDL_LoadBMP(img_filename);
	if ( img == NULL )
	{
		fprintf(stderr, "Impossible de charger le fichier bmp : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	img -> clip_rect.x = x;
	img -> clip_rect.y = y;

	SDL_SetColorKey( img, SDL_SRCCOLORKEY, SDL_MapRGB( img->format,255,255,255 ));

	return img;
}

void DrawImage (SDL_Surface *img, SDL_Surface *ecran)
{
	SDL_BlitSurface(img, NULL, ecran, &(img->clip_rect));
}

/*   Marque les cafards cachés  */
int MarkHiddenRoaches(Roach *roaches,int nbRoaches,SDL_Surface *rect)
{
    int i;
    int nVisible = 0;

    for ( i = 0; i < nbRoaches; i++ ) 
    {
	 	if (RoachInRect( roaches[i].x, roaches[i].y, rect->clip_rect.x,rect->clip_rect.y, rect->w, rect->h)) 
	 	{
	    	roaches[i].hidden = 1;
	 	}
	 	else 
	 	{
	    	roaches[i].hidden = 0;
	    	nVisible++;
	 	}
    }
    return nVisible;
}

/*   Teste si le point est dans le rectangle spécifié   */
int PointInRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight) 
{
    if (x < rectx) return 0;
    if (x > (rectx + rectwidth)) return 0;
    if (y < recty) return 0;
    if (y > (recty + rectheight)) return 0;
    
    return 1;
}





