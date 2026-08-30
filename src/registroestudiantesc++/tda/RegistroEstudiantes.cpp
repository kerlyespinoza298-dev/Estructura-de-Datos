#include "RegistroEstudiantes.hpp"

RegistroEstudiantes::RegistroEstudiantes() : cantidad(0) {}

bool RegistroEstudiantes::estaVacio() const { return cantidad == 0; }
bool RegistroEstudiantes::estaLleno() const { return cantidad >= CAPACIDAD; }
int RegistroEstudiantes::getCantidad() const { return cantidad; }
int RegistroEstudiantes::getCapacidad() const { return CAPACIDAD; }

int RegistroEstudiantes::buscarIndicePorId(long id) const {
    for (int i = 0; i < cantidad; i++) {
        if (estudiantes[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

const Estudiante* RegistroEstudiantes::buscarPorId(long id) const {
    int indice = buscarIndicePorId(id);
    return (indice != -1) ? &estudiantes[indice] : nullptr;
}

bool RegistroEstudiantes::registrar(const Estudiante& estudiante) {
    if (estaLleno() || buscarPorId(estudiante.getId()) != nullptr) {
        return false;
    }
    estudiantes[cantidad] = estudiante;
    cantidad++;
    return true;
}

std::vector<Estudiante> RegistroEstudiantes::obtenerTodos() const {
    std::vector<Estudiante> copia(estudiantes, estudiantes + cantidad);
    return copia;
}

bool RegistroEstudiantes::modificar(long id, const std::string& nuevoNombre, int nuevaEdad, double nuevoPromedio) {
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        return false;
    }
    estudiantes[indice].actualizarDatos(nuevoNombre, nuevaEdad, nuevoPromedio);
    return true;
}

bool RegistroEstudiantes::eliminar(long id) {
    int indice = buscarIndicePorId(id);
    if (indice == -1) return false;

    for (int i = indice; i < cantidad - 1; i++) {
        estudiantes[i] = estudiantes[i + 1];
    }
    cantidad--;
    return true;
}

bool RegistroEstudiantes::verificarIntegridad() const {
    return cantidad <= CAPACIDAD;
}