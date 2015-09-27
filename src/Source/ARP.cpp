// ARP.cpp: implementation of the ARP class.
//
//////////////////////////////////////////////////////////////////////

#include "ARP.h"
#include "App.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ARP::ARP() {
	unsigned char vacio[28];

	memset(vacio,0,28);

	ARP::ARP(vacio);
}

ARP::ARP(unsigned char valor[28])
{
	unsigned short int aux;

	//Tipo de Hardware
	memcpy(&aux, &valor[POS_ARP_TIPOHARDWARE], 2);
	_tipohardware = aux;
	
	//Tipo de protocolo
	memcpy(&aux, &valor[POS_ARP_TIPOPROTOCOLO], 2);
	_tipoprotocolo = aux;
	
	//Tamaño de la direccion de hardware
	setTamHardware(valor[POS_ARP_TAMHARDWARE]);

	//Tamaño de protocolo
	setTamProtocolo(valor[POS_ARP_TAMPROTOCOLO]);

	//Codigo de operacion
	memcpy(&aux, &valor[POS_ARP_OP], 2);
	_op = aux;

	//Ethernet origen
	for(int b=0;b<LON_ARP_ETHERNETORIGEN;b++) _ethorigen[b] = valor[POS_ARP_ETHERNETORIGEN+b];

	//IP origen
	for(b=0;b<LON_ARP_IPORIGEN;b++) _iporigen[b] = valor[POS_ARP_IPORIGEN+b];

	//Ethernet destino
	for(b=0;b<LON_ARP_ETHERNETDESTINO;b++) _ethdestino[b] = valor[POS_ARP_ETHERNETDESTINO+b];

	//IP destino
	for(b=0;b<LON_ARP_IPDESTINO;b++) _ipdestino[b] = valor[POS_ARP_IPDESTINO+b];

	//Para inicializar _arp
	toArray();
}


ARP::ARP(unsigned char ipo[LON_ARP_IPORIGEN], unsigned char ipd[LON_ARP_IPDESTINO],unsigned char ethd[LON_ARP_ETHERNETDESTINO],int operacion) {
	unsigned short int aux;
	
	//Tipo de Hardware
	_tipohardware = 256;

	//Tipo de protocolo
	aux=0x8;
	memcpy(&_tipoprotocolo, &aux, 2);

	//Tamaño de la direccion hardware
	_tamhardware = LON_ARP_ETHERNETORIGEN;

	//Tamaño de protocolo
	_tamprotocolo = LON_ARP_IPORIGEN;

	//Codigo de operacion
	aux = operacion;
	memcpy(&_op, &aux, 2);
	
	//Direccion hardware origen
	PD1.ObtenDirEstacion(_ethorigen);

	//Direccion IP origen
	memcpy(_iporigen, ipo, LON_ARP_IPORIGEN);

	//Direccion hardware destino
	memcpy(_ethdestino, ethd, LON_ARP_ETHERNETDESTINO);

	//Direccion IP destino
	memcpy(_ipdestino, ipd, LON_ARP_IPDESTINO);

	//Para inicializar _arp
	toArray();
}


ARP::~ARP(){}

//Getters
unsigned short int ARP::getTipoHardware() { return _tipohardware; }
unsigned short int ARP::getTipoProtocolo() { return _tipoprotocolo; }
unsigned char ARP::getTamHardware() { return _tamhardware; }
unsigned char ARP::getTamProtocolo() { return _tamprotocolo; }
unsigned short int ARP::getOp() { return _op; }
unsigned char* ARP::getMacOrigen() { return _ethorigen; }
unsigned char* ARP::getIpOrigen() { return _iporigen; }
unsigned char* ARP::getMacDestino() { return _ethdestino; }
unsigned char* ARP::getIpDestino() { return _ipdestino; }

//Setters
void ARP::setTipoHardware(unsigned short int valor) { _tipohardware = valor; }
void ARP::setTipoProtocolo(unsigned short int valor) { _tipoprotocolo = valor; }
void ARP::setTamHardware(unsigned char valor) { _tamhardware = valor; }
void ARP::setTamProtocolo(unsigned char valor) { _tamprotocolo = valor; }
void ARP::setOp(unsigned short int valor) { _op = valor; }
void ARP::setMacOrigen(unsigned char valor[LON_ARP_ETHERNETORIGEN]) { memcpy(&_ethorigen, &valor, LON_ARP_ETHERNETORIGEN); }
void ARP::setIpOrigen(unsigned char valor[LON_ARP_IPORIGEN]) { memcpy(&_iporigen, &valor, LON_ARP_IPORIGEN); }
void ARP::setMacDestino(unsigned char valor[LON_ARP_ETHERNETDESTINO]) { memcpy(&_ethdestino, &valor, LON_ARP_ETHERNETDESTINO); }
void ARP::setIpDestino(unsigned char valor[LON_ARP_IPDESTINO]) { memcpy(&_ipdestino, &valor, LON_ARP_IPDESTINO); }

//Utilidades
unsigned char* ARP::toArray() {
	unsigned char salida[28];

	//Tipo de Hardware
	memcpy(salida, &_tipohardware, LON_ARP_TIPOHARDWARE);
	
	//Tipo de protocolo
	memcpy(&salida[POS_ARP_TIPOPROTOCOLO], &_tipoprotocolo, LON_ARP_TIPOPROTOCOLO);
	
	//Tamaño de la direccion de hardware
	salida[POS_ARP_TAMHARDWARE] = _tamhardware;

	//Tamaño de protocolo
	salida[POS_ARP_TAMPROTOCOLO] = _tamprotocolo;

	//Codigo de operacion
	memcpy(&salida[POS_ARP_OP], &_op, LON_ARP_OP);

	//Ethernet origen
	memcpy(&salida[POS_ARP_ETHERNETORIGEN], _ethorigen, LON_ARP_ETHERNETORIGEN);

	//IP origen
	memcpy(&salida[POS_ARP_IPORIGEN], _iporigen, LON_ARP_IPORIGEN);

	//Ethernet destino
	memcpy(&salida[POS_ARP_ETHERNETDESTINO], _ethdestino, LON_ARP_ETHERNETDESTINO);

	//IP destino
	memcpy(&salida[POS_ARP_IPDESTINO], _ipdestino, LON_ARP_IPDESTINO);

	memcpy(_arp, salida, 28);

	return _arp;
}

void ARP::InvierteWord(unsigned short int &a)
{
	unsigned short int aux;
	memcpy(&aux,&a,2);//Copiamos los 2 bytes en un auxiliar
	a=a>>8;//Desplazamos a a la dcha. para poner el byte de mayor peso en el lugar de menor peso
	aux=aux<<8;//Desplazamos aux a la izda. para poner el byte de menor peso en el lugar de mayor peso
	a=a|aux;
}

void ARP::enviar(unsigned char d[LON_ARP_ETHERNETDESTINO]) {
	toArray();
	PD1.PonDirDestino(d);
	PD1.EnviarTramaIP(_arp, MAX_DATOS, 0x806);
}

void ARP::mostrar() {
	unsigned short int aux;

	cprintf("\nTipo de hardware: %d", _tipohardware);

	aux = _tipoprotocolo;
	//InvierteWord(aux);
	cprintf("\nTipo de protocolo: 0x%X", aux);

	cprintf("\nTamaño de hardware: %d", _tamhardware);

	cprintf("\nTamaño de protocolo: %d", _tamprotocolo);

	aux = _op;
	InvierteWord(aux);
	cprintf("\nCodigo de operacion: 0x%X", aux);

	cprintf("\nMAC Origen: ");
	_mostrarMAC(_ethorigen);

	cprintf("\nMAC Destino: ");
	_mostrarMAC(_ethdestino);

	cprintf("\nIP Origen: ");
	_mostrarIP(_iporigen);

	cprintf("\nIP Destino: ");
	_mostrarIP(_ipdestino);
}

void ARP::_mostrarMAC(unsigned char eth[LON_ARP_ETHERNETORIGEN]) {
	for (int i=0;i<LON_ARP_ETHERNETORIGEN;i++) {
		cprintf("%02X", eth[i]);
		if(i!=LON_ARP_ETHERNETORIGEN-1) cprintf(":");
	}
}

void ARP::_mostrarIP(unsigned char ip[LON_ARP_IPORIGEN]) {
	for (int i=0;i<LON_ARP_IPORIGEN;i++) {
		cprintf("%d", ip[i]);
		if(i!=LON_ARP_IPORIGEN-1) cprintf(".");
	}
}
