//NAWAE 2018/09/03
//Dispositif de création de Cocktails
//Deuxième programme Arduino / Projet Cocktail Paramétrable

//CHARGEMENT DES LIBRAIRIES
#include <LiquidCrystal.h> // Chargement de la librairie pour l'écran LCD

//CONFIGURATION LCD
//LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7);
LiquidCrystal monEcran(14,15,16,17,18,19); // Configuration de l'écran

//CONFIGURATION COCKTAILS & BOUTONS
const int bout1 = 13; // Constante du pin bouton HAUT
const int bout2 = 11; // Constante du pin bouton BAS
const int bout3 = 12; // Constante du pin bouton VALIDATION
const int boutparam = 10; // Constante du pin bouton pour paramétrer
int posMenu = 0; // Variable de position dans le menu
int choix = 0; // Variable pour stocker le choix
int nombreDeRecette = 12; // Permet le modula pour revenir à 1 en cycle navigation
String ligne1 = "CHOIX COCKTAIL :"; // Affichage de la première ligne
String ligne2[12] = {"1-Ti Punch      ", "2-Gin Tonic'    ", "3-Planteur      ", "4-Loeiza (SA)      ","5-Gimlet       ","6-Le Bali    ","7-Christopher      ","8-Le Leo       ","9-Flora Dora     ","10-Gin Fizz       ","11-Bacardi Cocktail      ","12-Le Cendrillon (SA)      "}; // Tableau de recette
const int orange = 1; // Pin rattaché à la board relais, auquel est relié l'alimentation du moteur contrôlant la pompe de jus d'Orange.
const int schwepps = 2; // Pin pour la board relais
const int grenadine = 3; // Pin pour la board relais
const int ananas = 4; // Pin pour la board relais
const int citron = 5; // Pin pour la board relais
const int rhum = 6; // Pin pour la board relais
const int gin = 7; // Pin pour la board relais
const int canadou = 8; // Pin pour la board relais
float temps; // Variable qui stocke la mesure du temps
float avancement; // Variable pour la gestion de la barre de progression dynamique

void setup() {

  // Configuration pour la board relais
  pinMode(1, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(2, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(3, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(4, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(5, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(6, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(7, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(8, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais

  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on les désactive tout de suite. Sécurité
    digitalWrite(i,HIGH);
  }

  // Configuration de l'écran
  monEcran.begin(16,2); // On initialise la communication LCD avec 16 colonnes et deux lignes
  setup_progressbar(); // On charge les caractères pour la barre de progression
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(2,0); // On centre
  monEcran.print("DEMARRAGE"); 
  monEcran.setCursor(5,1); // On passe à la ligne suivante
  monEcran.print("EN COURS"); 
  delay(10000); // Permet de voir le rechargement du programme
  monEcran.clear(); // On efface l'écran

  // Configuration des boutons
  pinMode(bout1, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout2, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout3, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutparam, INPUT); // Déclaration du bouton comme entrée

}

void loop() {
  navigation(); // Appel de la fonction navigation en boucle pour capter les boutons
  affichage(); // Appel de la fonction affichage en boucle pour rafraichir l'écran en fonction des boutons (posmenu)
}

//FONCTION NAVIGATION 
void navigation() {
  boolean etatBout1 = digitalRead(bout1); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout2 = digitalRead(bout2); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout3 = digitalRead(bout3); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutParam = digitalRead(boutparam); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  //Boucle pour naviguer dans les menus
  if (etatBout1 || etatBout2 || etatBout3 || etatBoutParam) { // Boucle pour les boutons avec delay
    if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
      posMenu = (posMenu + 1) % nombreDeRecette; // On avance la position de lecture, On avance dans le tableau
    }
    if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
      if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
        posMenu = (nombreDeRecette-1) % nombreDeRecette; // On évite le chiffre négatif en retournant à la fin du tableau.
      }
      else {
        posMenu = (posMenu - 1) % nombreDeRecette; // On recule dans le tableau
      }
    }
    if (etatBout3) { // Si on valide la sélection, lancement de la distribution
      choix = posMenu +1; // Récupération du numéro de la recette (uniquement pour une question d'affichage)
      distribution(); // Lancement de la distribution
      monEcran.clear(); // On efface l'écran
      posMenu = 0; // On réinitialise l'affichage
    }
    if (etatBoutParam) { // Si le bouton Initialisation est appuyé
      initialisation(); //On lance la fonction qui charge les tuyaux
    }
  delay(500); //attente pour éviter les répétitions
  } 
}
//FONCTION AFFICHAGE
void affichage() {
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print(ligne1); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print(ligne2[posMenu]); // On affiche la recette
}

//FONCTION DISTRIBUTION DU COCKTAIL
void distribution() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("RECETTE "); // On affiche la recette
  monEcran.setCursor(8, 1); // On postionne le curseur sur la deuxième ligne au 2ème pixel
  monEcran.print(choix); // On affiche la recette
  delay (1000);
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION ..."); // On affiche le titre
  temps = millis(); // On sauvegarde l'heure actuel

  switch (posMenu) { // En fonction de la position dans le menu lors de la selection, on charge les pompes
     case 0: // Ti Punch
      //On allume toutes les pomptes du cocktails
      digitalWrite(rhum, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(3)*100);
       draw_progressbar(avancement);
       pompeV2(canadou,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(citron,2);
       pompeV2(rhum,3);
      } while ((millis() - temps) < conv(3)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide (ici rhum)
      break;
     case 1: // Gin Tonic'
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(10)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,10); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(gin,3);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(10)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 2: // Planteur
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(orange,6); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,1);
       pompeV2(ananas,6);
       pompeV2(citron,2);
       pompeV2(rhum,6);
       pompeV2(canadou,1);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 3: // Loeiza SA
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(8)*100);
       draw_progressbar(avancement);
       pompeV2(orange,8); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,3);
       pompeV2(ananas,6);
       pompeV2(citron,2);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(8)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 4: // Gimlet
      //On allume toutes les pomptes du cocktails
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(citron,2); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(gin,4);
       pompeV2(canadou,2);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 5: // Le Bali
       //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(schwepps, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(5)*100);
       draw_progressbar(avancement);
       pompeV2(orange,5); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(schwepps,5);
       pompeV2(ananas,5);
       pompeV2(citron,5);
       pompeV2(rhum,5);
       pompeV2(canadou,2);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(5)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 6: // Christopher
       //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(7)*100);
       draw_progressbar(avancement);
       pompeV2(orange,7); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(rhum,6);
       pompeV2(gin,3);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(7)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 7: // Le Léo
      //On allume toutes les pomptes du cocktails
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(5)*100);
       draw_progressbar(avancement);
       pompeV2(grenadine,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(ananas,4);
       pompeV2(citron,2);
       pompeV2(gin,5);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(5)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 8: // Flora Dora
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(10)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,10); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,1);
       pompeV2(citron,3);
       pompeV2(gin,4);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(10)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 9: // Gin Fizz
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(12)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,12); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(citron,4);
       pompeV2(gin,6);
       pompeV2(canadou,1);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(12)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 10: // Bacardi Cocktail
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,3);
       pompeV2(citron,3);
       pompeV2(rhum,4);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 11: // Cendrillon
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(schwepps, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(orange,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(schwepps,6);
       pompeV2(ananas,3);
       pompeV2(citron,2);
       pompeV2(canadou,2);
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     default: // Sinon
      monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
      monEcran.print("Erreur!"); // On affiche Erreur
      delay(1000);
      break;
  }
  draw_progressbar(100);
  monEcran.setCursor(0, 0); // On postionne le curseur sur la première ligne
  monEcran.print("A LA VOTRE !"); // On affiche un message de dégustation
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on les désactive tout de suite. Sécurité
    digitalWrite(i,HIGH);
  }
}

//FONCTION CONVERSATION CL EN MILLISECONDES
//Permet de changer de moteur sans changer tout le code.
long conv(long a) { // a = nombre de CL
  return a*6000;
}

//FONCTION POMPES CL  fonction permettant de rentrer l'alcool, la dose, et de faire les vérifications nécessaires
void pompeV2(int a, int b) { // a = numéro du PIN activant le relais; b = centilitre convertit en temps;
  if(digitalRead(a)==0) // Si la pompe est encore activée, alors on compare, sinon, on l'a déjà éteinte.
  {
    if((millis() - temps) > conv(b)) // Si la différence entre le temps sauvegardé et le temps actuel, soit le temps passé dans la boucle, est plus grand que le nombre de cl * 6 sec, alors on peux éteindre la pompe, la dose a déjà été versée
    {
      digitalWrite(a, HIGH); // On ferme le courant.
    }
  }
}

//FONCTION INITIALISATION DES POMPES
void initialisation() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("INITIALISATION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche 
     
  for(int i = 1; i <= 8; i++) {
     // Pour chaque Pin du relais, on active
    digitalWrite(i, LOW);
  }
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // Chargement des tuyaux, on patiente
      // On allume les lumières de chargement en attendant
      // colorLED(strip.Color(0, 250, 0), 50); // Simulation de Chargement
     } while ((millis() - temps) < 7500); // Tant qu'on n'a pas rempli les tuyaux en 7,5 sec, on laisse allumé.

    for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, HIGH);
  }
}

//FONCTION PURGE DES POMPES
void purge() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("PURGE"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on active
    digitalWrite(i, LOW);
  }
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // DeChargement des tuyaux, on patiente
      // On allume les lumières de DeChargement en attendant
      // colorLED(strip.Color(250, 0, 0), 50); // Simulation de Purge
     } while ((millis() - temps) < 10000); // Tant qu'on n'a pas rempli les tuyaux d'eau en 10 sec, on laisse allumé.

    for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, HIGH);
  }
}

//Chargement de caractères pour afficher la barre de progression
/* Caractères personnalisés */
byte DIV_0_OF_5[8] = {
  B00000, 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
}; // 0 / 5

byte DIV_1_OF_5[8] = {
  B10000, 
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
}; // 1 / 5

byte DIV_2_OF_5[8] = {
  B11000, 
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
}; // 2 / 5 

byte DIV_3_OF_5[8] = {
  B11100, 
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
}; // 3 / 5

byte DIV_4_OF_5[8] = {
  B11110, 
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
}; // 4 / 5

byte DIV_5_OF_5[8] = {
  B11111, 
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
}; // 5 / 5

//Issue de https://www.carnetdumaker.net/articles/faire-une-barre-de-progression-avec-arduino-et-liquidcrystal/
/**
 * Fonction de configuration de l'écran LCD pour la barre de progression.
 * Utilise les caractères personnalisés de 0 à 5 (6 et 7 restent disponibles).
 */
void setup_progressbar() {

  /* Enregistre les caractères personnalisés dans la mémoire de l'écran LCD */
  monEcran.createChar(0, DIV_0_OF_5);
  monEcran.createChar(1, DIV_1_OF_5);
  monEcran.createChar(2, DIV_2_OF_5);
  monEcran.createChar(3, DIV_3_OF_5);
  monEcran.createChar(4, DIV_4_OF_5);
  monEcran.createChar(5, DIV_5_OF_5);
}

void draw_progressbar(byte percent) {
  monEcran.setCursor(0, 1);
  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 5) */
  byte nb_columns = map(percent, 0, 100, 0, 16 * 5);
  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < 16; ++i) {
    /* En fonction du nombre de colonnes restant à afficher */
    if (nb_columns == 0) { // Case vide
      monEcran.write((byte) 0);
    } else if (nb_columns >= 5) { // Case pleine
      monEcran.write(5);
      nb_columns -= 5;
    } else { // Derniére case non vide
      monEcran.write(nb_columns);
      nb_columns = 0;
    }
  }
  delay(10);
}

