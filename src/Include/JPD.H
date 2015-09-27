//
// Copyright (c) 2002 - 2003
//  Escuela Universitaria de Mérida. Universidad de Extremadura.
//  Todos los derechos reservados.
//	Clase:							PacketDriver
//	Modulo Principal: 				P2.C
//	Fichero: 						JPD.H
//	Fecha:							18/6/00
//
//	Ultima Modificaci¢n: 			16/10/04
//
//	Autores:
//		Francisco José Camberos Garrido
//		Juan Arias Masa
//
//	Versión:
#define	PKT_VERSION					"2.32b"  
//  ( Nota: Esta macro se encuentra repetida tanto en el fichero de )
//  ( cabecera como en el fuente para evitar hacer uso de ficheros  )
//  ( de cabecera y fuente de distintas versiones.                  )
//
//
//	Plataforma: 					Visual C++ 6.0
//
//	Descripci¢n:
//		Definici¢n de la clase PacketDriver, implementada en JPD.LIB
//      Este fichero y su asociado JPD.CPP (JPD.OBJ) ha sido revisados
//		para el programa AD1PKTWIN que implementa el protocolo AD1 sobre windows.

//
//	---------------------------------
//  - Funcionamiento de esta clase: -
//  ---------------------------------
//
//	Ahora mismo est  preparada para que se llame a la funcion:
//	  -IniciarAutomatico
//	y a continuaci¢n se pueden llamar bien a la funci¢n de enviar
//	tramas ethernet de difusi¢n (EnviarTramaDifusion) las veces que
//	se quiera o bien ala funci¢n de si hay nueva trama ethernet (HayTrama)
//	Si se quiere usar el EnviarTramaEstacionPar() debe haberse llamado
//	a la funci¢n PonDirDestino, para que se haya cargado la estaci¢n
//	destino de la trama ethernet que se mande enviar.
//
//
//


#ifndef __JPD_H
	#define __JPD_H

	#include <packet32.h>					// Necesario para "LPADAPTER", ...

  #include <queue>              // Necesario para "queue"
  using namespace std;          // Definición del espacio de nombres para el uso de la cola genérica de la STL

	//
	// Valor devuelto por las funciones de la librería cuando se produce un error
	//
	#define ERROR_PKT						-1

	//
	// Valor devuelto por las funciones de la librería en caso de funcionar correctamente
	//
	#define NO_ERROR						0L

	//
	// Tamaño máximo de todas las cadenas estáticas empleadas en esta librería.
	//
	#define TAM_CADENA					2048

	//
	// Tamaño máximo de una trama Ethernet
	//
	#define LONG_TRAMA_ETHERNET		1514

	//
	// Tipo predefinido para representar las tramas Ethernet
	//
	typedef BYTE TramaEthernet[LONG_TRAMA_ETHERNET];

	//
	// Tamaño de la dirección MAC para un dispositivo de tipo Ethernet
	//
	#define LONG_DIR_MAC_ETHERNET	6

	//
	// Lista con los tipos de adaptadores permitidos por esta librería
	// Se usa sólo en una de las funciones "IniciarAutomatico".
	//
	typedef enum _NDIS_MEDIUM
	{
		NdisMedium802_3,
		NdisMedium802_5,
		NdisMediumFddi,
		NdisMediumWan,
		NdisMediumLocalTalk,
		NdisMediumDix,		// definido por conveniencia, no es un tipo de medio real
		NdisMediumArcnetRaw,
	    NdisMediumArcnet878_2,
	    NdisMediumAtm,
	    NdisMediumWirelessWan,
	    NdisMediumIrda,
	    NdisMediumMax		// No es un medio real, definido como límite superior de esta enumeración
	} NDIS_MEDIUM, *PNDIS_MEDIUM;

	
   	class PacketDriver1
	{
    
	  public:
		BYTE		dirPropia[LONG_DIR_MAC_ETHERNET]; //dirección del adaptador

	  public:
			//-----------------------------------------------------------------
			//Constructor de la clase PacketDriver1----------------------------
			//
			//Descripci¢n: Inicializa algunos atributos de esta clase y obtiene
			//			   una lista con todos los adaptadores de red disponibles
			//			   en la máquina.
			//Argumentos:  -
			//
			//Retorno:     -
			//
			//Ver también: ~PacketDriver1();
			//
			//---------------------------------------------------------------------
			PacketDriver1();

			//-----------------------------------------------------------------
			//Destructor de la clase PacketDriver1----------------------------
			//
			//Descripci¢n: Limpia los atributos dinámicos de esta clase.
			//Argumentos:  -
			//
			//Retorno:     -
			//
			//Ver también: PacketDriver1();
			//
			//---------------------------------------------------------------------
			~PacketDriver1();

			//-----------------------------------------------------------------
			//Funcion IniciarAutomatico----------------------------------------
			//
			//Descripci¢n: Realiza la iniciacion del Packet Driver y lo asocia
			//			   de forma automática al primer dispositivo de tipo
			//			   Ethernet que encuentre. Para lo descrito
			//			   anteriormente hace uso de una serie de funciones
			//			   privadas de la clase.			
			//             Una vez que se ha llamado a esta rutina se pueden
			//             ejecutar las funciones:
			//                  HayNuevoTrama() y NuevoTrama()
			//             las veces que se quiera
			//
			//Argumentos:  ModoRecepcion
			//               Numero que indica los paquetes que vamos a recibir
			//               en funcion del tipo de direcci¢n de escucha seleccionada
			//
			//Retorno:     int
			//             	 NO_ERROR si todo va bien
			//               ERROR_PKT si hay algun problema
			//
			//NOTA:        -
			//
			//Ver también: TerminarAutomatico();
			//
			//---------------------------------------------------------------------
			int IniciarAutomatico(char ModoRecepcion);

			//-----------------------------------------------------------------
			//Funcion IniciarAutomatico----------------------------------------
			//
			//Descripci¢n: Realiza la iniciacion automática del Packet Driver.
			//             Para lo anterior hace uso de una serie de funciones
			//             privadas de la clase.
			//
			//			   Con esta función se asocia al dispositivo cuyo nombre
			//			   le pasamos por parámetro. El tipo de dispositivo
			//			   no tiene por qué ser necesariamente de tipo Ethernet.
			//
			//             Una vez que se ha llamado a esta rutina se pueden
			//             ejecutar las funciones:
			//                  HayNuevoTrama() y NuevoTrama()
			//             las veces que se quiera
			//
			//Argumentos:  ModoRecepcion
			//               Numero que indica los paquetes que vamos a recibir
			//               en funcion del tipo de direcci¢n de escucha seleccionada
			//			   numeroDispositivo
			//				 Posición que ocupa dentro de la lista de adaptadores
			//				 encontrados por esta librería.
			//
			//Retorno:     int
			//             	 NO_ERROR si todo va bien
			//               ERROR_PKT si hay algun problema
			//
			//NOTA:        Esta función es igual que la anterior, salvo que en
			//			   este caso se tiene que especificar el nombre del
			//			   dispositivo.
			//
			//Ver tambi‚n: TerminarAutomatico();
			//
			//---------------------------------------------------------------------
			int IniciarAutomatico(char ModoRecepcion, int numeroDispositivo);

			//-----------------------------------------------------------------
			//Funcion ObtenDirEstacion-----------------------------------------
			//
			//Descripci¢n: Escribe en el parametro dir, la direcci¢n de la estacion
			//				   donde se esta ejecutando este programa
			//
			//Argumentos:  dir
			//               -salida
			//               array de 6 BYTES
			//Retorno:     ---
			//
			//NOTA:
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			void ObtenDirEstacion (unsigned char *dir);


			//-----------------------------------------------------------------
			//Funcion EnviarTramaDifusion-------------------------------------
			//
			//Descripci¢n: Forma una trama Ethernet con los datos que vienen como
			//             parametro de entrada y la envia por la red. Como direcci¢n
			//             destino de la trama ir  0xffffffff.
			//             Si lon es mayor de 1500, se tom ran solo los primeros 1500
			//             bytes de la variable P.
			//
			//Argumentos:  P
			//              array de BYTES a enviar
			//             lon
			//              numero de BYTES que vienen en el array
			//
			//Retorno:     int
			//             	 NO_ERROR si todo va bien
			//               ERROR_PKT si hay algun problema
			//
			//NOTA:        Para que esta funci¢n funcione debe haberse llamado antes
			//             a la funci¢n IniciarAutomatico()
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int EnviarTramaDifusion(BYTE *P,int lon);

			//-----------------------------------------------------------------
			//Funcion EnviarTramaEstacionPar-----------------------------------
			//
			//Descripci¢n: Forma una trama Ethernet con los datos que vienen como
			//             parametro de entrada y la envia por la red. Como direcci¢n
			//             destino de la trama ir  la estaci¢n par que este seleccionada
			//             en el campo dirDestino del p.d.
			//             Si lon es mayor de 1500, se tom ran solo los primeros 1500
			//             bytes de la variable P.
			//
			//Argumentos:  P
			//              array de BYTES a enviar
			//             lon
			//              numero de BYTES que vienen en el array
			//
			//Retorno:     int
			//             	 NO_ERROR si todo va bien
			//               ERROR_PKT si hay algun problema
			//
			//NOTA:        Para que esta funci¢n funcione debe haberse llamado antes
			//             a la funci¢n IniciarAutomatico()
			//             e igualmente, a la funcion PonDirDestino()
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int EnviarTramaEstacionPar(BYTE *P,int lon);

			//-----------------------------------------------------------------
			//Funcion EnviarTramaIP---------------------------------------------
			//
			//Descripci¢n: Forma una trama Ethernet con los datos que vienen como
			//             parametro de entrada y la envia por la red. Como direcci¢n
			//             destino de la trama ir  la estaci¢n par que este seleccionada
			//             en el campo dirDestino del p.d.
			//             Si lon es mayor de 1500, se tom ran solo los primeros 1500
			//             bytes de la variable P.
			//
			//Argumentos:  P
			//              array de BYTES a enviar
			//             lon
			//              numero de BYTES que vienen en el array
			//             Protocolo
			//              protocolo IP que se va a enviar
			//
			//Retorno:     Entero
			//             	Valor negativo si hay algun error
			//              1 si todo va bien
			//
			//NOTA:        Para que esta funci¢n funcione debe haberse llamado antes
			//             a la funci¢n IniciarAutomatico()
			//             e igualmente, a la funcion PonDirDestino()
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int EnviarTramaIP (unsigned char *P,short int lon,short int protocolo);

			//-----------------------------------------------------------------
			//Funcion PonDirDestino---------------------------------------------
			//
			//Descripci¢n: Escribe en el campo dirDestino del packet driver la
			//             direccion destino de las tramas que se quieran enviar
			//
			//Argumentos:  dir
			//               array de 6 BYTES
			//Retorno:     ---
			//
			//NOTA:
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			void PonDirDestino(BYTE *dir);

			//-----------------------------------------------------------------
			//Funcion HayTrama-----------------------------------------------
			//
			//Descripci¢n: Esta funcion informa si hay un paquete disponible para
			//             su lectura. Se puede consultar las veces que se quiera
			//
			//Argumentos:  --
			//
			//Retorno:     Boolean
			//             	 TRUE si hay un paquete disponible.
			//               FALSE en caso contrario
			//
			//NOTA:
			//
			//Ver tambi‚n: NuevoTrama();
			//
			//---------------------------------------------------------------------
			boolean HayTrama ();

			//-----------------------------------------------------------------
			//Funcion LeerTrama-----------------------------------------------
			//
			//Descripci¢n: Esta funcion lee un un paquete de la red. No comprueba
			//             si ha llegado un nuevo paquete o no. Simplemente devuelve
			//             el £ltimo paquete.
			//
			//Argumentos:  array con el paquete ethernet que ha llegado
			//               Salida
			//
			//
			//Retorno:     int
			//             	 NO_ERROR si todo va bien
			//               ERROR_PKT si hay algun problema
			//
			//NOTA:
			//             Cambia el valor de LongTrama
			//
			//Ver tambi‚n: HayTrama();
			//
			//---------------------------------------------------------------------
			int LeerTrama(TramaEthernet &p);

			//---------------------------------------------------------------------
			//Funcion ObtenerNumAdaptadores----------------------------------------
			//
			//Descripci¢n: Obtiene el número de adaptadores "lógicos" de red que
			//			   han sido encontrados por el Packet Driver.
			//
			//Argumentos:  -
			//
			//
			//Retorno:     int
			//             	 Número de dispositivos "lógicos" de red que han sido
			//				   encontrados
			//
			//
			//Ver tambi‚n: ObtenerNombreAdaptador();
			//
			//---------------------------------------------------------------------
			int ObtenerNumAdaptadores();

			//Funcion ObtenerNombreAdaptador----------------------------------------
			//
			//Descripci¢n: Devuelve el nombre del adaptador "lógico" de red cuyo
			//			   índice se le pasa con el parámetro "numAdaptador".
			//
			//Argumentos:  numAdatador
			//             	 Número de dispositivos "lógicos" de red que han sido
			//				   encontrados.
			//			   nombreAdaptador
			//             	 Nombre del adaptador devuelto, que se corresponde
			//				  con el índice indicado por el parámetro anterior.
			//
			//Retorno:     int
			//             	 NO_ERROR si el índice pasado como parámetro corresponde
			//				   a algún dispositivo "lógico" de red.
			//               ERROR_PKT en caso contrario.
			//
			// NOTA:		Los dispositivos "lógicos" de red se numeran desde 0
			//				  hasta "n-1". Siendo "n" el número de adaptadores
			//				  "lógicos" devueltos por la función "ObtenerNumAdaptadores".
			//				El nombre del adaptador puede tener una longitud máxima
			//				  de 8192 caracteres.
			//			
			//Ver tambi‚n: ObtenerNumAdaptadores();
			//
			//---------------------------------------------------------------------
			int ObtenerNombreAdaptador(int numAdatador, char nombreAdaptador[TAM_CADENA]);

			//Funcion ObtenerUltimoError----------------------------------------
			//
			//Descripci¢n: Devuelve en el atributo ultimoError una descripción
			//			   del último error que se ha producido en esta libería.
			//
			//Argumentos:  ultimoError
			//             	 Descripción del error que se ha producido.
			//
			//Retorno:      -
			//
			// NOTA:		-
			//			
			//Ver tambi‚n:	-
			//
			//---------------------------------------------------------------------
			void ObtenerUltimoError(char *ultimoError);

			//-----------------------------------------------------------------
			//Funcion MostrarInformacionDelDriver------------------------------
			//
			//Descripci¢n: Escribe en pantalla la informaci¢n de la variable
			//             global PDE y de la versión de la librería.
			//
			//Argumentos:  informacion
			//				 Cadena de caracteres donde se va a escribir la
			//				 información referente a la librería.
			//
			//Retorno:     --
			//
			//
			//NOTA:
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			void MostrarInformacionDelDriver(char informacion[TAM_CADENA]);

			//-----------------------------------------------------------------
			//Funcion InvierteWORD-----------------------------------------------
			//
			//Descripci¢n: Invierte los BYTES de una palabra de dos BYTES, es decir
			//             si viene   0x45 0x67 devuelve 0x67 0x45
			//
			//Argumentos:  p  palabra a invertir
			//              Entrada/Salida
			//
			//Retorno:     --
			//
			//NOTA:
			//
			//
			//
			//---------------------------------------------------------------------
			void InvierteWord (unsigned int &p);

			void ObtenerVersion(char laVersion[TAM_CADENA]);

			int ObtenerTipoAdaptador(int numeroDispositivo);


		private:

			//-----------------------------------------------------------------
			//Funcion TerminarAutomatico---------------------------------------
			//
			//Descripci¢n: Realiza la terminaci¢n autom tica del Packet Driver
			//             para ello, es necesario que se haya llamado previamente
			//             a la funci¢n IniciarAutomatico.
			//			   Esta función no es pública, dado que se llama desde el 
			//             destructor de la clase.
			//
			//Argumentos:  --
			//
			//Retorno:     int
			//             	 NO_ERROR si el índice pasado como parámetro corresponde
			//				   a algún dispositivo "lógico" de red.
			//               ERROR_PKT en caso contrario.
			//
			//NOTA:
			//
			//Ver tambi‚n: IniciarAutomatico();
			//
			//---------------------------------------------------------------------
			int TerminarAutomatico();

			//-----------------------------------------------------------------
			//Funcion EnviaTrama---------------------------------------
			//
			//Descripci¢n: Pone la trama Ethernet que viene como par metro de entrada
			//             en la red.
			//
			//Argumentos:  buffer (trama ethernet a enviar)
			//               -Entrada
			//             lon
			//               -Entrada
			//               -Tama¤o de la trama que se envia
			//
			//Retorno:     Entero
			//               ERROR_PKT si no se puede enviar
			//               NO_ERROR si todo va bien
			//
			//NOTA:        Es imprescindible que el p.d. est‚ encontrado.
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int EnviaTrama(BYTE *buffer, unsigned lon);

			//-----------------------------------------------------------------
			//Funcion CierraPacketDriver---------------------------------------
			//
			//Descripci¢n: Cierra el p.d. para que lo pueda usar otro programa.
			//
			//            Termina las recepciones asociadas a un Handler.
			//            Con esta funci¢n terminamos la recepci¢n de un determinado
			//            tipo de paquetes que previamente se hab¡a establecido con
			//            access_type(). Como argumento se le debe pasar el handle
			//            asociado al tipo de paquetes que queremos dejar de procesar.
			//            Siempre que una aplicaci¢n active un handle, debe desactivarlo
			//            cuando haya terminado con ‚l.
			//
			//Argumentos:  --
			//
			//
			//Retorno:     Entero
			//              ERROR_PKT		si hay algun problema
			//              NO_ERROR si no hay problemas de cierre
			//
			//NOTA:
			//             Modifica la variable global de la clase PDE
			//             Modifica la variable global HandleEnUso
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int CierraPacketDriver();

			//-----------------------------------------------------------------
			//Funcion  ActivarRecepcion----------------------------------------
			//
			//Descripci¢n: Crea un hilo de ejecución encargado únicamente de
			//				ir almacenando las tramas recibidas en un buffer.
			//
			//Argumentos:  -
			//
			//
			//Retorno:     Entero
			//               ERROR_PKT si no se puede enviar
			//               NO_ERROR si todo va bien
			//
			//NOTA:       Modifica las variables globales:
			//                 PDE
			//                 HandleEnUso
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int ActivarRecepcion();

			//-----------------------------------------------------------------
			//Funcion  ActivarRecepcion----------------------------------------
			//
			//Descripci¢n: Termina el hilo de ejecución encargado de
			//				almacenar las tramas recibidas en el buffer.
			//
			//Argumentos:  --
			//
			//
			//Retorno:     --
			//
			//
			//NOTA:
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			void DesactivarRecepcion();

			//-----------------------------------------------------------------
			//Funcion  PonModoRecepcion----------------------------------------
			//
			//Descripci¢n: Coloca el modo de recepci¢n del interfaz. No todos los
			//             p.d. soportan todos los modos. Por defecto el p.d. se
			//             coloca en modo 3.
			//             Esta funci¢n pone el valor que haya en PDE.modorecpecion
			//
			//             En cualquier caso, los modos de recepci¢n son:
			//		 1 receptor inactivo. No recoge paquetes
			//		 2 s¢lo recibe paquetes dirigidos al interfaz
			//		 3 modo 2 y paquetes broadcast
			//       4 modo 3 y uso de direcciones multicast limitadas
			//		 5 modo 3 y uso de direcciones multicast ilimitadas
			//		 6 modo esp¡a. Recoge todos los paquetes
			//
			//Argumentos:
			//
			//
			//Retorno:     Entero
			//               ERROR_PKT si no se puede enviar
			//               NO_ERROR si todo va bien
			//
			//NOTA:
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int PonModoRecepcion(char modoRecepcion);

			//-----------------------------------------------------------------
			//Funcion  ObtenDireccion----------------------------------------
			//
			//Descripci¢n: Se encarga de copiar la direcci¢n de MAC que actualmente
			//             tiene el interfaz de red. La copia se realiza sobre un
			//             buffer que suministra el usuario. Tambi‚n se le indica el
			//             tama¤o de la direcci¢n MAC (en DIX Ethernet las direcciones
			//             MAC son de 6 octetos).
			//             El packet driver realiza la copia y nos indica el n£mero de
			//             bytes copiados. Si el packet driver nos devuelve un error
			//             NO_SPACE, entonces el tama¤o no es v lido
			//
			//Argumentos:  --
			//
			//
			//Retorno:     Entero
			//               ERROR_PKT si no se puede enviar
			//               NO_ERROR si todo va bien
			//
			//NOTA:       Modifica las variables globales:
			//                 PDE
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int ObtenDireccion();

			//-----------------------------------------------------------------
			//Funcion  ActivarRecepcion----------------------------------------
			//
			//Descripci¢n: Activa la recepcion de p.d. para ello obtiene un
			//             n£mero denominado LPAdapter y lo emplea para llamar
			//			   a la función "PacketOpenAdapter".
			//
			//Argumentos:  nombreDispositivo
			//				 Nombre del dispositivo desde el cual queremos
			//				 realizar la recepción y envío de paquetes.
			//
			//
			//Retorno:     --
			//
			//
			//NOTA:
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int AbrirPacketDriver(char *nombreDispositivo);

			//-------------------------------------------------------------------
			//Funci¢n  InicializarBufferRecepcion---------------------------------
			//
			//Descripci¢n: Configura el buffer donde se almacenan los paquetes
			//				capturados que todavía no han sido leidos.
			//
			//Argumentos:  --
			//
			//Retorno:     --
			//
			//
			//Retorno:     Entero
			//               ERROR_PKT si no se puede enviar
			//               NO_ERROR si todo va bien
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int InicializarBufferRecepcion();

			void LiberarBufferRecepcion();

			//-------------------------------------------------------------------
			//Funci¢n  Recibir----------------------------------------
			//
			//Descripci¢n: Llama a la funci¢n encargada de recibir los paquetes,
			//             Y en caso de que dicha funci¢n retorne, entonces
			//						 finaliza el hilo de ejecuci¢n actual.
			//
			//Argumentos:  pd
			//               Puntero a la instancia de la clase PacketDriver que
			//							 estamos usando.
			//
			//Retorno:     --
			//
			//
			//NOTA:       Es necesario que el argumento "pd" sea de tipo "void"
			//						para poder pasar el nombre de la funci¢n como argumento
			//						a las funciones declaradas en "PACKET32.H".
			//
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			friend void Recibir(void *pd);

			//-------------------------------------------------------------------
			//Funci¢n  RecibirTramas---------------------------------------------
			//
			//Descripci¢n: Funci¢n que contiene un bucle dentro del cual se
			//						 capturan todos los paquetes que recibe el dispositivo.
			//
			//Argumentos:  --
			//
			//Retorno:     --
			//
			//NOTA:
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			void RecibirTramas();

			//-------------------------------------------------------------------
			//Funci¢n  BuscarUnDispositivo-----------------------------------
			//
			//Descripci¢n: Busca un dispositivo del tipo especificado.
			//
			//Argumentos:  tipoBuscado
			//							 Identificador del tipo de dispositivo buscado
			//
			//
			//Retorno:     entero
			//               -1 si se ha producido alg£n error, en caso contrario devuelve
			//				  el número, dentro de la lista de adaptadores que corresponde
			//                al primer adaptador encontrado del tipo especificado.
			//
			//
			//NOTA:
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int BuscarUnDispositivo(int tipoBuscado);

			//-------------------------------------------------------------------
			//Funci¢n  PonerListaAdaptadores-----------------------------------
			//
			//Descripci¢n: Obtiene los nombres de los dispositivos de red
			//						 disponibles en esta estaci¢n y los almacena en una
			//						 variable interna de esta misma clase denominada
			//						 "listaAdaptadores".
			//
			//Argumentos:  --
			//
			//Retorno:     --
			//
			//
			//NOTA:
			//
			//Ver tambi‚n:
			//
			//---------------------------------------------------------------------
			int PonerListaAdaptadores();

			//-----------------------------------------------------------------
			//Funcion PintaDireccionEthernet-----------------------------------
			//
			//Descripci¢n: Pinta en pantalla la direcci¢n ethernet que venga como
			//             par metro de entrada.
			//
			//Argumentos:  array con la direcci¢n ethernet
			//              Entrada
			//              Direcci¢n Ethernet a pintar en pantalla
			//Retorno:     --
			//NOTA:
			//---------------------------------------------------------------------
			void PintaDireccionEthernet(BYTE dir[LONG_DIR_MAC_ETHERNET], char laDireccion[TAM_CADENA]);

			//
			// Número máximo de adaptadores lógicos de red permitidos por esta librería
			//
			#define MAX_NUM_ADAPTERS			10			
				
			//
			// Variable que contiene una lista con todos los adaptadores de red de
			// esta estaci¢n.
			//
			struct
			{
				int		AdapterNum;								// N£mero total de adaptadores detectados
				char	nombres[MAX_NUM_ADAPTERS][TAM_CADENA];	// Nombre de los adaptadores detectados
			} listaAdaptadores; // --struct--

			
			//
			// Tamaño del buffer de paquetes recibidos
			//
			#define PD_TAM_BUFFER				 512000

			//
			// Variable que contiene las variables necesarias para el manejo de
			// la librer¡a "PACKET32.H".
			//
			struct
			{
				LPADAPTER	lpAdapter;				// Handle del dispositivo
				BYTE		buffer[PD_TAM_BUFFER];	// Buffer de paquetes recibidos
							
				LPPACKET	Packet;					// Handle de la cola de paquetes recibidos
				char		modoRecepcion;			// Modo de recepci¢n: 1, 2, 3, 4, 5 o 6
				char		*nombreDispositivo;		//   nombre de la interfaz
				BYTE		dirOrigen[LONG_DIR_MAC_ETHERNET]; // Direcci¢n ethernet de la interfaz
				BYTE		dirDestino[LONG_DIR_MAC_ETHERNET]; // Direcci¢n MAC de la estaci¢n destino.
				BYTE		ptrTramaEnviar[LONG_TRAMA_ETHERNET]; // Trama a enviar a la estaci¢n destino.
				char		ultimoError[TAM_CADENA];
			} PDE; // --struct--

			queue<BYTE*>  colaTramas;
			
			//
			// Descripción del último error que se ha producido en el interior de
			// esta librería.
			//
			
			//
			// Variable que contiene las variables necesarias para conseguir la
			// concurrencia entre la recepci¢n de paquetes y el resto del programa.
			//
			struct
			{
				unsigned long handleThreadEscucha;		// Manejador del hilo encargado de la recepci¢n de paquetes.
				HANDLE  			mutexRecepcion,		// MUTEX empleado para implementar la exclusi¢n mutua
														//   con la recepci¢n de paquetes.
									mutexFin;			// MUTEX empleado para implementar la exclusi¢n mutua
														//   con la variable "finHilo".
				boolean				finHilo;			// Bandera booleana que indica si debe finalizar
														//   el hilo de recepci¢n o no.
			} hiloRecepcion;  // --struct--


	}; // Fin de la clase Packet Driver


#endif /*JPD*/


