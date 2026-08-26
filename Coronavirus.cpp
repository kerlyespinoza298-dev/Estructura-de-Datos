#include <iostream>
#include <string>
using namespace std;

// ============================================================
// TDA CORONAVIRUS: Registro y simulacion de una cadena de contagio (C++)
// Persona infectada -> contamina un objeto -> otra persona lo toca
// -> transporta el virus en sus manos -> se infecta al tocarse el rostro
// ============================================================

class Objeto; // declaracion adelantada

// CONCEPTO: clase base abstracta (metodo virtual puro) + encapsulamiento
class Persona {
protected:
    string nombre;                 // no primitivo
    bool infectada;                // CONCEPTO: dato primitivo bool
    bool virusEnManos;             // CONCEPTO: dato primitivo bool
    int minutosVirusEnManos;       // CONCEPTO: dato primitivo int

public:
    Persona(string nombre, bool infectada)
        : nombre(nombre), infectada(infectada), virusEnManos(false), minutosVirusEnManos(0) {}

    virtual ~Persona() {}

    string getNombre() const { return nombre; }
    bool isInfectada() const { return infectada; }
    bool isVirusEnManos() const { return virusEnManos; }

    void tocarObjeto(Objeto& objeto); // implementado despues de declarar Objeto
    void infectar() { infectada = true; }

    // DRY: las clases hijas reutilizan la impresion de datos comunes
    void mostrarDatosComunes() const {
        cout << "Nombre: " << nombre
             << " | Infectada: " << (infectada ? "Si" : "No")
             << " | Virus en manos: " << (virusEnManos ? "Si" : "No");
    }

    // CONCEPTO: metodos virtuales puros -> habilitan polimorfismo
    virtual void tocarseRostro() = 0;
    virtual void mostrarInformacion() const = 0;
};

// Clase que representa la superficie u objeto contaminado
class Objeto {
private:
    string nombre;
    bool contaminado; // CONCEPTO: dato primitivo bool

public:
    Objeto(string nombre) : nombre(nombre), contaminado(false) {}

    void contaminar(Persona& portador) {
        if (portador.isInfectada()) {
            contaminado = true;
        }
    }

    void limpiar() { contaminado = false; }

    string getNombre() const { return nombre; }
    bool isContaminado() const { return contaminado; }
};

void Persona::tocarObjeto(Objeto& objeto) {
    if (objeto.isContaminado()) {
        virusEnManos = true;
        minutosVirusEnManos = 30; // minutos que el virus sobrevive en las manos
    }
}

// CONCEPTO: herencia -> PersonaCuidadosa hereda de Persona
class PersonaCuidadosa : public Persona {
private:
    bool usaMascarilla;
    bool usaAlcohol;

public:
    PersonaCuidadosa(string nombre, bool infectada, bool usaMascarilla, bool usaAlcohol)
        : Persona(nombre, infectada), usaMascarilla(usaMascarilla), usaAlcohol(usaAlcohol) {}

    // CONCEPTO: polimorfismo -> override del metodo virtual puro
    void tocarseRostro() override {
        if (virusEnManos) {
            if (usaAlcohol) {
                virusEnManos = false; // se desinfecta antes de tocarse el rostro
            } else {
                infectar();
            }
        }
    }

    void mostrarInformacion() const override {
        cout << "[CUIDADOSA]  ";
        mostrarDatosComunes();
        cout << " | Mascarilla: " << (usaMascarilla ? "Si" : "No")
             << " | Alcohol: " << (usaAlcohol ? "Si" : "No") << endl;
    }
};

// CONCEPTO: herencia -> PersonaDescuidada hereda de Persona
class PersonaDescuidada : public Persona {
private:
    int frecuenciaTocarRostro;
    bool ignoraSintomas;

public:
    PersonaDescuidada(string nombre, bool infectada, int frecuenciaTocarRostro, bool ignoraSintomas)
        : Persona(nombre, infectada), frecuenciaTocarRostro(frecuenciaTocarRostro), ignoraSintomas(ignoraSintomas) {}

    // CONCEPTO: polimorfismo -> misma firma, comportamiento distinto
    void tocarseRostro() override {
        if (virusEnManos) {
            infectar();
        }
    }

    void mostrarInformacion() const override {
        cout << "[DESCUIDADA] ";
        mostrarDatosComunes();
        cout << " | Frecuencia rostro/hora: " << frecuenciaTocarRostro
             << " | Ignora sintomas: " << (ignoraSintomas ? "Si" : "No") << endl;
    }
};

// ============================================================
// TDA: RegistroSimulacion -> arreglo ESTATICO de punteros Persona*
// ============================================================
class RegistroSimulacion {
private:
    static const int CAPACIDAD = 10;   // primitivo int (constante de clase)
    Persona* personas[CAPACIDAD];      // CONCEPTO: arreglo estatico de tamano fijo
    int cantidad;                      // primitivo int

public:
    RegistroSimulacion() : cantidad(0) {
        for (int i = 0; i < CAPACIDAD; i++) personas[i] = nullptr;
    }

    ~RegistroSimulacion() {
        for (int i = 0; i < cantidad; i++) delete personas[i]; // libera memoria dinamica
    }

    bool estaLleno() const { return cantidad >= CAPACIDAD; }

    bool existeNombre(const string& nombre) const {
        for (int i = 0; i < cantidad; i++) {
            if (personas[i]->getNombre() == nombre) return true;
        }
        return false;
    }

    bool registrar(Persona* nuevaPersona) {
        if (nuevaPersona == nullptr || estaLleno() || existeNombre(nuevaPersona->getNombre())) {
            return false;
        }
        personas[cantidad] = nuevaPersona;
        cantidad++;
        return true;
    }

    Persona* buscarPorNombre(const string& nombre) const {
        for (int i = 0; i < cantidad; i++) {
            if (personas[i]->getNombre() == nombre) return personas[i];
        }
        return nullptr;
    }

    int getCantidad() const { return cantidad; }

    int contarInfectados() const {
        int total = 0;
        for (int i = 0; i < cantidad; i++) {
            if (personas[i]->isInfectada()) total++;
        }
        return total;
    }

    // CONCEPTO: llamada polimorfica a traves del puntero base Persona*
    void simularContactoConRostro() {
        if (cantidad == 0) { cout << "No hay personas registradas." << endl; return; }
        for (int i = 0; i < cantidad; i++) personas[i]->tocarseRostro();
        cout << "Simulacion completada: cada persona decidio tocarse el rostro." << endl;
    }

    void mostrarEstado() const {
        if (cantidad == 0) { cout << "No hay personas registradas." << endl; return; }
        cout << "\n--- ESTADO DEL REGISTRO (" << cantidad << "/" << CAPACIDAD << ") ---" << endl;
        for (int i = 0; i < cantidad; i++) personas[i]->mostrarInformacion();
        cout << "Total de infectados: " << contarInfectados() << endl;
    }
};

// ============================================================
// Funciones de consola (equivalentes a Scanner) y main()
// ============================================================
static RegistroSimulacion registro;
static Objeto objetoCompartido("manija de la puerta");

bool leerBoolean(const string& mensaje) {
    while (true) {
        cout << mensaje;
        string input;
        getline(cin, input);
        for (auto& c : input) c = tolower(c);
        if (input == "s" || input == "si") return true;
        if (input == "n" || input == "no") return false;
        cout << "Ingrese 's' para Si o 'n' para No." << endl;
    }
}

int leerRangoInt(const string& mensaje, int minVal, int maxVal) {
    while (true) {
        cout << mensaje;
        string input;
        getline(cin, input);
        try {
            int val = stoi(input);
            if (val >= minVal && val <= maxVal) return val;
            cout << "El valor debe estar entre " << minVal << " y " << maxVal << "." << endl;
        } catch (...) {
            cout << "Debe ingresar un numero entero valido." << endl;
        }
    }
}

string leerNombreUnico() {
    while (true) {
        cout << "Nombre: ";
        string nombre;
        getline(cin, nombre);
        if (nombre.empty()) {
            cout << "El nombre no puede estar vacio." << endl;
        } else if (registro.existeNombre(nombre)) {
            cout << "Error: ya existe una persona registrada con el nombre '" << nombre << "'." << endl;
        } else {
            return nombre;
        }
    }
}

// DRY: un solo flujo central para registrar cualquier tipo de persona
void registrarPersona(bool esCuidadosa) {
    if (registro.estaLleno()) {
        cout << "Error: el registro estatico esta lleno." << endl;
        return;
    }
    cout << (esCuidadosa ? "\n--- REGISTRO DE PERSONA CUIDADOSA ---" : "\n--- REGISTRO DE PERSONA DESCUIDADA ---") << endl;
    string nombre = leerNombreUnico();
    bool infectada = leerBoolean("Esta infectada? (s/n): ");

    Persona* nueva;
    if (esCuidadosa) {
        bool usaMascarilla = leerBoolean("Usa mascarilla? (s/n): ");
        bool usaAlcohol = leerBoolean("Usa alcohol/gel para desinfectarse? (s/n): ");
        nueva = new PersonaCuidadosa(nombre, infectada, usaMascarilla, usaAlcohol); // CONCEPTO: instanciacion (heap)
    } else {
        int frecuencia = leerRangoInt("Frecuencia con que se toca el rostro por hora (1 - 30): ", 1, 30);
        bool ignoraSintomas = leerBoolean("Ignora los sintomas? (s/n): ");
        nueva = new PersonaDescuidada(nombre, infectada, frecuencia, ignoraSintomas);
    }

    if (registro.registrar(nueva)) {
        cout << "Persona registrada con exito." << endl;
    } else {
        delete nueva; // si no se pudo registrar, se libera para evitar fuga de memoria
    }
}

void contaminarObjeto() {
    cout << "Nombre de la persona infectada que toca el objeto: ";
    string nombre;
    getline(cin, nombre);
    Persona* p = registro.buscarPorNombre(nombre);
    if (p == nullptr) { cout << "Esa persona no esta registrada." << endl; return; }
    objetoCompartido.contaminar(*p);
    cout << (p->isInfectada() ? "El objeto (" + objetoCompartido.getNombre() + ") quedo contaminado."
                              : p->getNombre() + " no esta infectada; el objeto no se contamino.") << endl;
}

void tocarObjeto() {
    cout << "Nombre de la persona que toca el objeto: ";
    string nombre;
    getline(cin, nombre);
    Persona* p = registro.buscarPorNombre(nombre);
    if (p == nullptr) { cout << "Esa persona no esta registrada." << endl; return; }
    p->tocarObjeto(objetoCompartido);
    cout << (objetoCompartido.isContaminado() ? "El virus paso a las manos de " + p->getNombre() + "."
                                               : "El objeto no estaba contaminado.") << endl;
}

int main() {
    int opcion = 0;
    do {
        cout << "\n=== TDA CORONAVIRUS: CADENA DE CONTAGIO ===" << endl;
        cout << "1. Registrar persona cuidadosa" << endl;
        cout << "2. Registrar persona descuidada" << endl;
        cout << "3. Persona infectada contamina el objeto" << endl;
        cout << "4. Otra persona toca el objeto" << endl;
        cout << "5. Simular que todos se tocan el rostro (Polimorfismo)" << endl;
        cout << "6. Mostrar estado del registro" << endl;
        cout << "7. Salir" << endl;
        cout << "Seleccione una opcion: ";

        string linea;
        getline(cin, linea);
        try { opcion = stoi(linea); } catch (...) { opcion = 0; }

        switch (opcion) {
            case 1: registrarPersona(true); break;
            case 2: registrarPersona(false); break;
            case 3: contaminarObjeto(); break;
            case 4: tocarObjeto(); break;
            case 5: registro.simularContactoConRostro(); break;
            case 6: registro.mostrarEstado(); break;
            case 7: cout << "Saliendo del programa..." << endl; break;
            default: cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 7);

    // El destructor de RegistroSimulacion libera con delete cada Persona* reservada con new
    return 0;
}
