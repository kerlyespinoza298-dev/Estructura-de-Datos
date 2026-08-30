#include "Estudiante.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>

std::string Estudiante::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void Estudiante::validarId(long id) {
    if (id < 10000000L || id > 9999999999L) {
        throw std::invalid_argument("El ID debe contener entre 8 y 10 digitos.");
    }
}

void Estudiante::validarNombre(const std::string& nombre) {
    std::string limpio = trim(nombre);
    if (limpio.empty()) {
        throw std::invalid_argument("El nombre no puede estar vacio.");
    }
    if (limpio.length() < 2) {
        throw std::invalid_argument("El nombre debe tener al menos 2 caracteres.");
    }
}

void Estudiante::validarEdad(int edad) {
    if (edad < 15 || edad > 100) {
        throw std::invalid_argument("La edad debe estar entre 15 y 100 anos.");
    }
}

void Estudiante::validarPromedio(double promedio) {
    if (promedio < 0.0 || promedio > 10.0) {
        throw std::invalid_argument("El promedio debe estar entre 0.0 y 10.0.");
    }
}

Estudiante::Estudiante() : id(0), nombre(""), edad(0), promedio(0.0) {}

Estudiante::Estudiante(long id, const std::string& nombre, int edad, double promedio) {
    validarId(id);
    this->id = id;
    actualizarDatos(nombre, edad, promedio);
}

long Estudiante::getId() const { return id; }
std::string Estudiante::getNombre() const { return nombre; }
int Estudiante::getEdad() const { return edad; }
double Estudiante::getPromedio() const { return promedio; }

void Estudiante::setNombre(const std::string& nombre) {
    validarNombre(nombre);
    this->nombre = trim(nombre);
}

void Estudiante::setEdad(int edad) {
    validarEdad(edad);
    this->edad = edad;
}

void Estudiante::setPromedio(double promedio) {
    validarPromedio(promedio);
    this->promedio = promedio;
}

void Estudiante::actualizarDatos(const std::string& nombre, int edad, double promedio) {
    validarNombre(nombre);
    validarEdad(edad);
    validarPromedio(promedio);
    this->nombre = trim(nombre);
    this->edad = edad;
    this->promedio = promedio;
}

std::string Estudiante::toString() const {
    std::ostringstream oss;
    oss << "ID: " << std::left << std::setw(10) << id
        << " | Nombre: " << std::setw(25) << nombre
        << " | Edad: " << std::right << std::setw(3) << edad
        << " | Promedio: " << std::fixed << std::setprecision(2) << std::setw(5) << promedio;
    return oss.str();
}