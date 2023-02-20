/*
 * jeu de dame
 * plateau.c
 *
 *  Created on: dec. 2022
 *      Author: PROUST--JUVIN François
 *              HERAULT Nathan
 *              PIEGELIN Baptiste
 */

#include "plateau.h"

/**
 *  initialisation du plateau
 */
Plateau *creerPlateau()
{
    Plateau *Plat = malloc(sizeof(Plateau));

    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            Plat->listeCases[x][y].type = 0;
            Plat->listeCases[x][y].x = x;
            Plat->listeCases[x][y].y = y;
            Plat->listeCases[x][y].jouable = 0;
            Plat->listeCases[x][y].selectionner = 0;
            Plat->listeCases[x][y].perdue = 0;
            if (!(y % 2 == 0) && (x % 2 == 0))
            {
                Plat->listeCases[x][y].type = -1;
            }
            else if ((y % 2 == 0) && !(x % 2 == 0))
            {
                Plat->listeCases[x][y].type = -1;
            }
            if (Plat->listeCases[x][y].type == -1)
            {
                if (y < 4)
                {
                    Plat->listeCases[x][y].type = 2;
                }
                else if (y > 5)
                {
                    Plat->listeCases[x][y].type = 1;
                    Plat->listeCases[x][y].jouable = 1;
                }
                else
                {
                    Plat->listeCases[x][y].type = 0;
                }
            }
        }
    }
    return Plat;
};

void changementJoueur(Plateau *plateau, int QuiVientDeJouer)
{
    int obligation = deplacementObligatoire(plateau, (QuiVientDeJouer == 1 ? 2 : 1));
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            if (QuiVientDeJouer == 2 || QuiVientDeJouer == 4)
            {
                if (plateau->listeCases[x][y].type == 1 || plateau->listeCases[x][y].type == 3)
                {
                    selectionCaseJouable(plateau, x, y, obligation);
                }
                else
                {
                    plateau->listeCases[x][y].jouable = 0;
                }
            }
            else if (QuiVientDeJouer == 1 || QuiVientDeJouer == 3)
            {
                if (plateau->listeCases[x][y].type == 2 || plateau->listeCases[x][y].type == 4)
                {
                    selectionCaseJouable(plateau, x, y, obligation);
                }
                else
                {
                    plateau->listeCases[x][y].jouable = 0;
                }
            }
        }
    }
    dessinerPlateauConsole(plateau);
};

void selectionnerCase(Plateau *plateau, int X, int Y)
{
    if (plateau->listeCases[X][Y].jouable)
    {
        plateau->listeCases[X][Y].selectionner = 1;
    }
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            printf("| %d ", plateau->listeCases[x][y].selectionner);
        }
        printf("|\n");
    }
};

void deselectionnerCase(Plateau *plateau)
{
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            plateau->listeCases[x][y].selectionner = 0;
        }
    }
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            printf("| %d ", plateau->listeCases[x][y].selectionner);
        }
        printf("|\n");
    }
};

void selectionCaseJouable(Plateau *plateau, int X, int Y, int obligation)
{

    if (obligation)
    {
        if (peutMangerCase(plateau, X, Y) == 1)
            plateau->listeCases[X][Y].jouable = 1;
    }
    else
    {
        if (peutBougerCase(plateau, X, Y) == 1)
            plateau->listeCases[X][Y].jouable = 1;
    }
};

int deplacementObligatoire(Plateau *plateau, int equipe)
{
    int retour = 0;
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            if (plateau->listeCases[x][y].type == equipe ||
                plateau->listeCases[x][y].type - 2 == equipe)
            {
                if (peutMangerCase(plateau, x, y))
                {
                    retour = 1;
                    break;
                }
            }
        }
    }
    return retour;
};

int peutBougerCase(Plateau *plateau, int X, int Y)
{
    int retour = 0;
    switch (plateau->listeCases[X][Y].type)
    {
    case 1:

        if ((X < 9 && plateau->listeCases[X + 1][Y - 1].type == 0) ||
            (X > 0 && plateau->listeCases[X - 1][Y - 1].type == 0))
        {
            retour = 1;
        }

        break;
    case 2:

        if ((X < 9 && plateau->listeCases[X + 1][Y + 1].type == 0) ||
            (X > 0 && plateau->listeCases[X - 1][Y + 1].type == 0))
        {
            retour = 1;
        }

        break;

    case 3:
    case 4:
        if ((X + 1 < 10 && Y - 1 >= 0 && plateau->listeCases[X + 1][Y - 1].type == 0) ||
            (X - 1 >= 0 && Y - 1 >= 0 && plateau->listeCases[X - 1][Y - 1].type == 0) ||
            (X + 1 < 10 && Y + 1 < 10 && plateau->listeCases[X + 1][Y + 1].type == 0) ||
            (X - 1 >= 0 && Y + 1 < 10 && plateau->listeCases[X - 1][Y + 1].type == 0))
        {
            retour = 1;
        }
    default:
        break;
    }
    return retour;
};

int peutMangerCase(Plateau *plateau, int X, int Y)
{
    int retour = 0;
    int xBetween;
    int yBetween;
    switch (plateau->listeCases[X][Y].type)
    {
    case 1:
        if ((X > 1 && Y > 1 && plateau->listeCases[X - 2][Y - 2].type == 0))
        {
            xBetween = X - 1;
            yBetween = Y - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 2 || plateau->listeCases[xBetween][yBetween].type == 4)
            {
                retour = 1;
            }
        }
        if ((X < 8 && Y > 1 && plateau->listeCases[X + 2][Y - 2].type == 0))
        {
            xBetween = X + 1;
            yBetween = Y - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 2 || plateau->listeCases[xBetween][yBetween].type == 4)
            {
                retour = 1;
            }
        }
        if ((X < 8 && Y < 8 && plateau->listeCases[X + 2][Y + 2].type == 0))
        {
            xBetween = X + 1;
            yBetween = Y + 1;
            if (plateau->listeCases[xBetween][yBetween].type == 2 || plateau->listeCases[xBetween][yBetween].type == 4)
            {
                retour = 1;
            }
        }
        if ((X > 1 && Y < 8 && plateau->listeCases[X - 2][Y + 2].type == 0))
        {
            xBetween = X - 1;
            yBetween = Y + 1;
            if (plateau->listeCases[xBetween][yBetween].type == 2 || plateau->listeCases[xBetween][yBetween].type == 4)
            {
                retour = 1;
            }
        }

        break;
    case 2:
        if ((X > 1 && Y > 1 && plateau->listeCases[X - 2][Y - 2].type == 0))
        {
            xBetween = X - 1;
            yBetween = Y - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 1 || plateau->listeCases[xBetween][yBetween].type == 3)
            {
                retour = 1;
            }
        }
        if ((X < 8 && Y > 1 && plateau->listeCases[X + 2][Y - 2].type == 0))
        {
            xBetween = X + 1;
            yBetween = Y - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 1 || plateau->listeCases[xBetween][yBetween].type == 3)
            {
                retour = 1;
            }
        }
        if ((X < 8 && Y < 8 && plateau->listeCases[X + 2][Y + 2].type == 0))
        {
            xBetween = X + 1;
            yBetween = Y + 1;
            if (plateau->listeCases[xBetween][yBetween].type == 1 || plateau->listeCases[xBetween][yBetween].type == 3)
            {
                retour = 1;
            }
        }
        if ((X > 1 && Y < 8 && plateau->listeCases[X - 2][Y + 2].type == 0))
        {
            xBetween = X - 1;
            yBetween = Y + 1;
            if (plateau->listeCases[xBetween][yBetween].type == 1 || plateau->listeCases[xBetween][yBetween].type == 3)
            {
                retour = 1;
            }
        }

        break;
    case 3:
    case 4:

        break;
    default:
        break;
    }
    return retour;
};

void bougerCase(Plateau *plateau, int X, int Y, int DejaSelect)
{
    int possible;
    if (DejaSelect)
    {
        int xCase;
        int yCase;
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                if (plateau->listeCases[x][y].selectionner == 1)
                {
                    xCase = x;
                    yCase = y;
                }
            }
        }
        switch (plateau->listeCases[xCase][yCase].type)
        {
        case 1:
            if (plateau->listeCases[X][Y].type == 0)
            {
                if (Y == yCase - 1 && abs(xCase - X) == 1)
                {
                    aBouger(plateau, xCase, yCase, X, Y);
                    changementJoueur(plateau, 1);
                }
                else
                {
                    MangerCase(plateau, xCase, yCase, X, Y);
                }
            }

            break;
        case 2:
            if (plateau->listeCases[X][Y].type == 0)
            {
                if (Y == yCase + 1 && abs(xCase - X) == 1)
                {
                    aBouger(plateau, xCase, yCase, X, Y);
                    changementJoueur(plateau, 2);
                }
                else
                {
                    MangerCase(plateau, xCase, yCase, X, Y);
                }
            }
            break;
        case 3:
            possible = 0;

            if (plateau->listeCases[X][Y].type == 0)
            {
                if (X > xCase && Y > yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase+i][yCase+i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase+i][yCase+i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X < xCase && Y > yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase-i][yCase+i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase-i][yCase+i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X > xCase && Y < yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase+i][yCase-i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase+i][yCase-i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X < xCase && Y < yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase-i][yCase-i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase-i][yCase-i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (possible == 1)
                {
                    aBouger(plateau, xCase, yCase, X, Y);
                    changementJoueur(plateau, 1);
                }
                else if(possible == 2)
                {
                    MangerCase(plateau, xCase, yCase, X, Y);
                }

            }
            break;
            
        case 4:
            possible = 0;

            if (plateau->listeCases[X][Y].type == 0)
            {
                if (X > xCase && Y > yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase+i][yCase+i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase+i][yCase+i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X < xCase && Y > yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase-i][yCase+i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase-i][yCase+i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X > xCase && Y < yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase+i][yCase-i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase+i][yCase-i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (X < xCase && Y < yCase && abs(xCase-X) == abs(yCase-Y))
                {
                    for (int i = 0; i <= abs(xCase - X); i++)
                    {
                        if (plateau->listeCases[xCase-i][yCase-i].type == 0 )
                        {
                            possible = 1;
                        }
                        if (plateau->listeCases[xCase-i][yCase-i].type != 0)
                        {
                            possible = 2;
                        }
                    }
                }
                if (possible == 1)
                {
                    aBouger(plateau, xCase, yCase, X, Y);
                    changementJoueur(plateau, 2);
                }
                else if (possible == 2)
                {
                    MangerCase(plateau, xCase, yCase, X, Y);
                }

            }
            break;
        default:
            break;
        }
    }
};

void MangerCase(Plateau *plateau, int xCase, int yCase, int X, int Y)
{
    int xBetween;
    int yBetween;
    switch (plateau->listeCases[xCase][yCase].type)
    {
    case 1:
        if (abs(Y - yCase) == 2 && abs(xCase - X) == 2)
        {
            xBetween = (xCase < X) ? xCase + 1 : xCase - 1;
            yBetween = (yCase < Y) ? yCase + 1 : yCase - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 2 || plateau->listeCases[xBetween][yBetween].type == 4)
            {
                plateau->listeCases[xBetween][yBetween].type = 0;
                aBouger(plateau, xCase, yCase, X, Y);
                if (peutMangerCase(plateau, X, Y) == 0)
                {
                    changementJoueur(plateau, 1);
                }
                else
                {
                    changementJoueur(plateau, 2);
                    plateau->listeCases[X][Y].selectionner = 1;
                }
            }
        }

        break;
    case 2:
        if (abs(Y - yCase) == 2 && abs(xCase - X) == 2)
        {
            xBetween = (xCase < X) ? xCase + 1 : xCase - 1;
            yBetween = (yCase < Y) ? yCase + 1 : yCase - 1;
            if (plateau->listeCases[xBetween][yBetween].type == 1 || plateau->listeCases[xBetween][yBetween].type == 3)
            {
                plateau->listeCases[xBetween][yBetween].type = 0;
                aBouger(plateau, xCase, yCase, X, Y);
                if (peutMangerCase(plateau, X, Y) == 0)
                {
                    changementJoueur(plateau, 2);
                }
                else
                {
                    changementJoueur(plateau, 1);
                    plateau->listeCases[X][Y].selectionner = 1;
                }
            }
        }

        break;

    default:
        break;
    }
};

// fait toutes les actions necessaire quand une case à bouger
void aBouger(Plateau *plateau, int xCase, int yCase, int X, int Y)
{
    plateau->listeCases[X][Y].type = plateau->listeCases[xCase][yCase].type;
    plateau->listeCases[xCase][yCase].type = 0;
    plateau->listeCases[xCase][yCase].selectionner = 0;
    if (plateau->listeCases[X][Y].type == 1 && Y == 0)
    {
        plateau->listeCases[X][Y].type = 3;
    }
    if (plateau->listeCases[X][Y].type == 2 && Y == 9)
    {
        plateau->listeCases[X][Y].type = 4;
    }
};

// retourne : si une case est déja selectionner / si oui son x sinon -1 / si oui son y sinon -1
int caseDejaSelectionner(Plateau *plateau)
{
    int r = 0;
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            if (plateau->listeCases[x][y].selectionner == 1)
            {
                r = 1;
            }
        }
    }

    return r;
};

// dessin du plateau dans la console
void dessinerPlateauConsole(Plateau *plateau)
{
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            printf("| %d ", plateau->listeCases[x][y].type);
        }
        printf("| ");
        for (int x = 0; x < 10; x++)
        {
            printf("| %d ", plateau->listeCases[x][y].jouable);
        }
        printf("| \n");
    }
}