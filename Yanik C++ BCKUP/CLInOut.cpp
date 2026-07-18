

// ************************** FICHIER: CLInOut.CPP ****************************
//
//     Definition des fonctions de la classe CLInOut.
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************

#include "clInOut.h"
#include "SerialComm.h"

// ************************************************************************
//
//              Definition de la fonction In a 8 bits
//
//     Description: Fonction de lecture 8 bits
//
//     NOM: UC CLInOut::ucIn(SI siAdresse)
//
//     Parametre d'entree:  SI siAdresse: Adresse du port a lire.
//
//     Parametre de sortie: UC : Caractere lu.
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
UC CLInOut::ucIn(SI siAdresse)
{
 return ucWInPortb(siAdresse);
}// UC CLInOut::ucIn(SI siAdresse)


// ************************************************************************
//
//              Definition de la fonction In a 16 bits
//
//    Description: Fonction de lecture 16 bits
//
//     NOM: SI CLInOut::siIn(SI siAdresse, SI siTypeAdresse)
//
//     Parametre d'entree:  SI siAdresse: Adresse du port a lire.
//                          SI siTypeAdresse: Type d'acces 16 bits.
//                SAMELSB:  2 acces 8 bits a la meme adresse, LSB en premier.
//                SAMEMSB:  2 acces 8 bits a la meme adresse, MSB en premier.
//                SUITELSB: 2 acces 8 bits adresses successives, LSB en premier
//                SUITEMSB: 2 acces 8 bits adresses successives, MSB en premier
//
//     Parametre de sortie: SI : Caractere lu.
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
SI CLInOut::siIn(SI siAdresse, SI siTypeAdresse)
{

 switch(siTypeAdresse)
  {
  case SAMELSB:
    unDeuxOctets.stOctet.ucLsb = ucWInPortb(siAdresse);
    unDeuxOctets.stOctet.ucMsb = ucWInPortb(siAdresse);
    break;

  case SAMEMSB:
    unDeuxOctets.stOctet.ucMsb = ucWInPortb(siAdresse);
    unDeuxOctets.stOctet.ucLsb = ucWInPortb(siAdresse);
    break;

  case SUITELSB:
    unDeuxOctets.stOctet.ucLsb = ucWInPortb(siAdresse);
    unDeuxOctets.stOctet.ucMsb = ucWInPortb(siAdresse+1);
    break;

  case SUITEMSB:
    unDeuxOctets.stOctet.ucMsb = ucWInPortb(siAdresse);
    unDeuxOctets.stOctet.ucLsb = ucWInPortb(siAdresse+1);
    break;
  }
return unDeuxOctets.siMot;
}// SI CLInOut::siIn(SI siAdresse, SI siTypeAdresse)

// ************************************************************************
//
//              Definition de la fonction Out a 8 bits
//
//     Description: Fonction d'ecriture 8 bits
//
//     NOM: void CLInOut::vOut(SI siAdresse, UC ucDonnee)
//
//     Parametre d'entree:  SI siAdresse: Adresse du port a lire.
//                          UC ucDonnee:  Donnee a ecrire.
//
//     Parametre de sortie: AUCUN
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
void CLInOut::vOut(SI siAdresse,  UC ucDonnee)
{
vWOutPortb(siAdresse, ucDonnee);
}// void CLInOut::vOut(SI siAdresse, UC ucDonnee)


// ************************************************************************
//
//              Definition de la fonction Out a 16 bits
//
//     Description: Fonction d'ecriture 16 bits
//
//     NOM: void CLInOut::vOut(SI siAdresse, SI siDonnee, SI siTypeAdresse)
//
//     Parametre d'entree:  SI siAdresse: Adresse du port a lire.
//                          UC ucDonnee:  Donnee a ecrire.
//                          SI siTypeAdresse: Types d'acces 16 bits.
//              TypeAdresse:
//                SAMELSB:  2 acces 8 bits a la meme adresse, LSB en premier.
//                SAMEMSB:  2 acces 8 bits a la meme adresse, MSB en premier.
//                SUITELSB: 2 acces 8 bits adresses successives, LSB en premier
//                SUITEMSB: 2 acces 8 bits adresses successives, MSB en premier
//
//     Parametre de sortie: AUCUN
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
void CLInOut::vOut(SI siAdresse, SI siDonnee, SI siTypeAdresse)
{

unDeuxOctets.siMot = siDonnee;
switch(siTypeAdresse)
  {
  case SAMELSB:
    vWOutPortb(siAdresse, unDeuxOctets.stOctet.ucLsb);
    vWOutPortb(siAdresse, unDeuxOctets.stOctet.ucMsb);
    break;

  case SAMEMSB:
    vWOutPortb(siAdresse, unDeuxOctets.stOctet.ucMsb);
    vWOutPortb(siAdresse, unDeuxOctets.stOctet.ucLsb);
    break;

  case SUITELSB:
    vWOutPortb(siAdresse,   unDeuxOctets.stOctet.ucLsb);
    vWOutPortb(siAdresse+1, unDeuxOctets.stOctet.ucMsb);
    break;

  case SUITEMSB:
    vWOutPortb(siAdresse,   unDeuxOctets.stOctet.ucMsb);
    vWOutPortb(siAdresse+1, unDeuxOctets.stOctet.ucLsb);
    break;
   }// switch(siTypeAdresse)
}// void CLInOut::vOut(SI siAdresse, SI siDonnee, SI siTypeAdresse)


// *********************** TITRE : ucWInPortb   *********************
//
//  PROG : UNIT1.CPP	                   	AUTEUR : DANIEL BRETON
//  DATE DE MODIFICATION :  10-11-99			VERSION: 1.0
//
//
//  DESCRIPTION : Fonction de d'ecriture sur un dispositif d'entree/sortie.
//
//  APPEL: void DLL_EXP ucWInPortb  (unsigned short int iAdresse,
//                                 unsigned char ucDonnee)
//
//  PROCEDURES APPELEES : AUCUNE
//
//  PARAMETRE D'ENTREE  : unsigned short int iAdresse: Adresse du dispositif
//                                                     d'E/S ou ecrire.
//
//  PARAMETRE DE SORTIE : unsigned char ucDonnee: Donnee lue du dispositif.
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
unsigned char CLInOut::ucWInPortb (unsigned short int iAdresse)
{
unsigned char ucLecture;

FormSCom->VaComm1->ReadChar(ucLecture);

/*  // fonctionne seulement sous win95,98,NT et pas sous XP
asm
  {
  push dx
  push ax
  mov dx,iAdresse
  in al,dx
  mov ucLecture,al
  pop ax
  pop dx
  }
*/
return(ucLecture);

}//unsigned char CLInOut::ucWInPortb (unsigned short int iAdresse)


// *********************** TITRE : vWOutPortb    *********************
//
//  PROG : UNIT1.CPP	                   	AUTEUR : DANIEL BRETON
//  DATE DE MODIFICATION :  10-11-99			VERSION: 1.0
//
//
//  DESCRIPTION : Fonction de d'ecriture sur un dispositif d'entree/sortie.
//
//  APPEL: void DLL_EXP vWOutPortb (unsigned short int iAdresse,
//                                 unsigned char ucDonnee)
//
//  PROCEDURES APPELEES : AUCUNE
//
//  PARAMETRE D'ENTREE  : unsigned short int iAdresse: Adresse du dispositif
//                                                     d'E/S ou ecrire.
//                        unsigned char ucDonnee: Donnee a écrire au dispositif.
//
//  PARAMETRE DE SORTIE : AUCUN (Nom, description)
//
//     Membres utilisees.
//       Fonctions       : AUCUNE
//       Donnees         : AUCUNE
//
//     Auteurs: Daniel Breton                               Date: 06-06-02
//
//     Modifie: **-**-**
//
// ************************************************************************
void CLInOut::vWOutPortb (unsigned short int iAdresse, unsigned char ucDonnee)
{
FormSCom->VaComm1->WriteChar(ucDonnee);
/*asm
  {
  push dx
  push ax
  mov dx,iAdresse
  mov al,ucDonnee
  out dx,al
  pop ax
  pop dx
  }*/
}//void CLInOut::vWOutPortb (unsigned short int iAdresse, unsigned char ucDonnee)


// ************************************************************************//


void CLInOut::vOutPic(SI siAdresse, int iCommand, UC ucDonnee)
{
FormSCom->VaComm1->WriteChar(siAdresse);
FormSCom->VaComm1->WriteChar(iCommand);
FormSCom->VaComm1->WriteChar(ucDonnee);
}

// ************************************************************************ //


UC CLInOut::ucInPic(void)
{
unsigned char ucLecture;

FormSCom->VaComm1->ReadChar(ucLecture);

return(ucLecture);

}
