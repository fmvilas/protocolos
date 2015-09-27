// DatagramaIP.cpp: implementation of the DatagramaIP class.
//
//////////////////////////////////////////////////////////////////////

#include "DatagramaIP.h"
#include <stdio.h>
#include <conio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DatagramaIP::DatagramaIP()
{
	_version =
	_ihl =
	_tipo =
	_longitud =
	_id = _df =
	_mf =
	_tiempovida =
	_protocolo =
	_checksum =
	_origen =
	_destino = 0;
}

DatagramaIP::DatagramaIP(unsigned char valor[20])
{
	setDatagrama(valor);
}

DatagramaIP::DatagramaIP(unsigned short int lon, unsigned char protocolo, unsigned char origen[4], unsigned char destino[4]) {
	_version = 4;
	_ihl = 5;
	_tipo = 0;
	_longitud = lon;
	_id = 44321; //inventado
	_df = false;
	_mf = false;
	_tiempovida = 128;
	_protocolo = protocolo;
	memcpy(&_origen, origen, 4);
	memcpy(&_destino, destino, 4);
	_checksum = calcularChecksum();
	InvierteWord(_checksum);
}

DatagramaIP::~DatagramaIP() {}


//Getters
unsigned char DatagramaIP::getVersion() { return _version; }
unsigned char DatagramaIP::getIHL() { return _ihl; }
unsigned char DatagramaIP::getTipo() { return _tipo; }
unsigned short int DatagramaIP::getLongitudTotal() { return _longitud; }
unsigned short int DatagramaIP::getIdentificador() { return _id; }
bool DatagramaIP::getDF() { return _df; }
bool DatagramaIP::getMF() { return _mf; }
unsigned char DatagramaIP::getTiempoVida() { return _tiempovida; }
unsigned char DatagramaIP::getProtocolo() { return _protocolo; }
unsigned short int DatagramaIP::getChecksum() { return _checksum; }
unsigned int DatagramaIP::getOrigen() { return _origen; }
unsigned int DatagramaIP::getDestino() { return _destino; }


//Setters
void DatagramaIP::setVersion(unsigned char valor) { _version = valor; }
void DatagramaIP::setIHL(unsigned char valor) { _ihl = valor; }
void DatagramaIP::setTipo(unsigned char valor) { _tipo = valor; }
void DatagramaIP::setLongitudTotal(unsigned short int valor) { _longitud = valor; }
void DatagramaIP::setIdentificador(unsigned short int valor) { _id = valor; }
void DatagramaIP::setDF(bool valor) { _df = valor; }
void DatagramaIP::setMF(bool valor) { _mf = valor; }
void DatagramaIP::setTiempoVida(unsigned char valor) { _tiempovida = valor; }
void DatagramaIP::setProtocolo(unsigned char valor) { _protocolo = valor; }
void DatagramaIP::setOrigen(unsigned int valor) { _origen = valor; }
void DatagramaIP::setDestino(unsigned int valor) { _destino = valor; }

//Utilidades
void DatagramaIP::toArray(unsigned char *target) {
	unsigned char tmp[20];
	unsigned char aux;
	
	//Fila 1
	aux = _version << 4;
	tmp[0] = aux | _ihl;

	tmp[1] = _tipo;

	tmp[2] = (unsigned char)(_longitud >> 8);
	tmp[3] = (unsigned char)(_longitud & 0x00FF);
	
	//Fila 2
	tmp[4] = (unsigned char)(_id >> 8);
	tmp[5] = (unsigned char)(_id & 0x00FF);

	aux = (unsigned char)_df << 1;
	aux = aux | (unsigned char)_mf;
	tmp[6] = aux << 5;
	tmp[7] = 0;
	
	//Fila 3
	tmp[8] = _tiempovida;
	
	tmp[9] = _protocolo;

	tmp[10] = (unsigned char)(_checksum >> 8);
	tmp[11] = (unsigned char)(_checksum & 0x00FF);

	//Fila 4
	tmp[12] = (unsigned char)(_origen & 0x000000FF);
	tmp[13] = (unsigned char)((_origen >> 8) & 0x0000FF);
	tmp[14] = (unsigned char)((_origen >> 16) & 0x00FF);
	tmp[15] = (unsigned char)(_origen >> 24);
	
	//Fila 5
	tmp[16] = (unsigned char)(_destino & 0x000000FF);
	tmp[17] = (unsigned char)((_destino >> 8) & 0x0000FF);
	tmp[18] = (unsigned char)((_destino >> 16) & 0x00FF);
	tmp[19] = (unsigned char)(_destino >> 24);


	//Copiamos el array temporal en la variable
	//de destino (target)
	memcpy(target, tmp, 20);
}

void DatagramaIP::setDatagrama(unsigned char valor[20]) {
	//Fila 1
	_version = valor[0] >> 4;
	_ihl = valor[0] & 0x0F;

	_tipo = valor[1];

	_longitud = ((unsigned short int)(valor[2]) << 8) | valor[3];
	//InvierteWord(_longitud);

	//Fila 2
	_id = ((unsigned short int)(valor[4]) << 8) | valor[5];
	//InvierteWord(_id);
	_mf = (valor[6] >> 5) & 1;
	_df = (valor[6] >> 6) & 1;

	//Fila 3
	_tiempovida = valor[8];
	_protocolo = valor[9];
	_checksum = ((unsigned short int)(valor[10]) << 8) | valor[11];
	//InvierteWord(_checksum);
	
	//Fila 4
	unsigned int aux;

	aux = valor[15] << 8;
	aux = aux | valor[14];
	aux = (aux << 8) | valor[13];
	aux = (aux << 8) | valor[12];
	_origen = aux;

	//Fila 5
	aux = valor[19] << 8;
	aux = aux | valor[18];
	aux = (aux << 8) | valor[17];
	aux = (aux << 8) | valor[16];
	_destino = aux;
}

unsigned short int DatagramaIP::calcularChecksum()
{

	unsigned short int	word16;
	unsigned long		sum=0;
	unsigned short int	i;
	unsigned char		aux[20];
	unsigned short int	buff[10];

	toArray(aux); //Pasamos el objeto a un array auxiliar.

	memcpy(&buff,&aux,20); //Copiamos el array auxiliar en un array unsigned short

	buff[5] = 0; //Ponemos el checksum del paquete original a cero.
	
	//Calculamos el checksum
	for (i=0;i<getIHL()*2;i=i+2){
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

void DatagramaIP::InvierteWord(unsigned short int &a)
{
	unsigned short int aux;
	memcpy(&aux,&a,2);//Copiamos los 2 bytes en un auxiliar
	a=a>>8;//Desplazamos a a la dcha. para poner el byte de mayor peso en el lugar de menor peso
	aux=aux<<8;//Desplazamos aux a la izda. para poner el byte de menor peso en el lugar de mayor peso
	a=a|aux;
}

void DatagramaIP::mostrar() {
	unsigned short int aux;

	cprintf("\nDatagrama IP:");
	cprintf("\n-------------");
	cprintf("\nVersion: IPv%d", getVersion());
	cprintf("\nIHL (Longitud de cabecera): %d", getIHL());
	cprintf("\nTipo servicio: %d", getTipo());
	cprintf("\nLongitud total: %d", getLongitudTotal());
	cprintf("\nIdentificador: %d", getIdentificador());
	cprintf("\nFlag DF: %d", getDF());
	cprintf("\nFlag MF: %d", getMF());
	cprintf("\nTiempo de vida: %d", getTiempoVida());
	cprintf("\nProtocolo: ");


	switch(getProtocolo()) {
		case pICMP:
			cprintf("ICMP");
		break;
		case IGMP:
			cprintf("IGMP");
		break;
		case TCP:
			cprintf("TCP");
		break;
		case pUDP:
			cprintf("UDP");
		break;
		case IGRP:
			cprintf("IGRP");
		break;
	}

	cprintf("\nChecksum: %d | 0x%X", getChecksum(), getChecksum());
	cprintf("\nDireccion Origen: ");
	
	for(int i=0;i<3;i++) {
		cprintf("%d.", (getOrigen() >> (i*8)) & 0x000000FF);
	}

	cprintf("%d", getOrigen() >> 24);

	cprintf("\nDireccion Destino: ");
	
	for(i=0;i<3;i++) {
		cprintf("%d.", (getDestino() >> (i*8)) & 0x000000FF);
	}

	cprintf("%d", getDestino() >> 24);

	aux = calcularChecksum();
	InvierteWord(aux);
	cprintf("\nChecksum calculado: %d | 0x%X", aux, aux);
}