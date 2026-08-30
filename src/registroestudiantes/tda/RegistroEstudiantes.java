package registroestudiantes.tda;

import registroestudiantes.model.Estudiante;

public class RegistroEstudiantes {
    private static final int CAPACIDAD = 26;
    private final Estudiante[] estudiantes = new Estudiante[CAPACIDAD];
    private int cantidad = 0;

    public boolean estaVacio() { return cantidad == 0; }
    public boolean estaLleno() { return cantidad >= CAPACIDAD; }
    public int getCantidad() { return cantidad; }
    public int getCapacidad() { return CAPACIDAD; }

    private int buscarIndicePorId(long id) {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i].getId() == id) {
                return i;
            }
        }
        return -1;
    }

    public Estudiante buscarPorId(long id) {
        int indice = buscarIndicePorId(id);
        return (indice != -1) ? estudiantes[indice] : null;
    }

    public boolean registrar(Estudiante estudiante) {
        if (estudiante == null || estaLleno() || buscarPorId(estudiante.getId()) != null) {
            return false;
        }
        estudiantes[cantidad] = estudiante;
        cantidad++;
        return true;
    }

    public Estudiante[] obtenerTodos() {
        Estudiante[] copia = new Estudiante[cantidad];
        System.arraycopy(estudiantes, 0, copia, 0, cantidad);
        return copia;
    }

    public boolean modificar(long id, String nuevoNombre, int nuevaEdad, double nuevoPromedio) {
        int indice = buscarIndicePorId(id);
        if (indice == -1) {
            return false;
        }
        estudiantes[indice].actualizarDatos(nuevoNombre, nuevaEdad, nuevoPromedio);
        return true;   
    }

    public boolean eliminar(long id) {
        int indice = buscarIndicePorId(id);
        if (indice == -1) return false;
        
        for (int i = indice; i < cantidad - 1; i++) {
            estudiantes[i] = estudiantes[i + 1];
        }
        cantidad--;
        estudiantes[cantidad] = null;
        return true;
    }

    public boolean verificarIntegridad() {
        for (int i = 0; i < cantidad; i++) {
            if (estudiantes[i] == null) return false;
        }
        return true;
    }
}