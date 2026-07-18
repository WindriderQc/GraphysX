/**** 
 ______________________________
| 							   |
|AUTEUR IonAce (jean84)        |
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

#define TCP_CONNEXION 1
#define UDP_CONNEXION 0

// Creation de'un nouveau type de donnees
typedef unsigned int U_INT;  // entier non-signe

class Reseau
{
	protected :	
	  // fonctions d'initialisation TCP
      SOCKET initTcpServeur(U_INT port);  
      SOCKET initTcpClient(char *pip, U_INT port);
      // fonction d'initialisation UDP
      SOCKET initUdp(bool *pSerCli, char *ip, U_INT port);
      // variable recevant le handle de la connexion en cours
      SOCKET s;
      // structure SOCKADDR_IN
      SOCKADDR_IN sin;
      // variable indiquant l'etat des connexions
      bool initSocket;
      // typeConnexion vaut true si mode TCP utilise ou false si mode UDP utilise
      bool typeConnexion; 
    
    public :
      Reseau();  // constructeur
      ~Reseau();  // destructeur
      // Si serCli vaut true, mode serveur active. Si serCli vaut false, mode client active
      bool initConnexionTcp(bool serCli, char *ip, U_INT port);
      bool initConnexionUdp(bool serCli, char *ip, U_INT port); 
      // Fonctions envoyants et recevant des donnees sur le reseau
      bool envoiBuf(char *pSBuff);
      bool recevBuf(char *pRBuff, int sizeBuff);
};

/* Constructeur */
Reseau::Reseau() {}

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
    sin.sin_addr.s_addr = INADDR_ANY;  // accepte n'importe quelle IP
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
	
	while (1)  // boucle infinie en attente d'une connexion sur s1
	{
		/* Si une connexion est realise sur le SOCKET s1, alors s2 prend le relai pour 
	    toute la suite de la communication et s1 se remet en ecoute.*/
	    s2 = accept(s1, (SOCKADDR*)&cin, &sinsize);
		if ( s2 != INVALID_SOCKET )
	    	return s2;  // HANDLE de la connexion renvoye
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
	connect(s1, (SOCKADDR *)&sin, sizeof(sin)) ;
	return s1;
}

SOCKET Reseau::initUdp(bool *pSerCli, char *ip, U_INT port)
{
    WSAData wsa;
    WSAStartup(MAKEWORD(2,0), &wsa);
    
    SOCKET s1;
    
    sin.sin_family = AF_INET;
    if ( *pSerCli )  // si mode serveur choisi
    {  
	    sin.sin_addr.s_addr = INADDR_ANY; // instruction pour le serveur
    } 
    else if ( !*pSerCli )  // si mode client choisi
    {  
        sin.sin_addr.s_addr = inet_addr(ip);  // instruction pour le client
    }
	sin.sin_port=htons(port);
	
	s1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	bind(s1,(SOCKADDR*)&sin,sizeof(sin));
	
	/* Contrairement au TCP, l'UDP ne se connecte jamais. C'est pourquoi il n'y a 
	aucune methode pour se connecter dans cette fonction. Une fois cet appel effectue,
	l'utilisateur n'a plus qu' a se soucier des fonctions envoiBuf() et recvBuf()
	pour communiquer avec un autre pc. */
	return s1;
}
    
/* Fonctions membres publiques */
bool Reseau::initConnexionTcp(bool serCli, char *ip, U_INT port)
{
	if ( !initSocket )  // si la connexion n'a pas ete deja initialise
	{
        if ( serCli ) // si l'utilisateur a choisie d'utiliser la fonction serveur
        {
        	s = initTcpServeur(port);  // recuperation du handle de la connexion
            if ( s != INVALID_SOCKET )  // si la connexion est valide
            {
            	initSocket = true;
            	typeConnexion = TCP_CONNEXION;  // on indique que le type de connexion est TCP
             	return true;
            }
        }
        else if ( !serCli ) // si l'utilisateur a choisie d'utiliser la fonction client
        {
            s = initTcpClient(ip, port);  // recuperation du handle de la connexion
            if ( s != INVALID_SOCKET )  // si la connexion est valide
            {
            	initSocket = true;
            	typeConnexion = TCP_CONNEXION;  
            	return true;
       	    }
        }
    }
    return false;
}

bool Reseau::initConnexionUdp(bool serCli, char *ip, U_INT port)
{
    if ( !initSocket )  // verifie si aucune connexion n'a ete realise auparavant
    {
        s = initUdp(&serCli, ip, port);  // initialisation du protocole UDP et recuperation du HANDLE a travers s
        if ( s != INVALID_SOCKET )  // si s est vailde (initiailisation reussi)
        {
            initSocket = true;
            typeConnexion = UDP_CONNEXION;  // on indique que le type de connexion est UDP
            return true;
        }
    }
    return false;  // en cas d'erreur
}
                
bool Reseau::envoiBuf(char *pSBuff)
{
    if ( initSocket )  // on verifie si la connexion a ete initialise
    {
        int accuseEnvoi;
        if ( typeConnexion )  // si protocole TCP utilise
        {
  	        accuseEnvoi = send(s, pSBuff, strlen(pSBuff), 0);
    	    if ( (accuseEnvoi != SOCKET_ERROR) && (accuseEnvoi != 0) )
    	        return true;  // en cas de reussite, on renvoie true
        }
        else if ( !typeConnexion )  // si protocole UDP utilise
        {
            accuseEnvoi = sendto(s, pSBuff, strlen(pSBuff), 0, (SOCKADDR *)&sin, sizeof(sin));
            if ( accuseEnvoi != SOCKET_ERROR )
                return true;
        }
   	}
    return false;  // si la conenxion n'a pas ete initlialise ou qu'il y a eu une erreur
}

bool Reseau::recevBuf(char *pRBuff, int sizeBuff)
{
	if ( initSocket )  // on verifie si la connexion a ete initialise
	{
	    int accuseReception;
	    if ( typeConnexion )  // si protocole TCP utilise
	    {
	        accuseReception = recv(s, pRBuff, sizeBuff-1, 0);
	        if ( (accuseReception != SOCKET_ERROR) && (accuseReception != 0) )
	        {
	            pRBuff[accuseReception] = '\0';
	    	    return true;  // en cas de reussite, on renvoie true
    	    }
	    }
	    else if ( !typeConnexion )  // si protocole UDP utilise
        {
            int sinSize = sizeof(sin);
	        accuseReception = recvfrom(s, pRBuff, sizeBuff-1, 0, (SOCKADDR *)&sin, &sinSize);
	        pRBuff[accuseReception] = '\0';
	        return true;
        }
	}
	return false;  // si la connexion n'a pas ete initlialise ou qu'il y a eu une erreur
}
