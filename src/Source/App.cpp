// App.cpp: implementation of the App class.
//
//////////////////////////////////////////////////////////////////////

#include "App.h"
#include "pantalla.h"

extern Pantalla p;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

App::App(const char *filename)
{
	cargarFichero(filename);

	//Direccion de broadcast y valor inicial para destino
	for(int i=0; i<LON_DIR_ETH; i++ ) {
		dirEBroadcast[i]=0xff;
	}
	
	_secuencia = 0;
}

App::~App()
{

}

void App::cargarFichero(const char *filename) {
	FILE *f;
	char linea[200], *atributo, valor[200];

	f = fopen(filename, "r+");

	while(!feof(f)) {
		fgets(linea, 200, f);
		if(linea[0] != '#' && strlen(linea) > 1) {
			atributo = strtok(linea, "=");
			memcpy(valor, &linea[strlen(atributo)+1], 200-strlen(atributo)-2);

			if(memcmp(atributo, "IpLocal", 7) == 0) {
				_string2ip(dirIPEsta, valor);
			} else if(memcmp(atributo, "IpDestino", 9) == 0) {
				_string2ip(dirIPDestino, valor);
			} else if(memcmp(atributo, "IpRouter", 8) == 0) {
				_string2ip(dirIPGateway, valor);
			} else if(memcmp(atributo, "ModoRecepcion", 13) == 0) {
				modoRecepcion = atoi(valor);
			}
		}
	}

	fclose(f);

	//Direccion de broadcast y valor inicial para destino
	for(int i=0; i<LON_DIR_ETH; i++ ) {
		dirEEsta[i]=0;
		dirEDestino[i]=0;
		dirEGateway[i]=0;
	}

	memset(CacheARP, 0, 10); //Vaciamos la tabla cache

	//Añadir Ip Local y Ethernet Local a la cache ARP
	cache_index = 0;
	PD1.ObtenDirEstacion(dirEEsta);	
	addToCache(dirIPEsta, dirEEsta);
}

int App::inicializar() {
	char opcion;
	bool salir = false;
	
	//Añadir Ip Local y Ethernet Local a la cache ARP
	cache_index = 0;
	PD1.ObtenDirEstacion(dirEEsta);	
	addToCache(dirIPEsta, dirEEsta);

	infoConfig();

	//Puerto ECHO UDP
	_puertoecho = 8;


	do {
		p.SetColor(47);
		cprintf("\n****************************************");
		cprintf("\n*             MENU PRINCIPAL           *");
		cprintf("\n****************************************");
		p.SetColor(39);
		cprintf("\n* 1.   Analizar paquetes entrantes     *");
		cprintf("\n* 2.   Uso del protocolo ARP           *");
		cprintf("\n* 3.   Uso del protocolo ICMP          *");
		cprintf("\n* 4.   Uso del protocolo UDP           *");
		cprintf("\n* 5.   Mostrar info de direcciones     *");
		cprintf("\n* 6.   Volver a cargar el fichero      *");
		cprintf("\n* ESC. Salir                           *");
		p.SetColor(47);
		cprintf("\n****************************************");
		p.SetColor(15);
		cprintf("\n\n>> ");

		opcion=getche();

		switch(opcion) {
			case '1':
				mostrarAyuda();
				cprintf("\n\n\tPulse cualquier tecla para continuar...");
				getch();
				cprintf("\n\nEscuchando...\n");
				_analizar();
			break;
			case '2':
				_menuarp();
				_analizar();
			break;
			case '3':
				_menuicmp();
				_analizar();
			break;
			case '4':
				_menuudp();
				_analizar();
			break;
			case '5':
				infoConfig();
				cprintf("\n\n\tPulse cualquier tecla para continuar...");
				getch();
				cprintf("\n\n");
			break;
			case '6':
				cargarFichero("RC_CFG.txt");
			break;
			case TECLASALIDA:
				salir=true;
			break;
		}
	} while(!salir);


	

	return NO_ERROR;
}

void App::enviarPeticion(bool router)
{
	if(!router)
	{
		memset(dirEDestino, 0, LON_ARP_ETHERNETDESTINO);
		arp = new ARP(dirIPEsta, dirIPDestino, dirEDestino, PETICION);
		p.SetColor(3*16+15);
		cprintf("\n\n**** Enviando paquete ARP para el destino *****");
		p.SetColor(15);
		arp->mostrar();
		p.SetColor(3*16+15);
		cprintf("\n***********************************************\n\n");
		p.SetColor(15);

		arp->enviar(dirEBroadcast);
	} else {
		memset(dirEGateway, 0, LON_ARP_ETHERNETDESTINO);
		arp = new ARP(dirIPEsta, dirIPGateway, dirEGateway, PETICION);
		p.SetColor(3*16+15);
		cprintf("\n\n***** Enviando paquete ARP para el router *****");
		p.SetColor(15);
		arp->mostrar();
		p.SetColor(3*16+15);
		cprintf("\n***********************************************\n\n");
		p.SetColor(15);
		arp->enviar(dirEBroadcast);
	}

}

bool App::esperarRespuesta(int e) {
	TramaEthernet TE;
	TEthernet *ethernet;
	unsigned char aux[28];
	time_t inicio, fin;
	int maximo=0;
	
	inicio=time(NULL);
	do
	{
		if (PD1.HayTrama()) 
		{
			PD1.LeerTrama(TE);
			ethernet = new TEthernet(TE);
			
			if(ethernet->esARP()) {
				memcpy(aux,ethernet->getDatos(),28);
				arpentrada = new ARP(aux);
				if(esNuestro(e)) {
					p.SetColor(3*16+15);
					cprintf("\n\n-----------------------------------");
					cprintf("\n-      Respuesta ARP recibida     -");
					cprintf("\n-----------------------------------");
					p.SetColor(15);
					arpentrada->mostrar();
					p.SetColor(3*16+15);
					cprintf("\n-----------------------------------\n");
					p.SetColor(15);
					return true;
				}
			}
		}

		fin=time(NULL);
	}
	while(difftime(fin, inicio) <= 4);

	return false;
}

bool App::esNuestro(int e) {
	if(e==DESTINO) {
		if(arpentrada->getOp() == RESPUESTA && memcmp(arpentrada->getMacDestino(),dirEEsta,4)==0)
		{
			memcpy(dirEDestino,arpentrada->getMacOrigen(),6);
			addToCache(arpentrada->getIpOrigen(), arpentrada->getMacOrigen());
			return true;
		}
	} else if(e==GATEWAY) {
		if(arpentrada->getOp() == RESPUESTA && memcmp(arpentrada->getMacDestino(),dirEEsta,4)==0)
		{
			memcpy(dirEGateway,arpentrada->getMacOrigen(),6);
			addToCache(arpentrada->getIpOrigen(), arpentrada->getMacOrigen());
			return true;
		}
	}
	
	return false;
}

void App::procesarARP(ARP *parp){
	parp->mostrar();

	//Si recibimos un paquete ARP con destinatario "nosotros",
	//enviamos la respuesta.
	if(memcmp(&dirIPEsta,parp->getIpDestino(),LON_ARP_IPDESTINO)==0 && parp->getOp()==PETICION)
	{
		p.SetColor(3*16+15);
		cprintf("\n\n||||||||||||||||||||||||||||||||||||||||");
		cprintf("\n   Recibido paquete ARP para nosotros.\n\n   Respondemos:\n");
		p.SetColor(15);
		arp = new ARP(dirIPEsta, parp->getIpOrigen(), parp->getMacOrigen(), RESPUESTA);
		arp->mostrar();
		arp->enviar(parp->getMacOrigen());
		p.SetColor(3*16+15);
		cprintf("\n||||||||||||||||||||||||||||||||||||||||\n");
		p.SetColor(15);
	}	
}

void App::_string2ip(unsigned char *v, char *s) {
	char aux[3];

	for(int i=0,pos=0,c=0;i<strlen(s);i++) {
		if(s[i] != '.') {
			aux[pos] = s[i];
			pos++;
		} else {
			v[c] = atoi(aux);
			pos=0;
			c++;
			for(int h=0;h<3;h++) aux[h] = ' ';
		}
	}
	v[c] = atoi(aux);
}

void App::_analizar() {
	TramaEthernet TE;
	bool salir = false;
	TEthernet *ethernet;
	DatagramaIP *ip;
	ICMP *icmp;
	UDP *udp;
	unsigned char cabecera[28];
	unsigned char aux[20];
	unsigned char ipaux[4];
	unsigned int iipaux;

	while(!salir) {
		if (PD1.HayTrama()) {
			PD1.LeerTrama(TE);
			ethernet = new TEthernet(TE);
			
			//ethernet->mostrar();
			if(ethernet->esIP() == true) {
				ip = new DatagramaIP(ethernet->getDatos());
				
				iipaux = ip->getOrigen();
				memcpy(&ipaux, &iipaux, 4);
				addToCache(ipaux, ethernet->getOrigenEthernet());

				if(ip->getProtocolo() != pICMP && ip->getProtocolo() != pUDP) {
					p.SetColor(8);
					cprintf("\n\n************* Paquete IP **************");
					ip->mostrar();
					p.SetColor(15);
				}

				//Si el protocolo es ICMP y el paquete es para nosotros...
				iipaux = ip->getDestino();
				memcpy(&ipaux, &iipaux, 4);
				if(ip->getProtocolo() == pICMP && memcmp(&ipaux, dirIPEsta, 4) == 0) {
					p.SetColor(1*16+15);
					cprintf("\n\n************* Paquete IP **************");
					p.SetColor(15);
					ip->mostrar();
					p.SetColor(1*16+15);
					cprintf("\n\n---------- Paquete ICMP entrante ----------");
					p.SetColor(15);
					memcpy(aux, &ethernet->getDatos()[20], 20);
					icmp = new ICMP(aux);
					icmp->mostrar();
					//Contestamos si podemos
					iipaux = ip->getOrigen();
					memcpy(&ipaux, &iipaux, 4);
					if(memcmp(&ipaux, dirIPEsta, 4) != 0) {
						switch(icmp->getTipo()) {
							case 8: //Solicitud de ECO, respondemos con ECO (0)
								p.SetColor(1*16+15);
								cprintf("\n\nLa solicitud de ECO es para nosotros, contestamos:\n");
								p.SetColor(15);
								ICMP *icmp_salida = new ICMP(ICMP_RESPUESTA_ECO, 0, icmp->getId(), icmp->getSecuencia());
								icmp_salida->enviar(dirIPEsta, ipaux, ethernet->getOrigenEthernet(), 8);
								icmp_salida->mostrar();
							break;
						}
					}
					p.SetColor(1*16+15);
					cprintf("\n-------------------------------------------");
					cprintf("\n*******************************************\n\n");
					p.SetColor(15);
				} else if(ip->getProtocolo() == pICMP && memcmp(&ipaux, dirIPEsta, 4) != 0) {
					//Si el protocolo es ICMP y el paquete no es para nosotros...
					iipaux = ip->getOrigen();
					memcpy(&ipaux, &iipaux, 4);
					if(memcmp(&ipaux, dirIPEsta, 4) != 0) { //Si el paquete no lo estamos enviando nosotros y tampoco es para nosotros...
						p.SetColor(1*16+15);
						cprintf("\n\n************* Paquete IP **************");
						p.SetColor(15);
						ip->mostrar();
						//Simplemente mostrarlo
						p.SetColor(1*16+15);
						cprintf("\n\n---- Paquete ICMP cirulando (no nos incumbe) ------");
						p.SetColor(15);
						memcpy(aux, &ethernet->getDatos()[20], 20);
						icmp = new ICMP(aux);
						icmp->mostrar();
						p.SetColor(1*16+15);
						cprintf("\n-------------------------------------------");
						cprintf("\n*******************************************\n\n");
						p.SetColor(15);
					}
				} else if(ip->getProtocolo() == pUDP && memcmp(ipaux, dirIPEsta, 4) != 0) {
					unsigned char udpaux[MAX_LON_UDP];
					iipaux = ip->getOrigen();
					memcpy(&ipaux, &iipaux, 4);

					if(memcmp(&ipaux, dirIPEsta, 4) != 0) { //Si el paquete no lo estamos enviando nosotros y tampoco es para nosotros...
						p.SetColor(4*16+15);
						cprintf("\n\n************* Paquete IP **************");
						p.SetColor(15);
						ip->mostrar();
						p.SetColor(4*16+15);
						cprintf("\n\n--------------- Paquete UDP ---------------");
						p.SetColor(15);
						memcpy(udpaux, &ethernet->getDatos()[20], MAX_LON_DATOS_UDP);
						udp = new UDP(udpaux);
						udp->mostrar();
						p.SetColor(4*16+15);
						cprintf("\n-------------------------------------------");
						cprintf("\n*******************************************\n\n");
						p.SetColor(15);
					}
				} else if(ip->getProtocolo() == pUDP && memcmp(ipaux, dirIPEsta, 4) == 0) {
					unsigned char udpaux[MAX_LON_UDP];
					p.SetColor(4*16+15);
					cprintf("\n\n************* Paquete IP **************");
					p.SetColor(15);
					ip->mostrar();
					p.SetColor(4*16+15);
					cprintf("\n\n----------- Paquete UDP entrante ----------");
					p.SetColor(15);
					memcpy(udpaux, &ethernet->getDatos()[20], MAX_LON_DATOS_UDP);
					udp = new UDP(udpaux);
					udp->mostrar();
					p.SetColor(4*16+15);
					cprintf("\n-------------------------------------------");
					cprintf("\n*******************************************\n\n");
					p.SetColor(15);
				}

				if(ip->getProtocolo() != pICMP && ip->getProtocolo() != pUDP) {
					p.SetColor(8);
					cprintf("\n*******************************************\n\n");
					p.SetColor(15);
				}

			} else if(ethernet->esARP() == true) {
				memcpy(cabecera, ethernet->getDatos(), 28);
				arpentrada = new ARP(cabecera);
				p.SetColor(3*16+15);
				cprintf("\n\n************** Paquete ARP ***************");
				p.SetColor(15);
				procesarARP(arpentrada);
				addToCache(arpentrada->getIpOrigen(), arpentrada->getMacOrigen());
				addToCache(arpentrada->getIpDestino(), arpentrada->getMacDestino());
				p.SetColor(3*16+15);
				cprintf("\n******************************************\n\n");
				p.SetColor(15);
			}

		}

		if(kbhit()) {
			char tecla_aux = getch();
			switch(tecla_aux) {
				case TECLASALIDA:
					salir = true;
				break;
				case 'a':
				case 'A':
					infoConfig();
					cprintf("\n\n\tPulse cualquier tecla para continuar...");
					getch();
					cprintf("\n\n");
				break;
				case 'c':
				case 'C':
					mostrarCache();
					cprintf("\n\n\tPulse cualquier tecla para continuar...");
					getch();
					cprintf("\n\n");
				break;
				default:
					mostrarAyuda();
					cprintf("\t\tPulse cualquier tecla para continuar...");
					getch();
					cprintf("\n\n");
				break;
			}
		}
	  
	}
}

void App::_menuarp() {
	char tecla;
	bool salir = false;

	p.SetColor(3*16+15);
	cprintf("\n----------------------------------------");
	cprintf("\n-         USO DEL PROTOCOLO ARP        -");
	cprintf("\n----------------------------------------");
	p.SetColor(3*16+7);
	cprintf("\n- 1.   Enviar peticion al destino      -");
	cprintf("\n- 2.   Enviar peticion al router       -");
	cprintf("\n- 3.   Mostrar cache ARP               -");
	cprintf("\n- 4.   Mostrar info de direcciones     -");
	cprintf("\n- ESC. Volver al menu principal        -");
	p.SetColor(3*16+15);
	cprintf("\n----------------------------------------");
	p.SetColor(15);
	cprintf("\n\n>> ");

	do {
		tecla=getche();
		switch(tecla) {
			case '1':
				enviarPeticion(false);
				esperarRespuesta(DESTINO);
				infoConfig();
				mostrarCache();
				return;
			break;

			case '2':
				enviarPeticion(true);
				esperarRespuesta(GATEWAY);
				infoConfig();
				mostrarCache();
				return;
			break;

			case '3':
				mostrarCache();
				cprintf("\n\n\tPulse cualquier tecla para continuar...");
				getch();
				cprintf("\n\n");
				return;
			break;

			case '4':
				infoConfig();
				cprintf("\n\n\tPulse cualquier tecla para continuar...");
				getch();
				cprintf("\n\n");
				return;
			break;

			case TECLASALIDA:
				salir = true;
			break;
		}
	} while(!salir);
}

void App::_menuicmp() {
	unsigned char router[6], ipd[4];
	char tecla, target;
	ICMP *icmp;
	bool respuestaARP = false;

	p.SetColor(1*16+15);
	cprintf("\n----------------------------------------");
	cprintf("\n-        USO DEL PROTOCOLO ICMP        -");
	cprintf("\n----------------------------------------");
	p.SetColor(1*16+7);
	cprintf("\n- 1.   Enviar peticion de ECO          -");
	cprintf("\n- 2.   Enviar peticion de MASCARA      -");
	cprintf("\n- 3.   Enviar peticion de TIMESTAMP    -");
	cprintf("\n- ESC. Volver al menu principal        -");
	p.SetColor(1*16+15);
	cprintf("\n----------------------------------------");
	p.SetColor(15);
	cprintf("\n\n>> ");
	
	do {
		tecla = getch();
		if(tecla == 27) return;
	} while(tecla < '1' || tecla > '3');

	cprintf("\n\nEnviar a: (1) Al destino    (2) Al router   (3) Al destino a traves del router");
	cprintf("\n\n>> ");

	do {
		target = getch();
		if(target == 27) return;
	} while(target < '1' || target > '3');

	switch(target) {
		case '1':
			memcpy(router, dirEDestino, 6);
			memcpy(ipd, dirIPDestino, 4);
		break;
		case '2':
			memcpy(router, dirEGateway, 6);
			memcpy(ipd, dirIPGateway, 4);
		break;
		case '3':
			memcpy(router, dirEGateway, 6);
			memcpy(ipd, dirIPDestino, 4);
		break;
	}

	int r;

	//No sabemos la MAC en cuestion
	if(target == '3') { //Averiguar MAC del gateway
		r = isInCache(dirIPGateway);
	} else { //Del destino elegido
		r = isInCache(ipd);
	}

	if(r != -1) { //Si la MAC existe en cache...
		memcpy(router, CacheARP[r].ethernet, 6);
	} else {
		cprintf("\n\nAveriguando direccion Ethernet...\n\n");
		//Averiguar la MAC
		do {
			if(target=='1') {
				enviarPeticion(false);
				respuestaARP = esperarRespuesta(DESTINO);
			} else {
				enviarPeticion(true);
				respuestaARP = esperarRespuesta(GATEWAY);
			}
		} while(!respuestaARP);

		
		//Asignar direccon ethernet correspondiente, al destino ethernet (variable router)
		switch(target) {
			case '1':
				memcpy(router, dirEDestino, 6);
			break;
			case '2':
			case '3':
				memcpy(router, dirEGateway, 6);
			break;
		}
	}

	mostrarCache();

	switch(tecla) {
		case '1':
			p.SetColor(1*16+15);
			cprintf("\n\n*********************************");
			cprintf("\n  ENVIADA SOLICITUD ICMP DE ECO  ");
			cprintf("\n*********************************");
			p.SetColor(15);
			icmp = new ICMP(ICMP_SOLICITUD_ECO, 0, _secuencia, _secuencia);
			icmp->enviar(dirIPEsta, ipd, router, 8);
			_secuencia++;
			icmp->mostrar();
			p.SetColor(1*16+15);
			cprintf("\n********************************\n\n");
			p.SetColor(15);
		break;
		case '2':
			p.SetColor(1*16+15);
			cprintf("\n\n*********************************");
			cprintf("\nENVIADA SOLICITUD ICMP DE MASCARA");
			cprintf("\n*********************************");
			p.SetColor(15);
			icmp = new ICMP(ICMP_SOLICITUD_MASCARA, 0, _secuencia, _secuencia);
			icmp->setMascara(0);
			icmp->enviar(dirIPEsta, ipd, router, 12);
			_secuencia++;
			p.SetColor(1*16+15);
			cprintf("\n********************************\n\n");
			p.SetColor(15);
		break;
		case '3':
			p.SetColor(1*16+15);
			cprintf("\n\n***********************************");
			cprintf("\nENVIADA SOLICITUD ICMP DE TIMESTAMP");
			cprintf("\n***********************************");
			p.SetColor(15);
			icmp = new ICMP(ICMP_SOLICITUD_TIMESTAMP, 0, _secuencia, _secuencia);
			icmp->setTimestampOrigen(0);
			icmp->setTimestampRecepcion(0);
			icmp->setTimestampTransmision(0);
			icmp->enviar(dirIPEsta, ipd, router, 20);
			_secuencia++;
			p.SetColor(1*16+15);
			cprintf("\n**********************************\n\n");
			p.SetColor(15);
		break;
	}
}

void App::_menuudp() {
	char target;
	bool salir = false, respuestaARP;
	unsigned char ipd[4], router[6];
	unsigned char texto[MAX_LON_DATOS_UDP];
	unsigned short int londatos;
	UDP *udp;

	p.SetColor(4*16+15);
	cprintf("\n-----------------------------------------------------");
	cprintf("\n-               USO DEL PROTOCOLO UDP               -");
	cprintf("\n-----------------------------------------------------");
	p.SetColor(4*16+7);
	cprintf("\n- 1.   Enviar mensaje al destino                    -");
	cprintf("\n- 2.   Enviar mensaje al router                     -");
	cprintf("\n- 3.   Enviar mensaje al destino a traves de router -");
	cprintf("\n- 4.   Cambiar el numero del puerto de ECHO         -");
	cprintf("\n- 5.   Mostrar info de direcciones                  -");
	cprintf("\n- ESC. Volver al menu principal                     -");
	p.SetColor(4*16+15);
	cprintf("\n-----------------------------------------------------");
	p.SetColor(15);
	cprintf("\n\n>> ");
	
	do {
		target = getch();
		if(target == 27) return;
	} while(target < '1' || target > '5');

	if(target >= '1' && target <= '3') {
		cprintf("\n\nEscribe aqui el mensaje:\n------------------------\n\n");

		gets((char*)texto);

		londatos = strlen((const char*)texto);
	}


	switch(target) {
		case '1':
			memcpy(router, dirEDestino, 6);
			memcpy(ipd, dirIPDestino, 4);
		break;
		case '2':
			memcpy(router, dirEGateway, 6);
			memcpy(ipd, dirIPGateway, 4);
		break;
		case '3':
			memcpy(router, dirEGateway, 6);
			memcpy(ipd, dirIPDestino, 4);
		break;
		case '5':
			infoConfig();
			cprintf("\n\n\tPulsa cualquier tecla para continuar...\n");
			getch();
			cprintf("\nEscuchando...");
			return;
		break;
		case '4':
			unsigned char aux = _puertoecho, tmp = _puertoecho;
			cprintf("\n\nActualmente el numero del puerto de ECHO es %d.", _puertoecho);
			cprintf("\n\nCambiar por: ");
			scanf("%d", &aux);
			
			_puertoecho = aux;

			if(_puertoecho != tmp) {
				p.SetColor(10);
				cprintf("\n\nEl numero de puerto ha sido cambiado a %d.\n\n", _puertoecho);
				p.SetColor(15);
			} else {
				p.SetColor(12);
				cprintf("\n\nEl numero de puerto no se ha cambiado!\n\n");
				p.SetColor(15);
			}
			return;
		break;
	}

	int r;

	//No sabemos la MAC en cuestion
	if(target == '3') { //Averiguar MAC del gateway
		r = isInCache(dirIPGateway);
	} else { //Del destino elegido
		r = isInCache(ipd);
	}

	if(r != -1) { //Si la MAC existe en cache...
		memcpy(router, CacheARP[r].ethernet, 6);
	} else {
		cprintf("\n\nAveriguando direccion Ethernet...\n\n");
		//Averiguar la MAC
		do {
			if(target=='1') {
				enviarPeticion(false);
				respuestaARP = esperarRespuesta(DESTINO);
			} else {
				enviarPeticion(true);
				respuestaARP = esperarRespuesta(GATEWAY);
			}

		} while(!respuestaARP);

		
		//Asignar direccon ethernet correspondiente, al destino ethernet (variable router)
		switch(target) {
			case '1':
				memcpy(router, dirEDestino, 6);
			break;
			case '2':
			case '3':
				memcpy(router, dirEGateway, 6);
			break;
		}
	}

	mostrarCache();

	p.SetColor(4*16+15);
	cprintf("\n\n*********************************");
	cprintf("\n       ENVIADO PAQUETE UDP       ");
	cprintf("\n*********************************");
	p.SetColor(15);
	udp = new UDP(_puertoecho, _puertoecho, texto);
	udp->enviar(dirIPEsta, ipd, router);
	udp->mostrar();
	p.SetColor(4*16+15);
	cprintf("\n********************************\n\n");
	p.SetColor(15);
}

void App::addToCache(unsigned char ip[4], unsigned char eth[6]) {
	if(isInCache(ip) != -1 || cache_index > 9 || *eth==0) return;
	
	memcpy(&CacheARP[cache_index].ip, ip, 4);
	memcpy(&CacheARP[cache_index].ethernet, eth, 6);
	CacheARP[cache_index].tipo = MAPA_DINAMICO;
	cache_index++;
}

int App::isInCache(unsigned char ip[4]) {
	int resultado = -1;
	for(int i=0;i<cache_index && resultado==-1;i++) {
		if(memcmp(ip, CacheARP[i].ip, 4) == 0) resultado = i;
	}

	return resultado;
}

void App::mostrarCache() {
	int j;
	p.SetColor(14);
	cprintf("\n\n\toooooooooooooooooooooooooooooooooooooooooooo");
	cprintf("\n\to                 CACHE ARP                o");
	cprintf("\n\toooooooooooooooooooooooooooooooooooooooooooo");
	cprintf("\n\t\t   Ethernet\t\tIP         ");
	cprintf("\n\t--------------------------------------------");
	for(int i=0;i<cache_index;i++) {
		cprintf("\n\t     ");
		for(j=0;j<5;j++)
			cprintf("%.2X:", CacheARP[i].ethernet[j]);

		cprintf("%.2X\t    ", CacheARP[i].ethernet[5]);

		for(j=0;j<3;j++)
			cprintf("%d.", CacheARP[i].ip[j]);

		cprintf("%d\t    ", CacheARP[i].ip[3]);
	}
	cprintf("\n\t--------------------------------------------\n\n");
	p.SetColor(15);
}

void App::infoConfig() {
	p.SetColor(10);
	cprintf("\n\n\tInformacion:");
	cprintf("\n\t----------------");
	cprintf("\n\tLocal:   %d.%d.%d.%d (", dirIPEsta[0], dirIPEsta[1], dirIPEsta[2], dirIPEsta[3]);
	
	//Ethernet Local
	for(int j=0;j<5;j++)
			cprintf("%.2X:", dirEEsta[j]);

	cprintf("%.2X)    ", dirEEsta[5]);
	
	cprintf("\n\tDestino: %d.%d.%d.%d (", dirIPDestino[0], dirIPDestino[1], dirIPDestino[2], dirIPDestino[3]);

	//Ethernet Destino
	for(j=0;j<5;j++)
			cprintf("%.2X:", dirEDestino[j]);

	cprintf("%.2X)    ", dirEDestino[5]);

	cprintf("\n\tRouter:  %d.%d.%d.%d (", dirIPGateway[0], dirIPGateway[1], dirIPGateway[2], dirIPGateway[3]);

	//Ethernet Router
	for(j=0;j<5;j++)
			cprintf("%.2X:", dirEGateway[j]);

	cprintf("%.2X)    ", dirEGateway[5]);

	cprintf("\n\tModo Recepcion: ");

	switch(modoRecepcion) {
		case 0:
			cprintf("MODO SIN RECIBIR");
		break;
		case 6:
			cprintf("MODO ESPIA");
		break;
	}

	cprintf(" (%d)\n\n", modoRecepcion);
	p.SetColor(15);
}

void App::mostrarAyuda() {
	p.SetColor(11);
	cprintf("\n\n\t\t                     AYUDA");
	cprintf("\n\t\t------------------------------------------------------");
	cprintf("\n\t\t Teclas de control:\n");
	cprintf("\n\t\t   - A:   Muestra informacion sobre las direcciones");
	cprintf("\n\t\t          cargadas desde el fichero de configuracion.\n");
	cprintf("\n\t\t   - C:   Muestra la cache ARP.\n");
	cprintf("\n\t\t   - ESC: Vuelve al menu anterior.\n");
	cprintf("\n\t\t------------------------------------------------------\n");
	p.SetColor(15);
}