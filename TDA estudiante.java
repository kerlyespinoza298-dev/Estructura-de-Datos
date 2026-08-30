import java.time.LocalDate;
import java.time.Period;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.time.format.ResolverStyle;
import java.util.Scanner;

// ============================================================
// CLASE PRINCIPAL
// ============================================================
public class Main {

    private static final Scanner scanner = new Scanner(System.in);
    private static final GestorEstudiantes gestor = new GestorEstudiantes();

    public static void main(String[] args) {
        int opcion;
        do {
            mostrarMenuPrincipal();
            opcion = leerEntero("Teclee su opcion (1-4): ");

            switch (opcion) {
                case 1 -> subMenuEstudiantes();
                case 2 -> subMenuCalificaciones();
                case 3 -> mostrarPromedioEstudiante();
                case 4 -> mostrarPromedioCurso();
                case 0 -> {}
                default -> System.out.println("Opcion no valida.");
            }
        } while (opcion != 0);

        System.out.println("\nPrograma finalizado.");
    }

    private static void mostrarMenuPrincipal() {
        System.out.println("\n=== GESTOR DE PERSONAS ===");
        System.out.println("1.- Estudiantes.");
        System.out.println("2.- Registro de calificaciones.");
        System.out.println("3.- Determinar el promedio de notas de un estudiante.");
        System.out.println("4.- Determinar el promedio de notas del curso.");
        System.out.println("0.- Salir.");
    }

    // ============================================================
    // OPCIÓN 1: SUBMENÚ ESTUDIANTES
    // ============================================================

    private static void subMenuEstudiantes() {
        int opcion;
        do {
            // Muestra siempre el listado al ingresar a la Opción 1
            gestor.mostrarTabla();

            System.out.println("\n--- SUBMENU ESTUDIANTES ---");
            System.out.println("1. Ingresar estudiante");
            System.out.println("2. Modificar estudiante");
            System.out.println("3. Eliminar estudiante");
            System.out.println("0. Volver al menu principal");

            opcion = leerEntero("Seleccione una opcion: ");

            switch (opcion) {
                case 1 -> ejecutarIngresoEstudiantes();
                case 2 -> ejecutarModificarEstudiante();
                case 3 -> ejecutarEliminarEstudiante();
                case 0 -> {}
                default -> System.out.println("Opcion no valida.");
            }
        } while (opcion != 0);
    }

    private static void ejecutarIngresoEstudiantes() {
        boolean continuar = true;
        while (continuar) {
            if (gestor.estaLleno()) {
                System.out.println("\nNo se permite insertar: Cupo maximo alcanzado (" + gestor.getCapacidad() + ").");
                break;
            }

            System.out.println("\n--- INGRESAR ESTUDIANTE ---");
            String cedula = leerCedulaValida("Cedula: ");

            if (gestor.buscar(cedula) != null) {
                System.out.println("Error: Ya existe un estudiante registrado con esa cedula.");
                continuar = solicitarConfirmacion("¿Desea intentar con otra cedula? (s/N): ");
                continue;
            }

            String nombres = leerTextoValido("Nombres: ");
            String apellidos = leerTextoValido("Apellidos: ");
            String fechaNacimiento = leerFechaValida("Fecha de nacimiento (DD/MM/AAAA): ");

            Estudiante nuevo = new Estudiante(cedula, nombres, apellidos, fechaNacimiento);
            if (gestor.registrar(nuevo)) {
                System.out.println("\nEstudiante registrado exitosamente.");
                System.out.println("Total: " + gestor.getCantidad() + "/" + gestor.getCapacidad());
            }

            if (gestor.estaLleno()) break;
            continuar = solicitarConfirmacion("¿Desea ingresar otro estudiante? (s/N): ");
        }
    }

    private static void ejecutarModificarEstudiante() {
        if (gestor.estaVacio()) {
            System.out.println("\nNo hay estudiantes registrados. No es posible modificar.");
            return;
        }

        boolean continuar = true;
        while (continuar) {
            gestor.mostrarTabla();
            int indice = leerEntero("Indique el autonumerico del estudiante a modificar: ") - 1;

            Estudiante est = gestor.buscarPorIndice(indice);
            if (est == null) {
                System.out.println("Error: Registro no encontrado.");
            } else {
                System.out.println("\nDatos actuales:\n" + est);
                String n = leerTextoOpcionalValido("Nuevos nombres (Enter para mantener): ", est.getNombres());
                String a = leerTextoOpcionalValido("Nuevos apellidos (Enter para mantener): ", est.getApellidos());
                String f = leerFechaOpcionalValida("Nueva fecha de nacimiento (Enter para mantener): ", est.getFechaNacimiento());

                gestor.modificar(indice, n, a, f);
                System.out.println("Estudiante modificado exitosamente.");
            }
            continuar = solicitarConfirmacion("¿Desea modificar otro estudiante? (s/N): ");
        }
    }

    private static void ejecutarEliminarEstudiante() {
        if (gestor.estaVacio()) {
            System.out.println("\nNo hay estudiantes registrados. No se permite eliminar.");
            return;
        }

        boolean continuar = true;
        while (continuar) {
            gestor.mostrarTabla();
            int indice = leerEntero("Indique el autonumerico del estudiante a eliminar: ") - 1;

            Estudiante est = gestor.buscarPorIndice(indice);
            if (est == null) {
                System.out.println("Error: Registro no encontrado.");
            } else {
                System.out.println("\nRegistro seleccionado:\n" + est);
                if (solicitarConfirmacion("¿Confirmar eliminacion? (s/N): ")) {
                    gestor.eliminar(indice);
                    System.out.println("Estudiante eliminado exitosamente.");
                } else {
                    System.out.println("Operacion cancelada.");
                }
            }

            if (gestor.estaVacio()) break;
            continuar = solicitarConfirmacion("¿Desea eliminar otro estudiante? (s/N): ");
        }
    }

    // ============================================================
    // OPCIÓN 2: REGISTRO DE CALIFICACIONES
    // ============================================================

    private static void subMenuCalificaciones() {
        if (gestor.estaVacio()) {
            System.out.println("\nNo hay estudiantes registrados. Primero registre estudiantes.");
            return;
        }

        Estudiante est = null;
        while (est == null) {
            String cedula = leerCedulaValida("Ingrese el numero de cedula del estudiante: ");
            est = gestor.buscar(cedula);

            if (est == null) {
                System.out.println("\nNotificacion: La cedula ingresada no pertenece a un estudiante registrado.");
                if (!solicitarConfirmacion("¿Desea ingresar otro numero de cedula? (s/N): ")) return;
            }
        }

        System.out.println("\n--- DATOS DEL ESTUDIANTE ---");
        System.out.println("Nombres: " + est.getNombres());
        System.out.println("Apellidos: " + est.getApellidos());
        System.out.println("Edad: " + est.calcularEdad() + " anios");

        int opcion;
        do {
            System.out.println("\n--- CALIFICACIONES DE ESTUDIANTE ---");
            System.out.println("Notas (" + est.getNumNotas() + "/" + est.getMaxNotas() + "): " + est.notasToString());
            System.out.println("1. Agregar nota");
            System.out.println("2. Modificar nota");
            System.out.println("3. Eliminar nota");
            System.out.println("0. Volver al menu principal");

            opcion = leerEntero("Seleccione una opcion: ");

            switch (opcion) {
                case 1 -> ejecutarAgregarNota(est);
                case 2 -> ejecutarModificarNota(est);
                case 3 -> ejecutarEliminarNota(est);
                case 0 -> {}
                default -> System.out.println("Opcion no valida.");
            }
        } while (opcion != 0);
    }

    private static void ejecutarAgregarNota(Estudiante est) {
        if (est.getNumNotas() >= est.getMaxNotas()) {
            System.out.println("\nSe han ingresado todas las calificaciones posibles.");
            return;
        }

        boolean continuar = true;
        while (continuar) {
            double nota = leerDouble("Ingrese nota (0.0-10.0): ", 0.0, 10.0);
            est.agregarNota(nota);

            System.out.println("Nota registrada.\n" + est.notasToString());

            if (est.getNumNotas() >= est.getMaxNotas()) {
                System.out.println("\nSe han ingresado todas las calificaciones posibles.");
                break;
            }

            continuar = solicitarConfirmacion("¿Desea agregar otra nota? (s/N): ");
        }
    }

    private static void ejecutarModificarNota(Estudiante est) {
        if (est.getNumNotas() == 0) {
            System.out.println("No hay notas registradas.");
            return;
        }

        System.out.println("\nNotas actuales: " + est.notasToString());
        int pos = leerEntero("Indice de nota a modificar (1-" + est.getNumNotas() + "): ") - 1;

        if (pos < 0 || pos >= est.getNumNotas()) {
            System.out.println("Error: Indice fuera de rango.");
            return;
        }

        System.out.printf("Nota actual: %.1f\n", est.getNota(pos));
        double nuevaNota = leerDouble("Nueva nota (0.0-10.0): ", 0.0, 10.0);

        est.modificarNota(pos, nuevaNota);
        System.out.println("Nota actualizada.\n" + est.notasToString());
    }

    private static void ejecutarEliminarNota(Estudiante est) {
        if (est.getNumNotas() == 0) {
            System.out.println("No hay notas registradas.");
            return;
        }

        System.out.println("\nNotas actuales: " + est.notasToString());
        int pos = leerEntero("Indice de nota a eliminar (1-" + est.getNumNotas() + "): ") - 1;

        if (pos < 0 || pos >= est.getNumNotas()) {
            System.out.println("Error: Indice fuera de rango.");
            return;
        }

        if (solicitarConfirmacion("¿Eliminar calificacion? (s/N): ")) {
            est.eliminarNota(pos);
            System.out.println("Nota eliminada.\n" + est.notasToString());
        } else {
            System.out.println("Operacion cancelada.");
        }
    }

    // ============================================================
    // OPCIÓN 3 Y 4: CONSULTAS DE PROMEDIO
    // ============================================================

    private static void mostrarPromedioEstudiante() {
        if (gestor.estaVacio()) {
            System.out.println("\nError: No se encontro un estudiante con el numero de cedula indicado.");
            return;
        }

        String cedula = leerCedulaValida("Ingrese el numero de cedula del estudiante: ");
        Estudiante est = gestor.buscar(cedula);

        if (est == null) {
            System.out.println("\nError: No se encontro un estudiante con el numero de cedula indicado.");
            return;
        }

        System.out.println("\n--- DATOS DEL ESTUDIANTE ---");
        System.out.println("Nombres: " + est.getNombres());
        System.out.println("Apellidos: " + est.getApellidos());
        System.out.println("Edad: " + est.calcularEdad() + " anios");
        System.out.printf("Promedio de calificaciones: %.2f\n", est.calcularPromedio());
    }

    private static void mostrarPromedioCurso() {
        System.out.println("\n--- PROMEDIO GENERAL DEL CURSO ---");
        if (!gestor.tieneNotasRegistradas()) {
            System.out.println("No se han registrado calificaciones de estudiantes.");
            return;
        }

        System.out.printf("Promedio general de calificaciones: %.2f\n", gestor.calcularPromedioGeneral());
    }

    // ============================================================
    // MÉTODOS AUXILIARES DE ENTRADA CON VALIDACIÓN
    // ============================================================

    private static int leerEntero(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String linea = scanner.nextLine().trim();
            try {
                return Integer.parseInt(linea);
            } catch (NumberFormatException e) {
                System.out.println("Entrada no valida. Ingrese un entero valido.");
            }
        }
    }

    private static double leerDouble(String mensaje, double min, double max) {
        while (true) {
            System.out.print(mensaje);
            String linea = scanner.nextLine().trim();
            try {
                double v = Double.parseDouble(linea);
                if (v >= min && v <= max) return v;
            } catch (NumberFormatException ignored) {}
            System.out.printf("Entrada no valida. Ingrese un valor entre %.1f y %.1f.\n", min, max);
        }
    }

    private static String leerCedulaValida(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String c = scanner.nextLine().trim();
            if (Validador.validarCedulaEcuador(c)) return c;
            System.out.println("Error: Cedula invalida (debe contener 10 digitos validos de Ecuador).");
        }
    }

    private static String leerTextoValido(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String t = scanner.nextLine().trim();
            if (Validador.validarTextoNombres(t)) return t;
            System.out.println("Error: Debe ingresar texto valido (solo letras, min. 2 caracteres por palabra).");
        }
    }

    private static String leerFechaValida(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String f = scanner.nextLine().trim();
            if (Validador.validarFechaNacimiento(f)) return f;
            System.out.println("Error: Fecha invalida. Use el formato estricto DD/MM/AAAA.");
        }
    }

    private static String leerTextoOpcionalValido(String mensaje, String actual) {
        while (true) {
            System.out.print(mensaje);
            String t = scanner.nextLine().trim();
            if (t.isEmpty()) return actual;
            if (Validador.validarTextoNombres(t)) return t;
            System.out.println("Error: Texto invalido. Solo se permiten letras.");
        }
    }

    private static String leerFechaOpcionalValida(String mensaje, String actual) {
        while (true) {
            System.out.print(mensaje);
            String f = scanner.nextLine().trim();
            if (f.isEmpty()) return actual;
            if (Validador.validarFechaNacimiento(f)) return f;
            System.out.println("Error: Fecha invalida. Use formato DD/MM/AAAA.");
        }
    }

    private static boolean solicitarConfirmacion(String mensaje) {
        System.out.print(mensaje);
        String r = scanner.nextLine().trim().toLowerCase();
        return r.equals("s") || r.equals("si");
    }
}

// ============================================================
// VALIDADOR DE ENTRADAS
// ============================================================
class Validador {
    public static boolean esNumero(String str) {
        return str != null && str.matches("\\d+");
    }

    public static boolean validarCedulaEcuador(String cedula) {
        if (cedula == null || cedula.length() != 10 || !esNumero(cedula)) return false;
        int provincia = Integer.parseInt(cedula.substring(0, 2));
        if ((provincia < 1 || provincia > 24) && provincia != 30) return false;
        int tercerDigito = Character.getNumericValue(cedula.charAt(2));
        if (tercerDigito < 0 || tercerDigito > 5) return false;
        int suma = 0;
        for (int i = 0; i < 9; i++) {
            int d = Character.getNumericValue(cedula.charAt(i));
            if (i % 2 == 0) {
                d *= 2;
                if (d > 9) d -= 9;
            }
            suma += d;
        }
        int digitoVerificador = (suma % 10 == 0) ? 0 : (10 - (suma % 10));
        return digitoVerificador == Character.getNumericValue(cedula.charAt(9));
    }

    public static boolean validarFechaNacimiento(String fechaStr) {
        if (fechaStr == null || fechaStr.length() != 10) return false;
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/uuuu").withResolverStyle(ResolverStyle.STRICT);
        try {
            LocalDate fechaNac = LocalDate.parse(fechaStr, formatter);
            LocalDate hoy = LocalDate.now();
            return !fechaNac.isBefore(LocalDate.of(1900, 1, 1)) && !fechaNac.isAfter(hoy);
        } catch (DateTimeParseException e) {
            return false;
        }
    }

    public static boolean validarTextoNombres(String texto) {
        if (texto == null || texto.trim().isEmpty()) return false;
        String[] palabras = texto.trim().split("\\s+");
        for (String palabra : palabras) {
            if (palabra.length() < 2 || !palabra.matches("[a-zA-ZáéíóúÁÉÍÓÚñÑ]+")) return false;
        }
        return true;
    }
}

// ============================================================
// CLASE ESTUDIANTE (REQUERIMIENTO 1 Y 2)
// ============================================================
class Estudiante {
    private static final int MAX_NOTAS = 7;
    private final String cedula;
    private String nombres;
    private String apellidos;
    private String fechaNacimiento;
    private final double[] notas;
    private int numNotas;

    public Estudiante(String cedula, String nombres, String apellidos, String fechaNacimiento) {
        this.cedula = cedula;
        this.nombres = nombres;
        this.apellidos = apellidos;
        this.fechaNacimiento = fechaNacimiento;
        this.notas = new double[MAX_NOTAS];
        this.numNotas = 0;
    }

    public String getCedula() { return cedula; }
    public String getNombres() { return nombres; }
    public String getApellidos() { return apellidos; }
    public String getFechaNacimiento() { return fechaNacimiento; }
    public int getNumNotas() { return numNotas; }
    public int getMaxNotas() { return MAX_NOTAS; }

    public void setNombres(String nombres) { this.nombres = nombres; }
    public void setApellidos(String apellidos) { this.apellidos = apellidos; }
    public void setFechaNacimiento(String fechaNacimiento) { this.fechaNacimiento = fechaNacimiento; }

    public int calcularEdad() {
        if (!Validador.validarFechaNacimiento(fechaNacimiento)) return -1;
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/uuuu");
        LocalDate fechaNac = LocalDate.parse(fechaNacimiento, formatter);
        return Period.between(fechaNac, LocalDate.now()).getYears();
    }

    public boolean agregarNota(double nota) {
        if (numNotas >= MAX_NOTAS) return false;
        notas[numNotas++] = nota;
        return true;
    }

    public double getNota(int i) { return (i >= 0 && i < numNotas) ? notas[i] : -1.0; }

    public boolean modificarNota(int i, double nota) {
        if (i < 0 || i >= numNotas) return false;
        notas[i] = nota;
        return true;
    }

    public boolean eliminarNota(int i) {
        if (i < 0 || i >= numNotas) return false;
        for (int j = i; j < numNotas - 1; j++) notas[j] = notas[j + 1];
        numNotas--;
        return true;
    }

    public double calcularPromedio() {
        if (numNotas == 0) return 0.0;
        double suma = 0.0;
        for (int i = 0; i < numNotas; i++) suma += notas[i];
        return suma / numNotas;
    }

    public String notasToString() {
        if (numNotas == 0) return "[ Sin calificaciones ]";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < numNotas; i++) sb.append(String.format("[%.1f] ", notas[i]));
        sb.append(String.format("| Promedio: %.2f", calcularPromedio()));
        return sb.toString();
    }

    @Override
    public String toString() {
        return "Cedula: " + cedula + " | Nombres: " + nombres + " | Apellidos: " + apellidos + " | F.Nac: " + fechaNacimiento;
    }
}

// ============================================================
// CLASE GESTOR DE ESTUDIANTES (MÉTODO BUSCAR EXPLICITO)
// ============================================================
class GestorEstudiantes {
    private static final int CAPACIDAD = 20;
    private final Estudiante[] estudiantes;
    private int cantidad;

    public GestorEstudiantes() {
        this.estudiantes = new Estudiante[CAPACIDAD];
        this.cantidad = 0;
    }

    public boolean estaVacio() { return cantidad == 0; }
    public boolean estaLleno() { return cantidad >= CAPACIDAD; }
    public int getCantidad() { return cantidad; }
    public int getCapacidad() { return CAPACIDAD; }

    // Requerimiento 2: Método buscar() explicito por cédula
    public Estudiante buscar(String cedula) {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getCedula().equals(cedula)) return estudiantes[i];
        }
        return null;
    }

    public Estudiante buscarPorIndice(int indice) {
        if (indice < 0 || indice >= cantidad) return null;
        return estudiantes[indice];
    }

    public boolean registrar(Estudiante nuevo) {
        if (estaLleno() || buscar(nuevo.getCedula()) != null) return false;
        estudiantes[cantidad++] = nuevo;
        return true;
    }

    public boolean modificar(int i, String nombres, String apellidos, String fechaNacimiento) {
        if (i < 0 || i >= cantidad) return false;
        estudiantes[i].setNombres(nombres);
        estudiantes[i].setApellidos(apellidos);
        estudiantes[i].setFechaNacimiento(fechaNacimiento);
        return true;
    }

    public boolean eliminar(int i) {
        if (i < 0 || i >= cantidad) return false;
        for (int j = i; j < cantidad - 1; j++) estudiantes[j] = estudiantes[j + 1];
        estudiantes[--cantidad] = null;
        return true;
    }

    public boolean tieneNotasRegistradas() {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getNumNotas() > 0) return true;
        }
        return false;
    }

    public double calcularPromedioGeneral() {
        double sumaPromedios = 0.0;
        int cont = 0;
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getNumNotas() > 0) {
                sumaPromedios += estudiantes[i].calcularPromedio();
                cont++;
            }
        }
        return (cont == 0) ? 0.0 : (sumaPromedios / cont);
    }

    public void mostrarTabla() {
        System.out.println("\n==============================================");
        if (estaVacio()) {
            System.out.println("LISTADO DE ESTUDIANTES: (No hay estudiantes registrados)");
        } else {
            System.out.println("LISTADO DE ESTUDIANTES (" + cantidad + "/" + CAPACIDAD + ")");
            System.out.println("==============================================");
            for (int i = 0; i < cantidad; i++) {
                System.out.println((i + 1) + ". " + estudiantes[i]);
            }
        }
        System.out.println("==============================================");
    }
}
