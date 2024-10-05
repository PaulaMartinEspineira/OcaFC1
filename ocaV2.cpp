// Silvia Rodrigo Cañete
// Paula Martín Espiñeira
#include <iostream>
#include <cstdlib> //Para usar las funciones rand() y srand() que dan n aleatorios.
#include <ctime>   //Para usar la funcion time(NULL)
#include <sstream> //Para probar la practica.
#include <fstream> //Para poder usar el fichero.
#include <string>  //Por si acaso, lo pone en la presentación.
#include <iomanip> //Para usar las funciones de pintar el tablero.

using namespace std;

//*************************** CONSTANTES ***************************
const int NUM_CASILLAS = 63;
const int CASILLA_SALIDA = 1;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int RETROCESO_LABERINTO = 12;
const int MAX_JUGADORES = 4;
const string FICHERO = "TableroClasico63casillas.txt";
const bool MODO_DEBUG = true;
// Numero de filas a dibujar
const int NUM_FILAS_A_DIBUJAR = 3;

// *************************** VARIABLES DE TIPOS ***************************
struct tJugador
{
	int casilla;
	int turnos;
};

struct listaJugadores
{
	tJugador aJugadores[MAX_JUGADORES];
	int cont;
};

enum tCasilla
{
	NORMAL,
	OCA,
	PUENTE1,
	PUENTE2,
	POZO,
	POSADA,
	LABERINTO,
	DADO1,
	DADO2,
	CARCEL,
	CALAVERA
};

// Array tTablero para representar la secuencia de 63 casillas que componen el tablero.
using tTablero = tCasilla[NUM_CASILLAS + 1];

//*************************** Funciones para pintar el tablero ***************************
void pintaTablero(const tTablero tablero, listaJugadores const &casillasJ);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(listaJugadores const &casillasJ, int fila, int casillasPorFila);
string casillaAstring(tCasilla casilla);

string casillaAstring(tCasilla casilla)
{
	string cadena;
	switch (casilla)
	{
	case NORMAL:
		cadena = " ";
		break;
	case OCA:
		cadena = "OCA";
		break;
	case DADO1:
	case DADO2:
		cadena = "DADO";
		break;
	case PUENTE1:
	case PUENTE2:
		cadena = "PNTE";
		break;
	case POSADA:
		cadena = "PSDA";
		break;
	case CALAVERA:
		cadena = "MUER";
		break;
	case LABERINTO:
		cadena = "LBRN";
		break;
	case POZO:
		cadena = "POZO";
		break;
	case CARCEL:
		cadena = "CRCL";
		break;
	}
	return cadena;
}

void pintaTablero(const tTablero tablero, listaJugadores const &LJ)
{

	int casillasPorFila = NUM_CASILLAS / NUM_FILAS_A_DIBUJAR;
	cout << '\n';
	for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++)
	{
		pintaBorde(casillasPorFila);
		pintaNumCasilla(fila, casillasPorFila);
		pintaTipoCasilla(tablero, fila, casillasPorFila);
		pintaJugadores(LJ, fila, casillasPorFila);
	}
	pintaBorde(casillasPorFila);
	cout << '\n';
}

void pintaBorde(int casillasPorFila)
{

	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|====";

	cout << "|" << '\n';
}

void pintaNumCasilla(int fila, int casillasPorFila)
{

	for (int i = 1; i <= casillasPorFila; i++)
		cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";

	cout << "|" << '\n';
}

void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila)
{

	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i + fila * casillasPorFila]);

	cout << "|" << '\n';
}

void pintaJugadores(listaJugadores const &LJ, int fila, int casillasPorFila)
{
	int casilla;

	int blancos = MAX_JUGADORES - LJ.cont;
	string b = "";
	for (int i = 0; i < blancos; i++)
		b = b + " ";
	cout << "|";
	for (int i = 1; i <= casillasPorFila; i++)
	{
		casilla = i - 1 + fila * casillasPorFila;
		for (int jug = 0; jug < LJ.cont; jug++)
		{
			if (LJ.aJugadores[jug].casilla - 1 == casilla)
				cout << jug + 1;
			else
				cout << " ";
		}
		cout << b;
		cout << "|";
	}
	cout << '\n';
}

//*************************** Funciones que verifican si una casilla es de tipo especial ***************************
bool esOca(const tTablero tablero, int casilla)
{
	return tablero[casilla] == OCA;
}

bool esPuente(const tTablero tablero, int casilla)
{
	return (tablero[casilla] == PUENTE1 || tablero[casilla] == PUENTE2);
}

bool esDados(const tTablero tablero, int casilla)
{
	return (tablero[casilla] == DADO1 || tablero[casilla] == DADO2);
}

bool esLaberinto(const tTablero tablero, int casilla)
{
	return tablero[casilla] == LABERINTO;
}

bool esMuerte(const tTablero tablero, int casilla)
{
	return tablero[casilla] == CALAVERA;
}

bool esPosada(const tTablero tablero, int casilla)
{
	return tablero[casilla] == POSADA;
}

bool esPrision(const tTablero tablero, int casilla)
{
	return tablero[casilla] == CARCEL;
}

bool esPozo(const tTablero tablero, int casilla)
{
	return tablero[casilla] == POZO;
}

bool esMeta(int casilla)
{
	return casilla >= NUM_CASILLAS;
}

//*************************** Funciones que calculan casilla de destino ***************************
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int &posicion)
{
	posicion++;
	while (tablero[posicion] != tipo)
	{
		posicion++;
	}
}

void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int &posicion)
{
	posicion--;
	while (tablero[posicion] != tipo)
	{
		posicion--;
	}
}

int siguienteOca(const tTablero tablero, int casilla)
{
	cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA\n";
	// La función buscaCasillaAvanzando buscará la siguiente casilla del tipo 'oca'.
	buscaCasillaAvanzando(tablero, OCA, casilla);
	cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << casilla << "\n";
	cout << "VUELVES A TIRAR\n";
	cout << "CASILLA ACTUAL: " << casilla << "\n";
	return casilla;
}

int siguientePuente(const tTablero tablero, int casilla)
{
	cout << "DE PUENTE A PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE\n";
	if (tablero[casilla] == PUENTE1)
	{
		// La función buscaCasillaAvanzando buscará la siguiente casilla del tipo 'puente2'.
		buscaCasillaAvanzando(tablero, PUENTE2, casilla);
		cout << "SALTAS AL SIGUIENTE PUENTE EN LA CASILLA: " << casilla << "\n";
	}
	else if (tablero[casilla] == PUENTE2)
	{
		// La función buscaCasillaRetrocediendo buscará la anterior casilla del tipo 'puente1'.
		buscaCasillaRetrocediendo(tablero, PUENTE1, casilla);
		cout << "SALTAS AL SIGUIENTE PUENTE EN LA CASILLA: " << casilla << "\n";
	}
	cout << "VUELVES A TIRAR\n";
	cout << "CASILLA ACTUAL: " << casilla << "\n";
	return casilla;
}

int siguienteDado(const tTablero tablero, int casilla)
{
	cout << "DE DADOS A DADOS Y TIRO PORQUE ME HA TOCADO\n";
	if (tablero[casilla] == DADO1)
	{
		// La función buscaCasillaAvanzando buscará la siguiente casilla del tipo 'dado2'.
		buscaCasillaAvanzando(tablero, DADO2, casilla);
		cout << "SALTAS A LOS SIGUIENTES DADOS EN LA CASILLA: " << casilla << "\n";
	}
	else if (tablero[casilla] == DADO2)
	{
		// La función buscaCasillaRetrocediendo buscará la anterior casilla del tipo 'dado1'.
		buscaCasillaRetrocediendo(tablero, DADO1, casilla);
		cout << "SALTAS A LOS SIGUIENTES DADOS EN LA CASILLA: " << casilla << "\n";
	}
	cout << "VUELVES A TIRAR\n";
	cout << "CASILLA ACTUAL: " << casilla << "\n";
	return casilla;
}

int siguienteLaberinto(int casilla)
{
	// Si caes en el laberinto, retrocedes 12 casillas.
	casilla -= RETROCESO_LABERINTO;
	cout << "HAS CAIDO EN EL LABERINTO\n";
	cout << "RETROCEDES A LA CASILLA " << casilla << "\n";
	return casilla;
}

int siguienteMuerte()
{
	// La muerte implica retroceder a la casilla de salida(casilla 1).
	int casilla = CASILLA_SALIDA;
	cout << "HAS CAIDO EN LA MUERTE\n";
	cout << "RETROCEDES A LA CASILLA DE SALIDA: " << casilla << "\n";
	return casilla;
}

//*************************** Funciones que regulan el juego ***************************
int efectoTiradas(const tTablero tablero, int casilla)
{
	int nturnos = 0;
	// Dependiendo de la casilla especial, se añaden turnos extra o se pierden turnos, y el jugador queda penalizado.
	if (esOca(tablero, casilla) || esPuente(tablero, casilla) || esDados(tablero, casilla))
	{
		nturnos = 1;
	}
	else if (esPosada(tablero, casilla))
	{
		nturnos = -TURNOS_POSADA;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_POSADA << " TURNO SIN TIRAR\n";
	}
	else if (esPrision(tablero, casilla))
	{
		nturnos = -TURNOS_PRISION;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_PRISION << " TURNOS SIN TIRAR\n";
	}
	else if (esPozo(tablero, casilla))
	{
		nturnos = -TURNOS_POZO;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_POZO << " TURNOS SIN TIRAR\n";
	}
	return nturnos;
}

int efectoPosicion(const tTablero tablero, int casilla)
{
	// Se comprueba si se ha caído en una casilla especial.
	if (esOca(tablero, casilla))
	{
		casilla = siguienteOca(tablero, casilla);
	}
	else if (esPuente(tablero, casilla))
	{
		casilla = siguientePuente(tablero, casilla);
	}
	else if (esDados(tablero, casilla))
	{
		casilla = siguienteDado(tablero, casilla);
	}
	else if (esLaberinto(tablero, casilla))
	{
		casilla = siguienteLaberinto(casilla);
	}
	else if (esMuerte(tablero, casilla))
	{
		casilla = siguienteMuerte();
	}
	else if (esPosada(tablero, casilla))
	{
		cout << "HAS CAIDO EN LA POSADA\n";
	}
	else if (esPrision(tablero, casilla))
	{
		cout << "HAS CAIDO EN LA PRISION\n";
	}
	else if (esPozo(tablero, casilla))
	{
		cout << "HAS CAIDO EN EL POZO\n";
	}
	return casilla;
}

int tirarDadoManual(stringstream &ss) // Coge los valores del dado escritos en el main, dentro del string ss.
{
	int dado = 0;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	ss >> dado;
	cout << dado;
	cout << "\nVALOR DEL DADO: " << dado << "\n";
	return dado;
}

int tirarDado() // Devuelve un valor aleatorio del dado.
{
	return rand() % 6 + 1;
}

int quienEmpieza(listaJugadores &LJ) // Devuelve un número aleatorio para ver que jugador empieza.
{
	return rand() % LJ.cont + 1;
}

void iniciaJugadores(listaJugadores &LJ) // Inicializamos el array de los jugadores con los valores iniciales de turnos = 0 y casilla = 1.
{
	for (int i = 0; i < LJ.cont; i++)
	{
		LJ.aJugadores[i].casilla = CASILLA_SALIDA;
		LJ.aJugadores[i].turnos = 0;
	}
}

void iniciaTablero(tTablero tablero) // Inicializamos el array tablero y ponemos todas las casillas en NORMAL, menos la última.
{
	for (int i = 0; i < (NUM_CASILLAS - 1); i++)
	{
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS] = OCA;
}

tCasilla stringAtCasilla(string str) // Transforma un string en	un valor del tipo enumerado.
{
	if (str == "OCA")
	{
		return OCA;
	}
	else if (str == "PUENTE1")
	{
		return PUENTE1;
	}
	else if (str == "PUENTE2")
	{
		return PUENTE2;
	}
	else if (str == "POZO")
	{
		return POZO;
	}
	else if (str == "POSADA")
	{
		return POSADA;
	}
	else if (str == "LABERINTO")
	{
		return LABERINTO;
	}
	else if (str == "DADO1")
	{
		return DADO1;
	}
	else if (str == "DADO2")
	{
		return DADO2;
	}
	else if (str == "CARCEL")
	{
		return CARCEL;
	}
	else if (str == "CALAVERA")
	{
		return CALAVERA;
	}
	else
	{
		return NORMAL;
	}
}

bool cargaTablero(tTablero tablero) /*lee el fichero de texto que contiene la disposición de las casillas especiales(exceptuando la oca de la casilla NUM_CASILLAS) y va actualizando las posiciones correspondientes del tablero 'tablero',que se recibe inicializado.*/
{
	ifstream archivo;
	//Inicializamos la posicion a 1 en vez de a 0 para que entre en el bucle pues el centinela es 0.
	int posicion = 1;
	string str;
	archivo.open(FICHERO);
	if (!archivo.is_open())
	{
		return false;
	}
	else
	{
		iniciaTablero(tablero);
		while (posicion != 0)
		{
			archivo >> posicion >> str;
			tablero[posicion] = stringAtCasilla(str);
		}
		archivo.close();
		return true;
	}
}

//*************************** Función principal ***************************
listaJugadores jugarPartida(stringstream &ss)
{
	srand(time(NULL));
	// Inicializamos las variables.
	listaJugadores LJ;
	int numJugadores;
	int jugador;
	tTablero tablero;

	// Si se carga el tablero correctamente se juega la partida, sino se devuelve una lista de jugadores con un único jugador.
	if (cargaTablero(tablero))
	{
		// En el modo depuración debe empezar siempre el jugador 1 y obtienes el número de jugadores con el string obtenido en el main.
		if (MODO_DEBUG)
		{
			ss >> numJugadores;
			cout << "El numero de jugadores es: " << numJugadores << "\n";
			LJ.cont = numJugadores;
			cout << "**** EMPIEZA EL JUGADOR: 1 ****\n";
			jugador = 1;
			cout << "CASILLA ACTUAL: " << CASILLA_SALIDA << "\n";
		}
		else
		{
			// Se introduce el número de jugadores con los que se desea jugar.
			cout << "Introduzca el numero de jugadores: \n";
			cin >> numJugadores;
			cout << "El número de jugadores es: " << numJugadores << "\n";
			LJ.cont = numJugadores;
			// Llamamos a la función quienempieza() para seleccionar que jugador debe empezar, pues se escoge aleatoriamente.
			jugador = quienEmpieza(LJ);
			cout << "**** EMPIEZA EL JUGADOR:" << jugador << "****\n";
			cout << "CASILLA ACTUAL: " << CASILLA_SALIDA << "\n";
		}

		// Inicializamos el array de jugadores.
		iniciaJugadores(LJ);

		// Pintamos el tablero al inicio de la partida, y cada vez que ocurre algún movimiento.
		pintaTablero(tablero, LJ);

		// El siguiente bucle se ejecuta mientras la partida no se haya acabado, es decir ningún jugador ha llegado a la meta.
		while (!esMeta(LJ.aJugadores[jugador - 1].casilla))
		{
			// Si no se tienen turnos de más o de menos, se juega normal, se tira el dado y se pasa a la casilla correspondiente.
			if (LJ.aJugadores[jugador - 1].turnos == 0)
			{
				if (MODO_DEBUG)
				{
					int dado = tirarDadoManual(ss);
					LJ.aJugadores[jugador - 1].casilla += dado;
				}
				else
				{
					int dado = tirarDado();
					cout << "INTRODUCE EL VALOR DEL DADO: ";
					cout << dado;
					cout << "\nVALOR DEL DADO: " << dado << "\n";
					LJ.aJugadores[jugador - 1].casilla += dado;
				}

				cout << "PASAS A LA CASILLA: " << LJ.aJugadores[jugador - 1].casilla << "\n";

				if (!esMeta(LJ.aJugadores[jugador - 1].casilla))
				{
					LJ.aJugadores[jugador - 1].casilla = efectoPosicion(tablero, LJ.aJugadores[jugador - 1].casilla);
					LJ.aJugadores[jugador - 1].turnos += efectoTiradas(tablero, LJ.aJugadores[jugador - 1].casilla);
					pintaTablero(tablero, LJ);
				}

				// En caso de haber ganado turnos extra, se vuelve a tirar hasta que se acaben los turnos extra y, mientras ninguno de los jugadores haya ganado.
				while (LJ.aJugadores[jugador - 1].turnos > 0 && !esMeta(LJ.aJugadores[jugador - 1].casilla))
				{
					if (MODO_DEBUG)
					{
						int dado = tirarDadoManual(ss);
						LJ.aJugadores[jugador - 1].casilla += dado;
					}
					else
					{
						int dado = tirarDado();
						cout << "INTRODUCE EL VALOR DEL DADO: ";
						cout << dado;
						cout << "\nVALOR DEL DADO: " << dado << "\n";
						LJ.aJugadores[jugador - 1].casilla += dado;
					}
					cout << "PASAS A LA CASILLA: " << LJ.aJugadores[jugador - 1].casilla << "\n";
					if (!esMeta(LJ.aJugadores[jugador - 1].casilla))
					{
						LJ.aJugadores[jugador - 1].casilla = efectoPosicion(tablero, LJ.aJugadores[jugador - 1].casilla);
						LJ.aJugadores[jugador - 1].turnos += efectoTiradas(tablero, LJ.aJugadores[jugador - 1].casilla);
						LJ.aJugadores[jugador - 1].turnos--;
						pintaTablero(tablero, LJ);
					}
				}

				// Una vez ha acabado el turno del jugador se cambia de jugador.
				if (!esMeta(LJ.aJugadores[jugador - 1].casilla))
				{
					if (jugador < LJ.cont)
					{
						jugador++;
					}
					else
					{
						jugador = 1;
					}
					cout << "\nTURNO PARA EL JUGADOR " << jugador << "\n";
					cout << "CASILLA ACTUAL: " << LJ.aJugadores[jugador - 1].casilla << "\n";
				}
			}
			// En caso de estar penalizado (no tener turnos), se cambia de jugador directamente.
			else
			{
				cout << "JUGADOR " << jugador << " PENALIZADO\n";
				LJ.aJugadores[jugador - 1].turnos++;
				if (jugador < LJ.cont)
				{
					jugador++;
				}
				else
				{
					jugador = 1;
				}
				cout << "\nTURNO PARA EL JUGADOR " << jugador << "\n";
				cout << "CASILLA ACTUAL: " << LJ.aJugadores[jugador - 1].casilla << "\n";
			}
		}
		// Una vez se sale del bucle principal, uno de los jugadores ha llegado a la meta (se muestra por pantalla).
		pintaTablero(tablero, LJ);
		cout << "----- GANA EL JUGADOR " << jugador << " -----";
		return LJ;
	}
	//En caso de no cargarse el tablero, se devuelve una lista de jugadores con un único jugador.
	else
	{
		LJ.cont = 1;
		iniciaJugadores(LJ);
		return LJ;
	}
}

/*int main(){
	jugarPartida();
}*/

int main()
{
	stringstream ss;
	ss << "3 2 5 60"; // Meter los valores del dado y jugadores que deseas que salgan si el MODO_DEBUG = true.
	listaJugadores LJ = jugarPartida(ss);
	for (int i = 0; i < LJ.cont; i++)
	{
		cout << " " << LJ.aJugadores[i].casilla << '\n';
	}
}
