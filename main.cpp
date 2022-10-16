// PRG1_E
//---------------------------------------------------------------------------------
// Fichier        : main.cpp
// Auteur(s)      : BEE Gianni & PIEMONTESI Gwendal // Behrmann Urs & Vasques Dario
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
    const int LARGEUR_TEXT             = 20;   // Ajouté
    const int LARGEUR_VALEUR           = 13;   // Ajouté
    const int PRECISION                = 2;    // Ajouté

    const float PRIX_BAGAGE                 = 2.60f;
    const float PRIX_PRISE_EN_CHARGE        = 5.00f;
    const float PRIX_TARIF_PAR_MINUTE_JOUR  = 1.00f;
    const float PRIX_TARIF_PAR_MINUTE_NUIT  = 1.60f;
    const string HEURE_TARIF_JOUR           = "8h00 - 20h00";

    const int MAX_BAGAGE        = 4;
    const int MAX_DISTANCE      = 500;
    const int MIN_VITESSE       = 30;
    const int MAX_VITESSE       = 120;
    const int FIN_NUIT_MIN      = 480;  // 8H00
    const int DEBUT_NUIT_MIN    = 1200; // 20H00
    const int MINUIT            = 1440; // 24H00

    //Variable de saisie et de calcul
    int vit_moyenne;
    int heure;
    int minute;

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
         << right << setw(LARGEUR_VALEUR) << HEURE_TARIF_JOUR << endl
         << endl; // Rajout d'un saut à la ligne pour marquer la séparation entre les 2 paragraphes

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

    // Distance
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

    // Vitesse
    cout    << "- "
            << left << setw(LARGEUR_TEXT) << "vitesse [km/h]"
            << right << setw(LARGEUR_VALEUR) << "[30 - 120] " << ':'; // Changement text pour correspondre à la donnée
    cin  >> vit_moyenne;
    VIDE_BUFFER;

    if (vit_moyenne > MAX_VITESSE || vit_moyenne < MIN_VITESSE) {
        MESSAGE_ERREUR;
        VIDE_BUFFER;
        return EXIT_FAILURE;
    }

    // Heure depart
    cout    << "- "
            << left << setw(LARGEUR_TEXT) << "depart "
            << right << setw(LARGEUR_VALEUR) << "[hh:mm] " << ':'; // Changement text pour correspondre à la donnée
    cin >> heure;
    cin.ignore();
    cin >> minute;

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

    int dureeVoyageJour = 0;        // Total du voyage de jour
    int dureeVoyageNuit = 0;        // Total du voyage de nuit
    int dureeVoyageCalcule = 0;     // Valeur temporaire pour le calcule des temps de voyages dans les zones "nuit" et "jour"

    // Tant que pas toutes les minutes sont comptabilisées, on reste dans la boucle.
    while( dureeVoyage > 0){
        // Test si le départ est durant les heures de nuit 0 h 00 - 7 h 59
        if (departEnMinute < FIN_NUIT_MIN) {

            // Test si la durée du voyage sera continuée durant les heures de jour qui commence à FIN_NUIT_MIN
            departEnMinute + dureeVoyage < FIN_NUIT_MIN ? dureeVoyageCalcule = dureeVoyage : dureeVoyageCalcule = FIN_NUIT_MIN - departEnMinute;

            dureeVoyageNuit += dureeVoyageCalcule;
            dureeVoyage -= dureeVoyageCalcule;
            departEnMinute = FIN_NUIT_MIN;
        }
        // Test si le départ est durant les heures de jour 8 h 00 - 19 h 59
        if (departEnMinute < DEBUT_NUIT_MIN){

            departEnMinute + dureeVoyage < DEBUT_NUIT_MIN ? dureeVoyageCalcule = dureeVoyage : dureeVoyageCalcule = DEBUT_NUIT_MIN - departEnMinute ;

            dureeVoyageJour += dureeVoyageCalcule;
            dureeVoyage -= dureeVoyageCalcule;
            departEnMinute = DEBUT_NUIT_MIN;
        }
        // Test si le départ est durant les heures de jour 20 h 00 - 23 h 59
        if (departEnMinute >= DEBUT_NUIT_MIN){

            departEnMinute + dureeVoyage < MINUIT ? dureeVoyageCalcule = dureeVoyage : dureeVoyageCalcule = MINUIT - departEnMinute;

            dureeVoyageNuit += dureeVoyageCalcule;
            dureeVoyage -= dureeVoyageCalcule;
            departEnMinute = 0;
        }
    }

    // Calcule prix total d'après le temps de trajet trouvé
    float total_bagage = PRIX_BAGAGE * (float)nb_bagage;
    float prix_trajet_jour = (float)dureeVoyageJour * PRIX_TARIF_PAR_MINUTE_JOUR;
    float prix_trajet_nuit = (float)dureeVoyageNuit * PRIX_TARIF_PAR_MINUTE_NUIT;
    float total_trajet = PRIX_PRISE_EN_CHARGE + total_bagage + prix_trajet_jour + prix_trajet_nuit;

    //Affichage du ticket
    //Total arrondi, toutes minutes commencée est due.
    //Ajout de la largeur du texte setw() et restructuration de l'affichage du output pour correspondre à la donnée
    cout  << fixed << setprecision(2);
    cout << endl << "Votre ticket" << endl
         << "===============================================" << endl
         << "- " << left << setw(LARGEUR_TEXT) << "Prise en charge" << ':' // Ajout
         << right << setw(LARGEUR_VALEUR) << PRIX_PRISE_EN_CHARGE << endl
         << "- " << left << setw(LARGEUR_TEXT) << "Supp bagages" << ':' // Ajout
         << right << setw(LARGEUR_VALEUR) << total_bagage << endl
         << "- " << left << setw(LARGEUR_TEXT) << "temps course" << endl // Ajout
         << right << setw(LARGEUR_VALEUR) << dureeVoyageJour << "'" << " @ " << PRIX_TARIF_PAR_MINUTE_JOUR << " :"
         << right << setw(LARGEUR_VALEUR) << prix_trajet_jour << endl
         << right << setw(LARGEUR_VALEUR) << dureeVoyageNuit << "'" << " @ " << PRIX_TARIF_PAR_MINUTE_NUIT << " :"
         << right << setw(LARGEUR_VALEUR) << prix_trajet_nuit << endl
         << "---------------------" << "    " << "--------------------" << endl
         << right << setw(LARGEUR_TEXT) << "TOTAL" << "  :"
         << right << setw(LARGEUR_VALEUR) << trunc(total_trajet) << endl << endl;
         //<< "Prix de la course jour : "  << dureeVoyageJour << " min / " << prix_trajet_jour            << endl
         //<< "Prix de la course nuit : "  << dureeVoyageNuit << " min / " << prix_trajet_nuit            << endl
         //<< "TOTAL             : "  << trunc(total_trajet)    << endl << endl;

    //---------------------------------------------------------------------------------
    //Fin de programme
    cout << "Pressez ENTER pour quitter";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');                    // Vider le buffer
    return EXIT_SUCCESS;
}
