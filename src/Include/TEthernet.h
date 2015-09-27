// TEthernet.h: interface for the TEthernet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETHERNET_H__A44B75E3_6948_4DE0_AEE8_57CF2880D97F__INCLUDED_)
#define AFX_TETHERNET_H__A44B75E3_6948_4DE0_AEE8_57CF2880D97F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/************************************************************************************
 *  Definiciones de constantes y tipos
 ************************************************************************************/

#include <string.h>

//Posiciones de la trama Ethernet
#define POS_DIR_DES	0
#define	POS_DIR_OR	6
#define	POS_TIP_LON	12
#define	POS_DATOS	14


//Tipo de Trama Ethernet y direccion
#define	LON_DIR_ETH		6			//6 bytes de direccion Ethernet 
#define MAX_TR_ETH		1514		//1514 bytes es la longitud de la trama Ethernet que podemos controlar con software
#define MAX_DATOS		1500
#define LON_DATAGRAMA_IP	65353	//Tamaño máximo del paquete que proporciona IP

typedef unsigned char Trama[MAX_TR_ETH];

/***********************************************************************************/

/** @brief Representa una trama Ethernet */
class TEthernet  
{
protected:
	unsigned char		_destinoeth[LON_DIR_ETH];
	unsigned char		_origeneth[LON_DIR_ETH];
	unsigned short int	_tl;
	unsigned char		_datos[MAX_DATOS];
	unsigned char		_trama[MAX_TR_ETH];

	void				_construirTrama();

public:
	//Constructores/Destructores
	TEthernet();
	/** @brief Construye el objeto a partir de un array de bytes (trama) */
	TEthernet(unsigned char[MAX_TR_ETH]);
	virtual ~TEthernet();

	//Getters
	unsigned char*		getDestinoEthernet();
	unsigned char*		getOrigenEthernet();
	unsigned short int	getTipoLongitud();
	unsigned char*		getDatos();
	unsigned char*		getTrama();
	bool				esIP();
	bool				esARP();

	//Setters
	void				setDestinoEthernet(unsigned char[LON_DIR_ETH]);
	void				setOrigenEthernet(unsigned char[LON_DIR_ETH]);
	void				setTipoLongitud(unsigned short int);
	void				setDatos(unsigned char[MAX_DATOS]);
	void				setTrama(unsigned char[MAX_TR_ETH]);

	//Utilidades
	/** @brief Muestra informacion sobre la trama Ethernet */
	void				mostrar();
};

#endif // !defined(AFX_TETHERNET_H__A44B75E3_6948_4DE0_AEE8_57CF2880D97F__INCLUDED_)
