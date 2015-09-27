// ICMP.h: interface for the ICMP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMP_H__FB3968FF_4E93_48C1_8AE8_C71E9272BA14__INCLUDED_)
#define AFX_ICMP_H__FB3968FF_4E93_48C1_8AE8_C71E9272BA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JPD.H"

//Posiciones
#define	POS_ICMP_TIPO					0
#define	POS_ICMP_CODIGO					1
#define	POS_ICMP_CHECKSUM				2
#define	POS_ICMP_ID						4
#define	POS_ICMP_NUMSEC					6

//ECO
#define	ICMP_SOLICITUD_ECO				8
#define	ICMP_RESPUESTA_ECO				0
#define LON_ICMP_ECO					8

//Mascara de Subred
#define	ICMP_SOLICITUD_MASCARA			17
#define	ICMP_RESPUESTA_MASCARA			18
#define POS_ICMP_MASCARA				8
#define	LON_ICMP_MASCARA				12

//Marca de Tiempo
#define	ICMP_SOLICITUD_TIMESTAMP		13
#define	ICMP_RESPUESTA_TIMESTAMP		14
#define POS_ICMP_TIMESTAMP_ORIGEN		8
#define POS_ICMP_TIMESTAMP_RECEPCION	12
#define	POS_ICMP_TIMESTAMP_TRANSMISION	16
#define LON_ICMP_TIMESTAMP				20

#define POS_ICMP_ID						4
#define POS_ICMP_SECUENCIA				6



#define MAX_LON_ICMP					1480 //Maxima longitud de un paquete ICMP (1500-20)

extern PacketDriver1 PD1;

/** @brief Representa un paquete ICMP */
class ICMP  
{
private:
	unsigned char			_tipo;
	unsigned char			_codigo;
	unsigned short int		_checksum;
	unsigned short int		_id;
	unsigned short int		_secuencia;
	unsigned int			_mascara;
	unsigned int			_timestamporigen;
	unsigned int			_timestamprecepcion;
	unsigned int			_timestamptransmision;

	unsigned char			_icmp[20];

public:
	ICMP();
	/** @brief Construye el objeto a partir de un array de bytes */
	ICMP(unsigned char*);
	/** @brief Construye el objeto a partir de los parametros especificados */
	ICMP(unsigned char, unsigned char, unsigned short int, unsigned short int);
	virtual ~ICMP();

	//Getters
	unsigned char			getTipo();
	unsigned char			getCodigo();
	unsigned short int		getChecksum();
	unsigned short int		getId();
	unsigned short int		getSecuencia();
	unsigned int			getMascara();
	unsigned int			getTimestampOrigen();
	unsigned int			getTimestampRecepcion();
	unsigned int			getTimestampTransmision();

	//Setters
	void					setTipo(unsigned char);
	void					setCodigo(unsigned char);
	void					setId(unsigned short int);
	void					setSecuencia(unsigned short int);
	void					setMascara(unsigned int);
	void					setTimestampOrigen(unsigned int);
	void					setTimestampRecepcion(unsigned int);
	void					setTimestampTransmision(unsigned int);

	//Utilidades
	/** @brief Convierte el objeto en un array de bytes (trama) listo para enviar */
	unsigned char*			toArray();
	/** @brief Muestra informacion por pantalla del paquete ICMP */
	void					mostrar();
	/** @brief Calcula el Checksum del paquete ICMP */
	unsigned short int		calcularChecksum();
	/** @brief Invierte una palabra de 16 bits */
	void					InvierteWord(unsigned short int &a);
	/** @brief Envia el paquete ICMP */
	void					enviar(unsigned char[4], unsigned char[4], unsigned char[6], int);

};

#endif // !defined(AFX_ICMP_H__FB3968FF_4E93_48C1_8AE8_C71E9272BA14__INCLUDED_)
