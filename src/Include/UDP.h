// UDP.h: interface for the UDP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDP_H__A9DE1FA2_225F_41A1_B630_EAED5BAE16B3__INCLUDED_)
#define AFX_UDP_H__A9DE1FA2_225F_41A1_B630_EAED5BAE16B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <stdio.h>
#include "DatagramaIP.h"
#include "JPD.H"

//Posiciones
#define		POS_UDP_PUERTO_ORIGEN	0
#define		POS_UDP_PUERTO_DESTINO	2
#define		POS_UDP_LONGITUD		4
#define		POS_UDP_CHECKSUM		6
#define		POS_UDP_DATOS			8


//Longitudes
#define		LON_UDP_PUERTO_ORIGEN	2
#define		LON_UDP_PUERTO_DESTINO	2
#define		LON_UDP_LONGITUD		2
#define		LON_UDP_CHECKSUM		2


#define		MAX_LON_DATOS_UDP		1472
#define		MAX_LON_UDP				1480
#define		MIN_LON_UDP				28

#define		PUERTO_ECHO				8


extern PacketDriver1 PD1;

/** @brief Representa un paquete UDP */
class UDP  
{
private:
	unsigned short int			_puertoorigen;
	unsigned short int			_puertodestino;
	unsigned short int			_longitud;
	unsigned short int			_checksum;
	unsigned char				_datos[MAX_LON_DATOS_UDP];
	unsigned char				_udp[MAX_LON_UDP];

public:
	/** @brief Construye el objeto mediante un array de bytes (trama) */
	UDP(unsigned char[MAX_LON_UDP]);
	/** @brief Construye el objeto mediante los parametros especificados */
	UDP(unsigned short int, unsigned short int, unsigned char[MAX_LON_DATOS_UDP]);
	virtual ~UDP();

	//Getters
	unsigned short int			getPuertoOrigen();
	unsigned short int			getPuertoDestino();
	unsigned short int			getLongitud();
	unsigned short int			getChecksum();
	unsigned char*				getDatos();

	//Setters
	void						setPuertoOrigen(unsigned short int);
	void						setPuertoDestino(unsigned short int);
	void						setDatos(unsigned char *);

	//Utilidades
	/** @brief Invierte una palabra de 16 bits */
	void InvierteWord(unsigned short int&);
	/** @brief Calcula el Checksum del paquete UDP */
	unsigned short int calcularChecksum(unsigned char[4], unsigned char[4]);
	/** @brief Convierte el objeto en un array de bytes (trama) listo para enviar */
	void toArray();
	/** @brief Muestra informacion por pantalla del paquete UDP */
	void mostrar();
	/** @brief Envia el paquete UDP */
	void enviar(unsigned char diripo[4], unsigned char diripd[4], unsigned char direth[6]);

};

#endif // !defined(AFX_UDP_H__A9DE1FA2_225F_41A1_B630_EAED5BAE16B3__INCLUDED_)
