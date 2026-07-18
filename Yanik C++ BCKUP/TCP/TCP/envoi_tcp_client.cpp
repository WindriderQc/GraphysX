// ********************************************
// Nom du code : envoi_tcp.cpp
// Auteur : _SebF AT frameIP.com
// date de création : 10 juin 2004
// version : 1.0
// Licence : Ce code est libre de toute utilisation.
// La seule condition existante est de faire référence au site http://www.frameip.com afin de respecter le travail d'autrui.
// ********************************************

// ********************************************************
// Les includes
// ********************************************************
#include <winsock2.h> // pour les fonctions socket
#include <cstdio> // Pour les Sprintf

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
char buffer[65535]; // Tampon contennant les données reçues ou envoyées
SOCKET id_de_la_socket; // Identifiant de la socket
SOCKADDR_IN information_sur_la_destination; // Déclaration de la structure des informations lié au serveur

int main (int argc, char* argv[])
	{
	printf("\nBonjour, vous etes du cote client. www.frameip.com\n");

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
	// Etablissement de l'ouverture de session
	// ********************************************************
	information_sur_la_destination.sin_family=AF_INET;
	information_sur_la_destination.sin_addr.s_addr=inet_addr("10.10.10.10"); // Indiquez l'adresse IP de votre serveur  
	information_sur_la_destination.sin_port=htons(33333); // Port écouté du serveur (33333)
	erreur=connect(id_de_la_socket,(struct sockaddr*)&information_sur_la_destination,sizeof(information_sur_la_destination));
	if (erreur!=0)
		printf("\nDesole, je n'ai pas pu ouvrir la session TCP : %d %d",erreur,WSAGetLastError());
	else
		printf("\nsetsockopt  : OK");

	// ********************************************************
	// Envoi des données
	// ********************************************************
	strcpy(buffer,"Coucou, je suis les donnees. www.frameip.com"); // Copie la chaine de caractère dans buffer
	nombre_de_caractere=send(id_de_la_socket,buffer,strlen(buffer),0);
	if (nombre_de_caractere==SOCKET_ERROR)
		printf("\nDesole, je n'ai pas envoyer les donnees du a l'erreur : %d",WSAGetLastError());
	else
		printf("\nsend        : OK");

	// ********************************************************
	// Fermeture de la session TCP Correspondant à la commande connect()
	// ********************************************************
	erreur=shutdown(id_de_la_socket,2); // 2 signifie socket d'émission et d'écoute
	if (erreur!=0)
		printf("\nDesole, je ne peux pas fermer la session TCP du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nshutdown    : OK");

	// ********************************************************
	// Fermeture de la socket correspondant à la commande socket()
	// ********************************************************
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