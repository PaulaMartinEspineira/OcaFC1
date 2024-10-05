#include <iostream>
#include <cstdlib> //Para usar las funciones rand() y srand() que dan n aleatorios
#include <ctime>   //Para usar la funcion time(NULL)
#include <sstream> //Para probar la practica

using namespace std;

//*************************** CONSTANTES ***************************
const int NUM_CASILLAS = 63;
const int CASILLA_SALIDA = 1;
const int CASILLA_PUENTE_1 = 6;
const int CASILLA_PUENTE_2 = 12;
const int CASILLA_DADOS_1 = 26;
const int CASILLA_DADOS_2 = 53;
const int CASILLA_LABERINTO = 42;
const int CASILLA_MUERTE = 58;
const int CASILLA_POSADA = 19;
const int CASILLA_PRISION = 52;
const int CASILLA_POZO = 31;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int RETROCESO_LABERINTO = 12;
const bool MODO_DEBUG = true;

// Las variables de tipo struct tJugador guardarán los datos de ambos jugadores.
struct tJugador
{
	int casilla = CASILLA_SALIDA;
	int nturnos = 0;
	int njugador = 0;
};

//*************************** Funciones que verifican si una casilla es de tipo especial ***************************
bool esOca(int casilla)
{
	// Las ocas se encuentran entre las casillas 5 y 63, y siguen un patrón(multiplos de 9 o múltiplos de 9 menos 5 unidades).
	return ((casilla <= 63) && (casilla == 5 || casilla % 9 == 0 || (casilla - 5) % 9 == 0));
}

bool esPuente(int casilla)
{
	return (casilla == CASILLA_PUENTE_1 || casilla == CASILLA_PUENTE_2);
}

bool esDados(int casilla)
{
	return (casilla == CASILLA_DADOS_1 || casilla == CASILLA_DADOS_2);
}

bool esLaberinto(int casilla)
{
	return casilla == CASILLA_LABERINTO;
}

bool esMuerte(int casilla)
{
	return casilla == CASILLA_MUERTE;
}

bool esPosada(int casilla)
{
	return casilla == CASILLA_POSADA;
}

bool esPrision(int casilla)
{
	return casilla == CASILLA_PRISION;
}

bool esPozo(int casilla)
{
	return casilla == CASILLA_POZO;
}

bool esMeta(int casilla)
{
	return casilla >= NUM_CASILLAS;
}

//*************************** Funciones que calculan casilla de destino ***************************
int siguienteOca(int casilla)
{
	if (casilla == 63) {
		casilla = 63;
	}
	else if (casilla % 9 == 0)
	{
		casilla = casilla + 5;
	}
	else
	{
		casilla = casilla + 4;
	}
	cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA\n";
	cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << casilla << "\n";
	cout << "VUELVES A TIRAR\n";
	cout << "CASILLA ACTUAL: " << casilla << "\n";
	return casilla;
}

int siguientePuente(int casilla)
{
	// Si caes en el 1er puente, pasas al 2do; y viceversa.
	if (casilla == CASILLA_PUENTE_1)
	{
		casilla = CASILLA_PUENTE_2;
		cout << "DE PUENTE A PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE\n";
		cout << "SALTAS AL SIGUIENTE PUENTE EN LA CASILLA: " << casilla << "\n";
		cout << "VUELVES A TIRAR\n";
		cout << "CASILLA ACTUAL: " << casilla << "\n";
	}
	else if (casilla == CASILLA_PUENTE_2)
	{
		casilla = CASILLA_PUENTE_1;
		cout << "DE PUENTE A PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE\n";
		cout << "SALTAS AL SIGUIENTE PUENTE EN LA CASILLA: " << casilla << "\n";
		cout << "VUELVES A TIRAR\n";
		cout << "CASILLA ACTUAL: " << casilla << "\n";
	}
	return casilla;
}

int siguienteDado(int casilla)
{
	// Si caes en los 1os dados, pasas a los 2dos; y viceversa.
	if (casilla == CASILLA_DADOS_1)
	{
		casilla = CASILLA_DADOS_2;
		cout << "DE DADOS A DADOS Y TIRO PORQUE ME HA TOCADO\n";
		cout << "SALTAS A LOS SIGUIENTES DADOS EN LA CASILLA: " << casilla << "\n";
		cout << "VUELVES A TIRAR\n";
		cout << "CASILLA ACTUAL: " << casilla << "\n";
	}
	else if (casilla == CASILLA_DADOS_2)
	{
		casilla = CASILLA_DADOS_1;
		cout << "DE DADOS A DADOS Y TIRO PORQUE ME HA TOCADO\n";
		cout << "SALTAS A LOS SIGUIENTES DADOS EN LA CASILLA: " << casilla << "\n";
		cout << "VUELVES A TIRAR\n";
		cout << "CASILLA ACTUAL: " << casilla << "\n";
	}
	return casilla;
}

int siguienteLaberinto()
{
	// Si caes en el laberinto, retrocedes 12 casillas.
	int casilla = CASILLA_LABERINTO - RETROCESO_LABERINTO;
	cout << "HAS CAIDO EN EL LABERINTO\n";
	cout << "RETROCEDES A LA CASILLA " << casilla << "\n";
	return casilla;
}

int siguienteMuerte()
{
	// La muerte implica retroceder a la casilla de salida(casilla 1).
	cout << "HAS CAIDO EN LA MUERTE\n";
	cout << "RETROCEDES A LA CASILLA DE SALIDA: " << CASILLA_SALIDA << "\n";
	return CASILLA_SALIDA;
}

//*************************** Funciones que regulan el juego ***************************
int efectoTiradas(int casilla)
{
	int nturnos = 0;
	// Dependiendo de la casilla especial, se añaden turnos extra o se pierden turnos, y el jugador queda penalizado.
	if (esOca(casilla) || esPuente(casilla) || esDados(casilla))
	{
		nturnos = 1;
	}
	else if (esPosada(casilla))
	{
		nturnos -= TURNOS_POSADA;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_POSADA << " TURNO SIN TIRAR\n";
	}
	else if (esPrision(casilla))
	{
		nturnos -= TURNOS_PRISION;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_PRISION << " TURNOS SIN TIRAR\n";
	}
	else if (esPozo(casilla))
	{
		nturnos -= TURNOS_POZO;
		cout << "QUEDAS PENALIZADO CON " << TURNOS_POZO << " TURNOS SIN TIRAR\n";
	}
	return nturnos;
}

int efectoPosicion(int casilla)
{
	// Se comprueba si se ha caído en una casilla especial.
	if (esOca(casilla))
	{
		casilla = siguienteOca(casilla);
	}
	else if (esPuente(casilla))
	{
		casilla = siguientePuente(casilla);
	}
	else if (esDados(casilla))
	{
		casilla = siguienteDado(casilla);
	}
	else if (esLaberinto(casilla))
	{
		casilla = siguienteLaberinto();
	}
	else if (esMuerte(casilla))
	{
		casilla = siguienteMuerte();
	}
	else if (esPosada(casilla))
	{
		cout << "HAS CAIDO EN LA POSADA\n";
	}
	else if (esPrision(casilla))
	{
		cout << "HAS CAIDO EN LA PRISION\n";
	}
	else if (esPozo(casilla))
	{
		cout << "HAS CAIDO EN EL POZO\n";
	}
	return casilla;
}

int tirarDadoManual(stringstream &ss)
{
	int dado = 0;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	ss >> dado;
	cout << dado;
	cout << "\nVALOR DEL DADO: " << dado << "\n";
	return dado;
}

int tirarDado()
{
	return rand() % 6 + 1;
}

int quienEmpieza()
{
	return rand() % 2 + 1;
}

pair<int, int> jugarPartida(stringstream &ss)
{
	tJugador jug1;
	tJugador jug2;
	tJugador jActual;
	jug1.njugador = 1;
	jug2.njugador = 2;
	srand(time(NULL));

	// En el modo depuración debe empezar siempre el jugador 1. En el modo normal se escoge aleatoriamente al jugador que empieza.
	if (MODO_DEBUG)
	{
		cout << "**** EMPIEZA EL JUGADOR: 1 ****\n";
		cout << "CASILLA ACTUAL: " << jActual.casilla << "\n";
		jActual = jug1;
	}
	else
	{
		// Llamamos a la función quienempieza() para seleccionar que jugador debe empezar.
		if (quienEmpieza() == jug1.njugador)
		{
			cout << "**** EMPIEZA EL JUGADOR: 1 ****\n";
			cout << "CASILLA ACTUAL: " << jug1.casilla << "\n";
			jActual = jug1;
		}
		else
		{
			cout << "**** EMPIEZA EL JUGADOR: 2 ****\n";
			cout << "CASILLA ACTUAL: " << jug2.casilla << "\n";
			jActual = jug2;
		}
	}

	// El siguiente bucle se ejecuta mientras la partida no se haya acabado.
	while (!esMeta(jActual.casilla))
	{

		// Si no se tienen turnos de más o de menos, se tira el dado y se pasa a la casilla correspondiente.
		if (jActual.nturnos == 0)
		{
			if (MODO_DEBUG)
			{
				int dado = tirarDadoManual(ss);
				jActual.casilla = jActual.casilla + dado;
			}
			else
			{
				int dado = tirarDado();
				cout << "INTRODUCE EL VALOR DEL DADO: ";
				cout << dado;
				cout << "\nVALOR DEL DADO: " << dado << "\n";
				jActual.casilla = jActual.casilla + dado;
			}

			cout << "PASAS A LA CASILLA: " << jActual.casilla << "\n";
			jActual.casilla = efectoPosicion(jActual.casilla);
			jActual.nturnos = efectoTiradas(jActual.casilla);

			// En caso de haber ganado turnos extra, se vuelve a tirar hasta que se acaben los turnos extra y, mientras ninguno de los jugadores haya ganado.
			while (jActual.nturnos > 0 && !esMeta(jActual.casilla))
			{
				if (MODO_DEBUG)
				{
					int dado = tirarDadoManual(ss);
					jActual.casilla = jActual.casilla + dado;
				}
				else
				{
					int dado = tirarDado();
					cout << "INTRODUCE EL VALOR DEL DADO: ";
					cout << dado;
					cout << "\nVALOR DEL DADO: " << dado << "\n";
					jActual.casilla = jActual.casilla + dado;
				}
				cout << "PASAS A LA CASILLA: " << jActual.casilla << "\n";
				jActual.casilla = efectoPosicion(jActual.casilla);
				jActual.nturnos += efectoTiradas(jActual.casilla);
				jActual.nturnos--;
			}

			// Una vez ha acabado el turno del jugador, se guardan los datos de la jugada en el struct del jugador correspondiente. Y se cambia de jugador.
			if (!esMeta(jActual.casilla))
			{
				if (jActual.njugador == 1)
				{
					jug1 = jActual;
					jActual = jug2;
				}
				else
				{
					jug2 = jActual;
					jActual = jug1;
				}
				cout << "\nTURNO PARA EL JUGADOR " << jActual.njugador << "\n";
				cout << "CASILLA ACTUAL: " << jActual.casilla << "\n";
			}
		}
		// En caso de estar penalizado(no tener turnos), se cambia de jugador directamente.
		else
		{
			cout << "JUGADOR " << jActual.njugador << " PENALIZADO\n";
			jActual.nturnos++;
			if (jActual.njugador == 1)
			{
				jug1 = jActual;
				jActual = jug2;
			}
			else
			{
				jug2 = jActual;
				jActual = jug1;
			}
			cout << "\nTURNO PARA EL JUGADOR " << jActual.njugador << "\n";
			cout << "CASILLA ACTUAL: " << jActual.casilla << "\n";
		}
	}

	// Guardamos los últimos datos para saber que jugador ganó la partida.
	if (jActual.njugador == 1)
	{
		jug1 = jActual;
		jActual = jug2;
	}
	else
	{
		jug2 = jActual;
		jActual = jug1;
	}

	// Una vez se sale del bucle principal, uno de los jugadores ha llegado a la meta. Se comprueba cual y se muestra por pantalla.
	if (jug1.casilla >= NUM_CASILLAS)
	{
		cout << "----- GANA EL JUGADOR 1 -----";
	}
	else
	{
		cout << "----- GANA EL JUGADOR 2 -----";
	}
	return { jug1.casilla, jug2.casilla };
}

//int main(){
//	jugarPartida();
//}

/*int main() {
	stringstream ss;
	ss << "4 5 1 11 20 2 4";
	pair<int, int> p = jugarPartida(ss);
	cout << p.first << ' ' << p.second << '\n';
}*/