#include "RegistroEstudiantes.hpp"
#include "Menu.hpp"
#include <iostream>

int main() {
    RegistroEstudiantes registro;
    Menu menu(registro);

    int opcion = -1;
    std::string input;

    do {
        menu.mostrar();
        std::getline(std::cin, input);
        try {
            opcion = std::stoi(input);
        } catch (...) {
            opcion = -1;
        }

        switch (opcion) {
            case 1: menu.registrar(); break;
            case 2: menu.listar(); break;
            case 3: menu.buscar(); break;
            case 4: menu.modificar(); break;
            case 5: menu.eliminar(); break;
            case 6: menu.verificar(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n"; break;
        }
    } while (opcion != 0);

    std::cout << "\nGracias por usar el sistema!\n";
    return 0;
}