#include <stdio.h>
#include <stdlib.h>
#include <conio.h> /*Pour recuperer les valeurs des touches flechees.*/

#include <time.h> /* Pour l'al�atoire */

#define M_SIZE 4  /*D�finit la taille de la matrice*/

#ifdef _WIN32    /*Si on dispose du syst�me windows.*/
#define CLEAR system("CLS")  /*On pourra utiliser la fonction CLEAR qui efface la fen�tre � chaque fois.*/
#define SYSTEME_WINDOWS 1   /* Ce booleen indique qu'on dispose d'un syst�me windows.*/
#else
#define SYSTEME_WINDOWS  0
#define CLEAR system("clean")        /*Remplace le clear system dans le cas o� le syst�me d'exploitation n'est pas windows.*/
#endif // _WIN32

/*Prototype des fonctions.*/

void affichage_matrice(int M[M_SIZE][M_SIZE]);
void initialisation_zero_matrice(int M[M_SIZE][M_SIZE]);
int nombre_aleatoire(void);
void place_nombre(int M[M_SIZE][M_SIZE], int *tableau_informations);
void verification_bloquage(int M[M_SIZE][M_SIZE], int *tableau_informations);

void deplacement_droite(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_fusion_droite(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_gauche(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_fusion_gauche(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_haut(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_fusion_haut(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_bas(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);
void deplacement_fusion_bas(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements);




/*Notre programme est agenc� de la mani�re suivante: dans un premier temps nous avons mis toutes les fonctions auxiliaires qui vont servir � l'affichage, l'al�atoire...
Ensuite, nous avons mis les 4 fonctions de d�placement (selon les 4 directions). Tout � la fin, se situe la fonction main qui va permettre l'execution du jeu.*/

/*Nous avons utilis� un premier tableau (intitul� tableau_informations ) qui va servir � stocker des informations � propos du jeu. Dans la premi�re case, nous
avons mis un booleen qui va indiquer si une fusion s'est op�r�e sur la ligne / la colonne. En effet, dans le 2048, on ne peut effectuer qu'une seule fusion par ligne/
colonne lors d'un d�placement.*/

/*La deuxi�me case contient le score qu'on incr�mente au fur et � mesure. La troisi�me case (tableau_informations[2]) contient un booleen (perdu) qui prend la valeur 1 si l'on
a perdu (on l'initialise � z�ro). La 4�me case (tableau_informations[3]) contient un booleen (gagne) qui prend la valeur 1 si l'on a gagn�, et qui reste � z�ro sinon.*/

/*Nous utiliserons toujours la notation i pour les lignes et j pour les colonnes des matrices.*/


void affichage_matrice(int M[M_SIZE][M_SIZE]) /* Fonction d'affichage de la matrice*/
{
    CLEAR;    /*Cela va effacer l'affichage � chaque nouveau coup afin que l'affichage soit plus lisible.*/
    int i = 0 , j = 0;
    for (i= 0 ; i < 4 ; i++ )
    {
        for (j = 0 ; j < 4 ; j++)
        {
            if (M[i][j] == 0)
            {
                printf(". \t");  /*On trouve que la matrice est plus lisible lorsqu'on remplace les z�ros par de simples points.*/
            }
            else
            {
                printf("%d \t" , M[i][j]);   /*On affiche la matrice avec un espacement entre chaque case.*/
            }

        }
        printf("\n");
    }
    printf("\n \n");
}



void initialisation_zero_matrice(int M[M_SIZE][M_SIZE]) /*Initialise la matrice � z�ro*/
{
    int i = 0 , j = 0;
    for (i =0 ; i < M_SIZE ; i++)
    {
        for (j = 0 ; j < 4 ; j++)   /*On �crit indiff�remment M_SIZE ou 4.*/
        {
            M[i][j]= 0 ;
        }
    }

}

int nombre_aleatoire(void) /*Donne un nombre (2 ou 4) avec une probabilit� respecetive de 9/10 et 1/10 , sert pour l'apparition du nouveau nombre.*/
{
    int tableau[10] = {2,2,2,2,2,2,2,2,2,4}; /*On remplit un tableau comportant 9 fois le chiffre 2 et une fois le chiffre 4.*/
    int i;
    i = rand() % 10;   /*Va donner un entier al�atoire entre 0 et 9.*/
    return(tableau[i]); /*On prend alors la position dans le tableau qui correspond.*/

}


void place_nombre(int M[M_SIZE][M_SIZE], int *tableau_informations) /*Place le nombre � l'endroit o� il y a des z�ros.*/
{
    int i = 0, j = 0;
    int position = 0;
    int compteur = 0;  /*Il sert � compter le nombre de positions vides (o� il y a des 0). */
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0; j < 4 ; j++)
        {
            if (M[i][j] == 0)
            {
                compteur++;  /*On parcourt la matrice, et on compte le nombre de places inoccup�es.*/
            }
        }
    }

    if (compteur == 1) /*Dans le cas o� il y a une unique case restante, lorsqu'on va la combler, il faut regarder si le joueur a perdu ou non.*/
    {
        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < 4 ; j++)
            {
                if (M[i][j] == 0) /*Lorsqu'on arrive � la position restante avec un z�ro, on met un nombre choisi al�atoirement � la place.*/
                {
                    compteur = 0; /*On remet le compteur � z�ro car il n'y a plus de places inoccup�es.*/
                    M[i][j] = nombre_aleatoire();
                    verification_bloquage(M, tableau_informations); /* Et on fait alors appel � une autre fonction (de type void) qui va v�rifier si le joueur est bloqu� ou non.*/
                }
            }
        }


    }

    if (compteur != 0)  /*Si le compteur est non nul, cela signifie qu'il y a des places libres pour ins�rer un nouveau chiffre.*/
    {
        position = rand()% compteur; /*On d�termine al�atoirement la position parmi toutes les positions inoccup�es.*/
        int trouve = 0;   /*Tant qu'on a pas trouv� la position, le booleen reste nul.*/
        i=0;
        j=0;
        while (trouve == 0 && i < 4) /*On parcourt alors la matrice pour rechercher la position correspondante.*/
        {
            while (trouve == 0 && j < 4)
            {
                if (M[i][j] == 0) /*Il s'agit d'une place inoccup�e.*/
                {
                    if (position ==0) /*On regarde si la position al�atoire correspond.*/
                    {
                        M[i][j] = nombre_aleatoire(); /*Si oui, on ins�re un 2 ou un 4, et on dit qu'on l'a trouv�.*/
                        trouve = 1; /*Ce booleen montre que l'on a trouv� la position al�atoire.*/
                    }
                    else
                    {
                        position--; /*Sinon, on passe � la place inoccup�e suivante jusqu'� ce que position soit �gal � z�ro.*/
                    }
                }
                j++;
            }
            j=0; /*On reinitialise car il s'agit d'une boucle while imbriqu�e.*/
            i++;
        }
    }


}


/*Cette fonction va servir � v�rifier si le joueur a perdu (il ne peut plus effectuer de d�placements).*/

void verification_bloquage(int M[M_SIZE][M_SIZE], int *tableau_informations)
{

    /*On va regarder pour chaque nombre, si ces voisins lui sont �gaux, auquel cas il est toujours possible de fusionner, donc le joueur
    n'a pas perdu.*/
    int i = 0;
    int j = 0;
    int perdu = 1;  /*On initialise le booleen qui va servir pour le tableau_informations (3�me case).*/
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < 3 ; j++)
        {
            if (M[i][j] == M[i][j+1])  /*On effectue un premier parcours horizontal qui va regarder si il y a deux cases �gales adjacentes sur la m�me ligne.*/
            {
                perdu = 0; /*Si il y a possibilit� de fusionner alors le joueur n'a pas perdu (le booleen redevient nul).*/
            }
        }

    }

    for (j = 0 ; j < 4 ; j++)
    {
        for (i = 0 ; i < 3 ; i++)
        {
            if (M[i][j] == M[i+1][j]) /* On effectue ensuite un parcours vertical qui va regarder si deux cases adjacentes sont �gales au sein d'une m�me colonne.*/
            {
                perdu = 0; /*Si oui, le joueur n'a pas perdu.*/
            }
        }
    }

    tableau_informations[2] = perdu; /*On stocke alors l'information dans la 3�me case, si le joueur a perdu ou non.*/


}



/*On passe maintenant aux fonctions qui vont servir pour le d�placement.*/

/*On se sert pour cela d'un autre tableau intitul� tableau_deplacements.
tableau_deplacements indique si un d�placement s'est produit dans l'une des 4 directions (0 = non et 1 = oui). Les 4 cases correspondent chacunes � une direction.
Tableau_deplacements[0] concerne la gauche, tableau_deplacements[1] le haut, tableau_deplacements[2] la droite, tableau_deplacements[3] le bas.*/

/*Une seconde particularit� du 2048 est lorsqu'il y a 4 cases remplies sur une m�me ligne ou colonne, si les deux cases de chaque c�t� sont �gales,
alors lorsqu'il y a un d�placement, on va exceptionellement en fusionner deux sur la m�me ligne /colonne. (ex:  [2,2,4,4] va donner [0,0,4,8] si on va le d�placer vers la droite)
On va appeler ce cas la double-fusion.*/

/*On s'int�resse dans un premier temps � un d�placement vers la droite.*/
/*On rappelle que les indices i correspondent aux lignes et j aux colonnes.*/

void deplacement_droite(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{
    int i = 0;
    int j = 2;
    for (i=0 ; i < 4 ; i++) /*Cette boucle va faire un parcours sur les lignes, en commen�ant par celle du haut.*/
    {
        if (M[i][0] == M[i][1] && M[i][2] == M[i][3] && M[i][0] != 0 && M[i][2] != 0) /*On regarde si une double-fusion est possible.*/
        {
            M[i][3] = 2*M[i][3];  /*Si oui, on va fusionner les cases.*/
            M[i][2] = 2*M[i][1];
            M[i][0] = 0;          /*Et mettre les restantes � z�ro.*/
            M[i][1] = 0;
            tableau_informations[1] = tableau_informations[1] + M[i][3] + M[i][2];  /*On augmente le score (contenu dans la seconde case du tableau_informations).*/
            if (M[i][3] == 2048 || M[i][2] == 2048)
            {
                tableau_informations[3] = 1; /*Si l'une des cases est �gale � 2048, cela signifie qu'on a gagn�, le booleen passe donc � 1.*/
            }
            tableau_deplacements[2] = 1; /*On indique �galement qu'un d�placement s'est op�r� vers la droite.*/
        }

        else  /*Si il n'y a pas possibilit� de faire une double-fusion, on traite le cas normal.*/
        {
            for (j = 2 ; j >= 0 ; j--) /* Pour chaque ligne, on commence par le bord vers lequel
                                    on souhaite d�placer (en l'occurence droit) et on se d�place vers le bord oppos� (gauche). (Cela ne sert � rien de commencer � la case 4)*/

            {
                deplacement_fusion_droite(i,j,M,tableau_informations, tableau_deplacements); /*Pour chacune des cases, on va appeler une fonction qui va se charger de
                                                                                                d�placer / fusionner les cases entre elles.*/
            }

            tableau_informations[0] = 0; /*Lorsqu'on a fini une ligne, on reinitialise le booleen qui indique qu'une fusion s'est effectuee sur cette ligne.*/

        }
    }
}


/*Voici la fonction de d�placement des cases. Nous utiliserons les indices i pour les lignes et j pour les colonnes (notation matricielle).*/

void deplacement_fusion_droite(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{

    int j = y;  /*On recup�re la colonne de la case.*/

    for (j = y ; j < 3 ; j++)       /*On fait une boucle qui parcourt de la position y (num�ro colonne) jusqu'au c�t� vers lequel on fait le d�placement (ici le c�t� droit).*/
    {
        if (M[x][j] == M[x][j+1] && tableau_informations[0] == 0 && M[x][j] != 0)
                        /*Si la cellule est egale � la cellule de droite, qu'il ne s'agit pas d'un z�ro et qu'une fusion ne s'est pas encore effectu�e sur la ligne
                            (tableau_informations[0] == 0) on peut alors fusionner.*/
        {
            M[x][j+1] *= 2; /*Le contenu de la cellule qui va recevoir la fusion est alors multipli�e par 2.*/
            M[x][j] = 0;    /*La cellule qu'on a fusionn� prend la valeur 0.*/

            tableau_deplacements[2] = 1;      /*Un d�placement dans la direction de droite s'est bien op�r�. */

            tableau_informations[0] = 1;      /* Dans le jeu du 2048, il n'y a qu'une seule fusion par d�placement sur la m�me ligne,
                                                    tableau_information[0] = 1 signifie qu'on vient d'en effectuer une.*/

            tableau_informations[1] = tableau_informations[1] + M[x][j+1]; /*Augmentation du score.*/

            if (M[x][j+1] == 2048)
            {
                tableau_informations[3] = 1; /*Si la fusion a fait apparaitre un 2048 alors le joueur a gagn�, le booleen contenu dans la 4�me case passe alors � 1.*/
            }
        }

        if (M[x][j+1] == 0 && M[x][j] != 0) /*Si la cellule qu'on souhaite d�placer est diff�rente de z�ro et que celle situ�e � sa droite est vide (�gale � z�ro).
                                                                On peut alors la d�placer.*/

        {
            M[x][j+1] = M[x][j]; /*On d�cale alors la cellule dans celle situ�e � sa droite.*/
            M[x][j] = 0;         /*La cellule qu'on a d�cal� devient nulle.*/
            tableau_deplacements[2] = 1;  /*On indique qu'on a bien effectue un d�placement vers la droite.*/
        }

    }


}





/*On effectue pareil dans les 3 autres directions. On prend soin � chaque fois de faire attention aux indices situ�s sur les bords*/

/*Dans la direction de gauche, le principe est le m�me sauf qu'on va diminuer la valeur de j (on se rapproche de la colonne de gauche)*/

void deplacement_gauche(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{
    int i = 0;
    int j = 1;
    for (i= 0 ; i < 4 ; i++)  /*Cette boucle va faire un parcours sur les lignes.*/
    {
        if (M[i][0] == M[i][1] && M[i][2] == M[i][3] && M[i][0] != 0 && M[i][2] != 0) /*Il s'agit du cas de la double-fusion.*/
        {
            M[i][0] = 2*M[i][0];
            M[i][1] = 2*M[i][2];
            M[i][2] = 0;
            M[i][3] = 0;
            tableau_informations[1] = tableau_informations[1] + M[i][0] + M[i][1];
            if (M[i][1] == 2048 || M[i][0] == 2048)
            {
                tableau_informations[3] = 1;
            }
            tableau_deplacements[0] = 1;
        }
        else
        {
            for (j = 1 ; j <= 3 ; j++) /* Pour chaque ligne, on commence par le bord vers lequel
                        on souhaite d�placer (en l'occurence gauche) et on se d�place vers le bord oppos� (droit).*/

            {
                deplacement_fusion_gauche(i,j,M,tableau_informations, tableau_deplacements);
            }
            tableau_informations[0] = 0; /*Pour chaque ligne, on reinitialise le booleen qui indique que la fusion s'est effectuee.*/
        }

    }
}



void deplacement_fusion_gauche(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{



        int j = y;

        for (j = y ; j > 0 ; j--)
        {
            if (M[x][j] == M[x][j-1] && tableau_informations[0] == 0 && M[x][j] !=0) /*Si la cellule est egale � la cellule de gauche, on les fusionne (additionne) dans la cellule de gauche.
                                                    La valeur de la cellule �tudi�e passe alors � z�ro. De m�me, on regarde si une fusion s'est d�j� op�r�e sur cette ligne.*/
            {
                M[x][j-1] *= 2;
                if (M[x][j-1] == 2048)
                {
                    tableau_informations[3] = 1;
                }
                M[x][j] = 0;
                tableau_informations[1] = M[x][j-1] + tableau_informations[1] ;
                tableau_deplacements[0] = 1;
                tableau_informations[0] = 1;      /* Dans le jeu du 2048, il n'y a qu'une seule fusion par d�placement sur la m�me ligne, fusion = 1 signifie qu'on vient d'en effectuer une.*/
            }

            if (M[x][j-1] == 0 && M[x][j] != 0) /*Si la cellule de droite est z�ro, on d�cale le contenu de la cellule dans celle de droite.*/
            {
                M[x][j-1] = M[x][j];
                M[x][j] = 0;
                tableau_deplacements[0] = 1;
            }

        }




}






/*Dans la direction du haut maintenant ! */

void deplacement_haut(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{
    int i = 1;
    int j = 0;
    for ( j = 0 ; j < 4 ; j++)  /*Cette boucle va faire un parcours sur les colonnes.*/
    {
        if (M[0][j] == M[1][j] && M[2][j] == M[3][j] && M[0][j] != 0 && M[2][j] != 0)
        {
            M[0][j] = 2*M[0][j];
            M[1][j] = 2 * M[2][j];
            M[2][j] = 0;
            M[3][j] = 0;
            tableau_informations[1] = tableau_informations[1] + M[0][j] + M[1][j];
            if (M[0][j] == 2048 || M[1][j] == 2048)
            {
                tableau_informations[3] = 1;
            }
            tableau_deplacements[1] = 1;
        }
        else
        {

            for (i = 1 ; i <= 3 ; i++) /* Pour chaque colonne, on commence par le bord vers lequel
                            on souhaite d�placer (en l'occurence haut) et on se d�place vers le bord oppos� (bas).*/

            {
                deplacement_fusion_haut(i,j,M,tableau_informations , tableau_deplacements);
            }
            tableau_informations[0] = 0; /*Pour chaque colonne, on reinitialise le booleen qui indique que la fusion s'est effectuee.*/
        }
    }
}


void deplacement_fusion_haut(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{



    int i = x;

    for (i = x ; i > 0 ; i--)       /*On fait une boucle qui parcourt de la position x jusqu'au bord haut (x=0)*/
    {
        if (M[i][y] == M[i-1][y] && tableau_informations[0] == 0 && M[i][y] !=0) /*Si la cellule est egale � la cellule du haut, on les fusionne (additionne) dans la cellule du haut.
                                                La valeur de la cellule �tudi�e passe alors � z�ro. De m�me, on regarde si une fusion s'est d�j� op�r�e sur cette colonne.*/
        {
            M[i-1][y] *= 2;
            if (M[i-1][y] == 2048)
            {
                tableau_informations[3] = 1;
            }
            M[i][y] = 0;
            tableau_informations[1] = M[i-1][y] + tableau_informations[1] ;
            tableau_deplacements[1] = 1;
            tableau_informations[0] = 1;     /* Dans le jeu du 2048, il n'y a qu'une seule fusion par d�placement sur la m�me colonne, fusion = 1 signifie qu'on vient d'en effectuer une.*/
        }
        if (M[i-1][y] == 0 && M[i][y] != 0) /*Si la cellule du haut est z�ro, on d�cale le contenu de la cellule dans celle du haut.*/
        {
            M[i-1][y] = M[i][y];
            M[i][y] = 0;
            tableau_deplacements[1] = 1;
        }

    }



}





/*Viens enfin la direction du bas.*/

void deplacement_bas(int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{
    int i = 2; /*On commence par l'endroit vers le bord sur lequel on d�place, puis on remonte.*/
    int j = 0;
    for ( j = 0 ; j < 4 ; j++)  /*Cette boucle va faire un parcours sur les colonnes.*/
    {
        if (M[0][j] == M[1][j] && M[2][j] == M[3][j] && M[0][j] != 0 && M[2][j] != 0)
        {
            M[3][j] = 2*M[3][j];
            M[2][j] = 2 * M[1][j];
            M[1][j] = 0;
            M[0][j] = 0;
            tableau_informations[1] = tableau_informations[1] + M[3][j] + M[2][j];
            if (M[2][j] == 2048 || M[3][j] == 2048)
            {
                tableau_informations[3] = 1;
            }
            tableau_deplacements[3] = 1;
        }
        else
        {
            for (i = 2 ; i >= 0 ; i--) /* Pour chaque colonne, on commence par le bord vers lequel
                        on souhaite d�placer (en l'occurence haut) et on se d�place vers le bord oppos� (bas).*/

            {
                deplacement_fusion_bas(i,j,M,tableau_informations, tableau_deplacements);
            }
            tableau_informations[0] = 0; /*Pour chaque colonne, on reinitialise le booleen qui indique que la fusion s'est effectuee.*/
        }
    }
}



void deplacement_fusion_bas(int x, int y, int M[M_SIZE][M_SIZE], int *tableau_informations, int *tableau_deplacements)
{


    int i = x;
    for (i = x ; i < 3 ; i++)       /*On fait une boucle qui parcourt de la position x jusqu'au bord bas (x=3)*/
    {
        if (M[i][y] == M[i+1][y] && tableau_informations[0] == 0 && M[i][y] !=0) /*Si la cellule est egale � la cellule du bas, on les fusionne (additionne) dans la cellule du bas.
                                                La valeur de la cellule �tudi�e passe alors � z�ro. De m�me, on regarde si une fusion s'est d�j� op�r�e sur cette colonne.*/
        {
            M[i+1][y] *= 2;
            if (M[i+1][y] == 2048)
            {
                tableau_informations[3] = 1;
            }
            M[i][y] = 0;
            tableau_deplacements[3] = 1;
            tableau_informations[0] = 1;      /* Dans le jeu du 2048, il n'y a qu'une seule fusion par d�placement sur la m�me colonne, fusion = 1 signifie qu'on vient d'en effectuer une.*/
            tableau_informations[1] = tableau_informations[1] + M[i+1][y] ;
        }

        if (M[i+1][y] == 0 && M[i][y] != 0) /*Si la cellule du bas est z�ro, on d�cale le contenu de la cellule dans celle du bas.*/
        {
            M[i+1][y] = M[i][y];
            M[i][y] = 0;
            tableau_deplacements[3] = 1;
        }


    }

}




/*Les fonctions de d�placement sont OK.*/



/*Fonction main, fonction principale.*/


int main()
{
    srand(time(NULL));   /*Sert � initialiser le temps qui va �tre utilis� pour l'al�atoire.*/
    int matrice[4][4];   /*On cr�e une matrice 4*4 qui va recevoir le jeu.*/

    int fleche1 = 0;  /*Cette variable correspond � la direction o� on joue, on utilise les fl�ches directionnelles si l'on est sous windows.*/
    int fleche2 = 0;  /*Est n�cessaire pour utiliser getchar.*/
    int touche = 0;  /*Dans le cas o� le syt�me n'est pas windows, on joue avec le pav� num�rique, r�cup�re la direction de d�placement.*/
    int gagne_une_fois = 0;   /*Ce booleen va servir lorsque le joueur va gagner (d�passer 2048) et souhaite continuer � jouer.*/
    int continu_jouer = 1; /* Tant que ce booleen est �gal � 1, on va continuer � jouer.*/
    int tableau_informations[4] = {0,0,0,0} ; /*La premiere case indique si la fusion s'est effectuee ou non, la seconde case contient le score, la troisi�me un booleen qui passe
                                                        � 1 si le joueur a perdu, et la derniere case un booleen qui passe � 1 si le joueur a gagn�.*/

    int tableau_deplacements[4] = {0,0,0,0} ; /* Ce tableau indique si des d�placements ont �t� effectu�e dans les 4 directions (dans l'ordre gauche, haut, droite, bas).
                                                            La case passe � 1 si un d�placement a pu s'op�rer.*/

    initialisation_zero_matrice(matrice); /*On commence par initialiser la matrice avec des z�ros.*/
    place_nombre(matrice, tableau_informations); /*On place le premier nombre.*/
    affichage_matrice(matrice); /*On affiche la matrice.*/

    while (continu_jouer == 1)  /*C'est la boucle principale, elle s'arrete quand le joueur ne souhaite plus jouer.*/
    {
        printf("Votre score est de : %d  \n \n", tableau_informations[1]);   /*On affiche toujours le score contenu dans le tableau_informations (il s'actualise � chaque coup).*/

        printf("Entrez la touche flechee correspondante ou le pave numerique si vous n'avez pas windows (2,4,8,6)"); /*On demande � l'utilisateur d'appuyer sur une des touches flechees.*/
        printf(" \n \n");

        if (SYSTEME_WINDOWS == 1) /*Si le syst�me est bien Windows alors on joue avec les touches flechees.*/
        {
            fleche1 = getch(); /*On demande � l'utilisateur de rentrer une direction.*/
            if (fleche1 == 224)
            {
                fleche2 = getch(); /*R�cup�re la direction de d�placement.*/
            }
        }

        else  /*Si l'utilisateur n'est pas sous Windows, on utilise scanf pour r�cup�rer les valeurs du pav� num�rique.*/
        {
            scanf("%d", &touche);
        }


        if (fleche2 == 80 || touche == 2) /*Si on demande un d�placement vers le bas.*/
        {
            deplacement_bas(matrice , tableau_informations , tableau_deplacements); /*On d�place vers le bas.*/

            if (tableau_deplacements[3] != 0)    /*Si le d�placement vers le bas a pu s'effectuer: */
            {
                tableau_deplacements[3] = 0;      /*On remet � z�ro la case qui indique que le d�placement s'est op�r�.*/
                place_nombre(matrice, tableau_informations);  /*Et on place un nouveau nombre al�atoire.*/

            }

            affichage_matrice(matrice); /*On affiche la matrice � chaque fois qu'on demande un d�placement.*/
        }


        if (fleche2 == 75 || touche == 4) /*On fait de m�me dans les 3 directions restantes, ici celle de gauche.*/
        {
            deplacement_gauche(matrice, tableau_informations, tableau_deplacements);
            if (tableau_deplacements[0] != 0)
            {
                tableau_deplacements[0] = 0;
                place_nombre(matrice,tableau_informations);
            }

            affichage_matrice(matrice);
        }


        if (fleche2 == 72 || touche == 8)    /*Puis viens celle du haut.*/
        {
            deplacement_haut(matrice, tableau_informations, tableau_deplacements);
            if (tableau_deplacements[1] != 0)
            {
                tableau_deplacements[1] = 0;
                place_nombre(matrice, tableau_informations);
            }
            affichage_matrice(matrice);
        }

        if (fleche2 == 77 || touche == 6) /*Et enfin, la direction de droite.*/
        {
            deplacement_droite(matrice, tableau_informations, tableau_deplacements);
            if (tableau_deplacements[2] != 0)
            {
                tableau_deplacements[2] = 0;
                place_nombre(matrice, tableau_informations);
            }

            affichage_matrice(matrice);
        }


        fleche1 = 0; /*On reinitialise la valeur de la fleche � chaque nouveau passage dans la boucle.*/
        fleche2 = 0;
        touche = 0; /*De m�me, on reinitialise la valeur de la touche.*/

        if (tableau_informations[3] == 1 && gagne_une_fois == 0) /*Si le boolen vaut 1 et que c'est la premire fois que le joueur atteint 2048. Cela signifie qu'il a gagn�.*/

        {
            affichage_matrice(matrice);
            printf("Vous avez gagne \n");
            printf("Votre score est de: %d \n", tableau_informations[1]);  /*Affichage du Score.*/
            printf("\n");
            printf("Voulez-vous continuer a jouer ? (1 pour continuer, 0 sinon) \n"); /*On demande au joueur s'il souhaite continuer � jouer.*/
            scanf("%d", &continu_jouer);   /*On r�cup�re alors la valeur de la variable.*/
            if (continu_jouer == 1)  /*Si il souhaite continuer � jouer (apr�s avoir atteint 2048).*/
            {
                affichage_matrice(matrice); /*On affiche la matrice.*/
                gagne_une_fois = 1;  /*Et on signale que le joueur a d�j� gagn� une fois, de cette fa�on il ne pourra par la suite que perdre*/
            }


        }

        if (tableau_informations[2] == 1)   /*Si la troisi�me case prend la valeur 1 alors le joueur a perdu.*/
        {

            affichage_matrice(matrice);
            tableau_informations[2] = 0; /*On reinitialise le tableau_informations au cas o� il souhaite rejouer.*/
            printf("Vous avez perdu \n");
            printf("Votre score est de: %d \n" , tableau_informations[1]); /*On affiche alors le score du joueur avec un petit message pour lui faire comprendre sa d�faite.*/
            printf("\n");
            printf("Voulez-vous rejouer ? (1 pour rejouer, 0 pour arreter) \n");   /*On demande au joueur s'il souhaite faire une nouvelle partie.*/
            scanf("%d", &continu_jouer);  /*On r�cup�re la valeur correspondante.*/
            printf("%d", continu_jouer);
            if (continu_jouer == 1)  /*Si le joueur souhaite rejouer alors continu_jouer reste �gale � 1.*/
            {
                initialisation_zero_matrice(matrice); /*Dans ce cas, on remet la matrice � z�ro.*/
                tableau_informations[1] = 0;   /*On reinitialise le score.*/
                place_nombre(matrice, tableau_informations); /*On place le premier nombre dans la matrice.*/
                affichage_matrice(matrice);  /*Et on affiche la matrice pour la nouvelle partie.*/
            }

        }



    }    /*On sort de la boucle lorsque le joueur ne souhaite plus continuer � rejouer apr�s qu'il a perdu.*/

    return 0; /*On n'oublie pas de retourner la valeur 0 pour la fonction main.*/
}


/*Bouthemy Marin / Louge Louis 1A */


