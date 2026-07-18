#include "StdAfx.h"
#include "TCPComm.h"

CLTCPComm::CLTCPComm(bool bMode)
{
	CommMode = bMode;

	if(CommMode == CLIENT)
		printf("\nBonjour, vous etes du cote client. www.frameip.com\n");
	else
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

}

CLTCPComm::~CLTCPComm(void)
{
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


void CLTCPComm::Run(void)
{
if(CommMode == SERVER)
	{
		SetListen(33333);  // Server comand
		Wait4SessionConnection();
		AcceptSessionConnection();
		ReceiveData();
	}
if(CommMode == CLIENT)
	{
		Connect("192.168.1.112", 33333);
		Send("Alouette");  // //convertir LA  STRUCTURE STEngineControl en une CHAINE DE CHAR pour l'envoi
	}

Disconnect();

if(CommMode == SERVER)
	{
		CloseAcceptSocket(); // Server Command
	}

// Destroy Class
}


// Server Command
void CLTCPComm::SetListen(unsigned short PortNumber)
{
	// ********************************************************
	// Lie la socket à une ip et un port d'écoute
	// ********************************************************
	information_sur_la_source.sin_family=AF_INET;
	information_sur_la_source.sin_addr.s_addr=INADDR_ANY; // Ecoute sur toutes les IP locales  
	information_sur_la_source.sin_port=htons(PortNumber); // Ecoute sur le port XXXXX
	erreur=bind(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,sizeof(information_sur_la_source));
	if (erreur!=0)
		printf("\nDesole, je ne peux pas ecouter ce port : %d %d",erreur,WSAGetLastError());
	else
		printf("\nbind        : OK");

}

void CLTCPComm::Connect(char* IPAdress, unsigned short PortNumber)
{
	// ********************************************************
	// Etablissement de l'ouverture de session
	// ********************************************************
	information_sur_la_destination.sin_family=AF_INET;
	information_sur_la_destination.sin_addr.s_addr=inet_addr(IPAdress); // Indiquez l'adresse IP de votre serveur  
	information_sur_la_destination.sin_port=htons(PortNumber); // Port écouté du serveur (XXXXX)
	erreur=connect(id_de_la_socket,(struct sockaddr*)&information_sur_la_destination,sizeof(information_sur_la_destination));
	if (erreur!=0)
		printf("\nDesole, je n'ai pas pu ouvrir la session TCP : %d %d",erreur,WSAGetLastError());
	else
		printf("\nsetsockopt  : OK");


}

void CLTCPComm::Send(char* DataString)
{
	// ********************************************************
	// Envoi des données
	// ********************************************************
	strcpy(buffer,DataString); // Copie la chaine de caractère dans buffer
	nombre_de_caractere=send(id_de_la_socket,buffer,strlen(buffer),0);
	if (nombre_de_caractere==SOCKET_ERROR)
		printf("\nDesole, je n'ai pas envoyer les donnees du a l'erreur : %d",WSAGetLastError());
	else
		printf("\nsend        : OK");



}
void CLTCPComm::SendMatrix(cTV_3DMATRIX Matrix)
{
	// ********************************************************
	// Envoi des données
	// ********************************************************
	//strcpy(buffer,DataString); // Copie la chaine de caractère dans buffer
	nombre_de_caractere=send(id_de_la_socket,(char*)&Matrix,sizeof(cTV_3DMATRIX),0);
	if (nombre_de_caractere==SOCKET_ERROR)
		printf("\nDesole, je n'ai pas envoyer les donnees du a l'erreur : %d",WSAGetLastError());
	else
		printf("\nsend        : OK");



}

void CLTCPComm::Disconnect(void)
{
	// ********************************************************
	// Fermeture de la session TCP Correspondant à la commande connect()
	// ********************************************************
	erreur=shutdown(id_de_la_socket,2); // 2 signifie socket d'émission et d'écoute
	if (erreur!=0)
		printf("\nDesole, je ne peux pas fermer la session TCP du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nshutdown    : OK");
}

void CLTCPComm::CloseAcceptSocket(void)
{
	// ********************************************************
	// Fermeture de la socket correspondant à la commande accept()
	// ********************************************************
	
	erreur=closesocket(id_de_la_nouvelle_socket);
	if (erreur!=0)
		printf("\nDesole, je ne peux pas liberer la socket du a l'erreur : %d %d",erreur,WSAGetLastError());
	else
		printf("\nclosesocket : OK");

	
}



void CLTCPComm::Wait4SessionConnection(void)
{

	// ********************************************************
	// Attente d'ouverture de session
	// ********************************************************
	erreur=99; // Initiation de erreur pour être sur que l'on va rentrer dans la boucle
	while(erreur!=0) // Boucle tant qu'une demande de session (SYN) tcp n'a pas été reçu
		erreur=listen(id_de_la_socket,1);
	printf("\nlisten      : OK");
}

void CLTCPComm::AcceptSessionConnection(void)
{
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
}
void CLTCPComm::ReceiveData(void)
{
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



	//  REMETTRE LA CHAINE DE CHAR RECU DANS un buffer de  STRUCTURE STEngineControl
}