//ce code ecrit tout ce qu'il ecoute sur le port serie. en envoyant get_data sur le port serie, il lit tout le contenut de la carte sd

#include <SPI.h>
#include <SD.h>

const int pinRX = 16;      // RX connecté à TXD232 du MAX232CSE
const int pinTX = 17;      // TX connecté à RXD232 du MAX232CSE
const int chipSelect = 5;  // Broche CS pour la carte SD
const int ledPin = 0;      // Broche pour la LED

File myFile; // Déclaration globale de la variable File

void setup() {
  pinMode(ledPin, OUTPUT); // Configure la broche de la LED en sortie
  digitalWrite(ledPin, LOW); // S'assurer que la LED est éteinte au démarrage
  SPI.begin(18, 19, 23, 5); // Configuration des broches pour SPI (SCK, MISO, MOSI, SS)

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, pinRX, pinTX); // Initialiser Serial2 avec des broches personnalisées pour RX et TX

  while (!Serial) {
    ; // Attendre la connexion du port série, nécessaire pour le port USB natif
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card Mount Failed");
    while (1); // Arrêter tout si l'initialisation échoue
  }
  Serial.println("Card Mount Successful");
  Serial.println("Ready to receive data...");
}

void loop() {
  if (Serial2.available()) { // Vérifie s'il y a des données disponibles à lire sur Serial2
    String data = Serial2.readStringUntil('\n'); // Lire les données jusqu'à ce qu'un caractère de nouvelle ligne soit trouvé
    Serial.print("Received on Serial2: ");
    Serial.println(data);

    digitalWrite(ledPin, HIGH); // Allumer la LED pour indiquer la réception des données

    if (data == "get_data") {
      // Si la commande reçue est "get_data", lire et afficher tout le contenu du fichier
      myFile = SD.open("/log.txt");
      if (myFile) {
        Serial.println("Contents of log.txt:");
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        myFile.close();
      } else {
        Serial.println("Error opening log.txt for reading");
      }
    } else {
      // Sinon, écrire les données reçues dans le fichier log.txt
      myFile = SD.open("/log.txt", FILE_APPEND);
      if (myFile) {
        myFile.println(data); // Écrire les données reçues dans le fichier
        myFile.close(); // Fermer le fichier après écriture
        Serial.println("Data logged to SD card.");
      } else {
        Serial.println("Error opening log.txt");
      }
    }

    digitalWrite(ledPin, LOW); // Éteindre la LED après l'opération
  }
}
