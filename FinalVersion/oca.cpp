// Silvia Rodrigo Cañete
// Paula Martín Espiñeira
#include <iostream>
#include <cstdlib> //Para usar las funciones rand() y srand() que dan n aleatorios.
#include <ctime>   //Para usar la funcion time(NULL)
#include <sstream> //Para probar la practica.
#include <fstream> //Para poder usar el fichero.
#include <string>
#include <iomanip> //Para usar las funciones de pintar el tablero.

using namespace std;

//*************************** CONSTANTES ************************************
const int NUM_CASILLAS = 63;
const int CASILLA_SALIDA = 1;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int RETROCESO_LABERINTO = 12;
const int MAX_JUGADORES = 4;
const string FICHTABLERO = "TableroClasico63casillas.txt";
const bool MODO_DEBUG = false;
const int NUM_FILAS_A_DIBUJAR = 3;
const int MAX_PARTIDAS = 10;
const string FPARTIDAS = "partidas.txt";

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

struct tPartida
{
    listaJugadores estadoJugador;
    tTablero tablero;
    int numJugador;
};

struct tListaPartidas
{
    tPartida partidaAplazada[MAX_PARTIDAS];
    int cont;
};

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

void iniciaJugadores(listaJugadores &LJ) // Inicializamos el array de los LJ con los valores iniciales de turnos = 0 y casilla = 1.
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

//********************** FUNCIONES VERSION 3 ***************************

void cargaTablero(tTablero tablero, ifstream &archivo) // Recibe el archivo ya abierto, bien sea del tableroClasico o de las partidas aplazadas.
{
    int posicion;
    string str;
    iniciaTablero(tablero);
    archivo >> posicion;
    while (posicion != 0)
    {
        archivo >> str;
        tablero[posicion] = stringAtCasilla(str);
        archivo >> posicion;
    }
}

bool cargaJugadores(listaJugadores &LJ, ifstream &archivo) // Carga la información de los jugadores en el array de partidasAplazadas.
{
    for (int i = 0; i < LJ.cont; i++)
    {
        archivo >> LJ.aJugadores[i].casilla >> LJ.aJugadores[i].turnos;
    }
    return true;
}

bool cargaPartidas(tListaPartidas &partidas) // Carga toda la información del fichero de partidas aplazadas.
{
    ifstream archivo;
    string str;
    archivo.open(FPARTIDAS);
    if (!archivo.is_open())
    {
        archivo.close();
        return false;
    }
    else
    {
        archivo >> partidas.cont;
        for (int i = 0; i < partidas.cont; i++)
        {
            cargaTablero(partidas.partidaAplazada[i].tablero, archivo);
            archivo >> partidas.partidaAplazada[i].estadoJugador.cont >> partidas.partidaAplazada[i].numJugador;
            cargaJugadores(partidas.partidaAplazada[i].estadoJugador, archivo);
        }
        archivo.close();
        return true;
    }
}

void EliminarPartida(tListaPartidas &partidas, int indice) // Elimina la partida que se haya acabado pasando posicion a posicion la información de un hueco del array al anterior.
{
    while (indice < partidas.cont)
    {
        partidas.partidaAplazada[indice] = partidas.partidaAplazada[indice + 1];
        indice++;
    }
    partidas.cont--;
}

string tCasilla2string(tCasilla c) // Pasa un valor de tipo enumerado a string.
{
    switch (c)
    {
    case OCA:
        return "OCA";
    case PUENTE1:
        return "PUENTE1";
    case PUENTE2:
        return "PUENTE2";
    case DADO1:
        return "DADO1";
    case DADO2:
        return "DADO2";
    case POSADA:
        return "POSADA";
    case CALAVERA:
        return "CALAVERA";
    case LABERINTO:
        return "LABERINTO";
    case POZO:
        return "POZO";
    case CARCEL:
        return "CARCEL";
    default:
        return "NORMAL";
    }
}

void guardaTablero(const tTablero tablero, ofstream &fichero) // Escribimos en el fichero posición a posición las casillas especiales.
{
    for (int i = 0; i <= NUM_CASILLAS; i++)
    {
        // Como unicamente queremos escribir las casillas especiales, las que tengan como valor "NORMAL" no entran en el if.
        if (tCasilla2string(tablero[i]) != "NORMAL")
        {
            fichero << i << " " << tCasilla2string(tablero[i]) << endl;
        }
    }
}

void guardaJugadores(const listaJugadores LJ, ofstream &fichero) // Guardamos la información actual de los jugadores en el archivo.
{
    for (int i = 0; i < LJ.cont; i++)
    {
        fichero << LJ.aJugadores[i].casilla << " " << LJ.aJugadores[i].turnos << endl;
    }
}

bool guardaPartidas(const tListaPartidas &partidas) // Guarda las partidas reescribiendolas todas desde cero y actualizando la información.
{
    ofstream fichero;
    fichero.open(FPARTIDAS);
    if (!fichero.is_open())
    {
        cout << "Carga fallida";
        fichero.close();
        return false;
    }
    else
    {
        // La variable es de tipo ofstream porque vamos a escribir en ella (es de escritura, no lectura)
        ofstream fichero;
        fichero.open(FPARTIDAS);
        int contador = 0;
        // Escribimos como primer dato el num de partidas y con un while escribimos una a una.
        fichero << partidas.cont << endl;
        while (contador < partidas.cont)
        {
            guardaTablero(partidas.partidaAplazada[contador].tablero, fichero);
            // Al acabar de escribir el tablero escribimos un 0, ps es el centinela de lectura.
            fichero << "0" << endl;
            // Escribimos el num de jugadores y el num del jugador que tiene el turno, y llamamos a guardaJugadores para escribir en que casilla están y cuantos turnos tienen.
            fichero << partidas.partidaAplazada[contador].estadoJugador.cont << endl;
            fichero << partidas.partidaAplazada[contador].numJugador << endl;
            guardaJugadores(partidas.partidaAplazada[contador].estadoJugador, fichero);
            contador++;
        }
        fichero.close();
        return true;
    }
}

bool insertaNuevaPartida(tListaPartidas &partidas, const tPartida &partidaOca) // Comprueba si es posible insertar una nueva partida, y en caso afirmativo la guarda en la siguiente posición libre del array y en el fichero.
{
    if (partidas.cont >= MAX_PARTIDAS)
    {
        cout << "No es posible guardar la partida pues el fichero ya tiene 10 partidas guardadas.\n";
        return false;
    }
    else
    {
        partidas.partidaAplazada[partidas.cont] = partidaOca;
        guardaPartidas(partidas);
        return true;
    }
}

bool abandona() // Comprueba si el usuario desea abandonar la partida.
{
    string abandonar;
    cout << "Si desea abandonar la partida escriba 'SI'. En caso contrario introduzca cualquier otro valor:  \n";
    cin >> abandonar;
    if (abandonar == "SI")
    {
        return true;
    }
    else
    {
        return false;
    }
}

//*************************** Funciones principales *******************************
tPartida jugarPartida(stringstream &ss, tPartida &partida)
{
    // Inicializamos las variables y las renombramos.
    int &jugador = partida.numJugador;
    tTablero &tablero = partida.tablero;
    listaJugadores &LJ = partida.estadoJugador;

    // Pintamos el tablero al inicio de la partida, y cada vez que ocurre algún movimiento. Si la partida ya existía nos mostrará en que posición quedaron los jugadores; si es nueva se encontrarán en la casilla de salida.
    pintaTablero(tablero, LJ);

    // El siguiente bucle se ejecuta mientras la partida no se haya acabado, es decir, ningún jugador ha llegado a la meta.Y salvo que el jugador decida abandonar la partida.
    while (!esMeta(LJ.aJugadores[jugador - 1].casilla) && !abandona())
    {
        // Si no se tienen turnos de más o de menos, se juega normal, se tira el dado y se pasa a la casilla correspondiente.
        if (LJ.aJugadores[jugador - 1].turnos == 0)
        {
            int dado;
            if (MODO_DEBUG)
            {
                dado = tirarDadoManual(ss);
            }
            else
            {
                dado = tirarDado();
                cout << "INTRODUCE EL VALOR DEL DADO: ";
                cout << dado;
                cout << "\nVALOR DEL DADO: " << dado << "\n";
            }

            LJ.aJugadores[jugador - 1].casilla += dado;
            cout << "PASAS A LA CASILLA: " << LJ.aJugadores[jugador - 1].casilla << "\n";

            if (!esMeta(LJ.aJugadores[jugador - 1].casilla))
            {
                LJ.aJugadores[jugador - 1].casilla = efectoPosicion(tablero, LJ.aJugadores[jugador - 1].casilla);
                LJ.aJugadores[jugador - 1].turnos += efectoTiradas(tablero, LJ.aJugadores[jugador - 1].casilla);
                pintaTablero(tablero, LJ);
            }

            // En caso de haber ganado turnos extra, se vuelve a tirar hasta que se acaben los turnos extra.
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
    // Una vez se sale del bucle principal, uno de los LJ ha llegado a la meta (se muestra por pantalla).
    if (esMeta(LJ.aJugadores[jugador - 1].casilla))
    {
        pintaTablero(tablero, LJ);
        cout << "----- GANA EL JUGADOR " << jugador << " -----";
    }
    return partida;
}

tListaPartidas LaOca(stringstream &ss)
{
    // Creamos las variables a utilizar a los largo de toda la función.
    srand(time(NULL));
    listaJugadores LJ;
    tTablero tablero;
    tListaPartidas partidas;
    string respuesta;
    int indice;
    int numJugadores;

    // Cargamos el fichero de partidas para que la información se guarde en el array desde un principio.
    cargaPartidas(partidas);

    // En modo DEBUG la información se lee siempre del ss, sino debemos introducirla por teclado.
    if (MODO_DEBUG)
    {
        ss >> respuesta;
    }
    else
    {
        cout << "Si desea comenzar una nueva partida escriba: 'NUEVA' o '1'.\nSi desea continuar una partida existente escriba: 'EXISTENTE' o '2': ";
        cin >> respuesta;
    }

    // Distinguimos casos; si la partida es nueva se lleva a cabo el siguiente if.
    if (respuesta == "NUEVA" || respuesta == "1")
    {
        // Jugamos la partida nueva en el tPartida partidaOca, y si luego es posible guardarla, copiamos su información en la siguiente posición libre del array.
        tPartida partidaOca;

        // Abrimos el fichero del tableroClásico y lo cargamos.
        ifstream entrada;
        entrada.open(FICHTABLERO);
        if (!entrada.is_open())
        {
            cout << "Carga fallida";
        }
        else
        {
            // Antes de cargar el tablero, preguntamos el número de jugadores y decidimos cual empieza, ya sea de forma aleatoria o dejando que empiece el 1 (modo DEBUG).
            if (MODO_DEBUG)
            {
                ss >> numJugadores;
                cout << "El numero de jugadores es: " << numJugadores << "\n";
                partidaOca.estadoJugador.cont = numJugadores;
                cout << "**** EMPIEZA EL JUGADOR: 1 ****\n";
                partidaOca.numJugador = 1;
                cout << "CASILLA ACTUAL: " << CASILLA_SALIDA << "\n";
            }
            else
            {
                cout << "Introduzca el numero de jugadores: ";
                cin >> numJugadores;
                cout << "\nEl número de jugadores es: " << numJugadores << "\n";
                partidaOca.estadoJugador.cont = numJugadores;
                // Llamamos a la función quienempieza() para seleccionar que jugador debe empezar, pues se escoge aleatoriamente.
                partidaOca.numJugador = quienEmpieza(partidaOca.estadoJugador);
                cout << "**** EMPIEZA EL JUGADOR:" << partidaOca.numJugador << "****\n";
                cout << "CASILLA ACTUAL: " << CASILLA_SALIDA << "\n";
            }
            // Cargamos el tablero, inicializamos los valores de los jugadores y cerramos el archivo. Inicializamos los valores de los jugadores también.
            cargaTablero(partidaOca.tablero, entrada);
            iniciaJugadores(partidaOca.estadoJugador);
            entrada.close();
        }

        // Llamamos a jugar partida para llevar a cabo la partida, y cuando esta acabe o se abandone nos retorna la partida con toda la información de los jugadores y el tablero.
        partidaOca = jugarPartida(ss, partidaOca);

        // Una vez jugada la partida y no acabada , pues si se acaba no se llega a escribir, comprobamos si es posible guardarla en el fichero o no.
        if (!esMeta(partidaOca.estadoJugador.aJugadores[partidaOca.numJugador - 1].casilla))
        {
            if (insertaNuevaPartida(partidas, partidaOca))
            {
                // Para que cuando guardemos todas las partidas en el archivo (guardaPartidas al final de la función LaOca), se escriba la información de la nueva partida, y aparezca que hay una partida extra.
                partidas.cont++;
            }
        }
    }

    // En caso de que la partida a jugar sea una existente, se lleva a cabo el siguiente else:
    else
    {
        cout << "\nEscriba el numero de la partida que desea continuar: ";
        cin >> indice;

        // Le restamos 1 al índice porque debemos recordar que los arrays comienzan a contar en la posición 0, entonces la partida 1 está en la posición 0, es decir, en la posición índice - 1.
        indice--;
        cout << "TURNO PARA EL JUGADOR: " << partidas.partidaAplazada[indice].numJugador;
        partidas.partidaAplazada[indice] = jugarPartida(ss, partidas.partidaAplazada[indice]);

        // Una vez jugada la partida, si llega a la meta debe borrarse.
        if (esMeta(partidas.partidaAplazada[indice].estadoJugador.aJugadores[partidas.partidaAplazada[indice].numJugador - 1].casilla))
        {
            EliminarPartida(partidas, indice);
        }
    }

    // Al finalizar la partida y haberse borrado las acabadas, actualizamos la información del fichero llamando a guardaPartidas.
    guardaPartidas(partidas);
    return partidas;
}

int main()
{
    stringstream ss;
    ss << "1 2 3 4 5 45 23 60";
    /*Aquí se escriben los valores de : nueva / existente, numero de jugadores y el valor de los dados.
    Si se desea abandonar/continuar la partida se escribirá por teclado.*/
    LaOca(ss);
}
