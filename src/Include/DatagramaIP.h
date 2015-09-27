// DatagramaIP.h: interface for the DatagramaIP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAGRAMAIP_H__C6665BDD_01B8_4523_A448_396B9388F225__INCLUDED_)
#define AFX_DATAGRAMAIP_H__C6665BDD_01B8_4523_A448_396B9388F225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "TEthernet.h"

/************************************************************************************
 *  Definiciones de constantes y tipos
 ************************************************************************************/

#define		POS_IP_VERSION		0
#define		POS_IP_IHL			4
#define		POS_IP_TIPO			8
#define		POS_IP_LONG_TOTAL	16
#define		POS_IP_ID			32
#define		POS_IP_FLAGS		48
#define		POS_IP_DESPLAZ		51
#define		POS_IP_TIEMPOVIDA	64
#define		POS_IP_PROTOCOLO	72
#define		POS_IP_CHECKSUM		80
#define		POS_IP_DIR_ORIGEN	96
#define		POS_IP_DIR_DESTINO	128
#define		POS_IP_OPCIONES		160


#define		LON_IP_VERSION		4
#define		LON_IP_IHL			4
#define		LON_IP_TIPO			8
#define		LON_IP_LONG_TOTAL	16
#define		LON_IP_ID			16
#define		LON_IP_FLAGS		3
#define		LON_IP_DESPLAZ		13
#define		LON_IP_TIEMPOVIDA	8
#define		LON_IP_PROTOCOLO	8
#define		LON_IP_CHECKSUM		16
#define		LON_IP_DIR			32

#define		pICMP				1
#define		IGMP				2
#define		TCP					6
#define		pUDP				17
#define		IGRP				88

/************************************************************************************/

/** @brief Representa un datagrama IP */
class DatagramaIP
{
private:
	unsigned char		_version;
	unsigned char		_ihl;
	unsigned char		_tipo;
	unsigned short int	_longitud;
	unsigned short int	_id;
	bool				_df, _mf;
	unsigned char		_tiempovida;
	unsigned char		_protocolo;
	unsigned short int	_checksum;
	unsigned int		_origen;
	unsigned int		_destino;

public:
	//Constructores/Destructores
	DatagramaIP();
	/** @brief Construye el datagrama IP a partir de un array de bytes(trama) */
	DatagramaIP(unsigned char[20]);
	/** @brief Construye el datagrama IP a partir de los parametros especificados */
	DatagramaIP(unsigned short int, unsigned char, unsigned char[4], unsigned char[4]);
	virtual ~DatagramaIP();

	//Getters
	unsigned char		getVersion();
	unsigned char		getIHL();
	unsigned char		getTipo();
	unsigned short int	getLongitudTotal();
	unsigned short int	getIdentificador();
	bool				getDF();
	bool				getMF();
	unsigned char		getTiempoVida();
	unsigned char		getProtocolo();
	unsigned short int	getChecksum();
	unsigned int		getOrigen();
	unsigned int		getDestino();


	//Setters
	void				setVersion(unsigned char);
	void				setIHL(unsigned char);
	void				setTipo(unsigned char);
	void				setLongitudTotal(unsigned short int);
	void				setIdentificador(unsigned short int);
	void				setDF(bool);
	void				setMF(bool);
	void				setTiempoVida(unsigned char);
	void				setProtocolo(unsigned char);
	void				setOrigen(unsigned int);
	void				setDestino(unsigned int);

	//Utilidades
	/** @brief Convierte el objeto en un array de bytes (trama) listo para enviar */
	void				toArray(unsigned char *);
	/** @brief Esta funcion se utiliza en conjunto con el constructor mediante array.
	Inicializa el objeto extrayendo la informacion del array de bytes. */
	void				setDatagrama(unsigned char[20]);
	/** @brief Calcula el Checksum del datagrama IP */
	unsigned short int	calcularChecksum();
	/** @brief Invierte una palabra de 16 bits */
	void				InvierteWord(unsigned short int &a);
	/** @brief Muestra informacion por pantalla del datagrama IP */
	void				mostrar();
};

#endif // !defined(AFX_DATAGRAMAIP_H__C6665BDD_01B8_4523_A448_396B9388F225__INCLUDED_)
