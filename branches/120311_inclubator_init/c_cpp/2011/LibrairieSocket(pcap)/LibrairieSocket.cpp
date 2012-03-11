// ********************************************
// Nom : LibrairieSocket.cpp
// Auteur : SebF@frameIP.com.pas.de.spam
// Date de création : 01 Mai 2003
// version : 3.10
// Licence : Cette librairie est libre de toute utilisation.
//           La seule condition existante est de faire référence
//           au site http://www.frameip.com afin de respecter le travail d'autrui.
// ********************************************

// ********************************************************
// Les includes
// ********************************************************
// Il faut ajouter dans les proprités du projet => C++ => Command Line :
// /I "C:\RepPerso\Personnel\Developpement\Projets\LibrairiePcap\Include"
// /I "C:\RepPerso\Personnel\Developpement\Projets\LibrairieSocket"
#include "LibrairieSocket.h"
#include "pcap.h"
#include "packet32.h"
#include "ntddndis.h"

// ********************************************************
// Les fonctions
// ********************************************************
unsigned long resolution_de_nom(bool liberation, char *nom_a_resoudre)
{
	HOSTENT *structure_nom_de_lhote; // déclare la variable structure_nom_de_lhote de type HOSTENT permetant la recupération du gethostbyname
	WSADATA initialisation_win32;

	// ********************************************************
	// Vérifie si le nom_a_resoudre est déjà une adresse ip
	// ********************************************************
	if (inet_addr(nom_a_resoudre)!=-1)    //Vérifie si nom_de_lhote_distant est un nom ou une IP
		return(inet_addr(nom_a_resoudre));
	else
	{
		// ********************************************************
		// Initialisation des socket Win32
		// ********************************************************
		WSAStartup(MAKEWORD(2,2),&initialisation_win32);

		// ********************************************************
		// Résolution du nom d'hote en adresse ip
		// ********************************************************
		if (liberation==TRUE)
			liberation_du_jeton(); // Rend la main à la fenêtre principale
		structure_nom_de_lhote=gethostbyname(nom_a_resoudre);
		if (liberation==TRUE)
			liberation_du_jeton(); // Rend la main à la fenêtre principale

		WSACleanup();
		if (structure_nom_de_lhote!=0)
			return(*((unsigned int*)structure_nom_de_lhote->h_addr_list[0]));
		else
			return(0); // Présent uniquement pour éviter un warning durant la compilation

	}
}

struct structure_ip_local recuperation_ip_local(bool liberation)
{
	struct structure_ip_local renvoi_des_ip_locales;
	char tampon[64];
	struct hostent *informations_recupere;
	struct in_addr *adresse_ip;
	WSADATA initialisation_win32;

	// ********************************************************
	// Initialisation
	// ********************************************************
	WSAStartup(MAKEWORD(2,2),&initialisation_win32);
	renvoi_des_ip_locales.nombre_d_ip_local=0;

	// ********************************************************
	// Récupération du nom
	// ********************************************************
	gethostname(tampon,sizeof(tampon)) ;

	// ********************************************************
	// Récupération de l'IP locale
	// ********************************************************
	informations_recupere=gethostbyname(tampon) ;

	// ********************************************************
	// Quite propement le winsock ouvert avec la commande WSAStartup
	// ********************************************************
	WSACleanup(); //  (A appeller autant de fois qu'il a été ouvert)

	// ********************************************************
	// Renvoi de la liste de toutes les IP locales
	// ********************************************************
	while ( (adresse_ip=(struct in_addr *)*informations_recupere->h_addr_list++) != NULL )
	{
		if (liberation==TRUE)
			liberation_du_jeton(); // Rend la main à la fenêtre principale
		strcpy(renvoi_des_ip_locales.adresse_ip_local[renvoi_des_ip_locales.nombre_d_ip_local],inet_ntoa(*adresse_ip));
		renvoi_des_ip_locales.nombre_d_ip_local++;
	}

	return(renvoi_des_ip_locales);
}

unsigned short calcul_du_checksum(bool liberation, unsigned short *data, int taille_de_data)
{
	unsigned long checksum=0;

	// ********************************************************
	// Complément à 1 de la somme des complément à 1 sur 16 bits
	// ********************************************************
	while(taille_de_data>1)
	{
		if (liberation==TRUE)
			liberation_du_jeton(); // Rend la main à la fenêtre principale
		checksum=checksum+*data++;
		taille_de_data=taille_de_data-sizeof(unsigned short);
	}
	if(taille_de_data)
		checksum=checksum+*(unsigned char*)data;
	checksum=(checksum>>16)+(checksum&0xffff);
	checksum=checksum+(checksum>>16);

	return (unsigned short)(~checksum);
}

unsigned short calcul_du_checksum_ip(bool liberation, struct ipv4 ipv4_tampon)
{          
	unsigned short checksum;

	// ********************************************************
	// Initialisation du checksum
	// ********************************************************
	ipv4_tampon.checksum=0; // Doit être à 0 pour le calcul

	// ********************************************************
	// Calcul du IP
	// ********************************************************
	checksum=calcul_du_checksum(liberation,(unsigned short *)&ipv4_tampon,sizeof(struct ipv4));

	return checksum;
}

unsigned short calcul_du_checksum_icmp(bool liberation, struct icmp icmp_tampon,char data_tampon[65535],unsigned int taille_de_data_tampon)
{
	char tampon[65535];
	unsigned short checksum;

	// ********************************************************
	// Initialisation du checksum
	// ********************************************************
	icmp_tampon.checksum=0; // Doit être à 0 pour le calcul

	// ********************************************************
	// Calcul du ICMP
	// ********************************************************
	memcpy(tampon,(unsigned short *)&icmp_tampon,sizeof(struct icmp));
	memcpy(tampon+sizeof(struct icmp),data_tampon,taille_de_data_tampon);
	checksum=calcul_du_checksum(liberation,(unsigned short*)tampon,sizeof(struct icmp)+taille_de_data_tampon);

	return(checksum);
}

unsigned short calcul_du_checksum_igmp(bool liberation, struct igmp igmp_tampon,char data_tampon[65535],unsigned int taille_de_data_tampon)
{
	char tampon[65535];
	unsigned short checksum;

	// ********************************************************
	// Initialisation du checksum
	// ********************************************************
	igmp_tampon.checksum=0; // Doit être à 0 pour le calcul

	// ********************************************************
	// Calcul du checksum IGMP
	// ********************************************************
	memcpy(tampon,(unsigned short *)&igmp_tampon,sizeof(struct igmp));
	memcpy(tampon+sizeof(struct igmp),data_tampon,taille_de_data_tampon);
	checksum=calcul_du_checksum(liberation,(unsigned short*)tampon,sizeof(struct igmp)+taille_de_data_tampon);

	return(checksum);
}

unsigned short calcul_du_checksum_tcp(bool liberation, unsigned long ip_source_tampon,unsigned long ip_destination_tampon,struct tcp tcp_tampon,char data_tampon[65535],unsigned int taille_de_data_tampon)
{
	struct pseudo_entete pseudo_tcp;
	char tampon[65535];
	unsigned short checksum;

	// ********************************************************
	// Initialisation du checksum
	// ********************************************************
	tcp_tampon.checksum=0; // Doit être à 0 pour le calcul

	// ********************************************************
	// Le calcul du Checksum TCP (Idem à UDP)
	// ********************************************************
	// Le calcul passe par une pseudo entete TCP + l'entete TCP + les Data
	pseudo_tcp.ip_source=ip_source_tampon;
	pseudo_tcp.ip_destination=ip_destination_tampon;
	pseudo_tcp.mbz=0;
	pseudo_tcp.type=IPPROTO_TCP;
	pseudo_tcp.length=htons((unsigned short)(sizeof(struct tcp)+taille_de_data_tampon));
	memcpy(tampon,&pseudo_tcp,sizeof(pseudo_tcp));
	memcpy(tampon+sizeof(pseudo_tcp),&tcp_tampon,sizeof(struct tcp));
	memcpy(tampon+sizeof(pseudo_tcp)+sizeof(struct tcp),data_tampon,taille_de_data_tampon);
	checksum=calcul_du_checksum(liberation,(unsigned short*)tampon,sizeof(pseudo_tcp)+sizeof(struct tcp)+taille_de_data_tampon);

	return(checksum);
}

unsigned short calcul_du_checksum_udp(bool liberation, unsigned long ip_source_tampon,unsigned long ip_destination_tampon,struct udp udp_tampon,char data_tampon[65535],unsigned int taille_de_data_tampon)
{
	struct pseudo_entete pseudo_udp;
	char tampon[65535];
	unsigned short checksum;

	// ********************************************************
	// Initialisation du checksum
	// ********************************************************
	udp_tampon.checksum=0; // Doit être à 0 pour le calcul

	// ********************************************************
	// Le calcul du Checksum UDP (Idem à TCP)
	// ********************************************************
	// Le calcul passe par une pseudo entete UDP + l'entete UDP + les Data
	pseudo_udp.ip_source=ip_source_tampon;
	pseudo_udp.ip_destination=ip_destination_tampon;
	pseudo_udp.mbz=0;
	pseudo_udp.type=IPPROTO_UDP;
	pseudo_udp.length=htons((unsigned short)(sizeof(struct udp)+taille_de_data_tampon));
	memcpy(tampon,&pseudo_udp,sizeof(pseudo_udp));
	memcpy(tampon+sizeof(pseudo_udp),&udp_tampon,sizeof(struct udp));
	memcpy(tampon+sizeof(pseudo_udp)+sizeof(struct udp),data_tampon,taille_de_data_tampon);
	checksum=calcul_du_checksum(liberation,(unsigned short*)tampon,sizeof(pseudo_udp)+sizeof(struct udp)+taille_de_data_tampon);

	return(checksum);
}

char * dimensionnement_de_data_a_envoyer(bool liberation, char data[65535],unsigned int taille_de_data,unsigned int taille_desiree)
{
	char tampon[65535];
	int i,nombre_de_fois;

	// ********************************************************
	// Test sur la longueur de data afin d'éviter la division par 0
	// ********************************************************
	if (taille_de_data==0)
		return "";

	// ********************************************************
	// Evite les dépassement de buffer
	// ********************************************************
	if (taille_desiree>65000)
		taille_desiree=65000;

	// ********************************************************
	// Dimensionne les data
	// ********************************************************
	if(taille_desiree==0)
		sprintf(tampon,"");
	else
	{
		// ********************************************************
		// Création d'une chaine de 65535 contennant x fois les data_a_envoyer
		// ********************************************************
		nombre_de_fois=taille_desiree/taille_de_data;
		for (i=0;i<nombre_de_fois;i++)
		{
			memcpy(tampon+i*taille_de_data,data,taille_de_data);
			if (liberation==TRUE)
				liberation_du_jeton(); // Rend la main à la fenêtre principale
		}

		// ********************************************************
		// Ajoute les derniers caratères. Exemple : "coucou coucou" il manque couc à la fin si la taille est de 17
		// ********************************************************
		if (taille_desiree-(nombre_de_fois*taille_de_data)!=0)
			memcpy(tampon+(nombre_de_fois*taille_de_data),data,taille_desiree-(nombre_de_fois*taille_de_data));

		// ********************************************************
		// Ajoute un 0 pour terminer la chaine
		// ********************************************************
		tampon[taille_desiree]=0;
	}
	return(tampon);
}

void liberation_du_jeton(void)
{
	// **************************************************
	// DoEvents
	// **************************************************
	// Se place dans une boucle et rend la main à la fenetre principale afin de permettre à l'utilisateur de manipuler la fenêtre malgrès une boucle
	// C'est l'équivalent de la fonction DoEvents en VB

	MSG Msg;
	while (PeekMessage(&Msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void pause(bool liberation, unsigned int temps_demande)
{
	bool sortie_de_la_boucle=FALSE;                 // Condition de sortie de la boucle
	LARGE_INTEGER cpu_frequence;                // Fréquence du CPU
	LARGE_INTEGER temps_de_reponse1;       // Nombre de clock CPU
	LARGE_INTEGER temps_de_reponse2;       // Nombre de clock CPU
	double temps_ecoule;                                      // Temps en ms entre réponse 1 et réponse 2

	// **************************************************
	// Initiation de la fréquence du cpu
	// **************************************************
	QueryPerformanceFrequency((LARGE_INTEGER *)&cpu_frequence);

	// **************************************************
	// Réccupération du temps de référence
	// **************************************************
	QueryPerformanceCounter((LARGE_INTEGER *) &temps_de_reponse1);

	// **************************************************
	// Boucle d'attente
	// **************************************************
	while(sortie_de_la_boucle==FALSE)
	{
		// **************************************************
		// Attente pour éviter l'occupation processeur
		// **************************************************
		Sleep(1);

		// **************************************************
		// Rend la main à la fenêtre principale pour ne pas la bloqué
		// **************************************************
		if (liberation==TRUE)
			liberation_du_jeton();

		// **************************************************
		// Calcul du temps écoulé en ms
		// **************************************************
		QueryPerformanceCounter((LARGE_INTEGER *)&temps_de_reponse2);
		temps_ecoule=((double)((temps_de_reponse2.QuadPart-temps_de_reponse1.QuadPart)*(double)1000.0/(double)cpu_frequence.QuadPart));

		// **************************************************
		// Vérification du temps écoulé
		// **************************************************
		if (temps_ecoule>=temps_demande)
			sortie_de_la_boucle=TRUE;
	}
}

struct gestion_des_arguments recuperation_des_arguments(bool liberation)
{
	char * ligne_de_commande;
	struct gestion_des_arguments arguments_tampon;
	unsigned int nombre_d_argument;

	// **************************************************
	// Initialisation des variables
	// **************************************************
	nombre_d_argument=0;
	ligne_de_commande=(char *)malloc(65535);

	// **************************************************
	// Récupération des arguments sous forme d'un chaine
	// **************************************************
	strcpy(ligne_de_commande,GetCommandLine());

	// **************************************************
	// Extraction du premier argument (le nom et chemain de l'executable)
	// **************************************************
	arguments_tampon.argv[nombre_d_argument]=strtok(ligne_de_commande," ");

	// **************************************************
	// Boucle pour extraire tout les autres arguments
	// **************************************************
	while (arguments_tampon.argv[nombre_d_argument]!=NULL)
	{
		nombre_d_argument++;
		arguments_tampon.argv[nombre_d_argument]=strtok(NULL," ");
		if (liberation==TRUE)
			liberation_du_jeton(); // Rend la main à la fenêtre principale
	}

	// **************************************************
	// Récupère le nombre d'arguments (incluant le nom de l'executable)
	// **************************************************
	arguments_tampon.argc=nombre_d_argument;

	return(arguments_tampon);
}

DWORD pid_de_la_console_parente(bool liberation)
{
	DWORD console_pid; // Numéro PID de la fenetre mode console parente
	HWND console_hwnd; // HWND de la fenetre mode console parente

	// **************************************************
	// Trouve le HWND de la console
	// **************************************************
	// Même si il y a plusieur cmd de démarré, FindWindows donne la bonne.
	// Je ne sais pas comment il fait.
	// Une hypothèse est qu'il commence par celle en premier plan
	// Pour tous soucis une fonction officiel existe voir la KB 124103

	if (liberation==TRUE)
		liberation_du_jeton(); // Rend la main à la fenêtre principale
	console_hwnd=FindWindow("ConsoleWindowClass",NULL);
	if (liberation==TRUE)
		liberation_du_jeton(); // Rend la main à la fenêtre principale

	// **************************************************
	// Convertit un HWND en PID
	// **************************************************
	GetWindowThreadProcessId(console_hwnd,&console_pid);

	return (console_pid);
}

unsigned long generation_d_une_adresse_ip_aleatoire(int ne_sert_a_rien)
{
	struct in_addr convertion_ip;

	ne_sert_a_rien=0;                       // Utilise la variable pour éviter un Warning
	convertion_ip.S_un.S_un_b.s_b1=(unsigned char)rand()%255+1;
	convertion_ip.S_un.S_un_b.s_b2=(unsigned char)rand()%255+1;
	convertion_ip.S_un.S_un_b.s_b3=(unsigned char)rand()%255+1;
	convertion_ip.S_un.S_un_b.s_b4=(unsigned char)rand()%255+1;

	return(convertion_ip.S_un.S_addr);
}

unsigned short convertion_offset_control_tcp(int ne_sert_a_rien,unsigned char offset,unsigned char reserved,bool urg,bool ack,bool psh,bool rst ,bool syn,bool fin)
{
	unsigned short convertion;

	ne_sert_a_rien=0;                       // Utilise la variable pour éviter un Warning

	// << n        décale de n vers la gauche. Ajoute donc n 0 binaire à droite.
	// >> n        décale de n vers la droite. Ajoute donc n 0 binaire à gauche.

	// **************************************************
	// Tronque les variables pour s'assurer quelles soient dans leurs ranges
	// **************************************************
	offset=offset<<2; // Offset étant sur 6 bits, je vires les deux bits de gauches
	offset=offset>>2; //Je remet deux 0 à gauche pour revenir au nombre d'origine
	reserved=reserved<<4; // Reserved étant sur 4 bits, je vires les quatres bits de gauche
	reserved=reserved>>4; // Je remet quatre 0 à gauche afin de revenir au nombre d'origine

	// **************************************************
	// Intégration d'Offset
	// **************************************************
	convertion=offset;

	// **************************************************
	// Intégration de Reserved
	// **************************************************
	convertion=convertion<<4;
	convertion=convertion+reserved;

	// **************************************************
	// Intégration des controles
	// **************************************************
	convertion=convertion<<1;
	convertion=convertion+urg;
	convertion=convertion<<1;
	convertion=convertion+ack;
	convertion=convertion<<1;
	convertion=convertion+psh;
	convertion=convertion<<1;
	convertion=convertion+rst;
	convertion=convertion<<1;
	convertion=convertion+syn;
	convertion=convertion<<1;
	convertion=convertion+fin;

	return(htons(convertion));
}

void gotoxy(short x,short y)
{
	HANDLE handle_de_la_console;        // Handle correspondant à la sortie du printf
	COORD coordonnees;                                               // Structure contenant les coordonnées x et y

	// **************************************************
	// Initialisation des coordonnées X et Y
	// **************************************************
	coordonnees.X=x;
	coordonnees.Y=y;

	// **************************************************
	// Récupère le Handle de la sortie du printf
	// **************************************************
	handle_de_la_console=GetStdHandle(STD_OUTPUT_HANDLE);

	// **************************************************
	// Place le pointer à x et à y
	// **************************************************
	SetConsoleCursorPosition(handle_de_la_console,coordonnees);
}

void clear_screen(void)                           
{
	HANDLE handle_de_la_console;                                           // Handle correspondant à la sortie du printf
	CONSOLE_SCREEN_BUFFER_INFO info_console;          // Structure récupérant les informations de la console
	DWORD tampon;                                                                                          // Tampon recupère le nombre de caractère écrit pour effacer l'écran (non utilisé) 
	COORD coordonnees={0,0};                                                // Structure contenant les coordonnées x et y

	// **************************************************
	// Récupération des informations concernant la fenetre console
	// **************************************************
	handle_de_la_console=GetStdHandle(STD_OUTPUT_HANDLE);                          // Récupère le Handle de la console (sortie du printf)
	GetConsoleScreenBufferInfo(handle_de_la_console,&info_console); // Récupère les information de la console

	// **************************************************
	// Efface l'écran
	// **************************************************
	FillConsoleOutputCharacter(handle_de_la_console,' ',info_console.dwSize.X*info_console.dwSize.Y,coordonnees,&tampon);

	// **************************************************
	// Place le pointer à x et à y
	// **************************************************
	gotoxy(0,0); // positionne le curseur en haut à gauche de l'écran
}

char * convertion_ip(int adresse_ip)
{
	SOCKADDR_IN translation;
	char tampon[65535];

	// ********************************************************
	// Transformation en IP de type xxx.xxx.xxx.xxx
	// ********************************************************
	translation.sin_addr.s_addr=adresse_ip;
	strcpy(tampon,inet_ntoa(translation.sin_addr));

	return (tampon);
}

char *  conversion_string_hexa(char data[65535],unsigned int taille_de_data)
{
	char tampon[65535];
	unsigned int i;
	int tempo_non_signe;

	// ********************************************************
	// Test sur la longueur de data afin d'éviter la division par 0
	// ********************************************************
	if (taille_de_data==0)
		return "";

	// ********************************************************
	// Evite les dépassement de buffer
	// ********************************************************
	if (taille_de_data>65000)
		taille_de_data=32000;

	// ********************************************************
	// Convertion en Hexa
	// ********************************************************
	for (i=0;i<taille_de_data;i++)
	{
		tempo_non_signe=data[i];
		if (tempo_non_signe<0) // Permet de convertir le nombre de -128;128 en 0;255
			tempo_non_signe+=256;
		sprintf(tampon+i*2,"%.2x",tempo_non_signe); // .2 permet de mettre le 0 des nombres inférieurs à 0F
	}

	return (tampon);
}

char *  conversion_hexa_string(char data[65535],unsigned int taille_de_data)
{
	char tampon[65535];
	unsigned int i;
	char a,b,c;

	// ********************************************************
	// Test sur la longueur de data afin d'éviter la division par 0
	// ********************************************************
	if (taille_de_data==0)
		return "";

	// ********************************************************
	// Passe tout en majuscule
	// ********************************************************
	memcpy(data,strupr(data),taille_de_data);

	// ********************************************************
	// Convertion en Hexa
	// ********************************************************
	for (i=0;i<taille_de_data;i=i+2)
	{
		a=data[i];
		b=data[i+1];

		c=(a>='0'&&a<='9')*(a-'0')+(a>='A'&&a<='F')*(a-'A'+10);
		c=c*16;
		c=c+(b>='0'&&b<='9')*(b-'0')+(b>='A'&&b<='F')*(b-'A'+10);
		sprintf(tampon+i/2,"%c",c);
	}

	return (tampon);
}

struct gestion_des_interfaces recuperation_des_interfaces()
{
	pcap_if_t *liste_des_interfaces;                                                          // Liste des interfaces
	pcap_if_t *liste_des_interfaces_copie;                                    // Copie de la liste des interfaces
	char buffer_d_erreur[PCAP_ERRBUF_SIZE+1];                          // Buffer de l'erreur réccupéré
	struct gestion_des_interfaces interfaces_tampon;          // Structure qui sera renvoyée
	unsigned int nombre_d_interface=0;                                                    // Nombre d'interface présente
	unsigned i;                                                                                                                             // Pour les boucles for

	// ********************************************************
	// Récupération des interfaces
	// ********************************************************
	if(pcap_findalldevs(&liste_des_interfaces,buffer_d_erreur)==-1)
		interfaces_tampon.nombre=0;
	else
	{
		// ********************************************************
		// Compte le nombre d'interface
		// ********************************************************
		liste_des_interfaces_copie=liste_des_interfaces;
		for(liste_des_interfaces_copie=liste_des_interfaces; liste_des_interfaces_copie; liste_des_interfaces_copie=liste_des_interfaces_copie->next)
			nombre_d_interface++;
		interfaces_tampon.nombre=nombre_d_interface;

		// ********************************************************
		// Récupère les informations relatives aux interfaces
		// ********************************************************
		liste_des_interfaces_copie=liste_des_interfaces;
		for (i=0;i<nombre_d_interface;i++)
		{
			sprintf(interfaces_tampon.description[i],liste_des_interfaces_copie->description);
			sprintf(interfaces_tampon.nom[i],liste_des_interfaces_copie->name);
			liste_des_interfaces_copie=liste_des_interfaces_copie->next;
		}

		// ********************************************************
		// Libère les interfaces
		// ********************************************************
		pcap_freealldevs(liste_des_interfaces);
	}

	return(interfaces_tampon);
}

struct adresse_mac recuperation_de_l_adresse_mac(char nom_de_l_interface_tampon[1000])
{
	struct adresse_mac adresse_mac_tampon;       // structure contenant l'adresse MAC qui sera renvoyée
	LPADAPTER lpAdapter=0;                                                   // Pointeur d'accès à l'interface
	PPACKET_OID_DATA OidData;                                        // Structure contenant la requête permettant de connaitre l'adresse MAC

	// ********************************************************
	// Accède à l'interface
	// ********************************************************
	lpAdapter=PacketOpenAdapter(nom_de_l_interface_tampon);

	// ********************************************************
	// Prépare un buffer à recevoir l'adresse MAC
	// ********************************************************
	// ! J'ai du convertir via la commande (PPACKET_OID_DATA) sinon erreur.
	OidData=(PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));

	// ********************************************************
	// Récupération de l'adresse MAC
	// ********************************************************
	OidData->Oid=OID_802_3_CURRENT_ADDRESS;
	OidData->Length=6;
	ZeroMemory(OidData->Data,6);
	PacketRequest(lpAdapter,FALSE,OidData);

	// ********************************************************
	// Transfert de l'adresse MAC dans la structure de retour
	// ********************************************************
	adresse_mac_tampon.adresse[0]=(unsigned char)(PCHAR)(OidData->Data)[0];
	adresse_mac_tampon.adresse[1]=(unsigned char)(PCHAR)(OidData->Data)[1];
	adresse_mac_tampon.adresse[2]=(unsigned char)(PCHAR)(OidData->Data)[2];
	adresse_mac_tampon.adresse[3]=(unsigned char)(PCHAR)(OidData->Data)[3];
	adresse_mac_tampon.adresse[4]=(unsigned char)(PCHAR)(OidData->Data)[4];
	adresse_mac_tampon.adresse[5]=(unsigned char)(PCHAR)(OidData->Data)[5];

	// ********************************************************
	// Libération du buffer de reception
	// ********************************************************
	free(OidData);

	// ********************************************************
	// Libération de l'accés à l'interface
	// ********************************************************
	PacketCloseAdapter(lpAdapter);

	return(adresse_mac_tampon);
}

struct adresse_mac transformation_string_en_mac(char * string)
{
	unsigned int nombre_d_octet=0;                                             // Compte le nombre d'octets traité dans la chaine de caractère
	char * tampon;                                                                                   // Récupère les différent octets séparé par des -
	bool erreur=false;                                                                               // Permet de vérifier si la chaine à convertir correspond à une adresse MAC
	struct adresse_mac adresse_mac_tampon;                  // Adresse MAC qui sera retournée
	unsigned short octet1,octet2;                                       // Permet de gérer un octet pas deux lettres pour C3 => C et 3
	unsigned int i;                                                                                      // Pour les boucles for

	// ********************************************************
	// Initialisation de adresse_mac_tampon afin d'éviter un warning
	// qui stipule la possibilité d'utilisation sans avoir été initialisé
	// ********************************************************
	adresse_mac_tampon.adresse[0]=0; // J'utilise pas de boucle for, sinon le compilateur m'indique toujours le Warning
	adresse_mac_tampon.adresse[1]=0;
	adresse_mac_tampon.adresse[2]=0;
	adresse_mac_tampon.adresse[3]=0;
	adresse_mac_tampon.adresse[4]=0;
	adresse_mac_tampon.adresse[5]=0;

	// ********************************************************
	// Récupère les caractères avant le "-"
	// ********************************************************
	tampon=strtok(string,"-");

	while (tampon!=NULL)
	{
		// ********************************************************
		// Valide que c'est bien sur 2 octets
		// ********************************************************
		if (strlen(tampon)!=2)
			erreur=true;
		else
		{
			// ********************************************************
			// Convertit en majuscule
			// ********************************************************
			tampon=strupr(tampon);

			// ********************************************************
			// Compare si c'est entre 0-9 ou A-F
			// ********************************************************
			if (( ((tampon[0]>=48)&&(tampon[0]<=57)) || ((tampon[0]>=65)&&(tampon[0]<=70)) ) && ( ((tampon[1]>=48)&&(tampon[1]<=57)) || ((tampon[1]>=65)&&(tampon[1]<=70)) ))
			{
				// ********************************************************
				// Réunis les deux caractères et les convertient en leurs valeurs hexa
				// ********************************************************
				if ((tampon[0]>=48)&&(tampon[0]<=57))
					octet1=16*(tampon[0]-48);
				else
					octet1=16*(tampon[0]-48-7); // 7 correspond au nombre de caracteres compris entre le 0 et le A
				if ((tampon[1]>=48)&&(tampon[1]<=57))
					octet2=tampon[1]-48;
				else
					octet2=tampon[1]-48-7; // 7 correspond au nombre de caracteres compris entre le 0 et le A
				adresse_mac_tampon.adresse[nombre_d_octet]=(unsigned char)(octet1+octet2);
			}
			else
				erreur=true;
		}

		// ********************************************************
		// Incrémente le nombre d'octet traité
		// ********************************************************
		nombre_d_octet++;

		// ********************************************************
		// Récupère les caractères avant le "-" suivant
		// ********************************************************
		tampon=strtok(NULL,"-");
	}

	// ********************************************************
	// Valide que le nombre d'octet de la chaine était bien 6
	// ********************************************************
	if ((nombre_d_octet!=6)||(erreur==true))
		for (i=0;i<6;i++)
			adresse_mac_tampon.adresse[i]=0;

	// ********************************************************
	// Renvoi l'adresse MAC
	// ********************************************************
	return (adresse_mac_tampon);
}

struct adresse_mac resolution_arp(char nom_de_l_interface_tampon[1000],bool mode_promiscuite_tampon,char adresse_ip_tampon[16],unsigned int attente_maximum)
{
	pcap_t *pointeur_interface;                                                                            // Pointeur indiquant l'interface utilisée
	char buffer_d_erreur[PCAP_ERRBUF_SIZE];                                              // Buffer de l'erreur réccupéré
	unsigned int i;                                                                                                             // Pour les boucles for
	struct mac entete_mac;                                                                                   // Entete MAC qui sera emis
	struct arp entete_arp;                                                                                     // Entete ARP qui sera envoyé
	struct adresse_mac adresse_mac_tampon;                                          // Adresse MAC tempo
	struct adresse_mac adresse_mac_de_retour;                           // Adresse MAC qui sera retournée
	structure_ip_local reception_des_ip_locales;               // Structure permettant de récupérer l'adresse IP source
	unsigned char trame_a_envoyer[65535];                                             // Chaine de caratère comprennant les entêtes et la data
	unsigned short taille_de_la_trame_a_envoyer=0;                     // Taille de la trame qui sera emise
	char ip_source[16];                                                                                                    // IP Source
	int resultat_de_l_envoi;                                                                       // Resultat de l'envoi
	const u_char *trame_recue;                                                                            // Chaine contenant la trame recue
	struct pcap_pkthdr *header;
	int compteur=0;                                                                                                                     // Compteur permettant de compter le nombre de comparaison correcte
	int temps_avant_la_boucle;                                                                             // Temps pris avant de rentrer dans la boucle. Ca permettra de connaitre la durée de la boucle.
	bool sortie_de_la_boucle;                                                                              // Condition de sortie de la boucle

	// ********************************************************
	// Initialisation de adresse_mac_tampon afin d'éviter un warning
	// qui stipule la possibilité d'utilisation sans avoir été initialisé
	// ********************************************************
	adresse_mac_de_retour.adresse[0]=0; // J'utilise pas de boucle for, sinon le compilateur m'indique toujours le Warning
	adresse_mac_de_retour.adresse[1]=0;
	adresse_mac_de_retour.adresse[2]=0;
	adresse_mac_de_retour.adresse[3]=0;
	adresse_mac_de_retour.adresse[4]=0;
	adresse_mac_de_retour.adresse[5]=0;

	// ********************************************************
	// Accède à l'interface
	// ********************************************************
	if ((pointeur_interface=pcap_open_live(nom_de_l_interface_tampon,65536,mode_promiscuite_tampon,1000,buffer_d_erreur))!=NULL)
	{
		// ********************************************************
		// Initialisation de l'entête MAC
		// ********************************************************
		adresse_mac_tampon=recuperation_de_l_adresse_mac(nom_de_l_interface_tampon);
		for (i=0;i<6;i++)
		{
			entete_mac.source[i]=adresse_mac_tampon.adresse[i];
			entete_mac.destination[i]=255;
		}
		entete_mac.type=htons(2054);

		// ********************************************************
		// Initialisation de l'entête ARP
		// ********************************************************
		entete_arp.mac_type=htons(1);
		entete_arp.protocol_type=htons(2048);
		entete_arp.mac_taille=6;
		entete_arp.protocol_taille=4;
		entete_arp.opocode=htons(1);
		for (i=0;i<6;i++)
			entete_arp.mac_source[i]=entete_mac.source[i];
		reception_des_ip_locales=recuperation_ip_local(false);
		strcpy(ip_source,reception_des_ip_locales.adresse_ip_local[0]);
		entete_arp.ip_source=resolution_de_nom(false,ip_source);
		for (i=0;i<6;i++)
			entete_arp.mac_destination[i]=entete_mac.destination[i];
		entete_arp.ip_destination=resolution_de_nom(false,adresse_ip_tampon);

		// ********************************************************
		// Concatenation des différentes couches
		// ********************************************************
		memcpy(trame_a_envoyer,(unsigned short *)&entete_mac,sizeof(struct mac));   
		memcpy(trame_a_envoyer+14,(unsigned short *)&entete_arp,sizeof(struct arp));
		taille_de_la_trame_a_envoyer=sizeof(struct mac)+sizeof(struct arp);

		// ********************************************************
		// Envoi de la trame
		// ********************************************************
		if (pcap_sendpacket(pointeur_interface,trame_a_envoyer,taille_de_la_trame_a_envoyer)== 0)
		{
			// ********************************************************
			// Initialisation avant de rentrer dans la boucle
			// ********************************************************
			temps_avant_la_boucle=GetTickCount();
			sortie_de_la_boucle=false;
			for(i=0;i<6;i++)
				adresse_mac_de_retour.adresse[i]=0;

			// ********************************************************
			// Boucle de reception
			// ********************************************************
			while(sortie_de_la_boucle==false)
			{
				// ********************************************************
				// Se met en attente de recupèrer la prochaine trame. Maximum une seconde
				// ********************************************************
				resultat_de_l_envoi=pcap_next_ex(pointeur_interface,&header,&trame_recue);

				// ********************************************************
				// Vérifie si la boucle dure trop longtemps
				// ********************************************************
				if (GetTickCount()-temps_avant_la_boucle>=1000*attente_maximum)
					sortie_de_la_boucle=true;

				// ********************************************************
				// Vérifie si il y a une trame recue
				// ********************************************************
				if (resultat_de_l_envoi!=0)
				{
					compteur=0;

					// ********************************************************
					// Vérification de l'adresse MAC source
					// ********************************************************
					for (i=0;i<6;i++)
						if (trame_recue[i]==entete_mac.source[i])
							compteur++;

					// ********************************************************
					// Vérification de l'Ether Type
					// ********************************************************
					if (trame_recue[12]==8)
						compteur++;
					if (trame_recue[13]==6)
						compteur++;

					// ********************************************************
					// Vérification de l'Hardware Type
					// ********************************************************
					if (trame_recue[14]==0)
						compteur++;
					if (trame_recue[15]==1)
						compteur++;

					// ********************************************************
					// Vérification de Protocol Type
					// ********************************************************
					if (trame_recue[16]==8)
						compteur++;
					if (trame_recue[17]==0)
						compteur++;

					// ********************************************************
					// Vérification de l'Hardware Size
					// ********************************************************
					if (trame_recue[18]==6)
						compteur++;

					// ********************************************************
					// Vérification de Protocol Size
					// ********************************************************
					if (trame_recue[19]==4)
						compteur++;

					// ********************************************************
					// Vérification de Opocode
					// ********************************************************
					if (trame_recue[20]==0)
						compteur++;
					if (trame_recue[21]==2)
						compteur++;

					// ********************************************************
					// Vérification de l'adresse MAC source
					// ********************************************************
					for (i=0;i<6;i++)
						if (trame_recue[i+32]==entete_mac.source[i])
							compteur++;

					// ********************************************************
					// Vérification si toutes les comparaisons sont OK
					// ********************************************************
					if (compteur==22)
					{
						for(i=0;i<6;i++)
							adresse_mac_de_retour.adresse[i]=trame_recue[i+22]; // 22 permet de se décalet pour réccuperer la mac address de l'emetteur
						sortie_de_la_boucle=true;
					}

					/*
					// ********************************************************
					// Affichage des trames recues
					// ********************************************************
					printf("\n");
					printf("Taille : %d Entete MAC : ",header->len);
					for (i=0;i<header->len;i++)
					printf("%.2X ",trame_recue[i]);
					printf("\n");
					*/
				}
			}
		}

		// ********************************************************
		// Fermeture de l'accès à l'interface
		// ********************************************************
		pcap_close(pointeur_interface);           
	}

	// ********************************************************
	// Renvoi l'adresse MAC trouvée
	// ********************************************************
	return(adresse_mac_de_retour);
}

void gestion_des_erreurs(unsigned int numero,char * fonction,unsigned int affichage,bool sortie)
{
	char message[1000]="Error number 000 : Error in the errors engine";

	// ********************************************************
	// Message d'erreur Winsock 1xx
	// ********************************************************
	if (strlwr(fonction)==(char *)"wsastartup")
		sprintf(message,"Error number 101.%d : Unable to initialize WSAStartup.",numero);
	if (strlwr(fonction)==(char *)"socket")
		sprintf(message,"Error number 102.%d : Unable to initialize Socket.",numero);
	if (strlwr(fonction)==(char *)"setsockopt")
		sprintf(message,"Error number 103.%d : Unable to initialize SetSockOpt.",numero);
	if (strlwr(fonction)==(char *)"sendto")
		switch (WSAGetLastError())
	{
		case 10004:
			sprintf(message,"Error number 112.%d : Unable to send your IP frame, Sorry, but your WinXP SP1 ou SP2 deny all TCP et spoofing IP, WSAGetLastError %d",numero,WSAGetLastError());
			break;
		case 10013:
			sprintf(message,"Error number 113.%d : Unable to send your IP frame, You must be local administrator, WSAGetLastError %d",numero,WSAGetLastError());
			break;
		default :
			sprintf(message,"Error number 111.%d : Unable to send your IP frame, WSAGetLastError %d",numero,WSAGetLastError());
			break;
	}

	// ********************************************************
	// Message d'erreur Pcap 2xx
	// ********************************************************
	if (strlwr(fonction)==(char *)"pcap_open_live")
		sprintf(message,"Error number 201.%d : Unable to open your adapter.",numero);
	if (strlwr(fonction)==(char *)"pcap_sendpacket")
		sprintf(message,"Error number 202.%d : Unable to send your Ethernet frame.",numero);

	// ********************************************************
	// Affichage de l'erreur en printf
	// ********************************************************
	if (affichage==1)
		printf("\n\n%s\n\n",message);

	// ********************************************************
	// Arrêt du programme
	// ********************************************************
	if (sortie==true)
		exit(0);
}
