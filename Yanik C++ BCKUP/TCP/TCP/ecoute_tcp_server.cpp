#include "StdAfx.h"
// ********************************************
// Nom du code : ecoute_tcp.cpp
// Auteur : _SebF AT frameIP.com
// date de création : 10 juin 2004
// version : 1.0
// Licence : Ce code est libre de toute utilisation.
// La seule condition existante est de faire référence au site http://www.frameip.com afin de respecter le travail d'autrui.
// ********************************************

// ********************************************************
// Les includes
// ********************************************************

#include <WinSock2.h> // pour les fonctions socket
//#include <cstdio> // Pour les Sprintf


// ********************************************************
// Les librairies
// ********************************************************
#pragma comment(lib,"ws2_32.lib")

// ********************************************************
// Définition des variables
// ********************************************************
WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
int erreur; // Variable permettant de récupérer la valeur de retour des fonctions utilisées
int tempo; // Variable temporaire de type int
int nombre_de_caractere; // Indique le nombre de caractères qui a été reçu ou envoyé
char buffer[65535]; // Tampon contenant les données reçues ou envoyées
SOCKET id_de_la_socket; // Identifiant de la socket
SOCKET id_de_la_nouvelle_socket; // Identifiant de la nouvelle socket
SOCKADDR_IN information_sur_la_source; // Déclaration de la structure des informations lié à l'écoute

int main (int argc, char* argv[])
	{
	printf("\nBonjour, vous etes du cote serveur. www.frameip.com\n");

	// ********************************************************
	// Initialisation de Winsock
	// ********************************************************
	erreur=WSAStartup(MAKEWORD(2,2),&initialisation_win32);
	if (erreur!=0)
		printf("\nDesole, je ne peux pas initialiser Winsock du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nWSAStartup  : OK");

	// ********************************************************
	// Ouverture d'une Socket
	// ********************************************************
	id_de_la_socket=socket(AF_INET,SOCK_STREAM,0);
	if (id_de_la_socket==INVALID_SOCKET)
		printf("\nDesole, je ne peux pas creer la socket du a l'erreur : %d",WSAGetLastError());
	else
		printf("\nsocket      : OK");

	// ********************************************************
	// Activation de l'option permettant d'activer l'algorithme de Nagle
	// ********************************************************
	tempo=1;
	erreur=setsockopt(id_de_la_socket,IPPROTO_TCP,TCP_NODELAY,(char *)&tempo,sizeof(tempo));
	if (erreur!=0)
		printf("\nDesole, je ne peux pas configurer cette options du à l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nsetsockopt  : OK");

	// ********************************************************
	// Lie la socket à une ip et un port d'écoute
	// ********************************************************
	information_sur_la_source.sin_family=AF_INET;
	information_sur_la_source.sin_addr.s_addr=INADDR_ANY; // Ecoute sur toutes les IP locales  
	information_sur_la_source.sin_port=htons(33333); // Ecoute sur le port 33333
	erreur=bind(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,sizeof(information_sur_la_source));
	if (erreur!=0)
		printf("\nDesole, je ne peux pas ecouter ce port : %d %d",erreur,WSAGetLastError());
	else
		printf("\nbind        : OK");

	// ********************************************************
	// Attente d'ouverture de session
	// ********************************************************
	erreur=99; // Initiation de erreur pour être sur que l'on va rentrer dans la boucle
	while(erreur!=0) // Boucle tant qu'une demande de session (SYN) tcp n'a pas été reçu
		erreur=listen(id_de_la_socket,1);
	printf("\nlisten      : OK");

	// ********************************************************
	// Acceptation de la demande d'ouverture de session
	// ********************************************************
	printf("\nAttente de la reception de demande d'ouverture de session tcp (SYN)");
	tempo=sizeof(information_sur_la_source); // Passe par une variable afin d'utiliser un pointeur
	id_de_la_nouvelle_socket=accept(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,&tempo);
	if(id_de_la_nouvelle_socket==INVALID_SOCKET)
		printf("\nDesole, je ne peux pas accepter la session TCP du a l'erreur : %d",WSAGetLastError());
	else
		printf("\naccept      : OK");

	// ********************************************************
	// Reception des données
	// ********************************************************
	nombre_de_caractere=recv(id_de_la_nouvelle_socket,buffer,1515,0);
	if (nombre_de_caractere==SOCKET_ERROR)
		printf("\nDesole, je n'ai pas recu de donnee");
	else
		{
		buffer[nombre_de_caractere]=0; // Permet de fermer le tableau après le contenu des data, car la fonction recv ne le fait pas
		printf("\nVoici les donnees : %s",buffer);
		}

	// ********************************************************
	// Fermeture de la session TCP Correspondant à la commande connect()
	// ********************************************************
	erreur=shutdown(id_de_la_nouvelle_socket,2); // 2 signifie socket d'émission et d'écoute
	if (erreur!=0)
		printf("\nDesole, je ne peux pas fermer la session TCP du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nshutdown    : OK");

	// ********************************************************
	// Fermeture des deux socket correspondant à la commande socket() et accept()
	// ********************************************************
	erreur=closesocket(id_de_la_nouvelle_socket);
	if (erreur!=0)
		printf("\nDesole, je ne peux pas liberer la socket du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nclosesocket : OK");

	erreur=closesocket(id_de_la_socket);
	if (erreur!=0)
		printf("\nDesole, je ne peux pas liberer la socket du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nclosesocket : OK");

	// ********************************************************
	// Quitte proprement le winsock ouvert avec la commande WSAStartup
	// ********************************************************
	erreur=WSACleanup(); // A appeler autant de fois qu'il a été ouvert.
	if (erreur!=0)
		printf("\nDesole, je ne peux pas liberer winsock du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nWSACleanup  : OK");
	}