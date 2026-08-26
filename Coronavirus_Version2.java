import java.util.Scanner;

// ============================================================
// TDA CORONAVIRUS: Registro y simulacion de una cadena de contagio
// Persona infectada -> contamina un objeto -> otra persona lo toca
// -> transporta el virus en sus manos -> se infecta al tocarse el rostro
// ============================================================

// CONCEPTO: Clase Abstracta y Encapsulamiento
abstract class Persona {
    protected String nombre;              // no primitivo
    protected boolean infectada;          // CONCEPTO: dato primitivo boolean
    protected boolean virusEnManos;       // CONCEPTO: dato primitivo boolean
    protected int minutosVirusEnManos;    // CONCEPTO: dato primitivo int

    public Persona(String nombre, boolean infectada) {
        this.nombre = nombre;
        this.infectada = infectada;
        this.virusEnManos = false;
        this.minutosVirusEnManos = 0;
    }

    public String getNombre() { return nombre; }
    public boolean isInfectada() { return infectada; }
    public boolean isVirusEnManos() { return virusEnManos; }

    // CONCEPTO: conecta a Persona con Objeto -> pieza que faltaba en la version anterior
    public void tocarObjeto(Objeto objeto) {
        if (objeto.isContaminado()) {
            this.virusEnManos = true;
            this.minutosVirusEnManos = 30; // minutos que el virus sobrevive en las manos
        }
    }

    protected void infectar() {
        this.infectada = true;
    }

    // DRY: las subclases reutilizan la impresion de atributos comunes
    protected void mostrarDatosComunes() {
        System.out.printf("Nombre: %-10s | Infectada: %-3s | Virus en manos: %-3s",
                nombre, (infectada ? "Sí" : "No"), (virusEnManos ? "Sí" : "No"));
    }

    // CONCEPTO: Polimorfismo (metodos abstractos)
    public abstract void tocarseRostro();
    public abstract void mostrarInformacion();
}

// CONCEPTO: Herencia (PersonaCuidadosa extends Persona)
class PersonaCuidadosa extends Persona {
    private boolean usaMascarilla;
    private boolean usaAlcohol;

    public PersonaCuidadosa(String nombre, boolean infectada, boolean usaMascarilla, boolean usaAlcohol) {
        super(nombre, infectada);
        this.usaMascarilla = usaMascarilla;
        this.usaAlcohol = usaAlcohol;
    }

    // CONCEPTO: Polimorfismo (@Override)
    @Override
    public void tocarseRostro() {
        if (virusEnManos) {
            if (usaAlcohol) {
                virusEnManos = false; // se desinfecta antes de tocarse el rostro
            } else {
                infectar();
            }
        }
    }

    @Override
    public void mostrarInformacion() {
        System.out.print("[CUIDADOSA]  ");
        mostrarDatosComunes();
        System.out.println(" | Mascarilla: " + (usaMascarilla ? "Sí" : "No")
                + " | Alcohol: " + (usaAlcohol ? "Sí" : "No"));
    }
}

// CONCEPTO: Herencia (PersonaDescuidada extends Persona)
class PersonaDescuidada extends Persona {
    private int frecuenciaTocarRostro;
    private boolean ignoraSintomas;

    public PersonaDescuidada(String nombre, boolean infectada, int frecuenciaTocarRostro, boolean ignoraSintomas) {
        super(nombre, infectada);
        this.frecuenciaTocarRostro = frecuenciaTocarRostro;
        this.ignoraSintomas = ignoraSintomas;
    }

    // CONCEPTO: Polimorfismo (@Override)
    @Override
    public void tocarseRostro() {
        if (virusEnManos) {
            infectar();
        }
    }

    @Override
    public void mostrarInformacion() {
        System.out.print("[DESCUIDADA] ");
        mostrarDatosComunes();
        System.out.println(" | Frecuencia rostro/hora: " + frecuenciaTocarRostro
                + " | Ignora síntomas: " + (ignoraSintomas ? "Sí" : "No"));
    }
}

// Clase que representa la superficie u objeto contaminado
class Objeto {
    private String nombre;
    private boolean contaminado; // CONCEPTO: dato primitivo boolean

    public Objeto(String nombre) {
        this.nombre = nombre;
        this.contaminado = false;
    }

    public void contaminar(Persona portador) {
        if (portador.isInfectada()) {
            this.contaminado = true;
        }
    }

    public void limpiar() {
        this.contaminado = false;
    }

    public String getNombre() { return nombre; }
    public boolean isContaminado() { return contaminado; }
}

// ============================================================
// CONCEPTO: TDA RegistroSimulacion (estructura estatica)
// ============================================================
class RegistroSimulacion {
    private static final int CAPACIDAD = 10;
    private final Persona[] personas = new Persona[CAPACIDAD]; // CONCEPTO: arreglo estatico
    private int cantidad = 0;

    public boolean estaLleno() {
        return cantidad >= CAPACIDAD;
    }

    public boolean existeNombre(String nombre) {
        for (int i = 0; i < cantidad; i++) {
            if (personas[i].getNombre().equalsIgnoreCase(nombre)) {
                return true;
            }
        }
        return false;
    }

    public boolean registrar(Persona nuevaPersona) {
        if (nuevaPersona == null || estaLleno() || existeNombre(nuevaPersona.getNombre())) {
            return false;
        }
        personas[cantidad] = nuevaPersona;
        cantidad++;
        return true;
    }

    public Persona buscarPorNombre(String nombre) {
        for (int i = 0; i < cantidad; i++) {
            if (personas[i].getNombre().equalsIgnoreCase(nombre)) {
                return personas[i];
            }
        }
        return null;
    }

    public int getCantidad() { return cantidad; }

    public int contarInfectados() {
        int total = 0;
        for (int i = 0; i < cantidad; i++) {
            if (personas[i].isInfectada()) total++;
        }
        return total;
    }

    // CONCEPTO: Polimorfismo en tiempo de ejecucion
    public void simularContactoConRostro() {
        if (cantidad == 0) {
            System.out.println("No hay personas registradas.");
            return;
        }
        for (int i = 0; i < cantidad; i++) {
            personas[i].tocarseRostro();
        }
        System.out.println("Simulación completada: cada persona decidió tocarse el rostro.");
    }

    public void mostrarEstado() {
        if (cantidad == 0) {
            System.out.println("No hay personas registradas.");
            return;
        }
        System.out.println("\n--- ESTADO DEL REGISTRO (" + cantidad + "/" + CAPACIDAD + ") ---");
        for (int i = 0; i < cantidad; i++) {
            personas[i].mostrarInformacion();
        }
        System.out.println("Total de infectados: " + contarInfectados());
    }
}

// ============================================================
// Main: menu interactivo por consola (Paso 1 de la metodologia: Scanner)
// ============================================================
public class Coronavirus {
    private static final Scanner scanner = new Scanner(System.in);
    private static final RegistroSimulacion registro = new RegistroSimulacion();
    private static final Objeto objetoCompartido = new Objeto("manija de la puerta");

    public static void main(String[] args) {
        int opcion = 0;
        do {
            System.out.println("\n=== TDA CORONAVIRUS: CADENA DE CONTAGIO ===");
            System.out.println("1. Registrar persona cuidadosa");
            System.out.println("2. Registrar persona descuidada");
            System.out.println("3. Persona infectada contamina el objeto");
            System.out.println("4. Otra persona toca el objeto");
            System.out.println("5. Simular que todos se tocan el rostro (Polimorfismo)");
            System.out.println("6. Mostrar estado del registro");
            System.out.println("7. Salir");
            System.out.print("Seleccione una opción: ");

            try {
                opcion = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                opcion = 0;
            }

            switch (opcion) {
                case 1 -> registrarPersona(true);
                case 2 -> registrarPersona(false);
                case 3 -> contaminarObjeto();
                case 4 -> tocarObjeto();
                case 5 -> registro.simularContactoConRostro();
                case 6 -> registro.mostrarEstado();
                case 7 -> System.out.println("Saliendo del programa...");
                default -> System.out.println("Opción inválida. Intente de nuevo.");
            }
        } while (opcion != 7);
    }

    // DRY: un solo metodo central para registrar cualquier tipo de persona
    private static void registrarPersona(boolean esCuidadosa) {
        if (registro.estaLleno()) {
            System.out.println("Error: el registro estático está lleno.");
            return;
        }

        System.out.println(esCuidadosa ? "\n--- REGISTRO DE PERSONA CUIDADOSA ---" : "\n--- REGISTRO DE PERSONA DESCUIDADA ---");
        String nombre = leerNombreUnico();
        boolean infectada = leerBoolean("¿Está infectada? (s/n): ");

        Persona nueva;
        if (esCuidadosa) {
            boolean usaMascarilla = leerBoolean("¿Usa mascarilla? (s/n): ");
            boolean usaAlcohol = leerBoolean("¿Usa alcohol/gel para desinfectarse? (s/n): ");
            nueva = new PersonaCuidadosa(nombre, infectada, usaMascarilla, usaAlcohol); // CONCEPTO: instanciación
        } else {
            int frecuencia = leerRangoInt("Frecuencia con que se toca el rostro por hora (1 - 30): ", 1, 30);
            boolean ignoraSintomas = leerBoolean("¿Ignora los síntomas? (s/n): ");
            nueva = new PersonaDescuidada(nombre, infectada, frecuencia, ignoraSintomas); // CONCEPTO: instanciación
        }

        if (registro.registrar(nueva)) {
            System.out.println("Persona registrada con éxito.");
        }
    }

    private static void contaminarObjeto() {
        System.out.print("Nombre de la persona infectada que toca el objeto: ");
        Persona p = registro.buscarPorNombre(scanner.nextLine().trim());
        if (p == null) {
            System.out.println("Esa persona no está registrada.");
            return;
        }
        objetoCompartido.contaminar(p);
        System.out.println(p.isInfectada()
                ? "El objeto (" + objetoCompartido.getNombre() + ") quedó contaminado."
                : p.getNombre() + " no está infectada; el objeto no se contaminó.");
    }

    private static void tocarObjeto() {
        System.out.print("Nombre de la persona que toca el objeto: ");
        Persona p = registro.buscarPorNombre(scanner.nextLine().trim());
        if (p == null) {
            System.out.println("Esa persona no está registrada.");
            return;
        }
        p.tocarObjeto(objetoCompartido);
        System.out.println(objetoCompartido.isContaminado()
                ? "El virus pasó a las manos de " + p.getNombre() + "."
                : "El objeto no estaba contaminado.");
    }

    private static String leerNombreUnico() {
        while (true) {
            System.out.print("Nombre: ");
            String nombre = scanner.nextLine().trim();
            if (nombre.isEmpty()) {
                System.out.println("El nombre no puede estar vacío.");
            } else if (registro.existeNombre(nombre)) {
                System.out.println("Error: ya existe una persona registrada con el nombre '" + nombre + "'.");
            } else {
                return nombre;
            }
        }
    }

    private static int leerRangoInt(String mensaje, int min, int max) {
        while (true) {
            try {
                System.out.print(mensaje);
                int val = Integer.parseInt(scanner.nextLine());
                if (val >= min && val <= max) return val;
                System.out.println("El valor debe estar entre " + min + " y " + max + ".");
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un número entero válido.");
            }
        }
    }

    private static boolean leerBoolean(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String input = scanner.nextLine().trim().toLowerCase();
            if (input.equals("s") || input.equals("si")) return true;
            if (input.equals("n") || input.equals("no")) return false;
            System.out.println("Ingrese 's' para Sí o 'n' para No.");
        }
    }
}