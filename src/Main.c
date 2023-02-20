/*
 * Jeu de dame
 * main.c
 *
 *  Created on: dec. 2022
 *      Author: PROUST--JUVIN François
 *              HERAULT Nathan
 *              PIEGELIN Baptiste
 */

/*  commande de compilation du jeu sur windows
        gcc src/main.c src/plateau.c -o bin/Jeu_Dame.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows ;
        gcc src/main.c src/plateau.c -o bin/Jeu_Dame_Debug.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2
*/

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "plateau.h"

#define WINDOWS_WIDTH 900
#define WINDOWS_HEIGHT 900
#define FPS_LIMIT 33

Plateau *p1;

void initPlateau();
void SDL_ExitWithError(const char *message);
void SDL_LimitFPS(unsigned int limit);

int main(int argc, char *argv[])
{
    // Version de SDL
    SDL_version nb;
    SDL_VERSION(&nb);
    printf("SDL version: %d.%d.%d \n", nb.major, nb.minor, nb.patch);

    /* -------------------------------------------------- */

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    /* -------------------------------------------------- */

    // initialisation du plateau de jeu + dessin dans la console
    initPlateau();

    // Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        SDL_ExitWithError("Init SDL");

    // création de la fenetre et du rendu
    if (SDL_CreateWindowAndRenderer(WINDOWS_WIDTH, WINDOWS_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu");
    SDL_SetWindowTitle(window, "Dames");

    // Initialisation des textures
    /* -------------------------------------------------- */
    SDL_Surface *image = NULL;
    SDL_Texture *Plateau = NULL;
    SDL_Texture *PionBlanc = NULL;
    SDL_Texture *PionNoir = NULL;
    SDL_Texture *DameBlanc = NULL;
    SDL_Texture *DameNoir = NULL;
    SDL_Texture *Selection = NULL;
    SDL_Texture *Acceuil = NULL;
    SDL_Texture *Pause = NULL;

    for (int i = 0; i < 8; i++)
    {
        switch (i)
        {
        case 0:
            image = SDL_LoadBMP("assets/plateau.bmp");
            break;
        case 1:
            image = SDL_LoadBMP("assets/pion_blanc.bmp");
            break;
        case 2:
            image = SDL_LoadBMP("assets/pion_noir.bmp");
            break;
        case 3:
            image = SDL_LoadBMP("assets/dame_blanc.bmp");
            break;
        case 4:
            image = SDL_LoadBMP("assets/dame_noir.bmp");
            break;
        case 5:
            image = SDL_LoadBMP("assets/selection.bmp");
            break;
        case 6:
            image = SDL_LoadBMP("assets/Acceuil.bmp");
            break;
        case 7:
            image = SDL_LoadBMP("assets/menu_pause_.bmp");
            break;
        default:
            break;
        }
        if (image == NULL)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_ExitWithError("impossible de charger l'image");
        }
        switch (i)
        {
        case 0:
            Plateau = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (Plateau == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 1:
            PionBlanc = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (PionBlanc == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 2:
            PionNoir = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (PionNoir == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 3:
            DameBlanc = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (DameBlanc == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 4:
            DameNoir = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (DameNoir == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 5:
            Selection = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (Selection == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 6:
            Acceuil = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (Acceuil == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        case 7:
            Pause = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (Pause == NULL)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de creer la texture");
            }
            break;
        default:
            break;
        }
    }

    /* -------------------------------------------------- */

    // rectangle de dessin
    SDL_Rect rectPlateau;
    SDL_Rect fondFenetre;
    SDL_Rect rectPiece[40];
    SDL_Rect rectSelection;
    SDL_Rect rectAcceuil;
    SDL_Rect rectPause;

    // parametrage des rectangle
    rectPlateau.x = 0;
    rectPlateau.y = 0;
    fondFenetre.x = 0;
    fondFenetre.y = 0;
    fondFenetre.w = WINDOWS_WIDTH;
    fondFenetre.h = WINDOWS_HEIGHT;
    rectSelection.w = 90;
    rectSelection.h = 90;
    rectAcceuil.x = 0;
    rectAcceuil.y = 0;
    rectPause.x = 0;
    rectPause.y = 0;
    int avancement = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (p1->listeCases[i][j].type != 0)
            {
                rectPiece[avancement].x = 10 + (p1->listeCases[i][j].x * 90);
                rectPiece[avancement].y = 10 + (p1->listeCases[i][j].y * 90);
                if (p1->listeCases[i][j].selectionner == 1)
                {
                    rectSelection.x = p1->listeCases[i][j].x * 90;
                    rectSelection.y = p1->listeCases[i][j].y * 90;
                }

                avancement++;
            }
        }
    }

    if (SDL_QueryTexture(Plateau, NULL, NULL, &rectPlateau.w, &rectPlateau.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("impossible de charger la texture Plateau");
    }

    if (SDL_QueryTexture(Acceuil, NULL, NULL, &rectAcceuil.w, &rectAcceuil.h) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("impossible de charger la texture Acceuil");
    }

    if (SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255) != 0)
        SDL_ExitWithError("impossible de changer la couleur pour le rendu");

    if (SDL_RenderFillRect(renderer, &fondFenetre) != 0)
        SDL_ExitWithError("impossible d'afficher le fond d'ecran");

    if (SDL_RenderCopy(renderer, Acceuil, NULL, &rectAcceuil) != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("impossible d'afficher la texture");
    }

    /* -------------------------------------------------- */

    SDL_bool partieEnCours = SDL_TRUE;
    SDL_bool boolAcceuil = SDL_TRUE;
    SDL_bool boolPlay = SDL_FALSE;
    SDL_bool boolPause = SDL_FALSE;

    // boucle du jeu
    /* -------------------------------------------------- */

    do
    {

        unsigned int frameLimit = 0;

        frameLimit = SDL_GetTicks() + FPS_LIMIT;
        SDL_LimitFPS(frameLimit);
        frameLimit = SDL_GetTicks() + FPS_LIMIT;
        SDL_Event event; // variable qui gere les evenements
        int CaseSelection = caseDejaSelectionner(p1);
        while (SDL_PollEvent(&event))
        {
            if (boolAcceuil)
            {
                switch (event.type)
                {
                case SDL_MOUSEMOTION:
                    // printf("%d / %d \n",event.motion.x,event.motion.y);
                    // event.motion.x;
                    // event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // printf("Clic en %dx/%dy\n", event.button.x, event.button.y);
                    // printf("Clic sur la case %dx/%dy\n", event.button.x / 90, event.button.y / 90);
                    if (event.button.x >= 215 && event.button.x < 215 + 460 &&
                        event.button.y >= 450 && event.button.y < 450 + 175)
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            boolAcceuil = SDL_FALSE;
                            boolPlay = SDL_TRUE;
                        }
                        if (event.button.button == SDL_BUTTON_RIGHT)
                        {
                        }
                    }
                    if (event.button.x >= 745 && event.button.x < 745 + 135 &&
                        event.button.y >= 20 && event.button.y < 20 + 50)
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            partieEnCours = SDL_FALSE;
                        }
                        if (event.button.button == SDL_BUTTON_RIGHT)
                        {
                        }
                    }

                    break;

                case SDL_QUIT:
                    partieEnCours = SDL_FALSE;
                    break;

                default:
                    break;
                }
            }

            if (!boolAcceuil && event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    boolPlay = !boolPlay;
                    boolPause = !boolPause;
                }
            }
            if (boolPlay)
            {
                switch (event.type)
                {
                case SDL_MOUSEMOTION:
                    // printf("%d / %d \n",event.motion.x,event.motion.y);
                    // event.motion.x;
                    // event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // printf("Clic en %dx/%dy\n", event.button.x, event.button.y);
                    // printf("Clic sur la case %dx/%dy\n", event.button.x / 90, event.button.y / 90);
                    if (event.button.x / 90 >= 0 && event.button.x / 90 < 10 &&
                        event.button.y / 90 >= 0 && event.button.y / 90 < 10)
                    {
                        if (event.button.button == SDL_BUTTON_RIGHT)
                        {
                            deselectionnerCase(p1);
                            printf("case deselectionner\n");
                        }
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            printf("%d\n", CaseSelection);
                            if (CaseSelection == 0)
                            {
                                selectionnerCase(p1, event.button.x / 90, event.button.y / 90);
                                printf("selection de la case %dx/%dy\n", event.button.x / 90, event.button.y / 90);
                            }
                            else
                            {
                                bougerCase(p1, event.button.x / 90, event.button.y / 90, CaseSelection);
                            }
                        }
                    }

                    break;

                case SDL_QUIT:
                    partieEnCours = SDL_FALSE;
                    break;

                default:
                    break;
                }
            }

            if (boolPause)
            {
                switch (event.type)
                {
                case SDL_MOUSEMOTION:
                    // printf("%d / %d \n",event.motion.x,event.motion.y);
                    // event.motion.x;
                    // event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // printf("Clic en %dx/%dy\n", event.button.x, event.button.y);
                    // printf("Clic sur la case %dx/%dy\n", event.button.x / 90, event.button.y / 90);
                    if (event.button.x >= 315 && event.button.x < 315 + 265 &&
                        event.button.y >= 420 && event.button.y < 420 + 100)
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            boolPlay = !boolPlay;
                            boolPause = !boolPause;
                        }
                        if (event.button.button == SDL_BUTTON_RIGHT)
                        {
                        }
                    }
                    if (event.button.x >= 315 && event.button.x < 315 + 265 &&
                        event.button.y >= 555 && event.button.y < 555 + 100)
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            partieEnCours = SDL_FALSE;
                        }
                        if (event.button.button == SDL_BUTTON_RIGHT)
                        {
                        }
                    }

                    break;

                case SDL_QUIT:
                    partieEnCours = SDL_FALSE;
                    break;

                default:
                    break;
                }
            }
        }

        // rendue du jeu
        /* -------------------------------------------------- */

        if (SDL_RenderFillRect(renderer, &fondFenetre) != 0)
            SDL_ExitWithError("impossible d'afficher la couleur du fond");

        if (boolAcceuil)
        {
            if (SDL_RenderCopy(renderer, Acceuil, NULL, &rectAcceuil) != 0)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible d'afficher la texture");
            }
        }

        if (boolPlay)
        {
            if (SDL_RenderCopy(renderer, Plateau, NULL, &rectPlateau) != 0)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible d'afficher la texture");
            }

            int avancement = 0;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (p1->listeCases[i][j].type != 0)
                    {
                        rectPiece[avancement].x = 10 + (p1->listeCases[i][j].x * 90);
                        rectPiece[avancement].y = 10 + (p1->listeCases[i][j].y * 90);
                        if (p1->listeCases[i][j].selectionner == 1)
                        {
                            rectSelection.x = p1->listeCases[i][j].x * 90;
                            rectSelection.y = p1->listeCases[i][j].y * 90;
                        }

                        avancement++;
                    }
                }
            }
            if (CaseSelection == 1)
                if (SDL_RenderCopy(renderer, Selection, NULL, &rectSelection) != 0)
                {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("impossible d'afficher la texture");
                }

            avancement = 0;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (p1->listeCases[i][j].type != 0)
                    {
                        switch (p1->listeCases[i][j].type)
                        {
                        case 1:
                            if (SDL_QueryTexture(PionBlanc, NULL, NULL, &rectPiece[avancement].w, &rectPiece[avancement].h) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible de charger la texture PionBlanc");
                            }
                            if (SDL_RenderCopy(renderer, PionBlanc, NULL, &rectPiece[avancement]) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible d'afficher la texture");
                            }
                            break;
                        case 2:
                            if (SDL_QueryTexture(PionNoir, NULL, NULL, &rectPiece[avancement].w, &rectPiece[avancement].h) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible de charger la texture PionNoir");
                            }
                            if (SDL_RenderCopy(renderer, PionNoir, NULL, &rectPiece[avancement]) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible d'afficher la texture");
                            }
                            break;
                        case 3:
                            if (SDL_QueryTexture(DameBlanc, NULL, NULL, &rectPiece[avancement].w, &rectPiece[avancement].h) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible de charger la texture DameBlanc");
                            }
                            if (SDL_RenderCopy(renderer, DameBlanc, NULL, &rectPiece[avancement]) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible d'afficher la texture");
                            }
                            break;
                        case 4:
                            if (SDL_QueryTexture(DameNoir, NULL, NULL, &rectPiece[avancement].w, &rectPiece[avancement].h) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible de charger la texture DameNoir");
                            }
                            if (SDL_RenderCopy(renderer, DameNoir, NULL, &rectPiece[avancement]) != 0)
                            {
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("impossible d'afficher la texture");
                            }
                            break;
                        default:
                            break;
                        }
                        avancement++;
                    }
                }
            }
        }
        if (boolPause)
        {
            if (SDL_QueryTexture(Pause, NULL, NULL, &rectPause.w, &rectPause.h) != 0)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible de charger la texture Pause");
            }
            if (SDL_RenderCopy(renderer, Pause, NULL, &rectPause) != 0)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ExitWithError("impossible d'afficher la texture");
            }
        }

        SDL_RenderPresent(renderer);

    } while (partieEnCours);

    // destruction des textures
    /* -------------------------------------------------- */
    SDL_DestroyTexture(Plateau);
    SDL_DestroyTexture(PionBlanc);
    SDL_DestroyTexture(PionNoir);
    SDL_DestroyTexture(DameNoir);
    SDL_DestroyTexture(DameBlanc);
    SDL_DestroyTexture(Selection);
    SDL_DestroyTexture(Acceuil);
    SDL_DestroyTexture(Pause);

    /* -------------------------------------------------- */

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
};

// initialisation du plateau
void initPlateau()
{
    p1 = creerPlateau();
}

// gestion des erreurs
void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

// gestion des FPS
void SDL_LimitFPS(unsigned int limit)
{
    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks)
        return;
    else if (limit > ticks + FPS_LIMIT)
        SDL_Delay(FPS_LIMIT);
    else
        SDL_Delay(limit - ticks);
}