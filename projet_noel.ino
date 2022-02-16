#include <Wire.h>
#include <DS3231.h>


DS3231 clock;
RTCDateTime dt;


// Volume musique
int VolumeMusiqueDehort = A2;

//ReglageVolumeMusiqueDehort(){
int volume=1023;

//FaireUnRandom
int musique;

//Lumieres_Off () _ Lumieres_On () _ Clignoter1 () _ Clignoter2 ()
int lumiereP = 4;
int lumiere1 = 3;
int lumiere2 = 2;


//Temps()
int annee;
int mois;
int jour;
int heure;
int minute;
int seconde;

//Reglage_Lum()
int Valeur_reglage_lumiere;

//Lumiere_dehors ()
int reading_lum ;
int photocell = 0;
int compteur=0;
int temps_init;
int temps_act;
int resultat;
int tempsD=10;

//Reglage_Tempo()
int Pot_tempo = 1;
int Valeur_pot_tempo;
int Temps_pot_tempo;

//Tempo()
boolean tempo_nb_boucle = true;
int tempo = 1000;
unsigned long seconde_initial = millis();
unsigned long seconde_actuel = millis();


//Automatisation_Alumage_Lumieres ()
boolean qui_clignote = true;
unsigned long x=0;


void Lumieres_Off () {
  digitalWrite(lumiereP, LOW);
  digitalWrite(lumiere1, LOW);
  digitalWrite(lumiere2, LOW);
  //myDFPlayer.sleep();
}

void Lumieres_On () {
  digitalWrite(lumiereP, HIGH);
  //myDFPlayer.randomAll();
}

void Clignoter1 () {
    digitalWrite(lumiere1, HIGH);
    digitalWrite(lumiere2, LOW);
}

void Clignoter2 () {
    digitalWrite(lumiere1, LOW);
    digitalWrite(lumiere2, HIGH);
}

void Temps(){
  dt = clock.getDateTime();
  annee = dt.year;
  mois = dt.month;
  jour = dt.day;
  heure = dt.hour;
  minute = dt.minute;
  seconde = dt.second;
}



int Reglage_Lum() {
  Valeur_reglage_lumiere = 10;
  return (Valeur_reglage_lumiere);
}

boolean Lumiere_dehors () {
  
  if (compteur==0){
    temps_init = minute;
  }
  
  reading_lum = analogRead(photocell);
  //Serial.println(reading_lum);
  if (reading_lum <= Reglage_Lum()){
    compteur=1;
  }
  else{
    compteur=0;
  }
  
  temps_act = minute;
  resultat =(temps_act-temps_init)%60;
  
  if (resultat<0){
    resultat+=60;
  }
  if (resultat >= 58){
    temps_init=(minute-15)%60;
  }
  if (resultat > tempsD){
    return true;
  }
  else{
    return false;
  }
}

int Reglage_Tempo() {
  Valeur_pot_tempo  = analogRead(Pot_tempo);
  Temps_pot_tempo = map(Valeur_pot_tempo, 0, 1023, 125, 4000);
  return (Temps_pot_tempo);
}

boolean Tempo () {
  if (tempo_nb_boucle){
    seconde_initial = millis();
  }
  else {
    seconde_actuel = millis();
    tempo = Reglage_Tempo();
    if ((seconde_actuel-seconde_initial) >= tempo) {
      tempo_nb_boucle = true;
      return (true);
    }
  }
  tempo_nb_boucle = false;
  return (false);
}


boolean Test_Temps() {
  return (heure >= 5 || heure == 0 && minute <= 45);
}


void Automatisation_Alumage_Lumieres (){
   Temps();
   if (Lumiere_dehors() && Test_Temps()) {
    x=0;
     Lumieres_On ();
     if (qui_clignote)
      if (!Tempo())
       Clignoter1();
      else 
        qui_clignote = false;
     else
      if (!Tempo())
        Clignoter2();
      else 
        qui_clignote = true;
     }
    else{
      x++;
      Lumieres_Off();
    }
}



void setup() {
  Serial.begin(9600);
  pinMode(lumiereP, OUTPUT);
  pinMode(lumiere1, OUTPUT);
  pinMode(lumiere2, OUTPUT);
  pinMode(photocell, INPUT);
  pinMode(Pot_tempo, INPUT);
  pinMode(A2, INPUT);
  clock.begin();
  /*
  // Manual (YYYY, MM, DD, HH, II, SS
  //clock.setDateTime(2019, 11, 2, 16, 30, 15);
  
  // Send sketch compiling time to Arduino
  //clock.setDateTime(__DATE__, __TIME__);
  */
  }

void loop() {
  Automatisation_Alumage_Lumieres();
}
