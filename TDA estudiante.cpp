#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cctype>

using namespace std;

// ============================================================
// VALIDACIONES DE ENTRADA
// ============================================================

bool esNumero(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

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

    diaOut = dia; mesOut = mes; anioOut = anio;
    return true;
}

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
            return false;
        }
    }
    if (letrasSeguidas > 0 && letrasSeguidas < 2) return false;
    return tieneLetras;
}

// ============================================================
// CLASE ESTUDIANTE (REQUERIMIENTO 1 Y 2)
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
    Estudiante() : cedula(""), nombres(""), apellidos(""), fechaNacimiento(""), numNotas(0) {}

    Estudiante(string cedula, string nombres, string apellidos, string fechaNacimiento)
        : cedula(cedula), nombres(nombres), apellidos(apellidos), fechaNacimiento(fechaNacimiento), numNotas(0) {}

    string getCedula() const { return cedula; }
    string getNombres() const { return nombres; }
    string getApellidos() const { return apellidos; }
    string getFechaNacimiento() const { return fechaNacimiento; }
    int getNumNotas() const { return numNotas; }
    int getMaxNotas() const { return MAX_NOTAS; }

    void setNombres(const string& n) { nombres = n; }
    void setApellidos(const string& a) { apellidos = a; }
    void setFechaNacimiento(const string& f) { fechaNacimiento = f; }

    int calcularEdad() const {
        int dia, mes, anio;
        if (!validarFechaNacimiento(fechaNacimiento, dia, mes, anio)) return -1;
        time_t t = time(nullptr);
        tm* actual = localtime(&t);
        int edad = (actual->tm_year + 1900) - anio;
        if ((actual->tm_mon + 1) < mes || ((actual->tm_mon + 1) == mes && actual->tm_mday < dia)) edad--;
        return edad;
    }

    bool agregarNota(double nota) {
        if (numNotas >= MAX_NOTAS) return false;
        notas[numNotas++] = nota;
        return true;
    }

    double getNota(int i) const { return (i >= 0 && i < numNotas) ? notas[i] : -1.0; }

    bool modificarNota(int i, double nota) {
        if (i < 0 || i >= numNotas) return false;
        notas[i] = nota;
        return true;
    }

    bool eliminarNota(int i) {
        if (i < 0 || i >= numNotas) return false;
        for (int j = i; j < numNotas - 1; j++) notas[j] = notas[j + 1];
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
        ss << "Cedula: " << cedula << " | Nombres: " << nombres << " | Apellidos: " << apellidos << " | F.Nac: " << fechaNacimiento;
        return ss.str();
    }

    string notasToString() const {
        if (numNotas == 0) return "[ Sin calificaciones ]";
        stringstream ss;
        ss << fixed << setprecision(1);
        for (int i = 0; i < numNotas; i++) ss << "[" << notas[i] << "] ";
        ss << "| Promedio: " << fixed << setprecision(2) << calcularPromedio();
        return ss.str();
    }
};

// ============================================================
// CLASE GESTOR DE ESTUDIANTES (MÉTODO BUSCAR EXPLICITO)
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

    // Requerimiento 2: Método buscar() explicito por cédula
    Estudiante* buscar(const string& cedula) {
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
        if (estaLleno() || buscar(nuevo.getCedula()) != nullptr) return false;
        estudiantes[cantidad++] = nuevo;
        return true;
    }

    bool modificar(int i, const string& n, const string& a, const string& f) {
        if (i < 0 || i >= cantidad) return false;
        estudiantes[i].setNombres(n);
        estudiantes[i].setApellidos(a);
        estudiantes[i].setFechaNacimiento(f);
        return true;
    }

    bool eliminar(int i) {
        if (i < 0 || i >= cantidad) return false;
        for (int j = i; j < cantidad - 1; j++) estudiantes[j] = estudiantes[j + 1];
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
        int cont = 0;
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getNumNotas() > 0) {
                sumaPromedios += estudiantes[i].calcularPromedio();
                cont++;
            }
        }
        return (cont == 0) ? 0.0 : (sumaPromedios / cont);
    }

    void mostrarTabla() const {
        cout << "\n==============================================\n";
        if (estaVacio()) {
            cout << "LISTADO DE ESTUDIANTES: (No hay estudiantes registrados)\n";
        } else {
            cout << "LISTADO DE ESTUDIANTES (" << cantidad << "/" << CAPACIDAD << ")\n";
            cout << "==============================================\n";
            for (int i = 0; i < cantidad; i++) {
                cout << (i + 1) << ". " << estudiantes[i].toString() << endl;
            }
        }
        cout << "==============================================\n";
    }
};

GestorEstudiantes gestor;

// ============================================================
// FUNCIONES DE LECTURA VALIDADAS
// ============================================================

int leerEntero(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string linea;
        getline(cin, linea);
        try {
            size_t pos;
            int v = stoi(linea, &pos);
            if (pos == linea.size()) return v;
        } catch (...) {}
        cout << "Entrada no valida. Ingrese un entero valido.\n";
    }
}

double leerDouble(const string& mensaje, double min, double max) {
    while (true) {
        cout << mensaje;
        string linea;
        getline(cin, linea);
        try {
            size_t pos;
            double v = stod(linea, &pos);
            if (pos == linea.size() && v >= min && v <= max) return v;
        } catch (...) {}
        cout << "Entrada no valida. Ingrese un valor entre " << min << " y " << max << ".\n";
    }
}

string leerCedulaValida(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string c; getline(cin, c);
        if (validarCedulaEcuador(c)) return c;
        cout << "Error: Cedula invalida (debe contener 10 digitos validos de Ecuador).\n";
    }
}

string leerTextoValido(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string t; getline(cin, t);
        if (validarTextoNombres(t)) return t;
        cout << "Error: Debe ingresar texto valido (solo letras, min. 2 caracteres por palabra).\n";
    }
}

string leerFechaValida(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string f; getline(cin, f);
        int d, m, a;
        if (validarFechaNacimiento(f, d, m, a)) return f;
        cout << "Error: Fecha invalida. Use el formato estricto DD/MM/AAAA.\n";
    }
}

string leerTextoOpcionalValido(const string& mensaje, const string& actual) {
    while (true) {
        cout << mensaje;
        string t; getline(cin, t);
        if (t.empty()) return actual;
        if (validarTextoNombres(t)) return t;
        cout << "Error: Texto invalido. Solo se permiten letras.\n";
    }
}

string leerFechaOpcionalValida(const string& mensaje, const string& actual) {
    while (true) {
        cout << mensaje;
        string f; getline(cin, f);
        if (f.empty()) return actual;
        int d, m, a;
        if (validarFechaNacimiento(f, d, m, a)) return f;
        cout << "Error: Fecha invalida. Use formato DD/MM/AAAA.\n";
    }
}

bool solicitarConfirmacion(const string& mensaje) {
    cout << mensaje;
    string r; getline(cin, r);
    for (char& c : r) c = tolower(c);
    return r == "s" || r == "si";
}

// ============================================================
// OPCIÓN 1: SUBMENÚ ESTUDIANTES
// ============================================================

void ejecutarIngresoEstudiantes() {
    bool continuar = true;
    while (continuar) {
        if (gestor.estaLleno()) {
            cout << "\nNo se permite insertar: Cupo maximo alcanzado (" << gestor.getCapacidad() << ").\n";
            break;
        }

        cout << "\n--- INGRESAR ESTUDIANTE ---\n";
        string cedula = leerCedulaValida("Cedula: ");

        if (gestor.buscar(cedula) != nullptr) {
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
        cout << "\nNo hay estudiantes registrados. No es posible modificar.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        gestor.mostrarTabla();
        int indice = leerEntero("Indique el autonumerico del estudiante a modificar: ") - 1;

        Estudiante* est = gestor.buscarPorIndice(indice);
        if (est == nullptr) {
            cout << "Error: Registro no encontrado.\n";
        } else {
            cout << "\nDatos actuales:\n" << est->toString() << endl;
            string n = leerTextoOpcionalValido("Nuevos nombres (Enter para mantener): ", est->getNombres());
            string a = leerTextoOpcionalValido("Nuevos apellidos (Enter para mantener): ", est->getApellidos());
            string f = leerFechaOpcionalValida("Nueva fecha de nacimiento (Enter para mantener): ", est->getFechaNacimiento());

            gestor.modificar(indice, n, a, f);
            cout << "Estudiante modificado exitosamente.\n";
        }
        continuar = solicitarConfirmacion("¿Desea modificar otro estudiante? (s/N): ");
    }
}

void ejecutarEliminarEstudiante() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados. No se permite eliminar.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        gestor.mostrarTabla();
        int indice = leerEntero("Indique el autonumerico del estudiante a eliminar: ") - 1;

        Estudiante* est = gestor.buscarPorIndice(indice);
        if (est == nullptr) {
            cout << "Error: Registro no encontrado.\n";
        } else {
            cout << "\nRegistro seleccionado:\n" << est->toString() << endl;
            if (solicitarConfirmacion("¿Confirmar eliminacion? (s/N): ")) {
                gestor.eliminar(indice);
                cout << "Estudiante eliminado exitosamente.\n";
            } else {
                cout << "Operacion cancelada.\n";
            }
        }

        if (gestor.estaVacio()) break;
        continuar = solicitarConfirmacion("¿Desea eliminar otro estudiante? (s/N): ");
    }
}

void subMenuEstudiantes() {
    int opcion;
    do {
        // Muestra el listado autonumérico siempre al inicio de la Opción 1
        gestor.mostrarTabla();

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
// OPCIÓN 2: REGISTRO DE CALIFICACIONES
// ============================================================

void ejecutarAgregarNota(Estudiante* est) {
    if (est->getNumNotas() >= est->getMaxNotas()) {
        cout << "\nSe han ingresado todas las calificaciones posibles.\n";
        return;
    }

    bool continuar = true;
    while (continuar) {
        double nota = leerDouble("Ingrese nota (0.0-10.0): ", 0.0, 10.0);
        est->agregarNota(nota);

        cout << "Nota registrada.\n" << est->notasToString() << endl;

        if (est->getNumNotas() >= est->getMaxNotas()) {
            cout << "\nSe han ingresado todas las calificaciones posibles.\n";
            break;
        }

        continuar = solicitarConfirmacion("¿Desea agregar otra nota? (s/N): ");
    }
}

void ejecutarModificarNota(Estudiante* est) {
    if (est->getNumNotas() == 0) {
        cout << "No hay notas registradas.\n";
        return;
    }

    cout << "\nNotas actuales: " << est->notasToString() << endl;
    int pos = leerEntero("Indice de nota a modificar (1-" + to_string(est->getNumNotas()) + "): ") - 1;

    if (pos < 0 || pos >= est->getNumNotas()) {
        cout << "Error: Indice fuera de rango.\n";
        return;
    }

    cout << fixed << setprecision(1) << "Nota actual: " << est->getNota(pos) << endl;
    double nuevaNota = leerDouble("Nueva nota (0.0-10.0): ", 0.0, 10.0);

    est->modificarNota(pos, nuevaNota);
    cout << "Nota actualizada.\n" << est->notasToString() << endl;
}

void ejecutarEliminarNota(Estudiante* est) {
    if (est->getNumNotas() == 0) {
        cout << "No hay notas registradas.\n";
        return;
    }

    cout << "\nNotas actuales: " << est->notasToString() << endl;
    int pos = leerEntero("Indice de nota a eliminar (1-" + to_string(est->getNumNotas()) + "): ") - 1;

    if (pos < 0 || pos >= est->getNumNotas()) {
        cout << "Error: Indice fuera de rango.\n";
        return;
    }

    if (solicitarConfirmacion("¿Eliminar calificacion? (s/N): ")) {
        est->eliminarNota(pos);
        cout << "Nota eliminada.\n" << est->notasToString() << endl;
    } else {
        cout << "Operacion cancelada.\n";
    }
}

void subMenuCalificaciones() {
    if (gestor.estaVacio()) {
        cout << "\nNo hay estudiantes registrados. Primero registre estudiantes.\n";
        return;
    }

    Estudiante* est = nullptr;
    while (est == nullptr) {
        string cedula = leerCedulaValida("Ingrese el numero de cedula del estudiante: ");
        est = gestor.buscar(cedula);

        if (est == nullptr) {
            cout << "\nNotificacion: La cedula ingresada no pertenece a un estudiante registrado.\n";
            if (!solicitarConfirmacion("¿Desea ingresar otro numero de cedula? (s/N): ")) return;
        }
    }

    cout << "\n--- DATOS DEL ESTUDIANTE ---\n"
         << "Nombres: " << est->getNombres() << "\n"
         << "Apellidos: " << est->getApellidos() << "\n"
         << "Edad: " << est->calcularEdad() << " anios\n";

    int opcion;
    do {
        cout << "\n--- CALIFICACIONES DE ESTUDIANTE ---\n"
             << "Notas (" << est->getNumNotas() << "/" << est->getMaxNotas() << "): "
             << est->notasToString() << "\n"
             << "1. Agregar nota\n"
             << "2. Modificar nota\n"
             << "3. Eliminar nota\n"
             << "0. Volver al menu principal\n";

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: ejecutarAgregarNota(est); break;
            case 2: ejecutarModificarNota(est); break;
            case 3: ejecutarEliminarNota(est); break;
            case 0: break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}

// ============================================================
// OPCIÓN 3 Y 4: CONSULTAS DE PROMEDIO
// ============================================================

void mostrarPromedioEstudiante() {
    if (gestor.estaVacio()) {
        cout << "\nError: No se encontro un estudiante con el numero de cedula indicado (Lista vacia).\n";
        return;
    }

    string cedula = leerCedulaValida("Ingrese el numero de cedula del estudiante: ");
    Estudiante* est = gestor.buscar(cedula);

    if (est == nullptr) {
        cout << "\nError: No se encontro un estudiante con el numero de cedula indicado.\n";
        return;
    }

    cout << "\n--- DATOS DEL ESTUDIANTE ---\n"
         << "Nombres: " << est->getNombres() << "\n"
         << "Apellidos: " << est->getApellidos() << "\n"
         << "Edad: " << est->calcularEdad() << " anios\n"
         << fixed << setprecision(2)
         << "Promedio de calificaciones: " << est->calcularPromedio() << endl;
}

void mostrarPromedioCurso() {
    cout << "\n--- PROMEDIO GENERAL DEL CURSO ---\n";
    if (!gestor.tieneNotasRegistradas()) {
        cout << "No se han registrado calificaciones de estudiantes.\n";
        return;
    }

    cout << fixed << setprecision(2)
         << "Promedio general de calificaciones: " << gestor.calcularPromedioGeneral() << endl;
}

// ============================================================
// MENÚ PRINCIPAL
// ============================================================

void mostrarMenuPrincipal() {
    cout << "\n=== GESTOR DE PERSONAS ===\n"
         << "1.- Estudiantes.\n"
         << "2.- Registro de calificaciones.\n"
         << "3.- Determinar el promedio de notas de un estudiante.\n"
         << "4.- Determinar el promedio de notas del curso.\n"
         << "0.- Salir.\n";
}

int main() {
    int opcion;
    do {
        mostrarMenuPrincipal();
        opcion = leerEntero("Teclee su opcion (1-4): ");

        switch (opcion) {
            case 1: subMenuEstudiantes(); break;
            case 2: subMenuCalificaciones(); break;
            case 3: mostrarPromedioEstudiante(); break;
            case 4: mostrarPromedioCurso(); break;
            case 0: break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);

    cout << "\nPrograma finalizado.\n";
    return 0;
}
