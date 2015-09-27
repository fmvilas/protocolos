// ICMP.cpp: implementation of the ICMP class.
//
//////////////////////////////////////////////////////////////////////

#include "ICMP.h"
#include "DatagramaIP.h"
#include <string.h>
#include <stdio.h>
#include <conio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ICMP::ICMP(unsigned char *cabeceraip) //Paquete entrante
{
	_tipo = cabeceraip[POS_ICMP_TIPO];
	_codigo = cabeceraip[POS_ICMP_CODIGO];
	memcpy(&_checksum, &cabeceraip[POS_ICMP_CHECKSUM], 2);
	memcpy(&_id, &cabeceraip[POS_ICMP_ID], 2);
	memcpy(&_secuencia, &cabeceraip[POS_ICMP_SECUENCIA], 2);
	
	switch(_tipo) {
		case 13: //Marca de tiempo
		case 14:
			memcpy(&_timestamporigen, &cabeceraip[POS_ICMP_TIMESTAMP_ORIGEN], 4);
			memcpy(&_timestamprecepcion, &cabeceraip[POS_ICMP_TIMESTAMP_RECEPCION], 4);
			memcpy(&_timestamptransmision, &cabeceraip[POS_ICMP_TIMESTAMP_TRANSMISION], 4);
		break;

		case 17: //Mascara de subred
		case 18:
			memcpy(&_mascara, &cabeceraip[POS_ICMP_MASCARA], 4);
		break;
	}
}

ICMP::ICMP(unsigned char tipo, unsigned char codigo, unsigned short int id, unsigned short int sec) //Crear paquete
{
	_tipo = tipo;
	_codigo = codigo;
	_id = id;
	_secuencia = sec;
	_checksum = calcularChecksum();
}

ICMP::ICMP() {}

ICMP::~ICMP() {}

//Getters
unsigned char ICMP::getTipo() { return _tipo; }
unsigned char ICMP::getCodigo() { return _codigo; }
unsigned short int ICMP::getChecksum() { return _checksum; }
unsigned short int ICMP::getId() { return _id; }
unsigned short int ICMP::getSecuencia() { return _secuencia; }
unsigned int ICMP::getMascara() { return _mascara; }
unsigned int ICMP::getTimestampOrigen() { return _timestamporigen; }
unsigned int ICMP::getTimestampRecepcion() { return _timestamprecepcion; }
unsigned int ICMP::getTimestampTransmision() { return _timestamptransmision; }

//Setters
void ICMP::setTipo(unsigned char valor) { _tipo = valor; _checksum = calcularChecksum();}
void ICMP::setCodigo(unsigned char valor) { _codigo = valor; _checksum = calcularChecksum(); }
void ICMP::setId(unsigned short int valor) { _id = valor; _checksum = calcularChecksum(); }
void ICMP::setSecuencia(unsigned short int valor) { _secuencia = valor; _checksum = calcularChecksum(); }
void ICMP::setMascara(unsigned int valor) { _mascara = valor; _checksum = calcularChecksum(); }
void ICMP::setTimestampOrigen(unsigned int valor) { _timestamporigen = valor; _checksum = calcularChecksum(); }
void ICMP::setTimestampRecepcion(unsigned int valor) { _timestamprecepcion = valor; _checksum = calcularChecksum(); }
void ICMP::setTimestampTransmision(unsigned int valor) { _timestamptransmision = valor; _checksum = calcularChecksum(); }

//Utilidades
unsigned char* ICMP::toArray() {
	unsigned char aux[20]; //20 es el maximo tamaño de ICMP para nuestra implementacion
	int longitud = 8;

	memcpy(&aux[POS_ICMP_TIPO], &_tipo, 1);
	memcpy(&aux[POS_ICMP_CODIGO], &_codigo, 1);
	memcpy(&aux[POS_ICMP_CHECKSUM], &_checksum, 2);
	memcpy(&aux[POS_ICMP_ID], &_id, 2);
	memcpy(&aux[POS_ICMP_SECUENCIA], &_secuencia, 2);
	
	switch(_tipo) {
		case 13: //Marca de tiempo
		case 14:
			memcpy(&aux[POS_ICMP_TIMESTAMP_ORIGEN], &_timestamporigen, 4);
			memcpy(&aux[POS_ICMP_TIMESTAMP_RECEPCION], &_timestamprecepcion, 4);
			memcpy(&aux[POS_ICMP_TIMESTAMP_TRANSMISION], &_timestamptransmision, 4);
			longitud+=12;
		break;

		case 17: //Mascara de subred
		case 18:
			memcpy(&aux[POS_ICMP_MASCARA], &_mascara, 4);
			longitud+=4;
		break;
	}

	memcpy(_icmp, aux, longitud);
	memset(&_icmp[longitud], '\x0', 1); //Ponemos el fin de cadena

	return _icmp;
}

unsigned short int ICMP::calcularChecksum()
{

	unsigned short int	word16;
	unsigned long		sum=0;
	unsigned short int	i;
	int					longitud=4;
	unsigned short int	buff[20];

	switch(_tipo) {
		case 13: //Marca de tiempo
		case 14:
			longitud+=6;
		break;

		case 17: //Mascara de subred
		case 18:
			longitud+=2;
		break;
	}
	
	toArray();
	memcpy(&buff, &_icmp, longitud*2);

	buff[1] = 0; //Ponemos el checksum del paquete original a cero.
	
	//Calculamos el checksum
	for (i=0;i<longitud;i=i+2){
		word16 =buff[i]+buff[i+1];
		//InvierteWord(word16);
		sum = sum + (unsigned long) word16;	
	}

	while (sum>>16)
	  sum = (sum & 0xFFFF)+(sum >> 16);


	//Complemento a uno del resultado
	sum = ~sum;
	
	return (unsigned short int) sum;
}

void ICMP::InvierteWord(unsigned short int &a)
{
	unsigned short int aux;
	memcpy(&aux,&a,2);//Copiamos los 2 bytes en un auxiliar
	a=a>>8;//Desplazamos a a la dcha. para poner el byte de mayor peso en el lugar de menor peso
	aux=aux<<8;//Desplazamos aux a la izda. para poner el byte de menor peso en el lugar de mayor peso
	a=a|aux;
}

void ICMP::mostrar() {
	unsigned short int aux;

	switch(_tipo) {
		case 0:
			cprintf("\n\nRESPUESTA DE ECO");
		break;
		case 8:
			cprintf("\n\nSOLICITUD DE ECO");
		break;

		case 13:
			cprintf("\n\nSOLICITUD DE TIMESTAMP");
		break;
		case 14:
			cprintf("\n\nRESPUESTA DE TIMESTAMP");
		break;

		case 17:
			cprintf("\n\nSOLICITUD DE MASCARA DE DIRECCION");
		break;
		case 18:
			cprintf("\n\nRESPUESTA DE MASCARA DE DIRECCION");
		break;
	}

	cprintf("\n\nTipo: %d", _tipo);
	cprintf("\nCodigo: %d", _codigo);
	cprintf("\nIdentificador: %d", _id);
	cprintf("\nSecuencia: %d", _secuencia);
	
	switch(_tipo) {
		case 13: //Marca de tiempo
		case 14:
			cprintf("\nMarca de tiempo Origen: %d", _timestamporigen);
			cprintf("\nMarca de tiempo Recepcion: %d", _timestamprecepcion);
			cprintf("\nMarca de tiempo Transmision: %d", _timestamptransmision);
		break;

		case 17: //Mascara de subred
		case 18:
			cprintf("\nMascara de subred: %d", _mascara);
		break;
	}

	aux = _checksum;
	InvierteWord(aux);
	cprintf("\nChecksum: %d | 0x%X", aux, aux);
	aux = calcularChecksum();
	InvierteWord(aux);
	cprintf("\nChecksum calculado: %d | 0x%X", aux, aux);
}

void ICMP::enviar(unsigned char diripo[4], unsigned char diripd[4], unsigned char direth[6], int lon) {
	unsigned char aux[40];
	DatagramaIP *ip = new DatagramaIP(20+lon, pICMP, diripo, diripd);
	ip->toArray(aux);

	memcpy(&aux[20], toArray(), lon);

	PD1.PonDirDestino(direth);
	PD1.EnviarTramaIP(aux, MAX_DATOS, 0x800);
	ip->mostrar();
	cprintf("\nEthernet Destino: ");
	for(int j=0;j<5;j++)
		cprintf("%.2X:", direth[j]);

	cprintf("%.2X", direth[5]);
}