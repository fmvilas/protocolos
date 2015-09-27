#define	PACKET_VERSION					"W-1.0"

#include "JPD.H"          //Clase PacketDriver1
#include <stdio.h>
#include <conio.h>
#include "TEthernet.h"
#include "DatagramaIP.h"
#include "ARP.h"
#include "App.h"
#include "pantalla.h"
  
//Constantes generales
#define TECLASALIDA 27
#define REC_SIN_RECIBIR   0
#define REC_MODO_ESPIA    6
//Variable global para acceder al packet driver
PacketDriver1 PD1;

Pantalla p;

unsigned char dirEEsta[LON_ARP_ETHERNETORIGEN];

int Iniciar();
void AcercaDe();

void main ()
{
	p.SetColor(10);
	App *app = new App("RC_CFG.txt");
	
	if (PD1.IniciarAutomatico(app->modoRecepcion)!=NO_ERROR) {
		cprintf("Error. No hay packet driver instalado. Pulsa tecla para salir...\n");
		getch();
		return;
	}
	if (Iniciar()!=NO_ERROR) {
		cprintf("Error. Problemas localizando la tarjeta. Pulsa tecla para salir...\n");
		getch();
		return;
	}
	
	app->inicializar();
}



int Iniciar()
{

	char salida[TAM_CADENA];
	
    PD1.MostrarInformacionDelDriver(salida);
	cputs(salida);
	cprintf("\n\n");
	AcercaDe();
	cprintf("\nPulsa cualquier tecla para continuar...\n");
	getch();
	return NO_ERROR;
}

void AcercaDe()
{
	cprintf("\n\n");
	/*cprintf("          *****   ******   ******\n");
	cprintf("         *     *  *     *  *     *\n");
	cprintf("         *******  ******   ******\n");
	cprintf("         *     *  *     *  *\n");
	cprintf("         *     *  *     *  *\n");*/
	cprintf("===========================================\n");
	cprintf("Autor: Francisco Mendez Vilas\n\n");
	cprintf("Redes de computadores\n");
	cprintf("Ingenieria Telematica\n");
	cprintf("Universidad de Extremadura\n");
	cprintf("===========================================\n");
}