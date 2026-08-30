#include "Menu.hpp"
#include <iostream>
#include <algorithm>

Menu::Menu(RegistroEstudiantes& reg) : registro(reg) {}

void Menu::mostrar() const {
    std::cout << "\n======================================================================\n";
    std::cout << "SISTEMA DE REGISTRO DE ESTUDIANTES (TDA ESTATICO - C++)\n";
    std::cout << "Capacidad: " << registro.getCapacidad() << " estudiantes\n";
    std::cout << "======================================================================\n";
    std::cout << "1. Registrar estudiante\n";
    std::cout << "2. Listar estudiantes\n";
    std::cout << "3. Buscar estudiante por ID\n";
    std::cout << "4. Modificar estudiante\n";
    std::cout << "5. Eliminar estudiante\n";
    std::cout << "6. Verificar integridad del arreglo\n";
    std::cout << "0. Salir\n";
    std::cout << "======================================================================\n";
    std::cout << "Seleccione una opcion: ";
}

void Menu::registrar() {
    if (registro.estaLleno()) {
        std::cout << "Error: Capacidad maxima alcanzada (" << registro.getCapacidad() << ").\n";
        return;
    }

    std::cout << "\n--- REGISTRAR ESTUDIANTE ---\n";
    long id = leerLong("ID (8-10 digitos): ");

    if (registro.buscarPorId(id) != nullptr) {
        std::cout << "Error: El ID " << id << " ya esta registrado.\n";
        return;
    }

    std::string nombre = leerTexto("Nombre completo: ");
    int edad = leerEnteroRango("Edad (15-100): ", 15, 100);
    double promedio = leerDoubleRango("Promedio (0.0-10.0): ", 0.0, 10.0);

    try {
        Estudiante nuevo(id, nombre, edad, promedio);
        if (registro.registrar(nuevo)) {
            std::cout << "Estudiante registrado exitosamente.\n";
            std::cout << "Total: " << registro.getCantidad() << "/" << registro.getCapacidad() << "\n";
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void Menu::listar() const {
    if (registro.estaVacio()) {
        std::cout << "El registro esta vacio.\n";
        return;
    }

    std::cout << "\n======================================================================\n";
    std::cout << "LISTADO DE ESTUDIANTES (" << registro.getCantidad() << "/" << registro.getCapacidad() << ")\n";
    std::cout << "======================================================================\n";

    auto lista = registro.obtenerTodos();
    for (size_t i = 0; i < lista.size(); ++i) {
        std::cout << (i + 1) << ". " << lista[i].toString() << "\n";
    }
    std::cout << "======================================================================\n";
}

void Menu::buscar() const {
    if (registro.estaVacio()) {
        std::cout << "El registro esta vacio.\n";
        return;
    }

    long id = const_cast<Menu*>(this)->leerLong("Ingrese el ID a buscar: ");
    const Estudiante* est = registro.buscarPorId(id);

    if (est != nullptr) {
        std::cout << "\nEstudiante encontrado:\n" << est->toString() << "\n";
    } else {
        std::cout << "No se encontro estudiante con el ID " << id << "\n";
    }
}

void Menu::modificar() {
    if (registro.estaVacio()) {
        std::cout << "El registro esta vacio.\n";
        return;
    }

    long id = leerLong("Ingrese el ID del estudiante a modificar: ");
    const Estudiante* est = registro.buscarPorId(id);

    if (est == nullptr) {
        std::cout << "No existe estudiante con el ID " << id << "\n";
        return;
    }

    std::cout << "\nDatos actuales:\n" << est->toString() << "\n";
    std::cout << "\nIngrese los nuevos datos (Enter para mantener el valor actual):\n";

    std::string nombre = leerTextoOpcional("Nuevo nombre: ", est->getNombre());
    int edad = leerEnteroOpcional("Nueva edad (15-100): ", est->getEdad(), 15, 100);
    double promedio = leerDoubleOpcional("Nuevo promedio (0.0-10.0): ", est->getPromedio(), 0.0, 10.0);

    try {
        if (registro.modificar(id, nombre, edad, promedio)) {
            std::cout << "Estudiante modificado con exito.\nDatos actualizados:\n";
            std::cout << registro.buscarPorId(id)->toString() << "\n";
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Error en la modificacion: " << e.what() << "\n";
        std::cout << "Los datos no han sido modificados.\n";
    }
}

void Menu::eliminar() {
    if (registro.estaVacio()) {
        std::cout << "El registro esta vacio.\n";
        return;
    }

    long id = leerLong("Ingrese el ID del estudiante a eliminar: ");
    const Estudiante* est = registro.buscarPorId(id);

    if (est == nullptr) {
        std::cout << "No existe estudiante con el ID " << id << "\n";
        return;
    }

    std::cout << "\nEstudiante a eliminar:\n" << est->toString() << "\n";
    std::cout << "Confirmar eliminacion? (s/N): ";

    std::string confirm;
    std::getline(std::cin, confirm);
    std::transform(confirm.begin(), confirm.end(), confirm.begin(), ::tolower);

    if (confirm == "s" || confirm == "si") {
        if (registro.eliminar(id)) {
            std::cout << "Estudiante eliminado correctamente.\n";
            std::cout << "Total restante: " << registro.getCantidad() << "/" << registro.getCapacidad() << "\n";
        }
    } else {
        std::cout << "Operacion cancelada.\n";
    }
}

void Menu::verificar() const {
    if (registro.verificarIntegridad()) {
        std::cout << "El arreglo es contiguo y no tiene espacios intermedios.\n";
    } else {
        std::cout << "Atencion: Existen espacios nulos intercalados.\n";
    }
}

std::string Menu::leerTexto(const std::string& mensaje) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        if (!input.empty()) return input;
        std::cout << "El campo no puede estar vacio.\n";
    }
}

std::string Menu::leerTextoOpcional(const std::string& mensaje, const std::string& actual) {
    std::cout << mensaje;
    std::string input;
    std::getline(std::cin, input);
    return input.empty() ? actual : input;
}

int Menu::leerEnteroRango(const std::string& mensaje, int min, int max) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        try {
            int valor = std::stoi(input);
            if (valor >= min && valor <= max) return valor;
            std::cout << "El valor debe estar entre " << min << " y " << max << ".\n";
        } catch (...) {
            std::cout << "Debe ingresar un numero entero valido.\n";
        }
    }
}

int Menu::leerEnteroOpcional(const std::string& mensaje, int actual, int min, int max) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        if (input.empty()) return actual;
        try {
            int valor = std::stoi(input);
            if (valor >= min && valor <= max) return valor;
            std::cout << "El valor debe estar entre " << min << " y " << max << ".\n";
        } catch (...) {
            std::cout << "Debe ingresar un numero entero valido.\n";
        }
    }
}

long Menu::leerLong(const std::string& mensaje) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        try {
            long valor = std::stol(input);
            if (valor >= 10000000L && valor <= 9999999999L) return valor;
            std::cout << "El ID debe tener entre 8 y 10 digitos.\n";
        } catch (...) {
            std::cout << "Debe ingresar un numero valido.\n";
        }
    }
}

double Menu::leerDoubleRango(const std::string& mensaje, double min, double max) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        try {
            double valor = std::stod(input);
            if (valor >= min && valor <= max) return valor;
            std::cout << "El valor debe estar entre " << min << " y " << max << ".\n";
        } catch (...) {
            std::cout << "Debe ingresar un numero decimal valido.\n";
        }
    }
}

double Menu::leerDoubleOpcional(const std::string& mensaje, double actual, double min, double max) {
    std::string input;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);
        if (input.empty()) return actual;
        try {
            double valor = std::stod(input);
            if (valor >= min && valor <= max) return valor;
            std::cout << "El valor debe estar entre " << min << " y " << max << ".\n";
        } catch (...) {
            std::cout << "Debe ingresar un numero decimal valido.\n";
        }
    }
}