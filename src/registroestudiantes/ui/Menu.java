package registroestudiantes.ui;

import registroestudiantes.model.Estudiante;
import registroestudiantes.tda.RegistroEstudiantes;
import java.util.Scanner;

public class Menu {
    private final Scanner scanner;
    private final RegistroEstudiantes registro;

    public Menu(Scanner scanner, RegistroEstudiantes registro) {
        this.scanner = scanner;
        this.registro = registro;
    }

    public void mostrar() {
        System.out.println("\n" + "=".repeat(70));
        System.out.println("SISTEMA DE REGISTRO DE ESTUDIANTES (TDA ESTATICO)");
        System.out.println("Capacidad: " + registro.getCapacidad() + " estudiantes");
        System.out.println("=".repeat(70));
        System.out.println("1. Registrar estudiante");
        System.out.println("2. Listar estudiantes");
        System.out.println("3. Buscar estudiante por ID");
        System.out.println("4. Modificar estudiante");
        System.out.println("5. Eliminar estudiante");
        System.out.println("6. Verificar integridad del arreglo");
        System.out.println("0. Salir");
        System.out.println("=".repeat(70));
        System.out.print("Seleccione una opcion: ");
    }

    public void registrar() {
        if (registro.estaLleno()) {
            System.out.println("Error: Capacidad maxima alcanzada (" + registro.getCapacidad() + ").");
            return;
        }

        System.out.println("\n--- REGISTRAR ESTUDIANTE ---");
        
        long id = leerLong("ID (8-10 digitos): ");
        
        if (registro.buscarPorId(id) != null) {
            System.out.println("Error: El ID " + id + " ya esta registrado.");
            return;
        }

        String nombre = leerTexto("Nombre completo: ");
        int edad = leerEnteroRango("Edad (15-100): ", 15, 100);
        double promedio = leerDoubleRango("Promedio (0.0-10.0): ", 0.0, 10.0);

        try {
            Estudiante nuevo = new Estudiante(id, nombre, edad, promedio);
            if (registro.registrar(nuevo)) {
                System.out.println("Estudiante registrado exitosamente.");
                System.out.println("Total: " + registro.getCantidad() + "/" + registro.getCapacidad());
            }
        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    public void listar() {
        if (registro.estaVacio()) {
            System.out.println("El registro esta vacio.");
            return;
        }
        
        System.out.println("\n" + "=".repeat(70));
        System.out.println("LISTADO DE ESTUDIANTES (" + registro.getCantidad() + "/" + registro.getCapacidad() + ")");
        System.out.println("=".repeat(70));
        
        Estudiante[] estudiantes = registro.obtenerTodos();
        for (int i = 0; i < estudiantes.length; i++) {
            System.out.printf("%2d. %s%n", (i + 1), estudiantes[i]);
        }
        System.out.println("=".repeat(70));
    }

    public void buscar() {
        if (registro.estaVacio()) {
            System.out.println("El registro esta vacio.");
            return;
        }
        
        long id = leerLong("Ingrese el ID a buscar: ");
        if (id == -1) return;
        
        Estudiante est = registro.buscarPorId(id);
        if (est != null) {
            System.out.println("\nEstudiante encontrado:");
            System.out.println(est);
        } else {
            System.out.println("No se encontro estudiante con el ID " + id);
        }
    }

    public void modificar() {
        if (registro.estaVacio()) {
            System.out.println("El registro esta vacio.");
            return;
        }
        
        long id = leerLong("Ingrese el ID del estudiante a modificar: ");
        
        Estudiante est = registro.buscarPorId(id);
        if (est == null) {
            System.out.println("No existe estudiante con el ID " + id);
            return;
        }

        System.out.println("\nDatos actuales:");
        System.out.println(est);
        System.out.println("\nIngrese los nuevos datos (Enter para mantener el valor actual):");

        String nombre = leerTextoOpcional("Nuevo nombre: ", est.getNombre());
        int edad = leerEnteroOpcional("Nueva edad (15-100): ", est.getEdad(), 15, 100);
        double promedio = leerDoubleOpcional("Nuevo promedio (0.0-10.0): ", est.getPromedio(), 0.0, 10.0);

        try {
            if (registro.modificar(id, nombre, edad, promedio)) {
                System.out.println("Estudiante modificado con exito.");
                System.out.println("Datos actualizados:");
                System.out.println(registro.buscarPorId(id));
            }
        } catch (IllegalArgumentException e) {
            System.out.println("Error en la modificacion: " + e.getMessage());
            System.out.println("Los datos no han sido modificados.");
        }
    }

    public void eliminar() {
        if (registro.estaVacio()) {
            System.out.println("El registro esta vacio.");
            return;
        }
        
        long id = leerLong("Ingrese el ID del estudiante a eliminar: ");
        if (id == -1) return;
        
        Estudiante est = registro.buscarPorId(id);
        if (est == null) {
            System.out.println("No existe estudiante con el ID " + id);
            return;
        }

        System.out.println("\nEstudiante a eliminar:");
        System.out.println(est);
        System.out.print("Confirmar eliminacion? (s/N): ");
        String confirm = scanner.nextLine().trim().toLowerCase();
        
        if (confirm.equals("s") || confirm.equals("si")) {
            if (registro.eliminar(id)) {
                System.out.println("Estudiante eliminado correctamente.");
                System.out.println("Total restante: " + registro.getCantidad() + "/" + registro.getCapacidad());
            }
        } else {
            System.out.println("Operacion cancelada.");
        }
    }

    public void verificar() {
        if (registro.verificarIntegridad()) {
            System.out.println("El arreglo es contiguo y no tiene espacios intermedios.");
        } else {
            System.out.println("Atencion: Existen espacios nulos intercalados.");
        }
    }

    // --- Metodos de lectura robustos (DRY) ---

    private String leerTexto(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String input = scanner.nextLine().trim();
            if (!input.isEmpty()) return input;
            System.out.println("El campo no puede estar vacio.");
        }
    }

    private String leerTextoOpcional(String mensaje, String actual) {
        System.out.print(mensaje);
        String input = scanner.nextLine().trim();
        return input.isEmpty() ? actual : input;
    }

    private int leerEnteroRango(String mensaje, int min, int max) {
        while (true) {
            try {
                System.out.print(mensaje);
                int valor = Integer.parseInt(scanner.nextLine().trim());
                if (valor >= min && valor <= max) return valor;
                System.out.println("El valor debe estar entre " + min + " y " + max + ".");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un numero entero valido.");
            }
        }
    }

    private int leerEnteroOpcional(String mensaje, int actual, int min, int max) {
        while (true) {
            System.out.print(mensaje);
            String input = scanner.nextLine().trim();
            if (input.isEmpty()) return actual;
            try {
                int valor = Integer.parseInt(input);
                if (valor >= min && valor <= max) return valor;
                System.out.println("El valor debe estar entre " + min + " y " + max + ".");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un numero entero valido.");
            }
        }
    }

    private long leerLong(String mensaje) {
        while (true) {
            try {
                System.out.print(mensaje);
                long valor = Long.parseLong(scanner.nextLine().trim());
                if (valor >= 10000000L && valor <= 9999999999L) return valor;
                System.out.println("El ID debe tener entre 8 y 10 digitos.");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un numero valido.");
            }
        }
    }

    private double leerDoubleRango(String mensaje, double min, double max) {
        while (true) {
            try {
                System.out.print(mensaje);
                double valor = Double.parseDouble(scanner.nextLine().trim());
                if (valor >= min && valor <= max) return valor;
                System.out.println("El valor debe estar entre " + min + " y " + max + ".");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un numero decimal valido.");
            }
        }
    }

    private double leerDoubleOpcional(String mensaje, double actual, double min, double max) {
        while (true) {
            System.out.print(mensaje);
            String input = scanner.nextLine().trim();
            if (input.isEmpty()) return actual;
            try {
                double valor = Double.parseDouble(input);
                if (valor >= min && valor <= max) return valor;
                System.out.println("El valor debe estar entre " + min + " y " + max + ".");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un numero decimal valido.");
            }
        }
    }
}