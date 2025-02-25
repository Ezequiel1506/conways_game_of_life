#include "Juego.h"

void initJuego(Juego * juego)
{
	juego->estadoCongelado = false;
	juego->cantidadTurnosCongelado=0;
	juego->nacimientosUltimoTurno=0, juego->muertesUltimoTurno=0;
	juego->totalNacimientos=0, juego->totalMuertes=0;
	juego->punteroTableroActual = &(juego->tableroActual);
	juego->punteroTableroProximoTurno = &(juego->tableroProximoTurno);
	initTablero(juego->punteroTableroActual);
	initTablero(juego->punteroTableroProximoTurno);
	setTurno(juego, 1);
	setEstadoCongelado(juego, false);
}

int getTurno(Juego * j) {
	return j -> turno;
}

void setTurno(Juego * j, int turno) {
	j -> turno = turno;
}

void setEstadoCongelado(Juego * j, bool estadoCongelado) {
	j->estadoCongelado = estadoCongelado;
}

void avanzarTurno(Juego * juego) {
	limpiarUltimasTransiciones(juego);
	actualizarTablero(juego);
	actualizarTransicionesTotales(juego);
	actualizarEstadoCongelado(juego);
	juego->turno++;
	

}

void actualizarTablero(Juego * juego) {
	int anchoMaximo = getAnchoMaximo(juego->punteroTableroActual);
	int altoMaximo = getAltoMaximo(juego->punteroTableroActual);
	Tablero * punteroTableroAuxiliar;
	for (int i = 1; i < altoMaximo; i++)
	{
		for (int j = 1; j < anchoMaximo; j++)
		{
			Celula celulaActual = getValor(juego->punteroTableroActual, i, j);
			int cantidadVecinosVivos = contarCantidadVecinosVivos(juego->punteroTableroActual, i, j);
			ESTADO_CELULA estadoCelulaNueva = definirProximoEstadoCelula(celulaActual, cantidadVecinosVivos);
			Celula celulaNueva;
			setEstado(&celulaNueva, estadoCelulaNueva);
			setValor(juego->punteroTableroProximoTurno, celulaNueva, i, j);
			contarTransiciones(juego, getEstado(&celulaActual), estadoCelulaNueva);
		}
	}
	punteroTableroAuxiliar = juego->punteroTableroActual;
	juego->punteroTableroActual = juego->punteroTableroProximoTurno;
	juego->punteroTableroProximoTurno = punteroTableroAuxiliar;
}



ESTADO_CELULA definirProximoEstadoCelula(Celula celula, int cantidadVecinosVivos) {
	if (getEstado(&celula) == MUERTA && cantidadVecinosVivos == 3) { 
		return VIVA; 
	}
	if (getEstado(&celula) == VIVA && cantidadVecinosVivos != 2 && cantidadVecinosVivos != 3) {
		return MUERTA;
	}
	return getEstado(&celula);
}

void limpiarUltimasTransiciones(Juego* juego) {
	juego->muertesUltimoTurno = 0;
	juego->nacimientosUltimoTurno = 0;
}

void contarTransiciones(Juego * juego, ESTADO_CELULA estadoCelulaActual, ESTADO_CELULA estadoCelulaNueva) {
	if (estadoCelulaActual == VIVA && estadoCelulaNueva == MUERTA) {
		juego->muertesUltimoTurno++;
	}
	if (estadoCelulaActual == MUERTA && estadoCelulaNueva == VIVA) {
		juego->nacimientosUltimoTurno++;
	}
}

void actualizarTransicionesTotales(Juego * juego) {
	juego->totalMuertes += juego->muertesUltimoTurno;
	juego->totalNacimientos += juego->nacimientosUltimoTurno;
}

void actualizarEstadoCongelado(Juego* juego) {

	if (juego->nacimientosUltimoTurno == 0 && juego->muertesUltimoTurno == 0) {
		juego->cantidadTurnosCongelado += 1;
		if (juego->cantidadTurnosCongelado >= 2) {
			juego->estadoCongelado = true;
		}
	}
	else {
		juego->cantidadTurnosCongelado = 0;
		juego->estadoCongelado = false;
	}

}


void imprimirMenuInicial(Juego * juego) {
	int numeroCelulas, fila, columna;
	cout << "Ingrese la cantidad de celulas que desea ingresar" << endl;
	cin >> numeroCelulas;
	for (int i = 0; i < numeroCelulas; i++) {
		cout << "Celula " << i + 1 << endl;
		cout << "Fila: " << endl;
		cin >> fila;
		cout << "Columna: " << endl;
		cin >> columna;
		setCelulaInicial(juego, fila, columna);
	}
	mostrarTablero(juego);

}

void setCelulaInicial(Juego * juego, int fila, int columna) {
	Celula c;
	setEstado(&c, VIVA);
	setValor(juego->punteroTableroActual, c, fila, columna);
}

void mostrarTablero(Juego * juego){
	int anchoMaximo = getAnchoMaximo(juego->punteroTableroActual);
	int altoMaximo = getAltoMaximo(juego->punteroTableroActual);
	for (int i = 1; i < altoMaximo; i++)
	{
		for (int j = 1; j < anchoMaximo; j++)
		{
			Celula c = getValor(juego->punteroTableroActual, i, j);
			cout << getEstado(&c);
		}
		cout << '\n';
	}
}

void mostrarEstadoJuego(Juego * juego) {
	mostrarTablero(juego);
	float promedioMuertes = juego->totalMuertes / juego->turno;
	float promedioNacimientos = juego->totalNacimientos / juego->turno;
	cout << "Celulas vivas: " << contarTotalCelulasVivas(juego->punteroTableroActual) << endl;
	cout << "Nacimientos: " << juego->nacimientosUltimoTurno << endl;
	cout << "Muertes: " << juego->muertesUltimoTurno << endl;
	cout << "Promedio de Nacimientos: " << promedioNacimientos << endl;
	cout << "Promedio de Muertes: " << promedioMuertes << endl;

	string mensajeJuegoCongelado = juego->estadoCongelado ? "si" : "no";
	cout << "Juego congelado: " << mensajeJuegoCongelado << endl;
}
	

void imprimirMenuContinuacion(Juego * juego) {
	int input;
	cout << "1.Ejecutar un turno" << endl;
	cout << "2.Reiniciar juego" << endl;
	cout << "3.Terminar juego" << endl;

	cin >> input;

	switch (input)
	{
	case 1:
		avanzarTurno(juego);
		mostrarEstadoJuego(juego);
		imprimirMenuContinuacion(juego);
		break;
	case 2:
		initJuego(juego);
		imprimirMenuInicial(juego);
		imprimirMenuContinuacion(juego);
		break;
	case 3:
		cout << "Juego terminado!" << endl;
		break;
	default:
		break;
	}

}