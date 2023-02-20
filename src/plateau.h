/*
 * jeu de dame
 * plateau.h
 *
 *  Created on: dec. 2022
 *      Author: PROUST--JUVIN François
 *              HERAULT Nathan
 *              PIEGELIN Baptiste
 */

#ifndef PLATEAU_H
#define PLATEAU_H


#include <stdlib.h>
#include <stdio.h>

/**
 * structure d'une case
 */
typedef struct cases
{
    int type; /** un type correspond à : 0->vide; 1->pionBlanc; 2->pionNoir; 3->dameBlanc; 4->dameNoir */
    int x;    /** coordonnée x de la case */
    int y;    /** coordonnée y de la case */
    int jouable; /** determine si la case est une case jouable */
    int selectionner; /** determine si la case est selectionner */
    int perdue; /** determine si le pion à été pris */
} Cases;


    /* -------------------------------------------------- */
    

/**
 * structure d'un plateau
 */
typedef struct plateau
{
    
    Cases listeCases[10][10]; /** tableau de cases de taille 10x10 */
} Plateau;

Plateau *creerPlateau();
void changementJoueur(Plateau *plateau, int aQui);
void selectionnerCase(Plateau *plateau, int X, int Y);
void deselectionnerCase(Plateau *plateau);
void selectionCaseJouable(Plateau *plateau, int X,int Y, int obligation);
int deplacementObligatoire(Plateau *plateau, int equipe);
int peutBougerCase(Plateau *plateau, int X, int Y);
int peutMangerCase(Plateau *plateau, int X, int Y);
void bougerCase(Plateau *plateau, int X, int Y, int DejaSelect);
void MangerCase(Plateau *plateau,int xCase, int yCase,int X, int Y);
void aBouger(Plateau *plateau, int xCase, int yCase, int X, int Y);
int caseDejaSelectionner(Plateau *plateau);
void dessinerPlateauConsole(Plateau *plateau);

#endif