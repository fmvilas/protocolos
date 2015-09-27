// App.h: interface for the App class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APP_H__3794BDC8_5E05_406B_9E22_13643369FDDB__INCLUDED_)
#define AFX_APP_H__3794BDC8_5E05_406B_9E22_13643369FDDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ARP.h"
#include "ICMP.h"
#include "DatagramaIP.h"
#include "UDP.h"
#include "JPD.H"          //Clase PacketDriver1
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <string.h>

//Variable global para acceder al packet driver
extern PacketDriver1 PD1;

#define DESTINO 1
#define GATEWAY 2

#define TECLASALIDA 27

/**
 @brief Representa una sola fila de la tabla cache, para luego generar un array que constituye la tabla.
*/
struct FilaCacheARP {
	unsigned char ethernet[6];
	unsigned char ip[4];
	unsigned char tipo;
};

typedef struct FilaCacheARP TablaARP;

extern unsigned char _puertoecho;
/**
 @brief Controla el flujo de la aplicacion
*/
class App  
{
private:
	int					_secuencia;

	void				_string2ip(unsigned char *, char *);
	void				_menuarp();
	void				_analizar();
	void				_menuicmp();
	void				_menuudp();
public:
	unsigned char		dirIPEsta[LON_ARP_IPORIGEN];
	unsigned char		dirIPDestino[LON_ARP_IPDESTINO];
	unsigned char		dirIPGateway[LON_ARP_IPDESTINO];
	unsigned char		dirEDestino[LON_ARP_ETHERNETDESTINO];
	unsigned char		dirEGateway[LON_ARP_ETHERNETDESTINO];
	unsigned char		dirEBroadcast[LON_ARP_ETHERNETDESTINO];
	unsigned char		dirEEsta[LON_ARP_ETHERNETORIGEN];
	unsigned char		modoRecepcion;
	ARP					*arp;
	ARP					*arpentrada;
	TablaARP			CacheARP[10];
	int					cache_index;


	App(const char*);
	virtual ~App();
	
	/** @brief Envia una peticion ARP */
	void enviarPeticion(bool);
	/** @brief Espera una respuesta ARP */
	bool esperarRespuesta(int);
	/** @brief Comprueba que el paquete ARP es para nosotros */
	bool esNuestro(int);
	/** @brief Procesa el paquete ARP y lo muestra */
	void procesarARP(ARP*);
	/** @brief Carga la informacion desde el fichero de configuracion */
	void cargarFichero(const char*);
	/** @brief Inicializa la aplicacion */
	int inicializar();
	/** @brief A&ntilde;ade un par de direcciones IP y Ethernet a la tabla cache ARP */
	void addToCache(unsigned char ip[4], unsigned char eth[6]);
	/** @brief Comprueba si la IP especificada existe en la tabla cache. Si lo esta devuelve el indice, sino -1 */
	int isInCache(unsigned char ip[4]);
	/** @brief Muestra la tabla cache */
	void mostrarCache();
	/** @brief Muestra la informacion sobre las direcciones IP y Ethernet de la maquina origen, destino y el router */
	void infoConfig();
	/** @brief Muestra la ayuda sobre las teclas que se pueden pulsar durante el analisis de paquetes */
	void mostrarAyuda();
};

#endif // !defined(AFX_APP_H__3794BDC8_5E05_406B_9E22_13643369FDDB__INCLUDED_)
