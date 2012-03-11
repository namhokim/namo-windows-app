// ********************************************************
// Permet que la structure de arp soit bien égale à 28, sans "#pragma pack(2);", la taille de la structure arp est de 32
// ********************************************************
#pragma pack(2)

// ********************************************************
// Les includes
// ********************************************************
#include <winsock2.h>
#include <cstdio>                                // Pour les Sprintf
const int IP_HDRINCL=2;      // Permet d'éviter d'intégrer #include <ws2tcpip.h> qui ne veux plus fonctionner
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1) 

// ********************************************************
// Les librairies
// ********************************************************
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"C:\\RepPerso\\Personnel\\Developpement\\Projets\\LibrairieSocket\\LibrairieSocket.lib")
//#pragma pack(1) // Permet de préciser l'arondie des variable de type :4 :7 etc ...

// ********************************************************
// Les fonctions
// ********************************************************
unsigned long resolution_de_nom(bool,char [65535]);
// Permet     : Résoud un nom d'hôte en adresse IP. Vérifie aussi si le nom d'hôte n'est pas déjà une IP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Char de 65535 contennant l'adresse IP à résoudre.
// Renvoi     : Unsigned Long contennant l'inet_addr de l'IP résolu.

struct structure_ip_local recuperation_ip_local(bool);
// Permet     : Récupère la liste de toutes les adresses IP locales à la machines.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Renvoi     : Struct structure_ip_local contennant La liste des ip locales ainsi que le nombre d'adresse ip locales présentent sur la machine.

unsigned short calcul_du_checksum(bool,unsigned short *, int);
// Permet     : Permet le calcul basique du checksum.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Les data au format char 65535 convertit en pointeur unsigned short.
// Argument 3 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Renvoi     : Le checksum sur deux octets (unsigned short).

unsigned short calcul_du_checksum_ip(bool,struct ipv4);
// Permet     : Permet le calcul du checksum de l'entête IP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Structure de l'entête IP dont il faut calculer le checksum.
// Argument 3 : Char 65535 contennant tous ce qu'il y après l'entête IP (Les entêtes tels que TCP UDP ainsi que les data).
// Renvoi     : Le checksum sur deux octets (unsigned short).
// Fonction ne travail pas correctement. Mauvais résultat obtenu.

unsigned short calcul_du_checksum_icmp(bool,struct icmp,char [65535],unsigned int);
// Permet     : Permet le calcul du checksum de l'entête ICMP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Structure de l'entête ICMP dont il faut calculer le checksum.
// Argument 3 : Char 65535 contennant tous ce qu'il y après l'entête ICMP.
// Argument 4 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Renvoi     : Le checksum sur deux octets (unsigned short).

unsigned short calcul_du_checksum_igmp(bool,struct igmp,char [65535],unsigned int);
// Permet     : Permet le calcul du checksum de l'entête IGMP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Structure de l'entête IGMP dont il faut calculer le checksum.
// Argument 3 : Char 65535 contennant tous ce qu'il y après l'entête IGMP.
// Argument 4 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Renvoi     : Le checksum sur deux octets (unsigned short).

unsigned short calcul_du_checksum_tcp(bool,unsigned long,unsigned long,struct tcp,char [65535],unsigned int);
// Permet     : Permet le calcul du checksum de l'entête TCP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Adresse IP source (de l'entête IP) au format Inet_addr (unsigned long).
// Argument 3 : Adresse IP destination (de l'entête IP) au format Inet_addr (unsigned long).
// Argument 4 : Structure de l'entête TCP dont il faut calculer le checksum.
// Argument 5 : Char 65535 contennant tous ce qu'il y après l'entête TCP.
// Argument 6 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Renvoi     : Le checksum sur deux octets (unsigned short).

unsigned short calcul_du_checksum_udp(bool,unsigned long,unsigned long,struct udp,char [65535],unsigned int);
// Permet     : Permet le calcul du checksum de l'entête UDP.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Adresse IP source (de l'entête IP) au format Inet_addr (unsigned long).
// Argument 3 : Adresse IP destination (de l'entête IP) au format Inet_addr (unsigned long).
// Argument 4 : Structure de l'entête UDP dont il faut calculer le checksum.
// Argument 5 : Char 65535 contennant tous ce qu'il y après l'entête UDP.
// Argument 6 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Renvoi     : Le checksum sur deux octets (unsigned short).

char * dimensionnement_de_data_a_envoyer(bool,char [65535],unsigned int,unsigned int);
// Permet     : Permet de multiplier une chaie existante sur une longueur données.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Chaine de caractères servant de référence.
// Argument 3 : longueur des data. Cet argument est présent car il ne faut pas utilser strlen(data) car data pourrait contenir des 0.
// Argument 4 : Taille désirée de la chaine. 
// Renvoi     : Un char * contennant la chaine référence dimentionnée à la longueur voulut. Exemple, data="abcd" taille=10 la chaine retournée sera alors "abcdabcdab".

void liberation_du_jeton (void);
// Permet     : Rend le processeur à la fenêtre graphique afin d'éviter quelle soit figée.

void pause(bool,unsigned int);
// Permet     : Fournit une pause au miliseconde près. De plus, elle permet d'obtenir une pause sans figer la fenêtre graphique.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Argument 2 : Nombre de miliseconde à attendre.

struct gestion_des_arguments recuperation_des_arguments(bool);
// Permet     : Récupère tous les arguments entré après l'exe ainsi que leurs nombres.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Renvoi     : La structure gestion_des_arguments contennant la liste des arguments ainsi que leur nombre.

DWORD pid_de_la_console_parente(bool);
// Permet     : Récupère le PID de la fenêtre console qui lancé l'exe.
// Argument 1 : Bool activant ou désactivant le rendu processeur à la fenêtre graphique.
// Renvoi     : Le PID au format DWORD.

unsigned long generation_d_une_adresse_ip_aleatoire(int);
// Permet     : Génère une adresse IPV4 aléatoirement.
// Argument 1 : Argument ne servant pas. Présent pour de futurs besoins et permet donc de garder une compatibilité. Mettre à 0.
// Renvoi     : L'adresse IP aléatoire au format Inet_addr (unsigned long).

unsigned short convertion_offset_control_tcp(int,unsigned char,unsigned char,bool,bool,bool,bool,bool,bool);
// Cette fonction n'est plus utilisée, Une nouvelle structure Tcp permet de ne pas réunir tous les variable en une seule
// Permet     : Convertit les champs Offset, Résérvé et Control de l'entête TCP en un seul nombre 16 bits.
// Argument 1 : Argument ne servant pas. Présent pour de futurs besoins et permet donc de garder une compatibilité. Mettre à 0.
// Argument 2 : Valeur du champ Offset (4 bits). Doit être compris entre 0 et 15.
// Argument 3 : Valeur du champ Résérvé (6 bits). Doit être compris entre 0 et 63.
// Argument 4 : Active ou désactive le champ URG.
// Argument 5 : Active ou désactive le champ ACK.
// Argument 6 : Active ou désactive le champ PSH.
// Argument 7 : Active ou désactive le champ RST.
// Argument 8 : Active ou désactive le champ SYN.
// Argument 9 : Active ou désactive le champ FIN.
// Renvoi     : Renvoi le nombre 16 bits (unsigned short) correspondant à la définition de la structure de l'entête TCP.

void gotoxy(short x,short y);
// Permet     : Place le curseur aux coordonnées x et y
// Argument 1 : coordonnées x.
// Argument 1 : coordonnées y.

void clear_screen(void);
// Permet     : D'effectuer un cls de la fenêtre console

char * convertion_ip(int);
// Permet     : De convertir une adresse IP du format Inet_addr en chaine du type "xxx.xxx.xxx.xxx";
// Argument 1 : Adresse IP de type Inet_addr.
// Renvoi     : Renvoi l'adresse IP du type chaine de caractère.

char * conversion_string_hexa(char [65535],unsigned int);
// Permet     : De convertir une chaine de caractère en Hexa
// Argument 1 : Chaine de caractere
// Argument 1 : Longueur de la chaine
// Renvoi     : Renvoi la correspondance Hexa de la chaine de caractere

char * conversion_hexa_string(char [65535],unsigned int);
// Permet     : De convertir une chaine hexa en chaine de caractère
// Argument 1 : Chaine Hexa
// Argument 1 : Longueur de la chaine
// Renvoi     : Renvoi la chaine de caractere correspondant

struct gestion_des_interfaces recuperation_des_interfaces();
// Permet     : Récupère tous les renseignements des interfaces du système ainsi que leur nombre.
// Renvoi     : La structure gestion_des_interfaces contennant la liste des interfaces ainsi que leur nombre.

struct adresse_mac recuperation_de_l_adresse_mac(char [1000]);
// Permet     : Récupère l'adresse MAC de l'interface selectionnée.
// Argument 1 : Indique le nom de l'interface demandée.
// Renvoi     : Une structure contennant l'adresse MAC décomposée en 6 octets.

struct adresse_mac transformation_string_en_mac(char *);
// Permet     : Transforme une chaine de caractère en adresse MAC.
// Argument 1 : Chaine de caractère à transformer.
// Renvoi     : Une structure contennant l'adresse MAC décomposée en 6 octets.

struct adresse_mac resolution_arp(char [1000],bool,char [16],unsigned int);
// Permet     : Trouve l'adresse MAC d'une IP donnée.
// Argument 1 : Numéro de l'interface à utiliser.
// Argument 2 : Indique l'activation ou pas du mode promiscuite.
// Argument 3 : Adresse IP à convertir.
// Argument 4 : Temps d'attente maximum pour la réponse ARP avant de considérer qu'il n'y a pas de réponse (En seconde).
// Renvoi     : Une structure contennant l'adresse MAC décomposée en 6 octets.

void gestion_des_erreurs(unsigned int,char *,unsigned int,bool);
// Permet     : Gère les erreurs rencontrées.
// Argument 1 : Si une fonction est utilisée plusieurs fois dans le programme, cette identifiant permet de savoir si c'est la première, seconde ....
// Argument 2 : Nom de la fonction en erreur.
// Argument 3 : permet d'indiquer si il ya un affichage (0=pas d'affichage - 1=affichage printf - 2=affichage graphique).
// Argument 4 : Indique si on doit arrêter le programme ou laisser continuer (true=stop).

// ********************************************************
// Les structures
// ********************************************************
struct gestion_des_arguments
            {
            unsigned int argc;                                             // Nombre d'argument
            char * argv[1024];                                          // Tableau contennant tous les arguments
            };

struct gestion_des_interfaces
            {
            unsigned int nombre;                                            // Nombre d'interface
            char description[100][1000];                                   // Tableau contennant toutes les descriptions
            char nom[100][1000];                                                                // Tableau contennant toutes les noms
            };

struct structure_ip_local
            {
            int                                            nombre_d_ip_local;
            char                                         adresse_ip_local[1000][16];
            };

struct adresse_mac
            {
            unsigned char               adresse[6];
            };

struct mac
            {
            unsigned char               destination[6];
            unsigned char               source[6];
            unsigned short              type;
            };

struct arp
            {
            unsigned short              mac_type;
            unsigned short              protocol_type;
            unsigned char               mac_taille;
            unsigned char               protocol_taille;
            unsigned short              opocode;
            unsigned char               mac_source[6];
            unsigned long                                       ip_source;
            unsigned char               mac_destination[6];
            unsigned long                                       ip_destination;
            };

struct ipv4 // htons pour les ushort et htonl pour les ulong
            {
            unsigned char               ihl:4;                 // L'ordre des champs ihl et Version est inversé
            unsigned char               version:4;                     // L'ordre des champs ihl et Version est inversé
            unsigned char               tos;
            unsigned short              length;
            unsigned short              id;
            unsigned short              offset:5;                       // Ajout d'offset2 afin de pouvoir utiliser l'invertion qui se fait de 8 bits en 8 bits et non pas en unsigned short
            unsigned short              flag_mf:1;                     // L'ordre des champs offset et flag est inversé
            unsigned short              flag_df:1;                     // L'ordre des champs offset et flag est inversé
            unsigned short              flag_zero:1;      // L'ordre des champs offset et flag est inversé
            unsigned short              offset2:8;                     // L'ordre des champs offset et flag est inversé
            unsigned char               ttl;
            unsigned char               type;
            unsigned short              checksum;
            unsigned long               ip_source;
            unsigned long               ip_destination;
            };

struct icmp // htons pour les ushort et htonl pour les ulong
            {
            unsigned char               type;
            unsigned char               code;
            unsigned short              checksum;
            unsigned short              id;
            unsigned short              sequence;
            };

struct igmp // htons pour les ushort et htonl pour les ulong
            {
            unsigned char               type:4;                         // L'ordre des champs Type et Version est inversé
            unsigned char               version:4;                     // L'ordre des champs Type et Version est inversé
            unsigned char               reserve;
            unsigned short              checksum;
            unsigned long               ip_destination;
            };

/*
// **********************************
// Ancienne structure TCP
// **********************************
struct tcp // htons pour les ushort et htonl pour les ulong
            {
            unsigned short              port_source;
      unsigned short                    port_destination;
            unsigned long               sequence;
            unsigned long               accuse;
            unsigned short              offset;
            unsigned short              window;
            unsigned short              checksum;
            unsigned short              pointeur;
            };
*/


// **********************************
// Nouvelle structure TCP
// **********************************
struct tcp  // htons pour les ushort et htonl pour les ulong
            { 
            unsigned short              port_source;
      unsigned short                    port_destination;
            unsigned long               sequence;
            unsigned long               accuse;

            unsigned short              reserved:4;       // Reserved est divisé en deux afin d'être directement intégré à la bonne place
            unsigned short              offset:4;
            unsigned short              flag_fin:1;         // L'ordre des champs sont inversé
            unsigned short              flag_syn:1;        // L'ordre des champs sont inversé
            unsigned short              flag_rst:1;         // L'ordre des champs sont inversé
            unsigned short              flag_psh:1;       // L'ordre des champs sont inversé
            unsigned short              flag_ack:1;       // L'ordre des champs sont inversé
            unsigned short              flag_urg:1;        // L'ordre des champs sont inversé
            unsigned short              reserved2:2;

            unsigned short              window;
            unsigned short              checksum;
            unsigned short              pointeur;
            };


struct udp // htons pour les ushort et htonl pour les ulong
            {
            unsigned short              port_source;
            unsigned short              port_destination;
            unsigned short              length;
            unsigned short              checksum;
            };

struct pseudo_entete 
            {  
            unsigned long               ip_source;                               // Adresse ip source
            unsigned long               ip_destination;              // Adresse ip destination
            char                                         mbz;                                        // Champs à 0
            char                                         type;                                        // Type de protocole (6->TCP et 17->UDP)
            unsigned short              length;                          // ( Taille de l'entete Pseudo + Entete TCP ou UDP + Data )
    };
