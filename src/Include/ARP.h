// ARP.h: interface for the ARP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARP_H__9C8D8A54_FC4E_4FBB_B026_08C37D9B49B7__INCLUDED_)
#define AFX_ARP_H__9C8D8A54_FC4E_4FBB_B026_08C37D9B49B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "TEthernet.h"
#include "JPD.H"

/************************************************************************************
 *  Definiciones de constantes y tipos
 ************************************************************************************/

//Posiciones (en bytes)
#define POS_ARP_TIPOHARDWARE	0
#define POS_ARP_TIPOPROTOCOLO	2
#define POS_ARP_TAMHARDWARE		4
#define POS_ARP_TAMPROTOCOLO	5
#define POS_ARP_OP				6
#define POS_ARP_ETHERNETORIGEN	8
#define POS_ARP_IPORIGEN		14
#define POS_ARP_ETHERNETDESTINO	18
#define POS_ARP_IPDESTINO		24

//Tamaños de los campos (en bytes)
#define LON_ARP_TIPOHARDWARE	2
#define LON_ARP_TIPOPROTOCOLO	2
#define LON_ARP_TAMHARDWARE		1
#define LON_ARP_TAMPROTOCOLO	1
#define LON_ARP_OP				2
#define LON_ARP_ETHERNETORIGEN	6
#define LON_ARP_IPORIGEN		4
#define LON_ARP_ETHERNETDESTINO	6
#define LON_ARP_IPDESTINO		4

#define	MAPA_DINAMICO			3

//Códigos de operación (estan invertidos para
//no tener que hacer InvierteWord todo el rato)
#define	PETICION				0x0100
#define	RESPUESTA				0x0200

/************************************************************************************/

extern PacketDriver1 PD1;

/**
 @brief Clase que representa un paquete ARP
*/
class ARP  
{
private:
	unsigned short int	_tipohardware;
	unsigned short int	_tipoprotocolo;
	unsigned char		_tamhardware;
	unsigned char		_tamprotocolo;
	unsigned short int	_op;
	unsigned char		_ethorigen[LON_ARP_ETHERNETORIGEN];
	unsigned char		_iporigen[LON_ARP_IPORIGEN];
	unsigned char		_ethdestino[LON_ARP_ETHERNETDESTINO];
	unsigned char		_ipdestino[LON_ARP_IPDESTINO];
	unsigned char		_arp[28];
	
	void				_mostrarMAC(unsigned char[LON_ARP_ETHERNETORIGEN]);
	void				_mostrarIP(unsigned char[LON_ARP_IPORIGEN]);

public:
	ARP();
	/**
	 @brief Constructor que recibe un array (trama) y lo convierte en objeto ARP
	*/
	ARP(unsigned char[28]);
	/**
	 @brief Constructor parametrizado. Construye el objeto segun los parametros pasados.
	*/
	ARP(unsigned char ipo[LON_ARP_IPORIGEN], unsigned char[LON_ARP_IPDESTINO],unsigned char[LON_ARP_ETHERNETDESTINO],int operacion);
	virtual ~ARP();

	//Getters
	unsigned short int	getTipoHardware();
	unsigned short int	getTipoProtocolo();
	unsigned char		getTamHardware();
	unsigned char		getTamProtocolo();
	unsigned short int	getOp();
	unsigned char*		getMacOrigen();
	unsigned char*		getIpOrigen();
	unsigned char*		getMacDestino();
	unsigned char*		getIpDestino();

	//Setters
	void				setTipoHardware(unsigned short int);
	void				setTipoProtocolo(unsigned short int);
	void				setTamHardware(unsigned char);
	void				setTamProtocolo(unsigned char);
	void				setOp(unsigned short int);
	void				setMacOrigen(unsigned char[LON_ARP_ETHERNETORIGEN]);
	void				setIpOrigen(unsigned char[LON_ARP_IPORIGEN]);
	void				setMacDestino(unsigned char[LON_ARP_ETHERNETDESTINO]);
	void				setIpDestino(unsigned char[LON_ARP_IPDESTINO]);

	//Utilidades
	/** @brief Convierte dos bytes al tipo short int */
	unsigned short int	toShortInt(unsigned char, unsigned char);
	/** @brief Convierte el objeto en un array de bytes (trama) listo para enviar */
	unsigned char*		toArray();
	/** @brief Envia el paquete ARP */
	void				enviar(unsigned char[LON_ARP_IPDESTINO]);
	/** @brief Invierte una palabra de 16 bits */
	void				InvierteWord(unsigned short int &a);
	/** @brief Muestra informacion por pantalla del paquete ARP */
	void				mostrar();
};

#endif // !defined(AFX_ARP_H__9C8D8A54_FC4E_4FBB_B026_08C37D9B49B7__INCLUDED_)
