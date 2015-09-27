// UDP.cpp: implementation of the UDP class.
//
//////////////////////////////////////////////////////////////////////

#include "UDP.h"
#include "App.h"
#include <conio.h>

unsigned char _puertoecho;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UDP::UDP(unsigned char paquete[MAX_LON_UDP]) {
	unsigned short int aux;

	memcpy(&_puertoorigen, &paquete[POS_UDP_PUERTO_ORIGEN], 2);
	InvierteWord(_puertoorigen);
	memcpy(&_puertodestino, &paquete[POS_UDP_PUERTO_DESTINO], 2);
	InvierteWord(_puertodestino);
	memcpy(&_longitud, &paquete[POS_UDP_LONGITUD], 2);
	memcpy(&_checksum, &paquete[POS_UDP_CHECKSUM], 2);

	InvierteWord(_longitud);
	aux = _longitud;
	memcpy(_datos, &paquete[POS_UDP_DATOS], aux - 8);

	if(aux-8 < MAX_LON_DATOS_UDP-1) {
		if((aux-8) % 2 == 0) {
			_datos[aux-8] = '\x0';
		} else {
			_datos[aux-8] = 0;
			_datos[aux-7] = '\x0';
		}
	}
}

UDP::UDP(unsigned short int po, unsigned short int pd, unsigned char datos[MAX_LON_DATOS_UDP]) {
	unsigned short int londatos = strlen((const char*)datos);

	_puertoorigen = po;
	_puertodestino = pd;
	_longitud = londatos+8;

	if(londatos < MAX_LON_DATOS_UDP-1) {
		if(londatos % 2 == 0) {
			datos[londatos] = '\x0';
		} else {
			datos[londatos] = 0;
			datos[londatos+1] = '\x0';
		}
	}

	memcpy(_datos, datos, _longitud);
}

UDP::~UDP() {}


//Getters
unsigned short int	UDP::getPuertoOrigen() { return _puertoorigen; }
unsigned short int	UDP::getPuertoDestino() { return _puertodestino; }
unsigned short int	UDP::getLongitud() { return _longitud; }
unsigned short int	UDP::getChecksum() { return _checksum; }
unsigned char*	UDP::getDatos() { return _datos; }

//Setters
void	UDP::setPuertoOrigen(unsigned short int valor) { _puertoorigen = valor; }
void	UDP::setPuertoDestino(unsigned short int valor) { _puertodestino = valor; }
void	UDP::setDatos(unsigned char *valor) {
	strcpy((char*)_datos, (const char*)valor);
}


//Utilidades
void UDP::InvierteWord(unsigned short int &a)
{
	unsigned short int aux;
	memcpy(&aux,&a,2);//Copiamos los 2 bytes en un auxiliar
	a=a>>8;//Desplazamos a a la dcha. para poner el byte de mayor peso en el lugar de menor peso
	aux=aux<<8;//Desplazamos aux a la izda. para poner el byte de menor peso en el lugar de mayor peso
	a=a|aux;
}


unsigned short int UDP::calcularChecksum(unsigned char ipo[4], unsigned char ipd[4]) {
	unsigned short int	word16;
	unsigned long		sum=0;
	unsigned short int	i;
	unsigned short int	buff[MAX_LON_DATOS_UDP];
	unsigned char aux[MAX_LON_DATOS_UDP+12];
	unsigned short int lon;

	lon = _longitud;
	
	//if(lon % 2 != 0) lon++;

	//Pseudocabecera
	memcpy(aux, ipo, 4);
	memcpy(&aux[4], ipd, 4);
	memset(&aux[8], 0, 1);
	memset(&aux[9], 17, 1);
	lon = lon >> 8;
	memcpy(&aux[10], &lon, 1);
	lon = _longitud & 0x00FF;
	memcpy(&aux[11], &lon, 1);
	//---------------

	lon = _longitud;
	if(lon % 2 != 0) lon++;

	toArray();
	memcpy(&aux[12], _udp, lon);

	//Ponemos checksum a 0 para calcular el nuevo
	memset(&aux[12+POS_UDP_CHECKSUM], 0, 2);

	memcpy(&buff,&aux,lon+12); //Copiamos el array auxiliar en un array unsigned short	
	
	//Calculamos el checksum
	for (i=0;i<(lon+12)/2;i=i+2){
		word16 =buff[i]+buff[i+1];
		sum = sum + (unsigned long) word16;	
	}

	while (sum>>16)
	  sum = (sum & 0xFFFF)+(sum >> 16);

	//Complemento a uno del resultado
	sum = ~sum;
	
	return (unsigned short int) sum;
}

void UDP::toArray() {
	unsigned short int po, pd, suma,lon;

	po = _puertoorigen;
	pd = _puertodestino;
	lon = _longitud;
	suma = _checksum;
	InvierteWord(po);
	InvierteWord(pd);
	InvierteWord(lon);

	memcpy(_udp, &po, 2);
	memcpy(&_udp[POS_UDP_PUERTO_DESTINO], &pd, 2);
	memcpy(&_udp[POS_UDP_LONGITUD], &lon, 2);
	memcpy(&_udp[POS_UDP_CHECKSUM], &suma, 2);
	if(_longitud % 2 == 0) {
		memcpy(&_udp[POS_UDP_DATOS], &_datos, _longitud);
	} else {
		memcpy(&_udp[POS_UDP_DATOS], &_datos, _longitud+1);
	}
}

void UDP::mostrar() {
	unsigned short int aux;

	cprintf("\n\nPuerto origen: %d", _puertoorigen);
	if(_puertoorigen == _puertoecho) {
		cprintf(" (ECHO)");
	}
	
	cprintf("\nPuerto destino: %d", _puertodestino);
	if(_puertodestino == _puertoecho) {
		cprintf(" (ECHO)");
	}

	cprintf("\nLongitud: %d", _longitud);
	aux = _checksum;
	InvierteWord(aux);
	cprintf("\nChecksum: %d | 0x%X", aux, aux);
	cprintf("\nDatos:\n%s", _datos);
}

void UDP::enviar(unsigned char diripo[4], unsigned char diripd[4], unsigned char direth[6]) {
	unsigned char aux[MAX_LON_UDP];
	unsigned short int lon;

	_checksum = calcularChecksum(diripo, diripd);

	lon = _longitud;
	
	DatagramaIP *ip = new DatagramaIP(20+lon, pUDP, diripo, diripd);
	ip->toArray(aux);

	toArray();

	memcpy(&aux[20], _udp, lon);

	PD1.PonDirDestino(direth);
	PD1.EnviarTramaIP(aux, MAX_DATOS, 0x800);
	ip->mostrar();
	cprintf("\nEthernet Destino: ");
	for(int j=0;j<5;j++)
		cprintf("%.2X:", direth[j]);

	cprintf("%.2X", direth[5]);
}