/****
 ______________________________
 | |
 |AUTEUR : IonAce (jean84) |
 |E-MAIL : ionace@latriyade.com |
 |______________________________|

 Cette classe permet d'initialiser une connexion en TCP ou en UDP, avec choix du
 mode serveur ou du mode client. Elle permet egalement d'envoyer des donnees sur
 le reseau et d'en recevoir quelque que soit le protocole utilise.

 Rermeciement à X. & Cosmobob de cppfrance.com pour leurs conseils.

 ****/

 // Include standard pour utiliser winsock
 #include <winsock2.h>
 // On lie la librairie de winsock
 #pragma comment(lib, "ws2_32.lib")

 // Creations de nouvelles donnees
 typedef unsigned int U_INT; // entier non-signe
 typedef enum MODE_CONNEXION {TCP, UDP}; // pour facilite le choix entre tcp et udp lors de la creation de l'objet
 typedef enum ERREURS {DISCONNECT, SENDFAILED, GETFAILED, OK, NOTDEFINE}; // pour faciliter la comprehension des erreurs reseaux
 typedef enum TYPE_SERVICE {SERVER, CLIENT}; // pour faciliter le choix entre serveur et client lors de la creation de l'objet
 typedef enum TYPE_CONNEXION {TCPCONNECT, UDPCONNECT};
 /* TYPE_CONNEXION => Permet a la classe de savoir quelle protocole utilise lors de l'envoi/reception de message. */

 class Reseau
 {
 protected :
 // fonctions d'initialisation TCP
 SOCKET initTcpServeur(U_INT port);
 SOCKET initTcpClient(char *pip, U_INT port);
 // fonction d'initialisation UDP
 SOCKET initUdp(TYPE_SERVICE *pSerCli, char *ip, U_INT port);
 // variable recevant le handle de la connexion en cours
 SOCKET s;
 // structure SOCKADDR_IN
 SOCKADDR_IN sin;
 // variable indiquant l'etat des connexions
 bool initSocket;
 // variable indiquant le type de connexion
 TYPE_CONNEXION tpc;

 public :
 /* Si serCli vaut true, mode serveur active. Si serCli vaut false, mode client active.
 Si modeConnexion = true, connexion en TCP sinon connexion en UDP. */
 Reseau(MODE_CONNEXION mc, TYPE_SERVICE serCli, char *ip, U_INT port ); // constructeur
 Reseau(Reseau& r); // constructeur de copie
 ~Reseau(); // destructeur

 // Petie fonction permettant de verifier l'etat des connexions. */
 bool etatConnexion();

 // Fonctions envoyants et recevant des donnees sur le reseau
 ERREURS envoiBuf(char *pSBuff);
 ERREURS recevBuf(char *pRBuff, int sizeBuff);
 };

 /* Constructeur */
 Reseau::Reseau(MODE_CONNEXION mc, TYPE_SERVICE serCli, char *ip, U_INT port )
 {
 initSocket = false; // si aucune verif n'est bonne, initSocket reste aisni sur false
 if ( mc == TCP ) // connexion TCP
 {
 if ( serCli == SERVER ) // si l'utilisateur a choisie d'utiliser la fonction serveur
 {
 s = initTcpServeur(port);
 if ( s != INVALID_SOCKET ){ // si la connexion est valide
 initSocket = true;
 tpc = TCPCONNECT; // on indique que le type de connexion est TCP
 }
 }
 else if ( serCli == CLIENT ) // si l'utilisateur a choisie d'utiliser la fonction client
 {
 s = initTcpClient(ip, port); // recuperation du handle de la connexion
 if ( s != INVALID_SOCKET ){ // si la connexion est valide
 initSocket = true;
 tpc = TCPCONNECT;
 }
 }
 }
 else if ( mc == UDP ) // connexion UDP
 {
 s = initUdp(&serCli, ip, port); // initialisation du protocole UDP et recuperation du HANDLE a travers s
 if ( s != INVALID_SOCKET ) // si s est valide (initialisation reussi)
 {
 initSocket = true;
 tpc = UDPCONNECT; // on indique que le type de connexion est UDP
 }
 }
 }

 /* Constructeur de copie */
 Reseau::Reseau(Reseau& r)
 {
 SOCKET nS = r.s; // copie de s
 SOCKADDR_IN nSin = r.sin; // copie se sin
 bool nInitSocket = r.initSocket; // copie de initSocket
 TYPE_CONNEXION nTpc = r.tpc; // copie de tpc (pas super utile mais au moins
 // tout le ponde dispose de son espace memoire
 // et y a pas d'ambiguite)
 }

 /* Destructeur */
 Reseau::~Reseau()
 {
 if ( initSocket )
 {
 closesocket(s);
 WSACleanup();
 initSocket = false;
 }
 }

 /* Fonctions membres privees */
 SOCKET Reseau::initTcpServeur(U_INT port)
 {
 // Initialisation des composants winsock
 WSAData wsa;
 WSAStartup(MAKEWORD(2,0), &wsa);

 // Creation d'une structure SOCKADDR_IN indispensable pour utiliser winsock
 SOCKADDR_IN cin;

 /* Declaration des variables de type socket utilise par le serveur. Ces variables
 representent le HANDLE de la connexion en cours.*/
 SOCKET s1;
 SOCKET s2;

 // Remplissage de la structure SOCKADDR_IN
 sin.sin_addr.s_addr = INADDR_ANY; // accepte n'importe quelle IP
 sin.sin_family = AF_INET;
 sin.sin_port = htons(port);

 // Definition du socket
 s1 = socket(AF_INET,SOCK_STREAM,0);

 // Remplissage du socket
 bind(s1,(SOCKADDR*)&sin,sizeof(sin));

 // Mise en ecoute du serveur
 listen(s1,0);

 int sinsize;
 int err=0;

 sinsize=sizeof(cin);

 while (1) // boucle infinie en attente d'une connexion sur s1
 {
 /* Si une connexion est realise sur le SOCKET s1, alors s2 prend le relai pour
 toute la suite de la communication et s1 se remet en ecoute.*/
 s2 = accept(s1, (SOCKADDR*)&cin, &sinsize);
 if ( s2 == INVALID_SOCKET )
 return INVALID_SOCKET; // Erreur renvoye
 return s2;

 }
 }

 SOCKET Reseau::initTcpClient(char *pip, U_INT port)
 {
 /* Pour toutes les declarations, voir iniTcpServeur(). C'est quasiment identique
 sauf que l'on n'utilise qu'une srtucture SOCKADDR_IN et qu'une variable de type SOCKET.*/
 WSAData wsa;
 WSAStartup(MAKEWORD(2,0), &wsa);

 SOCKET s1;

 // inet_addr() convertie la chaine de caractere en adresse IP valide.
 sin.sin_addr.s_addr = inet_addr(pip);
 sin.sin_family = AF_INET;
 sin.sin_port = htons(port);

 s1=socket(AF_INET,SOCK_STREAM,0);

 bind(s1,(SOCKADDR*)&sin,sizeof(sin));

 /* La difference avec le serveur et que le client n'attend pas de connexion.
 Il essaye directement de se connecter et s'il echoue, il renvoie INVALID_SOCKET,
 au lieu de la connexion en cours, a travers s1. */
 int result = connect(s1, (SOCKADDR *)&sin, sizeof(sin));
 if ( result )
 return INVALID_SOCKET;
 return s1;
 }

 SOCKET Reseau::initUdp(TYPE_SERVICE *pSerCli, char *ip, U_INT port)
 {
 WSAData wsa;
 WSAStartup(MAKEWORD(2,0), &wsa);

 SOCKET s1;

 sin.sin_family = AF_INET;
 // si mode serveur choisi
 if ( *pSerCli == SERVER ){
 sin.sin_addr.s_addr = INADDR_ANY; // instruction pour le serveur
 }
 // si mode client choisi
 else if ( *pSerCli == CLIENT ){
 sin.sin_addr.s_addr = inet_addr(ip); // instruction pour le client
 }
 sin.sin_port=htons(port);

 s1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
 int result = bind(s1,(SOCKADDR*)&sin,sizeof(sin));
 if ( result )
 return INVALID_SOCKET; // en cas d'erreur
 /* Contrairement au TCP, l'UDP ne se connecte jamais. C'est pourquoi il n'y a
 aucune methode pour se connecter dans cette fonction. Une fois cet appel effectue,
 l'utilisateur n'a plus qu' a se soucier des fonctions envoiBuf() et recvBuf()
 pour communiquer avec un autre pc. */
 return s1;
 }

 /* Fonctions membres publiques */
 bool Reseau::etatConnexion()
 {
 /* Si la connexion est etablie, initSocket vaut true. En cas d'erreur, elle
 vaudra false. */
 return initSocket;
 }

 ERREURS Reseau::envoiBuf(char *pSBuff)
 {
 if ( initSocket ) // on verifie si la connexion a ete initialise
 {
 int accuseEnvoi;
 if ( tpc == TCPCONNECT ) // si protocole TCP utilise
 {
 accuseEnvoi = send(s, pSBuff, strlen(pSBuff), 0);
 if ( accuseEnvoi == SOCKET_ERROR ) // erreur socket
 return SENDFAILED;
 else if ( accuseEnvoi == 0 ) // connexion interrompue
 return DISCONNECT;
 else // tout est ok
 return OK;
 }
 else if ( tpc == UDPCONNECT ) // si protocole UDP utilise
 {
 accuseEnvoi = sendto(s, pSBuff, strlen(pSBuff), 0, (SOCKADDR *)&sin, sizeof(sin));
 if ( accuseEnvoi == SOCKET_ERROR ) // erreur socket
 return SENDFAILED;
 else // tout est ok
 return OK;
 }
 }
 return NOTDEFINE; // si la connexion n'a pas ete initlialise
 }

 ERREURS Reseau::recevBuf(char *pRBuff, int sizeBuff)
 {
 if ( initSocket ) // on verifie si la connexion a ete initialise
 {
 int accuseReception;
 if ( tpc == TCPCONNECT ) // si protocole TCP utilise
 {
 accuseReception = recv(s, pRBuff, (sizeBuff-1), 0);

 if ( accuseReception == SOCKET_ERROR ){ // erreur lors de la reception
 return GETFAILED;
 } else if ( accuseReception == 0 ){ // connexion interrompue
 return DISCONNECT;
 } else {
 pRBuff[accuseReception] = '\0';
 return OK; // en cas de reussite, on renvoi OK
 }
 }
 else if ( tpc == UDPCONNECT ) // si protocole UDP utilise
 {
 int sinSize = sizeof(sin);
 accuseReception = recvfrom(s, pRBuff, (sizeBuff-1), 0, (SOCKADDR *)&sin, &sinSize);

 if ( accuseReception == SOCKET_ERROR ){
 return GETFAILED;
 } else if ( accuseReception == 0 ){
 return DISCONNECT;
 } else {
 pRBuff[accuseReception] = '\0';
 return OK;
 }
 }
 }
return NOTDEFINE; // si la connexion n'a pas ete initlialise
} 













//  Exemple d'utilisation


#include <stdio.h>
#include <windows.h>

int main (void)
{

  U_INT port = 40000;
  ERREURS er;  // gestion avance des erreurs
  bool result;
  Reseau *r = NULL;
  r = new Reseau(TCP, CLIENT, adresse, port);  // regarde le source pour ça
  result = etatConnexion();
  
  if ( !result ){
    printf("Erreur.\n");
    system("pause");
    return 0;
  }

  er = r->envoiBuf("coucou\0");

  if ( er ==  NOTDEFINE ){
    printf("Socket non-initialise.\n");
  } else if ( er == DISCONNECT ){
    printf("Connexion interrompue.\n");
  } else if ( er == SENDFAILED ){
    printf("Erreur pendant l'envoi.\n");
  } else if ( er == OK ){
    printf("Packet envoye.\n");
  } else {
    printf("Erreur interne.\n");
  }

  if ( r ){
    delete r;
    r = NULL;
  }

  return 0;
}