#ifndef REGISTROESTUDIANTES_HPP
#define REGISTROESTUDIANTES_HPP

#include "Estudiante.hpp"
#include <vector>

class RegistroEstudiantes {
private:
    static const int CAPACIDAD = 26; // Cambiado a 26
    Estudiante estudiantes[CAPACIDAD];
    int cantidad;

    int buscarIndicePorId(long id) const;

public:
    RegistroEstudiantes();

    bool estaVacio() const;
    bool estaLleno() const;
    int getCantidad() const;
    int getCapacidad() const;

    const Estudiante* buscarPorId(long id) const;
    bool registrar(const Estudiante& estudiante);
    std::vector<Estudiante> obtenerTodos() const;
    bool modificar(long id, const std::string& nuevoNombre, int nuevaEdad, double nuevoPromedio);
    bool eliminar(long id);
    bool verificarIntegridad() const;
};

#endif