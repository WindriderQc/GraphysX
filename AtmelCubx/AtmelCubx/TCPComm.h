#pragma once


// ********************************************************
// Les includes
// ********************************************************
#include <WinSock2.h> // pour les fonctions socket
#include <StdIO.h>

// ********************************************************
// Les librairies
// ********************************************************
#pragma comment(lib,"ws2_32.lib")


#define SERVER 1
#define CLIENT 0


class CLTCPComm
{
public:
	CLTCPComm(bool bMode);
	~CLTCPComm(void);

public:
	bool CommMode;

	void Run(void);
	void SetListen(unsigned short PortNumber);
	void Connect(char* IPAdress, unsigned short PortNumber);
	void Send(char* DataString);
	void Disconnect(void);
	void CloseAcceptSocket(void);
	void Wait4SessionConnection(void);
	void AcceptSessionConnection(void);
	void ReceiveData(void);



private:
	WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
	int erreur;					  // Variable permettant de récupérer la valeur de retour des fonctions utilisées
	int tempo;					  // Variable temporaire de type int
	int nombre_de_caractere;	  // Indique le nombre de caractères qui a été reçu ou envoyé
	char buffer[65535];			  // Tampon contennant les données reçues ou envoyées
	SOCKET id_de_la_socket;		  // Identifiant de la socket
	
	// Variable Client
	SOCKADDR_IN information_sur_la_destination; // Déclaration de la structure des informations lié au serveur
	
	// Variable Serveur
	SOCKET id_de_la_nouvelle_socket;			// Identifiant de la nouvelle socket
	SOCKADDR_IN information_sur_la_source;		// Déclaration de la structure des informations lié à l'écoute
};
