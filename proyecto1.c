#include <gtk/gtk.h>

// Declaracion de variables
GtkWidget *buttons[3][3];
char turno = 'X';
int movimientos = 0;

// Declaracion de funciones
void realizar_movimiento(GtkWidget *widget, gpointer data);
gboolean verificar_ganador();
void mostrar_mensaje(const gchar *mensaje, gboolean reiniciar);

int main(int argc, char *argv[]) {
    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Juego del Gato");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear una cuadricula para organizar los botones
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Crear y agregar botones a la cuadricula
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            buttons[i][j] = gtk_button_new_with_label("");
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(realizar_movimiento), NULL);
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
    const gchar *label = gtk_button_get_label(GTK_BUTTON(widget));
    if (strcmp(label, "") != 0) return;

    // Obtener la fila y columna del boton presionado
    int fila, columna;
    for (fila = 0; fila < 3; fila++) {
        for (columna = 0; columna < 3; columna++) {
            if (buttons[fila][columna] == widget) break;
        }
        if (buttons[fila][columna] == widget) break;
    }

    // Actualizar el label del boton y el turno
    gtk_button_set_label(GTK_BUTTON(widget), turno == 'X' ? "X" : "O");
    turno = (turno == 'X') ? 'O' : 'X';
    movimientos++;

    // Verificar si hay un ganador o si hay un empate
    if (movimientos >= 5 && verificar_ganador()) {
        mostrar_mensaje(turno == 'O' ? "Ganador: X" : "Ganador: O", TRUE);
    } else if (movimientos == 9) {
        mostrar_mensaje("Empate", TRUE);
    }
}

// Funcion para verificar si hay un ganador
gboolean verificar_ganador() {
    const gchar *label;

    // Verificar filas y columnas
    for (int i = 0; i < 3; i++) {
        label = gtk_button_get_label(GTK_BUTTON(buttons[i][0]));
        if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[i][1]))) == 0 &&
            strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[i][2]))) == 0) return TRUE;

        label = gtk_button_get_label(GTK_BUTTON(buttons[0][i]));
        if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][i]))) == 0 &&
            strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][i]))) == 0) return TRUE;
    }

    // Verificar diagonales
    label = gtk_button_get_label(GTK_BUTTON(buttons[0][0]));
    if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][1]))) == 0 &&
        strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][2]))) == 0) return TRUE;

    label = gtk_button_get_label(GTK_BUTTON(buttons[0][2]));
    if (strcmp(label, "") != 0 && strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[1][1]))) == 0 &&
        strcmp(label, gtk_button_get_label(GTK_BUTTON(buttons[2][0]))) == 0) return TRUE;

    return FALSE;
}

// Funcion para mostrar un mensaje de resultado
void mostrar_mensaje(const gchar *mensaje, gboolean reiniciar) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", mensaje);
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    // Reiniciar el juego si es necesario
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
