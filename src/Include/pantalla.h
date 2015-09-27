
/*-----------------------------------------------------------
 		
	Modulo Principal: 	main.CPP
	Fichero: 			pantalla.cpp
	Clase:	  Pantalla

	Fecha:					6/10/08
	Autor:					Juan Arias Masa

	Plataforma: 			Visual C++ 6.0

	Ultima Modificaci¢n: 	??/?/2008
	
	  version:				   Beta 0.1   versión a

	Descripci¢n:
		Implementa las funciones básicas para el manejo de la pantalla
		tanto la entrada como la salida de informacion.

 -------------------------------------------------------------*/

#include <windows.h>
#include <process.h>

// Definicion de atributos de colores usados para el interface
#define GRIS				 8
#define AZUL				 9
#define VERDE				10
#define CYAN				11
#define ROJO				12
#define MAGENTA				13
#define AMARILLO			14
#define BLANCO				15

#ifndef ___Pantalla_h
#define ___Pantalla_h

class Pantalla   {
public:
	//Constructor:
//	Pantalla();
	//Destructor:
//	~Pantalla();
	
/*-----------------------------------------------------------------
	Funcion: TituloVentana
	Descripci¢n:
	   Establece un titulo para la ventana	de la consola Win32 
	Parámetros:
		s: Cadena con el texto del título
	Return:----
*/
void TituloVentana(char *);



/*-----------------------------------------------------------------
  Funcion:		PonCursor()
  Descripci¢n:		Activa o desactiva el cursor de texto 
  Parametros:	CursorActivo . Si es true activa el cursor y si es false lo desactiva
 		   
  Retorna:		----
*/
void PonCursor(bool);



/*---------------------------------------------------------------------------------*\
  Funcion:		EscribeCadena()
  Descripci¢n:		Muestra en pantalla la cadena pasada como parametro, en las 
				coordenadas especificadas y con el atributo especificado
  Parametros: 
 				X: Coordenada Horizontal del Cursor
 				Y: Coordenada Vertical del Cursor
  Comentario: 
 				No varia la posicion del cursor ni el atributo que estuvieran 
 				establecidos antes de la llamada a la funcion
 			   
  Retorna:		------	
 *---------------------------------------------------------------------------------*/
void EscribeCadena(char *s, short int X,short int Y, short int Atributo);


/*---------------------------------------------------------------------------------*\
  Funcion:	GotoXY()
  Descripci¢n: Establece coordenadas de la consola en las que situa el cursor
  Parametros: 
 			X: Coordenada Horizontal del Cursor
 			Y: Coordenada Vertical del Cursor
 			   
  Retorna: Ninguno		
 *---------------------------------------------------------------------------------*/
void GotoXY (short int X, short int Y);



/*---------------------------------------------------------------------------------*\
  Funcion:		SetColor()
  Descripci¢n:	Establece el atributo de color con el que se muestran los caracteres 
  Parametros:	Atributo de Color (Entero)
Ejemplos:
#define BLANCO_AZUL			31
#define BLANCO_VERDE		 63
#define BLANCO_NEGRO		 15
#define AZUL_BLANCO			121
#define	AMARILLO_AZUL		 30
#define	AMARILLO_VERDE		 62
#define	GRIS_AZUL			 23
#define NEGRO_BLANCO		112 	
#define NORMAL				7
	   

  Retorna:		Ninguno		
 *---------------------------------------------------------------------------------*/
void SetColor(short int Atributo);




/*---------------------------------------------------------------------------------*\
  Funcion:		Scroll()
  Descripci¢n:		Mueve una porcion de pantalla a otra posicion
  Parametros: 
 				EsquinaSuperior: Esquina superior izda de la zona a mover	
 				EsquinaInferior: Esquina inferior dcha de la zona a mover	
				NuevaCoordenada: Esquina superio derecha de la zona donde se mueva
   			   
  Retorna:		Ninguno	
 *---------------------------------------------------------------------------------*/

void Scroll(COORD EsquinaSuperior, COORD EsquinaInferior, COORD NuevaCoordenada);




/*---------------------------------------------------------------------------------*\
  Funcion:		GetText()
  Descripci¢n:		Almacena en un buffer los caracteres y atributos de una zona de
                pantalla
  Parametros: 
 				shX1: Columna superior izda de la zona a guardar	
 				shY1: fila superior izda de la zona a guardar	
 				shX1: Columna inferior dcha de la zona a guardar	
 				shX1: fila inferior dcha de la zona a guardar	
 				
  Retorna:		Puntero al buffer que almacena la zona de pantalla
 *---------------------------------------------------------------------------------*/

CHAR_INFO *GetText(short int shX1, short int shY1, short int shX2, short int shY2);




/*---------------------------------------------------------------------------------*\
  Funcion:		PutText()
  Descripci¢n:		Coloca en pantalla elconjunto de caracteres y atributos contenidos
                en un buffer
  Parametros: 
 				shX1: Columna superior izda de la zona donde se muestra el buffer	
 				shY1: fila superior izda de la zona a donde se muestra el buffer	
 				shX1: Columna inferior dcha de la zona donde se muestra el buffer	
 				shX1: fila inferior dcha de la zona donde se muestra el buffer
				Buffer:Contiene los caracteres y atributos a mostrar
 				
  Retorna:		Puntero al buffer que almacena la zona de pantalla
 *---------------------------------------------------------------------------------*/
void PutText(short int shX1, short int shY1, short int shX2, short int shY2,CHAR_INFO *Buffer);



/*-------------------------------------------------------------------------------------------*

   Funcion:		Caja()
   Parametros:	
				shY1,shX1:Coordenadas de la esquina superior izquierda.            
				shY2,shX2:Coordenadas de la esquina inferior derecha.              
				cMarco:Cadena de 9 caracteres que identfica el aspecto de la caja.
					   El caracter numero 9 especifica el caracter de relleno de la caja. 		
				shColoBox: Atributo de color para la caja.                         

   Descripci¢n:	Dibuja una caja en pantalla, utilizando las coordenadas pasadas
				como esquinas de la caja, para el dibujo utiliza los caracteres
				pasados.
   Retorna:		Ninguno

 *-------------------------------------------------------------------------------------------*/

void Caja(short int shX1,short int shY1,short int shX2,short int shY2,char *cMarco,short int shColoBox);

	
private:



} ;

#endif /* fin pantalla.h */


			  
