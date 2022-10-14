// PRG1_E
//---------------------------------------------------------------------------------
// Fichier        : main.cpp
// Auteur(s)      : BEE Gianni & PIEMONTESI Gwendal // Behrmann Urs & Vasques Dario Testrr
// But            : Labo 1 update - Programme pour le prix d'une course de taxi
// Modifications  :
// Remarque(s)    : NILL
// Compilateur    : MinGW w64 9.0 / C++20
// ---------------------------------------------------------------------------------

#include <iostream>     // Gestion des I/O
#include <cmath>        // Gestion des fonctions d'arrondi
#include <stdlib.h>     // Return Sucess
#include <iomanip>      // Setprecision

#define MESSAGE_ERREUR cout << endl << "Valeur hors limite. Pressez ENTER pour quitter"
#define VIDE_BUFFER cin.ignore(numeric_limits<streamsize>::max(), '\n')

using namespace std;    // Simplification std::cin <=> cin ; std::cout <=> cin

int main() {

    //Constantes
    // DVa : Modification de double en float
    const int LARGEUR_TEXT      = 20;   // Ajouté
    const int LARGEUR_VALEUR    = 10;   // Ajouté
    const int LARGEUR_NBRE      = 2;    // Ajouté
    const int PRECISION         = 2;    // Ajouté

    const float PRIX_BAGAGE                 = 2.60f;
    const float PRIX_PRISE_EN_CHARGE        = 5.00f;
    const float PRIX_TARIF_PAR_MINUTE_JOUR  = 1.00f;
    const float PRIX_TARIF_PAR_MINUTE_NUIT  = 1.60f;

    const int MAX_BAGAGE        = 4;
    const int MAX_DISTANCE      = 500;
    const int MIN_VITESSE       = 30;
    const int MAX_VITESSE       = 120;
    const int FIN_NUIT_MIN      = 480;  // 8H00
    const int DEBUT_NUIT_MIN    = 1200; // 20H00

    //Variable de saisie et de calcul
    int vit_moyenne;
    int heure;
    int minute;
    char lecture;

    int nb_bagage;
    int dist_course;

    // DVa : Ajout de la précision à l'affichage des chiffres
    cout << fixed << setprecision(PRECISION) <<endl;

    //---------------------------------------------------------------------------------
    //Message de bienvenue et présentation condition des conditions du voyageur
    // DVa : Ajout de la largeur du texte setw() et restructuration de l'affichage du output
    cout << "Bonjour," << endl
         << "Ce programme calcule le prix d'une course de taxi." << endl << endl
         << "Ci-dessous, les conditions pour votre voyage : " << endl
         << "=====================TARIF=====================" << endl
         << "- " << left << setw(LARGEUR_TEXT) << "Prise en charge" << ':'
         << right << setw(LARGEUR_VALEUR) << PRIX_PRISE_EN_CHARGE << endl
         << "- " << left << setw(LARGEUR_TEXT) << "Supp par bagage" << ":"
         << right << setw(LARGEUR_VALEUR) << PRIX_BAGAGE << endl
         << "- " << left << setw(LARGEUR_TEXT) << "tarif/minute (jour)" << ":"
         << right << setw(LARGEUR_VALEUR) << PRIX_TARIF_PAR_MINUTE_JOUR << endl
         << "- " << left << setw(LARGEUR_TEXT) << "tarif/minute (nuit)" << ":"
         << right << setw(LARGEUR_VALEUR) << PRIX_TARIF_PAR_MINUTE_NUIT << endl
         << "- " << left << setw(LARGEUR_TEXT) << "tarif jour" << ":"
         << right << setw(18) << "8h00" << " - " << "20h00" << endl;

    //---------------------------------------------------------------------------------
    //Variable de saisi par l'utilisateur
    cout    << "votre commande" << endl
            << "==============" << endl
            << "- "
            << left << setw(LARGEUR_TEXT) << "nbre de bagage"
            << right << setw(LARGEUR_VALEUR) << "[0 - 4] " << ':'; // Changement de l'output pour correspondre à la donnée
    cin  >> nb_bagage;
    VIDE_BUFFER;

    // Test de la saisie afin de savoir si elle appartient à l'intervale
    if (nb_bagage > MAX_BAGAGE || nb_bagage < 0) {
        MESSAGE_ERREUR;
        VIDE_BUFFER;
        return EXIT_FAILURE;
    }

    cout    << "- "
            << left << setw(LARGEUR_TEXT) << "distance [km]"
            << right << setw(LARGEUR_VALEUR) << "[0 - 500] " << ':'; // Changement text pour correspondre à la donnée
    cin  >> dist_course;
    VIDE_BUFFER;

    if (dist_course > MAX_DISTANCE || dist_course < 0) {
        MESSAGE_ERREUR;
        VIDE_BUFFER;
        return EXIT_FAILURE;
    }

    cout    << "- "
            << left << setw(LARGEUR_TEXT) << "vitesse [km/h]"
            << right << setw(LARGEUR_VALEUR) << "[30 -120] " << ':'; // Changement text pour correspondre à la donnée
    cin  >> vit_moyenne;
    VIDE_BUFFER;

    if (vit_moyenne > MAX_VITESSE || vit_moyenne < MIN_VITESSE) {
        MESSAGE_ERREUR;
        VIDE_BUFFER;
        return EXIT_FAILURE;
    }

    cout    << "- "
            << left << setw(LARGEUR_TEXT) << "depart"
            << right << setw(LARGEUR_VALEUR) << "[hh:mm] " << ':'; // Changement text pour correspondre à la donnée
    cin  >> heure >> lecture >> minute;
    VIDE_BUFFER;

    if ( heure >= 24 || heure < 0 || minute > 59 || minute < 0 ) {
        MESSAGE_ERREUR;
        VIDE_BUFFER;
        return EXIT_FAILURE;
    }

    //---------------------------------------------------------------------------------
    //Calcul du prix de la course
    int departEnMinute = heure * 60 + minute;
    int dureeVoyage = (int)ceil(dist_course * 60.0 / vit_moyenne);
    int dureeVoyageJour = 0;
    int dureeVoyageNuit = 0;

    if (departEnMinute < FIN_NUIT_MIN){
        if (departEnMinute + dureeVoyage < FIN_NUIT_MIN ){
            dureeVoyageNuit = dureeVoyage;
        }else{
            dureeVoyageNuit = FIN_NUIT_MIN - departEnMinute;
            dureeVoyageJour = dureeVoyage - dureeVoyageNuit;
        }
    }else if (departEnMinute < DEBUT_NUIT_MIN) {
        if (departEnMinute + dureeVoyage < DEBUT_NUIT_MIN){
            dureeVoyageJour = DEBUT_NUIT_MIN - departEnMinute;
            dureeVoyageNuit = dureeVoyage - dureeVoyageJour;
        }else{
            dureeVoyageJour = dureeVoyage;
        }
    }else{
        dureeVoyageNuit = dureeVoyage;
    }

    float total_bagage = PRIX_BAGAGE * (float)nb_bagage;                                         // Conversion implicite en INT
    float prix_trajet_jour = (float)dureeVoyageJour * PRIX_TARIF_PAR_MINUTE_JOUR;
    float prix_trajet_nuit = (float)dureeVoyageNuit * PRIX_TARIF_PAR_MINUTE_NUIT;
    float total_trajet = PRIX_PRISE_EN_CHARGE + total_bagage + prix_trajet_jour + prix_trajet_nuit;

    //Affichage du ticket
    //Total arrondi, toute minutes commencée est due.
    cout  << fixed << setprecision(2);
    cout << "Votre ticket" << endl
         << "===============================================" << endl
         << "Prise en charge   : "  << PRIX_PRISE_EN_CHARGE   << endl
         << "Supp bagages      : "  << total_bagage           << endl
         << "Prix de la course jour : "  << dureeVoyageJour << " min / " << prix_trajet_jour            << endl
         << "Prix de la course nuit : "  << dureeVoyageNuit << " min / " << prix_trajet_nuit            << endl
         << "TOTAL             : "  << trunc(total_trajet)    << endl << endl;

    //---------------------------------------------------------------------------------
    //Fin de programme
    cout << "Pressez ENTER pour quitter";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');                    // Vider le buffer
    return EXIT_SUCCESS;
}
