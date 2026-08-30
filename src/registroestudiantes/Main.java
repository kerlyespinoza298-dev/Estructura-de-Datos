package registroestudiantes;

import registroestudiantes.model.Estudiante;
import registroestudiantes.tda.RegistroEstudiantes;
import registroestudiantes.ui.Menu;
import java.util.Scanner;

public class Main {
    private static final Scanner scanner = new Scanner(System.in);
    private static final RegistroEstudiantes registro = new RegistroEstudiantes();
    private static final Menu menu = new Menu(scanner, registro);

    public static void main(String[] args) {
        registrarEstudiantesEjemplo();

        int opcion;
        do {
            menu.mostrar();
            opcion = leerOpcion();
            procesarOpcion(opcion);
        } while (opcion != 0);

        scanner.close();
        System.out.println("\n¡Gracias por usar el sistema!");
    }

    private static void registrarEstudiantesEjemplo() {
        Estudiante[] ejemplos = {
            new Estudiante(12345678L, "Ana Maria Perez", 20, 8.5),
            new Estudiante(23456789L, "Carlos Jose Gomez", 22, 7.8),
            new Estudiante(34567890L, "Maria Fernanda Lopez", 19, 9.2),
            new Estudiante(45678901L, "Juan Pablo Ruiz", 21, 6.5),
            new Estudiante(56789012L, "Laura Isabel Mora", 23, 9.8)
        };
        
        System.out.println("Registrando estudiantes de ejemplo...");
        for (Estudiante est : ejemplos) {
            registro.registrar(est);
        }
        System.out.println(registro.getCantidad() + " estudiantes registrados.");
    }

    private static int leerOpcion() {
        try {
            return Integer.parseInt(scanner.nextLine().trim());
        } catch (NumberFormatException e) {
            return -1;
        }
    }

    private static void procesarOpcion(int opcion) {
        switch (opcion) {
            case 1 -> menu.registrar();
            case 2 -> menu.listar();
            case 3 -> menu.buscar();
            case 4 -> menu.modificar();
            case 5 -> menu.eliminar();
            case 6 -> menu.verificar();
            case 0 -> { }
            default -> System.out.println("Opcion no valida.");
        }
    }
}