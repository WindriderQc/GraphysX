

// ************************** FICHIER: CLInOut.h ******************************
//
//     Fichier contenant la definition de la classe CLInOut.
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************

#if !defined __CLINOUT_H
  #define __CLINOUT_H


  #define SAMELSB   0
  #define SAMEMSB   1
  #define SUITELSB  2
  #define SUITEMSB  3

// liste des commandes envoyer au PIC

  #define NO_DATA            NULL
  #define LIRE_PIECE         0x01
  #define ECRIRE_PIECE       0x02
  #define LIRE_WANTED_TEMP   0x01
  #define ECRIRE_WANTED_TEMP 0x02



  #if !defined UC
    #define UC unsigned char
    #define SI short int
  #endif

// ****************************** CLInOut **********************************
//
//              Definition de la classe CLInOut
//
//   Description:  La classe contient des fonctions In et Out qui peuvent
//                 travailler a 8 ou 16 bits. Pour les In et Out a 16 bits
//                 il faut fournir un parametre qui donne le type d'echange.
//
//                 SAMELSB:  2 acces 8 bits a la meme adresse, LSB en premier.
//                 SAMEMSB:  2 acces 8 bits a la meme adresse, MSB en premier.
//                 SUITELSB: 2 acces 8 bits adresses successives, LSB en premier
//                 SUITEMSB: 2 acces 8 bits adresses successives, MSB en premier
//
//                 On retrouve comme fonctions membres:
//       1: Fonction d'ecriture 8  bits d'un port. (vOut)
//       1: Fonction de lecture 8  bits d'un port. (ucIn)
//       1: Fonction d'ecriture 16 bits d'un port. (vOut)
//       1: Fonction de lecture 16 bits d'un port. (siIn)
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
class CLInOut
{
public:
  CLInOut(){}                  // Constructeur par defaut (vide).


private:
  struct Str1                  // Structure d'ordre general.
    {
    UC ucLsb;
    UC ucMsb;
    };

  union OctetMot               // Union accessible en 8 bits .stOctet.ucLsb
    {                          //                            .stOctet.ucMsb
    struct Str1 stOctet;       //                 en 16 bits .siMot
    SI siMot;
    };


  union OctetMot unDeuxOctets; // unDeuxOctets accessible en 16 bits (siMot)
                               //                 en  8 bits (stOctet.ucLsb)
                               //                            (stOctet.ucMsb)


// Fonction en assembleur pour réaliser un ouptortb et inportb.
void vWOutPortb (unsigned short int iAdresse, unsigned char ucDonnee);
unsigned char ucWInPortb (unsigned short int iAdresse);

protected:


  UC ucIn(SI siAdresse);                                  // In a 8 bits.

  SI siIn(SI siAdresse, SI siTypeAdresse);                // In a 16 bits.

  void vOut(SI siAdresse, UC ucDonnee);                    // Out a 8 bits.

  void vOut(SI siAdresse, SI siDonnee, SI siTypeAdresse); // Out a 16 bits.

  void vOutPic(SI siAdresse, int iCommand, UC ucDonnee);

  UC ucInPic(void);

};// class CLInOut
#endif
