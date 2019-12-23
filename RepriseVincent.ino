//NAWAE 2018/09/03
//Dispositif de création de Cocktails
//Deuxième programme Arduino / Projet Cocktail Paramétrable

//CHARGEMENT DES LIBRAIRIES
#include <LiquidCrystal.h> // Chargement de la librairie pour l'écran LCD
#include <pitches.h> // Chargement de la librairie pour la musique
#include <Adafruit_NeoPixel.h> // Chargement de la librairie pour Ring NeoPixe

//CONFIGURATION LCD
//LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7);
LiquidCrystal monEcran(52,50,48,46,44,42); // Configuration de l'écran

//CONFIGURATION MUSIQUE
const int SPEAKER = 54;
#define SILENCE  0
#define EIGHTH   1
#define QUARTER  2
#define HALF 3

//CONFIGURATION DE L'ANNEAU DE LEDS
const int LEDRING = 55;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, LEDRING, NEO_GRB + NEO_KHZ800);

// CONFIGURATION DU BLUETOOTH
char Buf;   // 1 caractère
String Ligne;  // La ligne de stockage
long Ligne2;

//CONFIGURATION COCKTAILS & BOUTONS
const int bout1 = 11; // Constante du pin bouton HAUT
const int bout2 = 13; // Constante du pin bouton BAS
const int bout3 = 12; // Constante du pin bouton VALIDATION
const int boutunique = 10; // Constante du pin bouton pour distribution unique
int posMenu = 0; // Variable de position dans le menu
int mode = 0; // Permet de selectionner le menu
int sauvnompompe = 0; // Variable qui permettra de garder le rang dans le tableau et non la valeur du i/o
const String ligne1[] = {"MODE :              ","Cocktails :           ","Distribution :   ","Setup :         ","Quantite :      ", "Chargement :    ", "Purge :         ", "Bluetooth :         "}; // Affichage de la première ligne
const String Menu[] = {"1 - Cocktails      ", "2 - Distribution", "3 - Setup       "}; // Affichage du mode Menu 0
String ligne2 = Menu[0]; // Initiation de la ligne 2 sur le menu pour le premier démarrage
const int nombreDeMenu = 3; // Constante du nombre de menu pour le modulo
const String Recettes[] = {"1-Margarita      ", "2-Bourbon Coca    ", "3-Rhum Coca      ", "4-Cosmopolitan      ","5-Blue Lagoon      ","6-Daiquiri      ","7-Long Island Ice Tea     ","8-Coca Cola       ","9-Cuba Libre     ","10-Black Russian       ", "11-Screwdriver     ", "12-42nd Street  ", "13-Orange Blossom  ", "14-Bourbon Sour   ", "15-Bourbon      ", "16-Tequila Sunrise  ", "17-Red Lion     ", "18-Rainbow Shot The Sheriff  ", "19-Kentucky Road  ", "20-White Lady   ", "RETOUR             "}; // Tableau de recette
//const String Recettes[13] = {"1-Ti Punch      "}; // Tableau de recette
const int nombreDeRecettes = 21; // Permet le modula pour revenir à 1 en cycle navigation (+1 pour RETOUR)
const String Pompes[] = {"Coca                ", "Jack Daniel's   ", "Woodford Reserve   ", "Vodka           ", "Rhum Blanc      ", "Gin           ", "Bacardi         ", "Canadou       ", "Jus d'Orange    ", "Tequila          ", "Cointreau    ", "Curacao Bleu    ", "Citron Vert   ", "Cranberry   ", "Vermouth Dry   ", "Citron           ", "Rye Whisky      ", "Kahlua       ", "Grenadine       ", "Invite Mistere     ", "RETOUR            "}; // Affichage du menu pour la distribution
//const String Pompes[] = {"Pompe 1 ", "Pompe 2 "}; // Affichage du menu pour la distribution
const int nombreDePompes = 21; // Constante du nombre de pompe pour le modulo (+1 pour RETOUR)
int lapompe ; // Variable pour sauvegarder le numéro de la pompe
const String CL[] = {"1 cl            ", "2 cl            ", "3 cl            ", "4 cl            ", "5 cl            ", "6 cl            ", "7 cl            ", "8 cl            ", "9 cl            ", "10 cl          "}; // Affichage pour la distribution unique
//const String CL[] = {"1 cl    ", "2 cl    ", "3 cl    "}; // Affichage pour la distribution unique
const int nombreDeCL = 10; // Constante du nombre de CL pour le modulo
const String Setup[] = {"Chargement    ", "Purge     ", "RETOUR    "}; // Menu SETUP
const int nombreDeSetup = 3; // Constante du Setup pour le modulo
const int coca = 22; // Pin rattaché à la board relais, auquel est relié l'alimentation du moteur contrôlant la pompe de jus d'Orange.
const int jack = 23; // Pin pour la board relais
const int woodford = 24; // Pin pour la board relais
const int vodka = 25; // Pin pour la board relais
const int rhum = 26; // Pin pour la board relais
const int gin = 27; // Pin pour la board relais
const int bacardi = 28; // Pin pour la board relais
const int canadou = 29; // Pin pour la board relais
const int orange = 30; // Pin pour la board relais
const int tequila = 31; // Pin pour la board relais
const int cointreau = 32; // Pin pour la board relais
const int curacao = 33; // Pin pour la board relais
const int citronv = 34; // Pin pour la board relais
const int cranberry = 35; // Pin pour la board relais
const int vermouth = 36; // Pin pour la board relais
const int citron = 37; // Pin pour la board relais
const int rye = 39; // Pin pour la board relais
const int kahlua = 47; // Pin pour la board relais
const int grenadine = 43; // Pin pour la board relais
const int whisky = 45; // Pin pour la board relais


float temps; // Variable qui stocke la mesure du temps
float avancement; // Variable pour la gestion de la barre de progression dynamique

void setup() {

  // Configuration pour la board relais
  pinMode(22, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(23, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(24, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(25, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(26, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(27, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(28, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(29, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(30, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(31, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(32, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(33, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(34, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(35, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(36, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(37, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(39, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(47, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(43, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(45, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais

  coupurepompes(); // Pour chaque pin du relais, on coupe par sécurité.

  // Configuration de l'écran
  monEcran.begin(16,2); // On initialise la communication LCD avec 16 colonnes et deux lignes
  setup_progressbar(); // On charge les caractères pour la barre de progression
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(2,0); // On centre
  monEcran.print("DEMARRAGE"); 
  monEcran.setCursor(5,1); // On passe à la ligne suivante
  monEcran.print(" EN COURS"); 

  // Configuration des boutons
  pinMode(bout1, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout2, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout3, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutunique, INPUT); // Déclaration du bouton comme entrée

  //Configuration du haut-parleur
  pinMode(SPEAKER, OUTPUT); // Configuration du pin speaker comme sortie.

  // Configuration de l'anneau de leds
  pinMode(LEDRING, OUTPUT); // Configuration du pin 22 comme sortie pour les leds ring.
  strip.begin();
  colorLED(strip.Color(0, 250, 0), 200); // Simulation de Chargement
  colorLED(strip.Color(0, 250, 0), 200); // Simulation de Chargement
  colorLED(strip.Color(0, 0, 0), 1); // Simulation de Chargement  

  // Configuration Bluetooth
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  Serial3.begin(9600); // 38400 (OK en 9600 AT+PASS) Pour le mode commande et KEY (Jaune) branché sur 3,3, sinon 9600 et câble débranché
}

void loop() {
  String message;
  int i;
  int j;
  j=0; // Initialisation de la variable permettant de gérer 255 couleurs. 
  do // Boucle do permettant de requeter les fonctions de navigations à chaque boucle de couleurs. Permettant un refresh fréquent.
  {
    navigation(); // Appel de la fonction navigation en boucle pour capter les boutons
    affichage(); // Appel de la fonction affichage en boucle pour rafraichir l'écran en fonction des boutons (posmenu)

    // Début Bluetooth
   while (Serial3.available() > 0) {  // Tant qu'on a des données sur le port série bluetooth, on les lit
    Buf = Serial3.read();  // On lit le premier caractère
    //Serial.println(Buf);   // Debug affichage Série PC
    if (Buf == '*') { // Si le caractère * apparait, c'est que la valeur envoyée est terminée
      Serial.println("Sequence: "); // Debug affichage de la séquence Série PC
      delay(10);
      Serial.println(Ligne);
      delay(10);
    }
    else { // S'il n'y a pas de caractère * qui apparait
      Ligne = Ligne + Buf;  // Alors on concatène le caractère dans la chaine
      //Serial.println(Ligne); // Debug affichage de la séquence Série PC
      delay(10);
    }
   }
   if (Ligne != "") { // Si Ligne de valeur reçue est vide, on ne fait rien sinon 
    Ligne2 = Ligne.toInt(); // On transforme le String en long via toInt()
    switch (Ligne2) { // En fonction du retour du long reçu on fait une action
      case 1: // Si la valeur 0 est envoyée, c'est donc le premier cocktail, Ti'Punch
        Serial.println("Service du cocktail numéro 1 !");
        posMenu = 0; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 2:
        Serial.println("Service du cocktail numéro 2 !");
        posMenu = 1; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 3:
        Serial.println("Service du cocktail numéro 3 !");
        posMenu = 2; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 4:
        Serial.println("Service du cocktail numéro 4 !");
        posMenu = 3; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 5:
        Serial.println("Service du cocktail numéro 5 !");
        posMenu = 4; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 6:
        Serial.println("Service du cocktail numéro 6 !");
        posMenu = 5; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 7:
        Serial.println("Service du cocktail numéro 7 !");
        posMenu = 6; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 8:
        Serial.println("Service du cocktail numéro 8 !");
        posMenu = 7; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 9:
        Serial.println("Service du cocktail numéro 9 !");
        posMenu = 8; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 10:
        Serial.println("Service du cocktail numéro 10 !");
        posMenu = 9; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 11:
        Serial.println("Service du cocktail numéro 11 !");
        posMenu = 10; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 12:
        Serial.println("Service du cocktail numéro 12 !");
        posMenu = 11; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 13:
        Serial.println("Service du cocktail numéro 13 !");
        posMenu = 12; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 14:
        Serial.println("Service du cocktail numéro 14 !");
        posMenu = 13; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 15:
        Serial.println("Service du cocktail numéro 15 !");
        posMenu = 14; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 16:
        Serial.println("Service du cocktail numéro 16 !");
        posMenu = 15; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 17:
        Serial.println("Service du cocktail numéro 17 !");
        posMenu = 16; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 18:
        Serial.println("Service du cocktail numéro 18 !");
        posMenu = 17; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 19:
        Serial.println("Service du cocktail numéro 19 !");
        posMenu = 18; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      case 20:
        Serial.println("Service du cocktail numéro 20 !");
        posMenu = 19; 
        distribution();
        mode=0; // On revient au mode général
        posMenu = 0;
        ligne2 = Menu[posMenu];
        break;
      default:
        Serial.println("Erreur");
        break;
    }
   }
    Ligne =""; // On vide la variable Ligne pour la prochaine commande
    //Fin Bluetooth

    for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
    }
    strip.show(); // On lance la mise à jour des couleurs
    delay(50); // On attend 50 millisec avant de rechanger
  j++; // On incrémente le compteur, et on passe à une autre couleur.
  } while (j<256);

}

//FONCTION NAVIGATION 
void navigation() {
  boolean etatBout1 = digitalRead(bout1); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout2 = digitalRead(bout2); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout3 = digitalRead(bout3); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutUnique = digitalRead(boutunique); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  //Boucle pour naviguer dans les menus
  switch (mode) {
    case 0: // Cas du mode Menu principal
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDeMenu; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Menu[posMenu]; // On met à jour la valeur de la deuxième ligne avec le nouveau champ.
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDeMenu-1) % nombreDeMenu; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDeMenu; // On recule dans le tableau
       }
       ligne2 = Menu[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, on rentre dans un menu
       monEcran.clear(); // On efface l'écran
       mode = posMenu + 1; // On positionne le mode en fonction de la position du curseur +1 pour l'affichage
       switch (mode) {
          case 1:
            ligne2 = Recettes[0]; // Si c'est la première selection, alors cocktails, donc on affiche le menu des recettes
            posMenu = 0; // On repositionne le curseur au début
            break;
          case 2:
            ligne2 = Pompes[0]; // Si c'est la deuxième selection, alors distribution unique, donc on affiche le tableau des pompes
            posMenu = 0;
            break;
          case 3:
            ligne2 = Setup[0]; // Si c'est la troisième selection, alors on lance le mode SETUP
            posMenu = 0;
            break;
          default:
            ligne2 = "Erreur";
            break;
       }
       }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
  }
     delay(200); //attente pour éviter les répétitions
     break;
    case 1: //On lance la phase cocktails
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDeRecettes; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Recettes[posMenu]; 
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDeRecettes-1) % nombreDeRecettes; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDeRecettes; // On recule dans le tableau
       }
       ligne2 = Recettes[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, on lance la distribution du cocktails ou on retourne à l'accueil
        if (posMenu == (nombreDeRecettes-1)) { // Si la position dans le menu cocktails est la dernière recette, alors c'est un retour, sinon on lance la distribution
          mode = 0;
          ligne2 = Menu[0];
          posMenu = 0;
        }
        else {
          distribution();
          mode=0; // On revient au mode général
          posMenu = 0;
          ligne2 = Menu[posMenu];
          }
        }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
    case 2: // On lance la phase de distribution
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDePompes; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Pompes[posMenu];
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDePompes-1) % nombreDePompes; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDePompes; // On recule dans le tableau
       }
       ligne2 = Pompes[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, 
        if (posMenu == (nombreDePompes-1)) { // Si la position dans le menu cocktails est la dernière , alors c'est un retour, sinon on lance la distribution unique
          mode = 0; // Renvoi vers le menu général
          ligne2 = Menu[0]; // Deuxieme ligne avec le choix du mode
          posMenu = 0;
        }
        else { // On a validé le choix de la pompe. On sauvegarde le numéro, et on propose le nombre de cl.
       switch (posMenu) {
          case 0:
            // La pompe 1
            lapompe = 22;
            break;
          case 1:
            lapompe = 23;
            break;
          case 2:
            lapompe = 24;
            break;
          case 3:
            lapompe = 25;
            break;
          case 4:
            lapompe = 26;
            break;
          case 5:
            lapompe = 27;
            break;
          case 6:
            lapompe = 28;
            break;
          case 7:
            lapompe = 29;
            break;
          case 8:
            lapompe = 30;
            break;
          case 9:
            lapompe = 31;
            break;
          case 10:
            lapompe = 32;
            break;
          case 11:
            lapompe = 33;
            break;
          case 12:
            lapompe = 34;
            break;
          case 13:
            lapompe = 35;
            break;
          case 14:
            lapompe = 36;
            break;
          case 15:
            lapompe = 37;
            break;
          case 16:
            lapompe = 39;
            break;
          case 17:
            //La Pompe 18, 41 HS
            lapompe = 47;
            break;
          case 18:
            lapompe = 43;
            break;
          case 19:
            lapompe = 45;
            break;
          default:
            ligne2 = "Erreur";
            break;
       }
       sauvnompompe = posMenu ; // On sauvegarde la place dans le tableau des pomptes pour l'afficher lors de la distribution
       posMenu = 0 ;
       mode = 4 ; // On passe en mode "caché" en affichant le menu mode 4 pour les Cls.
       ligne2 = CL[posMenu];
        }
      }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
    case 3: // SETUP
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDeSetup; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Setup[posMenu];
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDeSetup-1) % nombreDeSetup; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDeSetup; // On recule dans le tableau
       }
       ligne2 = Setup[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection,
        if (posMenu == (nombreDeSetup-1)) { // Si la position dans le menu est la dernière , alors c'est un retour, sinon on lance
          mode = 0; // Renvoi vers le menu général
          ligne2 = Menu[0]; // Deuxieme ligne avec le choix du mode
          posMenu = 0;
        }
        else {
        if (posMenu == 0) { // Si on est sur le 0, cela signifie le Chargement
        mode = 5; // Chargement du mode caché pour le chargement
        ligne2 = Pompes[0];
        posMenu = 0;        
       }
       else { // On lance la purge
        mode = 6; // Chargement du mode caché pour la purge
        ligne2 = Pompes[0];
        posMenu = 0;        
       }
       }
       }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
    case 4: // Sous menu pour le nombre de Cl pour le Shaker
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDeCL; // On avance la position de lecture, On avance dans le tableau
       ligne2 = CL[posMenu];
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDeCL-1) % nombreDeCL; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDeCL; // On recule dans le tableau
       }
       ligne2 = CL[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, on lance la fonction de distribution unique 
        monEcran.clear(); // On efface l'écran
        monEcran.setCursor(0, 0); // On initialise le curseur en haut
        monEcran.print("DISTRIBUTION"); // On affiche le titre
        monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
        monEcran.print("UNIQUE "); // On affiche la recette
        monEcran.setCursor(8, 1); // On postionne le curseur sur la deuxième ligne au 2ème pixel
        delay (1000);
        monEcran.setCursor(0, 0); // On initialise le curseur en haut
        monEcran.print(Pompes[sauvnompompe]); // On affiche le champ texte du tableau pompes lors de la validation de l'utilisateur
        //On allume la pompe
      temps = millis(); // On sauvegarde l'heure actuel
      digitalWrite(lapompe, LOW); // Récupération de la sauvegarde du menu 2
      do // On lance une boucle qui ne s'arrêtera qu'à la fin de la distribution.
      {
       avancement = ((millis() - temps)/conv(posMenu+1)*100); // Le "temps" posMenu+1 correspond à la position dans le menu lors du choix du nombre de CL. Si on choisi 4CL, alors on demande la position (3) +1. La fonction conv(4) permet ensuite de transformer cela en temps.
       draw_progressbar(avancement);
       pompeV2(lapompe, (posMenu+1)); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
      } while ((millis() - temps) < conv(posMenu+1)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide 
      draw_progressbar(100);
      monEcran.setCursor(0, 0); // On postionne le curseur sur la première ligne
      monEcran.print("QUANTITE SERVIE"); // On affiche un message de dégustation
      delay(2000);
      monEcran.clear(); // On efface l'écran pour éviter de voir le reste de la barre de progression
      posMenu = 0 ; // On remet au debut de la ligne
      //mode = 0 ;
      mode = 2 ; // Repositionnement du mode 2, mode de distribution unique pour shaker, pour éviter de devoir renaviguer pour un autre ingrédient.
      //ligne2 = Menu[0];
      ligne2 = Pompes[0]; // On réaffiche la deuxieme ligne correspondant au choix de la Pompes pour le shaker
     }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
    case 5: // Sous menu pour le chargement unitaire des pompes.
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDePompes; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Pompes[posMenu];
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDePompes-1) % nombreDePompes; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDePompes; // On recule dans le tableau
       }
       ligne2 = Pompes[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, 
        if (posMenu == (nombreDePompes-1)) { // Si la position dans le menu  est la dernière , alors c'est un retour, sinon on lance le chargement
          mode = 0; // Renvoi vers le menu général
          ligne2 = Menu[0]; // Deuxieme ligne avec le choix du mode
          posMenu = 0;
        }
        else { // On a validé le choix de la pompe. On lance le chargement du tuyau
      // Chargement du tuyau unitairement, on va récupérer la position dans le menu, et prendre le pin qui y fait référence via un switch, puis on laissera charger x cl
       switch (posMenu) {
          case 0:
            // La pompe 1
            lapompe = 22;
            break;
          case 1:
            lapompe = 23;
            break;
          case 2:
            lapompe = 24;
            break;
          case 3:
            lapompe = 25;
            break;
          case 4:
            lapompe = 26;
            break;
          case 5:
            lapompe = 27;
            break;
          case 6:
            lapompe = 28;
            break;
          case 7:
            lapompe = 29;
            break;
          case 8:
            lapompe = 30;
            break;
          case 9:
            lapompe = 31;
            break;
          case 10:
            lapompe = 32;
            break;
          case 11:
            lapompe = 33;
            break;
          case 12:
            lapompe = 34;
            break;
          case 13:
            lapompe = 35;
            break;
          case 14:
            lapompe = 36;
            break;
          case 15:
            lapompe = 37;
            break;
          case 16:
            lapompe = 39;
            break;
          case 17:
            //La Pompe 18, 41 HS
            lapompe = 47;
            break;
          case 18:
            lapompe = 43;
            break;
          case 19:
            lapompe = 45;
            break;
          default:
            ligne2 = "Erreur";
            break;
       }
      temps = millis(); // On sauvegarde l'heure actuel
      digitalWrite(lapompe, LOW); // On ouvre la pompe
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du chargement.
      {
       avancement = ((millis() - temps)/conv(1)*100);
       draw_progressbar(avancement);
       pompeV2(lapompe,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(1)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      monEcran.clear(); // On efface l'écran pour éviter de voir le reste de la barre de progression
      //20191214 On commente pour ne pas voir afficher Coca à chaque Chargement //posMenu = 0 ; // On remet au debut de la ligne
      //mode = 0 ;
      mode = 5 ; // Repositionnement du mode , mode de chargement unique, pour éviter de devoir renaviguer pour un autre chargement.
      //ligne2 = Menu[0];
      ligne2 = Pompes[posMenu]; // On réaffiche la deuxieme ligne correspondant au choix de la Pompes
        }
      }
     if (etatBoutUnique) { // Si le bouton whisky est appuyé
       distrunique(); //On lance la fonction qui charge le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
    case 6: // Sous menu pour la purge unitaire des pompes.
     if (etatBout1 || etatBout2 || etatBout3 || etatBoutUnique) { // Boucle pour les boutons avec delay
     if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
       posMenu = (posMenu + 1) % nombreDePompes; // On avance la position de lecture, On avance dans le tableau
       ligne2 = Pompes[posMenu];
     }
     if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
       if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
          posMenu = (nombreDePompes-1) % nombreDePompes; // On évite le chiffre négatif en retournant à la fin du tableau.
       }
       else {
         posMenu = (posMenu - 1) % nombreDePompes; // On recule dans le tableau
       }
       ligne2 = Pompes[posMenu];
     }
      if (etatBout3) { // Si on valide la sélection, 
        if (posMenu == (nombreDePompes-1)) { // Si la position dans le menu  est la dernière , alors c'est un retour, sinon on lance le chargement
          mode = 0; // Renvoi vers le menu général
          ligne2 = Menu[0]; // Deuxieme ligne avec le choix du mode
          posMenu = 0;
        }
        else { // On a validé le choix de la pompe. On lance la purge du tuyau
        // Chargement du tuyau unitairement, on va récupérer la position dans le menu, et prendre le pin qui y fait référence via un switch, puis on laissera charger x cl
       switch (posMenu) {
          case 0:
            // La pompe 1
            lapompe = 22;
            break;
          case 1:
            lapompe = 23;
            break;
          case 2:
            lapompe = 24;
            break;
          case 3:
            lapompe = 25;
            break;
          case 4:
            lapompe = 26;
            break;
          case 5:
            lapompe = 27;
            break;
          case 6:
            lapompe = 28;
            break;
          case 7:
            lapompe = 29;
            break;
          case 8:
            lapompe = 30;
            break;
          case 9:
            lapompe = 31;
            break;
          case 10:
            lapompe = 32;
            break;
          case 11:
            lapompe = 33;
            break;
          case 12:
            lapompe = 34;
            break;
          case 13:
            lapompe = 35;
            break;
          case 14:
            lapompe = 36;
            break;
          case 15:
            lapompe = 37;
            break;
          case 16:
            lapompe = 39;
            break;
          case 17:
            //La Pompe 18, 41 HS
            lapompe = 47;
            break;
          case 18:
            lapompe = 43;
            break;
          case 19:
            lapompe = 45;
            break;
          default:
            ligne2 = "Erreur";
            break;
       }
       temps = millis(); // On sauvegarde l'heure actuel
      digitalWrite(lapompe, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du chargement.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(lapompe,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      monEcran.clear(); // On efface l'écran pour éviter de voir le reste de la barre de progression
      //20191214 On commente pour ne pas voir afficher Coca à chaque Chargement //posMenu = 0 ; // On remet au debut de la ligne
      //mode = 0 ;
      mode = 6 ; // Repositionnement du mode , mode de purge unique, pour éviter de devoir renaviguer pour une autre purge.
      //ligne2 = Menu[0];
      ligne2 = Pompes[posMenu]; // On réaffiche la deuxieme ligne correspondant au choix de la Pompes
        }
      }
     if (etatBoutUnique) { // Si le bouton supplémentaire est appuyé
       distrunique(); //On lance la distribution pour le whisky
     }
     }
     delay(200); //attente pour éviter les répétitions
     break;
  }
}

//FONCTION AFFICHAGE
void affichage() {
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print(ligne1[mode]); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print(ligne2); // On affiche la recette
}

//FONCTION DISTRIBUTION DU COCKTAIL
void distribution() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION    "); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print(Recettes[posMenu]); // On affiche la recette
  delay (1000);
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION ..."); // On affiche le titre
  temps = millis(); // On sauvegarde l'heure actuel
  int i; // Variable pour le rainbow maison
  int j; // Variable pour le rainbow maison
  j=0; // Initialisation de la variable pour les 255 couleurs

  switch (posMenu) { // En fonction de la position dans le menu lors de la selection, on charge les pompes
     case 0: // Margarita
      //On allume toutes les pomptes du cocktails
      digitalWrite(tequila, LOW);
      digitalWrite(cointreau, LOW);
      digitalWrite(citronv, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent = conv(x), x étant le plus grand liquide
      {
       avancement = ((millis() - temps)/conv(5)*100);
       draw_progressbar(avancement);
       pompeV2(tequila,5); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(cointreau,3);
       pompeV2(citronv,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
      
      
      } while ((millis() - temps) < conv(5)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide (ici tequila)
      break;
     case 1: // Bourbon Coca
      //On allume toutes les pomptes du cocktails
      digitalWrite(coca, LOW);
      digitalWrite(jack, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(3)*100);
       draw_progressbar(avancement);
       pompeV2(coca,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(jack,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(3)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 2: // Rhum Coca
      //On allume toutes les pomptes du cocktails
      digitalWrite(coca, LOW);
      digitalWrite(bacardi, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(3)*100);
       draw_progressbar(avancement);
       pompeV2(coca,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(bacardi,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
      //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(3)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 3: // Cosmopolitan
      //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(cointreau, LOW);
      digitalWrite(citronv, LOW);
      digitalWrite(cranberry, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(cointreau,2);
       pompeV2(citronv,1);
       pompeV2(cranberry,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 4: // Blue Lagoon
      //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(curacao, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(curacao,3);
       pompeV2(citron,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 5: // Daiquiri
       //On allume toutes les pomptes du cocktails
      digitalWrite(bacardi, LOW);
      digitalWrite(canadou, LOW);
      digitalWrite(citronv, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(bacardi,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(canadou,1);
       pompeV2(citronv,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 6: // Long island iced tea
       //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(gin, LOW);
      digitalWrite(tequila, LOW);
      digitalWrite(cointreau, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(2)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,2); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(rhum,2);
       pompeV2(gin,2);
       pompeV2(tequila,2);
       pompeV2(cointreau,2);
       pompeV2(citron,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(2)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 7: // Coca
      //On allume toutes les pomptes du cocktails
      digitalWrite(coca, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(1)*100);
       draw_progressbar(avancement);
       pompeV2(coca,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(1)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 8: // Cuba Libre
      //On allume toutes les pomptes du cocktails
      digitalWrite(coca, LOW);
      digitalWrite(bacardi, LOW);
      digitalWrite(citronv, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(coca,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(bacardi,6);
       pompeV2(citronv,4);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 9: // Black Russian
      //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(kahlua, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,6); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(kahlua,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 10: // Screwdriver
      //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(orange, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(12)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(orange,12);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(12)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
      
     case 11: // 42nd Street
      //On allume toutes les pomptes du cocktails
      digitalWrite(jack, LOW);
      digitalWrite(cointreau, LOW);
      digitalWrite(vermouth, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(jack,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(cointreau,2);
       pompeV2(vermouth,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 12: // Orange Blossom
      //On allume toutes les pomptes du cocktails
      digitalWrite(gin, LOW);
      digitalWrite(orange, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(gin,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(orange,4);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 13: // Bourbon Sour
      //On allume toutes les pomptes du cocktails
      digitalWrite(jack, LOW);
      digitalWrite(canadou, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(jack,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(canadou,1);
       pompeV2(citron,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 14: // Bourdon 
      //On allume toutes les pomptes du cocktails
      digitalWrite(woodford, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(woodford,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 15: // Tequila Sunrise 
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(tequila, LOW);
      digitalWrite(grenadine, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(8)*100);
       draw_progressbar(avancement);
       pompeV2(orange,8); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(tequila,4);
       pompeV2(grenadine,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(8)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 16: // Red Lion
      //On allume toutes les pomptes du cocktails
      digitalWrite(gin, LOW);
      digitalWrite(orange, LOW);
      digitalWrite(cointreau, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(3)*100);
       draw_progressbar(avancement);
       pompeV2(gin,2); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(orange,1);
       pompeV2(cointreau,3);
       pompeV2(citron,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(3)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 17: // rainbow shot the sheriff
      //On allume toutes les pomptes du cocktails
      digitalWrite(vodka, LOW);
      digitalWrite(orange, LOW);
      digitalWrite(curacao, LOW);
      digitalWrite(grenadine, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(10)*100);
       draw_progressbar(avancement);
       pompeV2(vodka,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(orange,10);
       pompeV2(curacao,2);
       pompeV2(grenadine,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(10)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 18: // Kentucky road
      //On allume toutes les pomptes du cocktails
      digitalWrite(coca, LOW);
      digitalWrite(jack, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(1)*100);
       draw_progressbar(avancement);
       pompeV2(coca,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(jack,4);
       pompeV2(citron,4);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(1)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 19: // White Lady
      //On allume toutes les pomptes du cocktails
      digitalWrite(gin, LOW);
      digitalWrite(cointreau, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(2)*100);
       draw_progressbar(avancement);
       pompeV2(gin,2); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(cointreau,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison  
       //Serial.println((millis() - temps));
      } while ((millis() - temps) < conv(2)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     default: // Sinon
      monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
      monEcran.print("Erreur!"); // On affiche Erreur
      delay(1000);
      break;
  }
  coupurepompes(); // Pour chaque pin du relais, on coupe par sécurité.
  draw_progressbar(100);
  monEcran.setCursor(0, 0); // On postionne le curseur sur la première ligne
  monEcran.print("A LA VOTRE !    "); // On affiche un message de dégustation

  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
}

//FONCTION CONVERSATION CL EN MILLISECONDES
//Permet de changer de moteur sans changer tout le code.
long conv(long a) { // a = nombre de CL
  return a*6000; // Ici, c'est donc le temps à mettre en milliseconde qui correspond à 1 cl en fonction de la puissance des moteurs.
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

//FONCTION WHISKY
void distrunique() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("Distribution    "); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("Whisky          "); // On affiche
  delay (1000);
  temps = millis(); // On sauvegarde l'heure actuel
  digitalWrite(whisky, LOW);
  do // On lance une boucle qui ne s'arrêtera qu'à la fin du chargement.
  {
   avancement = ((millis() - temps)/conv(4)*100);
   draw_progressbar(avancement);
   pompeV2(posMenu,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
   //Serial.println((millis() - temps));
   } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
   digitalWrite(whisky, HIGH);
   posMenu = 0 ;
   mode = 0 ; 
   ligne2 = Menu[posMenu];
   //Simpson();
}

//FONCTION INITIALISATION DES POMPES
void chargement() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("INITIALISATION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche 
  digitalWrite(22,LOW);
  digitalWrite(23,LOW);
  digitalWrite(24,LOW);
  digitalWrite(25,LOW);
  digitalWrite(26,LOW);
  digitalWrite(27,LOW);
  digitalWrite(28,LOW);
  digitalWrite(29,LOW);
  digitalWrite(30,LOW);
  digitalWrite(31,LOW);
  digitalWrite(32,LOW);
  digitalWrite(33,LOW);
  digitalWrite(34,LOW);
  digitalWrite(35,LOW);
  digitalWrite(36,LOW);
  digitalWrite(37,LOW);
  digitalWrite(39,LOW);
  digitalWrite(41,LOW);
  digitalWrite(43,LOW);
  digitalWrite(45,LOW);
  digitalWrite(47,LOW);
  digitalWrite(49,LOW);
  digitalWrite(51,LOW);
  digitalWrite(53,LOW);
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // Chargement des tuyaux, on patiente
      // On allume les lumières de chargement en attendant
      colorLED(strip.Color(0, 250, 0), 50); // Simulation de Chargement
     } while ((millis() - temps) < 7500); // Tant qu'on n'a pas rempli les tuyaux en 7,5 sec, on laisse allumé.

    for(int i = 22; i <= 53; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, HIGH);
  }
    Simpson();
}

//FONCTION PURGE DES POMPES
void purge() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("PURGE"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche
  digitalWrite(22,LOW);
  digitalWrite(23,LOW);
  digitalWrite(24,LOW);
  digitalWrite(25,LOW);
  digitalWrite(26,LOW);
  digitalWrite(27,LOW);
  digitalWrite(28,LOW);
  digitalWrite(29,LOW);
  digitalWrite(30,LOW);
  digitalWrite(31,LOW);
  digitalWrite(32,LOW);
  digitalWrite(33,LOW);
  digitalWrite(34,LOW);
  digitalWrite(35,LOW);
  digitalWrite(36,LOW);
  digitalWrite(37,LOW);
  digitalWrite(39,LOW);
  digitalWrite(41,LOW);
  digitalWrite(43,LOW);
  digitalWrite(45,LOW);
  digitalWrite(47,LOW);
  digitalWrite(49,LOW);
  digitalWrite(51,LOW);
  digitalWrite(53,LOW);
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // DeChargement des tuyaux, on patiente
      // On allume les lumières de DeChargement en attendant
      colorLED(strip.Color(250, 0, 0), 50); // Simulation de Purge
     } while ((millis() - temps) < 10000); // Tant qu'on n'a pas rempli les tuyaux d'eau en 10 sec, on laisse allumé.

    for(int i = 22; i <= 53; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, LOW);
  }
    Simpson();
}

//FONCTION MUSIQUE ZELDA
//NOTE_G5, NOTE_FS5, NOTE_DS5, NOTE_G4, NOTE_FS4, NOTE_E5, NOTE_GS5, NOTE_C6
void Zelda() {
    tone(SPEAKER, NOTE_G5, 125);
    delay(162);
    tone(SPEAKER, NOTE_FS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_DS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_G4, 125);
    delay(162);
    tone(SPEAKER, NOTE_FS4, 125);
    delay(162);
    tone(SPEAKER, NOTE_E5, 125);
    delay(162);
    tone(SPEAKER, NOTE_GS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_C6, 500);
    delay(650);
  }

//FONCTION SECURITE POMPES
// On coupe toutes les pomptes
void coupurepompes() {
    digitalWrite(22,HIGH);
    digitalWrite(23,HIGH);
    digitalWrite(24,HIGH);
    digitalWrite(25,HIGH);
    digitalWrite(26,HIGH);
    digitalWrite(27,HIGH);
    digitalWrite(28,HIGH);
    digitalWrite(29,HIGH);
    digitalWrite(30,HIGH);
    digitalWrite(31,HIGH);
    digitalWrite(32,HIGH);
    digitalWrite(33,HIGH);
    digitalWrite(34,HIGH);
    digitalWrite(35,HIGH);
    digitalWrite(36,HIGH);
    digitalWrite(37,HIGH);
    digitalWrite(39,HIGH);
    digitalWrite(43,HIGH);
    digitalWrite(45,HIGH);
    digitalWrite(47,HIGH);
    digitalWrite(49,HIGH);
  }

//FONCTION MUSIQUE SIMPSON

void Simpson() {
  int bpm = 100;
  int bps = bpm / 60;
  int fullNoteMs = 1000 / bps;
  int halfNoteMs = fullNoteMs / 2;
  int quarterNoteMs = halfNoteMs / 2;
  int eighthNoteMs = quarterNoteMs / 2;
  int note = 0;
  int duration = 0;
  int simpsons[] = {HALF, NOTE_C4, QUARTER, NOTE_E4, NOTE_FS4, EIGHTH, NOTE_A4, HALF, NOTE_G4, QUARTER, NOTE_E4, NOTE_C4, NOTE_G3, EIGHTH, NOTE_FS3, NOTE_FS3, NOTE_FS3, QUARTER, NOTE_G3};
  int numNotes = sizeof(simpsons)/sizeof(int);

    do {
     if (simpsons[note] == HALF) {
     duration = halfNoteMs;
    } else {
        if (simpsons[note] == QUARTER) {
        duration = quarterNoteMs;
        } else {
          if (simpsons[note] == EIGHTH) {
          duration = eighthNoteMs;
          } else {
            if (simpsons[note] == SILENCE) {
              noTone(SPEAKER);
              delay(duration);
            } else {
              tone(SPEAKER, simpsons[note]);
              delay(duration);
              noTone(SPEAKER); 
              }
            }
          }   
        }
    note++;
  } while (note <= numNotes);

}

//FONCTION NEOPIXEL
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorLED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<=(strip.numPixels()-1); i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i-1, 0);
    strip.show();
    delay(wait);
  }
    strip.setPixelColor(strip.numPixels()-1, 0);
    strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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
