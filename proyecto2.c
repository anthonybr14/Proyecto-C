#include <stdio.h>
#include <stdlib.h>

// Definir el dominio total y la resolución del dominio
#define DOMINIO_MIN -100000
#define DOMINIO_MAX 100000
#define PASO 0.1

// Estructura para almacenar los coeficientes de una función cuadrática
typedef struct {
    double a, b, c; // Coeficientes de la función cuadrática ax^2 + bx + c
} Cuadratica;

// Función para evaluar una función cuadrática en un punto x
double evaluar_cuadratica(Cuadratica f, double x) {
    return f.a * x * x + f.b * x + f.c;
}

// Función para calcular el error entre dos valores
double calcular_error(double f, double g) {
    return 100 * ((f - g) / f);
}

// Función para encontrar el subdominio donde la diferencia entre las funciones es menor que el error
void encontrar_subdominio(Cuadratica funcion1, Cuadratica funcion2, double porcentaje_error, double *inicio, double *fin) {
    double x;
    for (x = DOMINIO_MIN; x <= DOMINIO_MAX; x += PASO) {
        double y1 = evaluar_cuadratica(funcion1, x);
        double y2 = evaluar_cuadratica(funcion2, x);
        double error = calcular_error(y1, y2);
        if (error <= porcentaje_error) {
            *inicio = x;
            break;
        }
    }
    for (x = DOMINIO_MAX; x >= DOMINIO_MIN; x -= PASO) {
        double y1 = evaluar_cuadratica(funcion1, x);
        double y2 = evaluar_cuadratica(funcion2, x);
        double error = calcular_error(y1, y2);
        if (error <= porcentaje_error) {
            *fin = x;
            break;
        }
    }
}

int main() {
    // Coeficientes de las dos funciones cuadráticas
    Cuadratica funcion1, funcion2;

    printf("Ingrese los coeficientes de la primera función cuadrática (ax^2 + bx + c):\n");
    printf("a: ");
    scanf("%lf", &funcion1.a);
    printf("b: ");
    scanf("%lf", &funcion1.b);
    printf("c: ");
    scanf("%lf", &funcion1.c);

    printf("\nIngrese los coeficientes de la segunda función cuadrática (dx^2 + ex + f):\n");
    printf("d: ");
    scanf("%lf", &funcion2.a);
    printf("e: ");
    scanf("%lf", &funcion2.b);
    printf("f: ");
    scanf("%lf", &funcion2.c);

    // Porcentaje de error proporcionado por el usuario
    double porcentaje_error;
    printf("\nIngrese el porcentaje de error permitido (0 a 100): ");
    scanf("%lf", &porcentaje_error);

    // Encontrar el subdominio donde la diferencia entre las funciones no supere el porcentaje de error
    double inicio, fin;
    encontrar_subdominio(funcion1, funcion2, porcentaje_error, &inicio, &fin);

    // Abrir el archivo para escribir los datos a graficar
    FILE *datos = fopen("datos.dat", "w");
    if (datos == NULL) {
        fprintf(stderr, "Error al abrir el archivo de datos\n");
        return 1;
    }

    // Calcular las curvas para el subdominio encontrado y escribir los datos al archivo
    double x;
    for (x = inicio; x <= fin; x += PASO) {
        double y1 = evaluar_cuadratica(funcion1, x);
        double y2 = evaluar_cuadratica(funcion2, x);
        fprintf(datos, "%lf %lf %lf\n", x, y1, y2);
    }
    fclose(datos);

    // Generar el script de GNUplot para graficar los datos
    FILE *gnuplot = popen("gnuplot -persist", "w");
    if (gnuplot == NULL) {
        fprintf(stderr, "Error al abrir GNUplot\n");
        return 1;
    }

    fprintf(gnuplot, "set title 'Funciones Cuadráticas'\n");
    fprintf(gnuplot, "set xlabel 'x'\n");
    fprintf(gnuplot, "set ylabel 'y'\n");
    fprintf(gnuplot, "plot 'datos.dat' using 1:2 with lines title 'Función 1', \
                                    'datos.dat' using 1:3 with lines title 'Función 2', \
                                    %lf title 'Error <= %.2lf'\n", inicio, porcentaje_error);

    // Cerrar el script de GNUplot
    fflush(gnuplot);
    fprintf(gnuplot, "exit\n");
    pclose(gnuplot);

    return 0;
}