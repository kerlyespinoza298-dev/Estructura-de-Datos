#ifndef ESTUDIANTE_HPP
#define ESTUDIANTE_HPP

#include <string>

class Estudiante {
private:
    long id;
    std::string nombre;
    int edad;
    double promedio;

    static void validarId(long id);
    static void validarNombre(const std::string& nombre);
    static void validarEdad(int edad);
    static void validarPromedio(double promedio);
    static std::string trim(const std::string& str);

public:
    Estudiante();
    Estudiante(long id, const std::string& nombre, int edad, double promedio);

    long getId() const;
    std::string getNombre() const;
    int getEdad() const;
    double getPromedio() const;

    void setNombre(const std::string& nombre);
    void setEdad(int edad);
    void setPromedio(double promedio);

    void actualizarDatos(const std::string& nombre, int edad, double promedio);
    std::string toString() const;
};

#endif