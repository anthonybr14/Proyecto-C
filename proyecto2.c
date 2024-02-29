// Proyecto C / Parte 1

// Descripcion: Este Programa su funcion es 
// grafica la funciones y nos da su interseccion 
// de error y su subdominio.

// Integrantes: Antony Brenes Rodriguez / C11238
//             Josué Rodríguez Mora    / C26646
//             Allan Villalobos Rojas  / C28447

#include <stdio.h>
#include <stdlib.h>

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
    // Definir el rango de valores de x donde buscar
    double paso = 0.01; // El tamaño del paso determina la precisión del resultado
    double x = -100.0; // Empieza desde un valor negativo muy pequeño
    double diferencia;

    // Encontrar el inicio del subdominio
    while (1) {
        // Evaluar las funciones en el punto x
        double f1 = evaluar_cuadratica(funcion1, x);
        double f2 = evaluar_cuadratica(funcion2, x);
        
        // Calcular la diferencia entre las dos funciones
        diferencia = calcular_error(f1, f2);

        // Si la diferencia está dentro del porcentaje de error, establecer este punto como inicio
        if (diferencia <= porcentaje_error) {
            *inicio = x;
            break;
        }
        
        // Incrementar el valor de x
        x += paso;
    }

    // Encontrar el final del subdominio
    while (1) {
        // Evaluar las funciones en el punto x
        double f1 = evaluar_cuadratica(funcion1, x);
        double f2 = evaluar_cuadratica(funcion2, x);
        
        // Calcular la diferencia entre las dos funciones
        diferencia = calcular_error(f1, f2);

        // Si la diferencia excede el porcentaje de error, establecer este punto como fin
        if (diferencia > porcentaje_error) {
            *fin = x;
            break;
        }
        
        // Incrementar el valor de x
        x += paso;
    }

    // Imprimir el subdominio encontrado
    printf("Subdominio encontrado donde el error es menor o igual al %.2lf%%: [%.2lf, %.2lf]\n", porcentaje_error, *inicio, *fin);
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
    printf("Subdominio encontrado: [%.2lf, %.2lf]\n", inicio, fin);

    // Generar el script de GNUplot para graficar las funciones cuadráticas y el área entre ellas
    FILE *gnuplot = popen("gnuplot -persist", "w");
    if (gnuplot == NULL) {
        fprintf(stderr, "Error al abrir GNUplot\n");
        return 1;
    }

    fprintf(gnuplot, "set title 'Área entre Funciones Cuadráticas'\n");
    fprintf(gnuplot, "set xlabel 'x'\n");
    fprintf(gnuplot, "set ylabel 'y'\n");
    fprintf(gnuplot, "set style fill transparent solid 0.5\n"); // Ajusta el estilo de relleno a gris transparente
    fprintf(gnuplot, "plot %lf * x**2 + %lf * x + %lf title 'Función 1' with lines lc rgb 'blue', \
                           %lf * x**2 + %lf * x + %lf title 'Función 2' with lines lc rgb 'red', \
                           %lf * x**2 + %lf * x + %lf title 'Área entre Funciones' with filledcurve x1 fillcolor rgb 'gray'\n", 
                           funcion1.a, funcion1.b, funcion1.c, 
                           funcion2.a, funcion2.b, funcion2.c, 
                           funcion1.a, funcion1.b, funcion1.c);
    fprintf(gnuplot, "set arrow from %.2lf,graph(0,0) to %.2lf,graph(1,1) nohead lc rgb 'black' lw 1.5\n", inicio, inicio);
    fprintf(gnuplot, "set arrow from %.2lf,graph(0,0) to %.2lf,graph(1,1) nohead lc rgb 'black' lw 1.5\n", fin, fin);

    fflush(gnuplot);
    fprintf(gnuplot, "exit\n");
    pclose(gnuplot);

    return 0;
}
