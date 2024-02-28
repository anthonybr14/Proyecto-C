// Proyecto C / Parte 1

// Descripcion: Este programa tiene como objetivo
// ejecutar un juego llamado Gato, donde se ejecuta
// una plantilla con 9 casillas. Al presionar cada casilla
// este cambia su imagen por una 'X' o 'O'. Hasta que se forme
// una linea horizontal, vertical o diagonal de la misma
// letra el juego termina.

// Integrantes: Antony Brenes Rodriguez / C11238
//             Josué Rodríguez Mora    / C26646
//             Allan Villalobos Rojas  / C28447


#include <gtk/gtk.h>

// Declaracion de variables
GtkWidget *buttons[3][3];  // Matriz que almacena las casillas
char turno = 'X';          // Indica el turno actual si 'X' o 'O'
int movimientos = 0;       // Contador de movimientos realizados

// Declaracion de funciones
void realizar_movimiento(GtkWidget *widget, gpointer data);
gboolean verificar_ganador();
void mostrar_mensaje(const gchar *mensaje, gboolean reiniciar);

int main(int argc, char *argv[]) {
   
    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  // Crea una ventana
    gtk_window_set_title(GTK_WINDOW(window), "Juego del Gato");  // Establece el titulo de la ventana
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);  // Establece el ancho del borde la ventana
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  // EStablece la senal detroy con la funcion de la llamada

    // Crear una cuadricula para organizar los botones
    
    GtkWidget *grid = gtk_grid_new();  // Crea una cuadricula
    gtk_container_add(GTK_CONTAINER(window), grid); // Agrega una cuadricula

    // Crear y agregar botones a la cuadricula
    
    for (int i = 0; i < 3; i++) {  // Agrega filas
        for (int j = 0; j < 3; j++) {  // Agrega colunnas
            buttons[i][j] = gtk_button_new_with_label("");  // Crea botones vacios
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);  // Adjunta los botones a la cuadricula
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(realizar_movimiento), NULL); // Conecta los botones con la funcion de llamada
        }
    }

    // Mostrar la ventana principal y ejecutar el bucle de eventos de GTK
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

// Funcion para realizar un movimiento en el juego del Gato
void realizar_movimiento(GtkWidget *widget, gpointer data) {
    // Verificar si el boton ya ha sido presionado
    const gchar *label = gtk_button_get_label(GTK_BUTTON(widget)); // Obtiene las etiquetas de los botones
    if (strcmp(label, "") != 0) return; // Compara la etiqueta de los botones

    // Funcion que obtiene la fila y columna del boton presionado
    int fila, columna;
    for (fila = 0; fila < 3; fila++) {
        for (columna = 0; columna < 3; columna++) {
            if (buttons[fila][columna] == widget) break;
        }
        if (buttons[fila][columna] == widget) break;
    }

    // Actualizar el label del boton y el turno
    gtk_button_set_label(GTK_BUTTON(widget), turno == 'X' ? "X" : "O"); // Establece las etiquetas de los botones
    turno = (turno == 'X') ? 'O' : 'X'; // Convierte las casillas en un boton
    movimientos++; // Incrementa el contador de movimientos

    // Funcion que verificar si hay un ganador o si hay un empate
    if (movimientos >= 5 && verificar_ganador()) {
        mostrar_mensaje(turno == 'O' ? "Ganador: X" : "Ganador: O", TRUE);
    } else if (movimientos == 9) {
        mostrar_mensaje("Empate", TRUE);
    }
}

// Funcion que verifica si hay un ganador
gboolean verificar_ganador() {
    const gchar *label;

    // Esta funcion verifica si hay tres etiquetas seguidas o vacias
    // en las filas o columnas para designar un ganador
    for (int i = 0; i < 3; i++) {
        label = gtk_button_get_label(GTK_BUTTON(buttons[i][0]));
        if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[i][1]))) == 0 &&
            strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[i][2]))) == 0) return TRUE;

        label = gtk_button_get_label(GTK_BUTTON(buttons[0][i]));
        if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][i]))) == 0 &&
            strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][i]))) == 0) return TRUE;
    }

    // Funcion que verifica si hay tres letras seguidas en las diagonales
    
    label = gtk_button_get_label(GTK_BUTTON(buttons[0][0]));
    if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][1]))) == 0 &&
        strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][2]))) == 0) return TRUE;

    label = gtk_button_get_label(GTK_BUTTON(buttons[0][2]));
    if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][1]))) == 0 &&
        strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][0]))) == 0) return TRUE;

    return FALSE;
}

// Funcion que muestra un mensaje del resultado del juego

void mostrar_mensaje(const gchar *mensaje, gboolean reiniciar) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", mensaje);
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    // Funcion que reinicia el juego si es necesario
    if (reiniciar) {
        turno = 'X';
        movimientos = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                gtk_button_set_label(GTK_BUTTON(buttons[i][j]), "");
            }
        }
    }
}
