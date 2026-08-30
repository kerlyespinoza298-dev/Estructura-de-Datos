#ifndef MENU_HPP
#define MENU_HPP

#include "RegistroEstudiantes.hpp"
#include <string>

class Menu {
private:
    RegistroEstudiantes& registro;

    std::string leerTexto(const std::string& mensaje);
    std::string leerTextoOpcional(const std::string& mensaje, const std::string& actual);
    int leerEnteroRango(const std::string& mensaje, int min, int max);
    int leerEnteroOpcional(const std::string& mensaje, int actual, int min, int max);
    long leerLong(const std::string& mensaje);
    double leerDoubleRango(const std::string& mensaje, double min, double max);
    double leerDoubleOpcional(const std::string& mensaje, double actual, double min, double max);

public:
    explicit Menu(RegistroEstudiantes& reg);

    void mostrar() const;
    void registrar();
    void listar() const;
    void buscar() const;
    void modificar();
    void eliminar();
    void verificar() const;
};

#endif