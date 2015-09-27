//////////////////////////////////////////////////////////////////////
//
// TEthernet.cpp: implementation of the TEthernet class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "TEthernet.h"
#include <conio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TEthernet::TEthernet()
{
	*_destinoeth= NULL;
	*_origeneth	= NULL;
	_tl			= 0;
	*_datos		= NULL;
	*_trama		= NULL;
}

TEthernet::TEthernet(unsigned char trama[MAX_TR_ETH])
{
	memcpy(&_destinoeth, &trama[0], LON_DIR_ETH);
	memcpy(&_origeneth, &trama[POS_DIR_OR], LON_DIR_ETH);
	memcpy(&_tl, &trama[POS_TIP_LON], 2);
	memcpy(&_datos, &trama[POS_DATOS], MAX_DATOS);
	memcpy(&_trama, &trama, MAX_TR_ETH);
}

TEthernet::~TEthernet()
{

}

//////////////////////////////////////////////////////////////////////
// Getters
//////////////////////////////////////////////////////////////////////

unsigned char* TEthernet::getDestinoEthernet() { return _destinoeth; }
unsigned char* TEthernet::getOrigenEthernet() { return _origeneth; }
unsigned short int TEthernet::getTipoLongitud() { return _tl; }
unsigned char* TEthernet::getDatos() { return _datos; }
unsigned char* TEthernet::getTrama() { return _trama; }
bool TEthernet::esIP() { return (_tl == 0x0008); }
bool TEthernet::esARP() { return (_tl == 0x0608); }

//////////////////////////////////////////////////////////////////////
// Setters
//////////////////////////////////////////////////////////////////////

void TEthernet::setDestinoEthernet(unsigned char valor[LON_DIR_ETH]) {
	memcpy(&_destinoeth, &valor, LON_DIR_ETH);
	_construirTrama();
}

void TEthernet::setOrigenEthernet(unsigned char valor[LON_DIR_ETH]) {
	memcpy(&_origeneth, &valor, LON_DIR_ETH);
	_construirTrama();
}

void TEthernet::setTipoLongitud(unsigned short int valor) {
	memcpy(&_tl, &valor, 2);
	_construirTrama();
}

void TEthernet::setDatos(unsigned char valor[MAX_DATOS]) {
	memcpy(&_datos, &valor, MAX_DATOS);
	_construirTrama();
}

void TEthernet::setTrama(unsigned char valor[MAX_TR_ETH]) {
	memcpy(&_trama, &valor, MAX_TR_ETH);
}

void TEthernet::_construirTrama() {
	memcpy(&_trama, &_destinoeth, LON_DIR_ETH);
	memcpy(&_trama[POS_DIR_OR], &_origeneth, LON_DIR_ETH);
	memcpy(&_trama[POS_TIP_LON], &_tl, 2);
	memcpy(&_trama[POS_DATOS], &_datos, MAX_DATOS);
}


//////////////////////////////////////////////////////////////////////
// Utilidades
//////////////////////////////////////////////////////////////////////

void TEthernet::mostrar() {
	cprintf("\nEthernet:");
	cprintf("\n---------");
	cprintf("\nMAC Destino: ");
	for (int i=0;i<LON_DIR_ETH;i++) {
		cprintf("%02X", _destinoeth[i]);
		if(i!=LON_DIR_ETH-1) cprintf(":");
	}
	cprintf("\nMAC Origen: ");
	for (i=0;i<LON_DIR_ETH;i++) {
		cprintf("%02X", _origeneth[i]);
		if(i!=LON_DIR_ETH-1) cprintf(":");
	}
}