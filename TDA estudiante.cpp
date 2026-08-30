#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cctype>

using namespace std;

// ============================================================
// FUNCIONES AUXILIARES Y CONTROLES STRICTOS DE VALIDACIÓN
// ============================================================

bool esNumero(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Control 1: Cédula Ecuatoriana (Algoritmo Módulo 10)
bool validarCedulaEcuador(const string& cedula) {
    if (cedula.length() != 10 || !esNumero(cedula)) return false;

    int provincia = stoi(cedula.substr(0, 2));
    if ((provincia < 1 || provincia > 24) && provincia != 30) return false;

    int tercerDigito = cedula[2] - '0';
    if (tercerDigito < 0 || tercerDigito > 5) return false;

    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int d = cedula[i] - '0';
        if (i % 2 == 0) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        suma += d;
    }

    int digitoVerificador = (suma % 10 == 0) ? 0 : (10 - (suma % 10));
    return digitoVerificador == (cedula[9] - '0');
}

bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// Control 2: Fecha de Nacimiento estricta (DD/MM/AAAA)
bool validarFechaNacimiento(const string& fechaStr, int& diaOut, int& mesOut, int& anioOut) {
    if (fechaStr.length() != 10) return false;

    int dia, mes, anio;
    char sep1, sep2;
    istringstream iss(fechaStr);

    if (!(iss >> dia >> sep1 >> mes >> sep2 >> anio) || !iss.eof()) return false;
    if (sep1 != '/' || sep2 != '/') return false;

    time_t t = time(nullptr);
    tm* actual = localtime(&t);
    int anioAct = actual->tm_year + 1900;
    int mesAct = actual->tm_mon + 1;
    int diaAct = actual->tm_mday;

    if (anio < 1900 || anio > anioAct) return false;
    if (mes < 1 || mes > 12) return false;

    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && esBisiesto(anio)) diasPorMes[2] = 29;

    if (dia < 1 || dia > diasPorMes[mes]) return false;

    if (anio == anioAct) {
        if (mes > mesAct) return false;
        if (mes == mesAct && dia > diaAct) return false;
    }

    diaOut = dia;
    mesOut = mes;
    anioOut = anio;
    return true;
}

// Control 3: Nombres / Apellidos (Solo letras, mínimo 2 caracteres por palabra)
bool validarTextoNombres(const string& texto) {
    if (texto.empty()) return false;

    int letrasSeguidas = 0;
    bool tieneLetras = false;

    for (char c : texto) {
        if (isalpha(c)) {
            letrasSeguidas++;
            tieneLetras = true;
        } else if (isspace(c)) {
            if (letrasSeguidas > 0 && letrasSeguidas < 2) return false;
            letrasSeguidas = 0;
        } else {
            return false; // Rechaza dígitos y caracteres especiales
        }
    }

    if (letrasSeguidas > 0 && letrasSeguidas < 2) return false;
    return tieneLetras;
}

// ============================================================
// TDA ESTUDIANTE
// ============================================================

class Estudiante {
private:
    static const int MAX_NOTAS = 7;
    string cedula;
    string nombres;
    string apellidos;
    string fechaNacimiento;
    double notas[MAX_NOTAS];
    int numNotas;

public:
    Estudiante()
        : cedula(""), nombres(""), apellidos(""), fechaNacimiento(""), numNotas(0) {}

    Estudiante(string cedula, string nombres, string apellidos, string fechaNacimiento)
        : cedula(cedula), nombres(nombres), apellidos(apellidos), fechaNacimiento(fechaNacimiento), numNotas(0) {}

    string getCedula() const { return cedula; }
    string getNombres() const { return nombres; }
    string getApellidos() const { return apellidos; }
    string getFechaNacimiento() const { return fechaNacimiento; }
    int getNumNotas() const { return numNotas; }
    int getMaxNotas() const { return MAX_NOTAS; }

    void setNombres(const string& nombres) { this->nombres = nombres; }
    void setApellidos(const string& apellidos) { this->apellidos = apellidos; }
    void setFechaNacimiento(const string& fechaNacimiento) { this->fechaNacimiento = fechaNacimiento; }

    int calcularEdad() const {
        int dia, mes, anio;
        if (!validarFechaNacimiento(fechaNacimiento, dia, mes, anio)) return -1;

        time_t tiempoActual = time(nullptr);
        tm* fechaActual = localtime(&tiempoActual);

        int anioActual = fechaActual->tm_year + 1900;
        int mesActual = fechaActual->tm_mon + 1;
        int diaActual = fechaActual->tm_mday;

        int edad = anioActual - anio;
        if (mesActual < mes || (mesActual == mes && diaActual < dia)) {
            edad--;
        }
        return edad;
    }

    bool agregarNota(double nota) {
        if (numNotas >= MAX_NOTAS) return false;
        notas[numNotas++] = nota;
        return true;
    }

    double getNota(int indice) const {
        if (indice < 0 || indice >= numNotas) return -1.0;
        return notas[indice];
    }

    bool modificarNota(int indice, double nota) {
        if (indice < 0 || indice >= numNotas) return false;
        notas[indice] = nota;
        return true;
    }

    bool eliminarNota(int indice) {
        if (indice < 0 || indice >= numNotas) return false;
        for (int i = indice; i < numNotas - 1; i++) {
            notas[i] = notas[i + 1];
        }
        numNotas--;
        return true;
    }

    double calcularPromedio() const {
        if (numNotas == 0) return 0.0;
        double suma = 0.0;
        for (int i = 0; i < numNotas; i++) suma += notas[i];
        return suma / numNotas;
    }

    string toString() const {
        stringstream ss;
        ss << "Cedula: " << cedula
           << " | Nombres: " << nombres
           << " | Apellidos: " << apellidos
           << " | F.Nac: " << fechaNacimiento;
        return ss.str();
    }

    string notasToString() const {
        if (numNotas == 0) return "[ Sin calificaciones ]";
        stringstream ss;
        ss << fixed << setprecision(1);
        for (int i = 0; i < numNotas; i++) {
            ss << "[" << notas[i] << "] ";
        }
        ss << "| Promedio: " << fixed << setprecision(2) << calcularPromedio();
        return ss.str();
    }
};

// ============================================================
// TDA GESTOR DE ESTUDIANTES
// ============================================================

class GestorEstudiantes {
private:
    static const int CAPACIDAD = 20;
    Estudiante estudiantes[CAPACIDAD];
    int cantidad;

public:
    GestorEstudiantes() : cantidad(0) {}

    bool estaVacio() const { return cantidad == 0; }
    bool estaLleno() const { return cantidad >= CAPACIDAD; }
    int getCantidad() const { return cantidad; }
    int getCapacidad() const { return CAPACIDAD; }

    Estudiante* buscarPorCedula(const string& cedula) {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getCedula() == cedula) return &estudiantes[i];
        }
        return nullptr;
    }

    Estudiante* buscarPorIndice(int indice) {
        if (indice < 0 || indice >= cantidad) return nullptr;
        return &estudiantes[indice];
    }

    bool registrar(const Estudiante& nuevo) {
        if (estaLleno() || buscarPorCedula(nuevo.getCedula()) != nullptr) return false;
        estudiantes[cantidad++] = nuevo;
        return true;
    }

    bool modificar(int indice, const string& nombres, const string& apellidos, const string& fechaNacimiento) {
        if (indice < 0 || indice >= cantidad) return false;
        estudiantes[indice].setNombres(nombres);
        estudiantes[indice].setApellidos(apellidos);
        estudiantes[indice].setFechaNacimiento(fechaNacimiento);
        return true;
    }

    bool eliminar(int indice) {
        if (indice < 0 || indice >= cantidad) return false;
        for (int i = indice; i < cantidad - 1; i++) {
            estudiantes[i] = estudiantes[i + 1];
        }
        cantidad--;
        return true;
    }

    bool tieneNotasRegistradas() const {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getNumNotas() > 0) return true;
        }
        return false;
    }

    double calcularPromedioGeneral() const {
        double sumaPromedios = 0.0;
        int estudiantesConNotas = 0;
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getNumNotas() > 0) {
                sumaPromedios += estudiantes[i].calcularPromedio();
                estudiantesConNotas++;
            }
        }
        if (estudiantesConNotas == 0) return 0.0;
        return sumaPromedios / estudiantesConNotas;
    }

    void mostrarTabla() const {
        if (estaVacio()) return;

        cout << "\n==============================================\n"
             << "LISTADO DE ESTUDIANTES (" << cantidad << "/" << CAPACIDAD << ")\n"
             << "==============================================\n";
        for (int i = 0; i < cantidad; i++) {
            cout << (i + 1) << ". " << estudiantes[i].toString() << endl;
        }
        cout << "==============================================\n";
    }
};

GestorEstudiantes gestor;

// ============================================================
// FUNCIONES DE ENTRADA VALIDADAS
// ============================================================

int leerEntero(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string linea;
        getline(cin, linea);
        try {
            size_t posicion;
            int valor = stoi(linea, &posicion);
            if (posicion == linea.size()) return valor;
        } catch (...) {}
        cout << "Entrada no valida. Ingrese un entero valido.\n";
    }
}

double leerDouble(const string& mensaje, double minimo, double maximo) {
    while (true) {
        cout << mensaje;
        string linea;
        getline(cin, linea);
        try {
            size_t posicion;
            double valor = stod(linea, &posicion);
            if (posicion == linea.size() && valor >= minimo && valor <= maximo) return valor;
        } catch (...) {}
        cout << "Entrada no valida. Ingrese un valor entre " << minimo << " y " << maximo << ".\n";
    }
}

string leerCedulaValida(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string cedula;
        getline(cin, cedula);
        if (validarCedulaEcuador(cedula)) return cedula;
        cout << "Error: Cedula invalida (debe contener 10 digitos validos de Ecuador).\n";
    }
}

string leerTextoValido(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string texto;
        getline(cin, texto);
        if (validarTextoNombres(texto)) return texto;
        cout << "Error: Debe ingresar texto valido (solo letras, min. 2 caracteres por palabra).\n";
    }
}

string leerFechaValida(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string fecha;
        getline(cin, fecha);
        int d, m, a;
        if (validarFechaNacimiento(fecha, d, m, a)) return fecha;
        cout << "Error: Fecha invalida. Use el formato estricto DD/MM/AAAA (ej. 15/08/2002).\n";
    }
}

string leerTextoOpcionalValido(const string& mensaje, const string& actual) {
    while (true) {
        cout << mensaje;
        string texto;
        getline(cin, texto);
        if (texto.empty()) return actual;
        if (validarTextoNombres(texto)) return texto;
        cout << "Error: Texto invalido. Solo se permiten letras.\n";
    }
}

string leerFechaOpcionalValida(const string& mensaje, const string& actual) {
    while (true) {
        cout << mensaje;
        string fecha;
        getline(cin, fecha);
        if (fecha.empty()) return actual;
        int d, m, a;
        if (validarFechaNacimiento(fecha, d, m, a)) return fecha;
        cout << "Error: Fecha invalida. Use formato DD/MM/AAAA.\n";
    }
}

bool solicitarConfirmacion(const string& mensaje) {
    cout << mensaje;
    string respuesta;
    getline(cin, respuesta);
    for (char& c : respuesta) c = tolower(c);
    return respuesta == "s" || respuesta == "si";
}

// ============================================================
// LÓGICA DE SUBMENÚS
// ============================================================

void ejecutarIngresoEstudiantes() {
    bool continuar = true;
    while (continuar) {
        if (gestor.estaLleno()) {
            cout << "\nError: Cupo maximo alcanzado (" << gestor.getCapacidad() << ").\n";
            break;
        }

        cout << "\n--- INGRESAR ESTUDIANTE ---\n";
        string cedula = leerCedulaValida("Cedula: ");

        if (gestor.buscarPorCedula(cedula) != nullptr) {
            cout << "Error: Ya existe un estudiante registrado con esa cedula.\n";
            continuar = solicitarConfirmacion("¿Desea intentar con otra cedula? (s/N): ");
            continue;
        }

        string nombres = leerTextoValido("Nombres: ");
        string apellidos = leerTextoValido("Apellidos: ");
        string fechaNacimiento = leerFechaValida("Fecha de nacimiento (DD/MM/AAAA): ");

        Estudiante nuevo(cedula, nombres, apellidos, fechaNacimiento);

        if (gestor.registrar(nuevo)) {
            cout << "\nEstudiante registrado exitosamente.\n";
            cout << "Total: " << gestor.getCantidad() << "/" << gestor.getCapacidad() << endl;
        }

        if (gestor.estaLleno()) break;

        continuar = solicitarConfirmacion("¿Desea ingresar otro estudiante? (s/N): ");
    }
}

void ejecutarModificarEstudiante() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados para modificar.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        gestor.mostrarTabla();
        int indice = leerEntero("Numero de autonumerico del estudiante a modificar: ") - 1;

        Estudiante* estudiante = gestor.buscarPorIndice(indice);

        if (estudiante == nullptr) {
            cout << "Error: Registro no encontrado.\n";
        } else {
            cout << "\nDatos actuales:\n" << estudiante->toString() << endl;

            string nombres = leerTextoOpcionalValido("Nuevos nombres (Enter para mantener): ", estudiante->getNombres());
            string apellidos = leerTextoOpcionalValido("Nuevos apellidos (Enter para mantener): ", estudiante->getApellidos());
            string fecha = leerFechaOpcionalValida("Nueva fecha de nacimiento (Enter para mantener): ", estudiante->getFechaNacimiento());

            gestor.modificar(indice, nombres, apellidos, fecha);
            cout << "Estudiante modificado exitosamente.\n";
        }

        continuar = solicitarConfirmacion("¿Desea modificar otro estudiante? (s/N): ");
    }
}

void ejecutarEliminarEstudiante() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados para eliminar.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        gestor.mostrarTabla();
        int indice = leerEntero("Numero de autonumerico a eliminar: ") - 1;

        Estudiante* estudiante = gestor.buscarPorIndice(indice);

        if (estudiante == nullptr) {
            cout << "Error: Registro no encontrado.\n";
        } else {
            cout << "\nRegistro seleccionado:\n" << estudiante->toString() << endl;

            if (solicitarConfirmacion("¿Confirmar eliminacion? (s/N): ")) {
                gestor.eliminar(indice);
                cout << "Estudiante eliminado.\n";
            } else {
                cout << "Operacion cancelada.\n";
            }
        }

        if (gestor.estaVacio()) break;
        continuar = solicitarConfirmacion("¿Desea eliminar otro estudiante? (s/N): ");
    }
}

void subMenuEstudiantes() {
    // Si no hay datos registrados, pasa directo a capturar la información
    if (gestor.estaVacio()) {
        ejecutarIngresoEstudiantes();
        if (gestor.estaVacio()) return;
    }

    int opcion;
    do {
        if (!gestor.estaVacio()) {
            gestor.mostrarTabla();
        }

        cout << "\n--- SUBMENU ESTUDIANTES ---\n"
             << "1. Ingresar estudiante\n"
             << "2. Modificar estudiante\n"
             << "3. Eliminar estudiante\n"
             << "0. Volver al menu principal\n";

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: ejecutarIngresoEstudiantes(); break;
            case 2: ejecutarModificarEstudiante(); break;
            case 3: ejecutarEliminarEstudiante(); break;
            case 0: break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}

// ============================================================
// GESTIÓN DE CALIFICACIONES
// ============================================================

void ejecutarAgregarNota(Estudiante* estudiante) {
    if (estudiante->getNumNotas() >= estudiante->getMaxNotas()) {
        cout << "Se han ingresado todas las calificaciones posibles.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        double nota = leerDouble("Ingrese nota (0.0-10.0): ", 0.0, 10.0);
        estudiante->agregarNota(nota);

        cout << "Nota registrada.\n" << estudiante->notasToString() << endl;

        if (estudiante->getNumNotas() >= estudiante->getMaxNotas()) {
            cout << "Se han ingresado todas las calificaciones posibles.\n";
            break;
        }

        continuar = solicitarConfirmacion("¿Desea agregar otra nota? (s/N): ");
    }
}

void ejecutarModificarNota(Estudiante* estudiante) {
    if (estudiante->getNumNotas() == 0) {
        cout << "No hay notas registradas.\n";
        return;
    }

    cout << "\nNotas actuales: " << estudiante->notasToString() << endl;
    int posicion = leerEntero("Indice de nota a modificar (1-" + to_string(estudiante->getNumNotas()) + "): ") - 1;

    if (posicion < 0 || posicion >= estudiante->getNumNotas()) {
        cout << "Error: Indice fuera de rango.\n";
        return;
    }

    cout << fixed << setprecision(1) << "Nota actual: " << estudiante->getNota(posicion) << endl;
    double nuevaNota = leerDouble("Nueva nota (0.0-10.0): ", 0.0, 10.0);

    estudiante->modificarNota(posicion, nuevaNota);
    cout << "Nota actualizada.\n" << estudiante->notasToString() << endl;
}

void ejecutarEliminarNota(Estudiante* estudiante) {
    if (estudiante->getNumNotas() == 0) {
        cout << "No hay notas registradas.\n";
        return;
    }

    cout << "\nNotas actuales: " << estudiante->notasToString() << endl;
    int posicion = leerEntero("Indice de nota a eliminar (1-" + to_string(estudiante->getNumNotas()) + "): ") - 1;

    if (posicion < 0 || posicion >= estudiante->getNumNotas()) {
        cout << "Error: Indice fuera de rango.\n";
        return;
    }

    if (solicitarConfirmacion("¿Eliminar calificacion? (s/N): ")) {
        estudiante->eliminarNota(posicion);
        cout << "Nota eliminada.\n" << estudiante->notasToString() << endl;
    } else {
        cout << "Operacion cancelada.\n";
    }
}

void subMenuCalificaciones() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados. Primero registre estudiantes.\n";
        return;
    }

    Estudiante* estudiante = nullptr;
    while (estudiante == nullptr) {
        string cedula = leerCedulaValida("Ingrese la cedula del estudiante: ");
        estudiante = gestor.buscarPorCedula(cedula);

        if (estudiante == nullptr) {
            cout << "Error: Estudiante no encontrado.\n";
            if (!solicitarConfirmacion("¿Desea intentar con otra cedula? (s/N): ")) return;
        }
    }

    cout << "\nEstudiante encontrado:\n"
         << "Nombres: " << estudiante->getNombres() << "\n"
         << "Apellidos: " << estudiante->getApellidos() << "\n"
         << "Edad: " << estudiante->calcularEdad() << " anios\n";

    int opcion;
    do {
        cout << "\n--- CALIFICACIONES DE ESTUDIANTE ---\n"
             << "Notas (" << estudiante->getNumNotas() << "/" << estudiante->getMaxNotas() << "): "
             << estudiante->notasToString() << "\n"
             << "1. Agregar nota\n"
             << "2. Modificar nota\n"
             << "3. Eliminar nota\n"
             << "0. Volver\n";

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: ejecutarAgregarNota(estudiante); break;
            case 2: ejecutarModificarNota(estudiante); break;
            case 3: ejecutarEliminarNota(estudiante); break;
            case 0: break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}

// ============================================================
// CONSULTAS
// ============================================================

void mostrarPromedioEstudiante() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados.\n";
        return;
    }

    string cedula = leerCedulaValida("Ingrese la cedula del estudiante: ");
    Estudiante* estudiante = gestor.buscarPorCedula(cedula);

    if (estudiante == nullptr) {
        cout << "Error: No se encontro un estudiante con la cedula indicada.\n";
        return;
    }

    cout << "\n--- DATOS DEL ESTUDIANTE ---\n"
         << "Nombres: " << estudiante->getNombres() << "\n"
         << "Apellidos: " << estudiante->getApellidos() << "\n"
         << "Edad: " << estudiante->calcularEdad() << " anios\n"
         << fixed << setprecision(2)
         << "Promedio de calificaciones: " << estudiante->calcularPromedio() << endl;
}

void mostrarPromedioCurso() {
    cout << "\n--- PROMEDIO DEL CURSO ---\n";
    if (!gestor.tieneNotasRegistradas()) {
        cout << "No se han registrado calificaciones de estudiantes.\n";
        return;
    }

    cout << fixed << setprecision(2)
         << "Promedio general del curso: " << gestor.calcularPromedioGeneral() << endl;
}

// ============================================================
// MENÚ PRINCIPAL
// ============================================================

void mostrarMenuPrincipal() {
    cout << "\n==========================================\n"
         << "=== GESTOR DE PERSONAS ===\n"
         << "==========================================\n"
         << "1.- Estudiantes.\n"
         << "2.- Registro de calificaciones.\n"
         << "3.- Determinar el promedio de notas de un estudiante.\n"
         << "4.- Determinar el promedio de notas del curso.\n"
         << "0.- Salir.\n"
         << "==========================================\n";
}

void procesarOpcionPrincipal(int opcion) {
    switch (opcion) {
        case 1: subMenuEstudiantes(); break;
        case 2: subMenuCalificaciones(); break;
        case 3: mostrarPromedioEstudiante(); break;
        case 4: mostrarPromedioCurso(); break;
        case 0: break;
        default: cout << "Opcion no valida.\n";
    }
}

int main() {
    cout << "==========================================\n"
         << "     GESTOR DE ESTUDIANTES\n"
         << "     Cupo maximo: 20 estudiantes\n"
         << "     Maximo notas por estudiante: 7\n"
         << "==========================================\n";

    int opcion;
    do {
        mostrarMenuPrincipal();
        opcion = leerEntero("Teclee su opcion (0-4): ");
        procesarOpcionPrincipal(opcion);
    } while (opcion != 0);

    cout << "\n¡Gracias por usar el sistema!\n";
    return 0;
}
