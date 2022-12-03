#include <stdio.h>
#include <stdlib.h>
#include "answer.h"
#include "odu_30.h"
#include "graph.h"

int readInitData(char *file, double *x_0, double *x_n, double *y_0, double *y_n, int *n, double *e) {
    FILE *in = fopen(file, "r");
    if (in == NULL) {
        fprintf(stderr, "Cannot open input file\n");
        return -1;
    }
    int valueCount = fscanf(in, "%lf %lf %lf %lf %d %lf", x_0, x_n, y_0, y_n, n, e);
    if (valueCount != 6) {
        fprintf(stderr, "%d values has been read, required 6\n", valueCount);
        return -2;
    } else if (*n <= 0 || *e <= 0) {
        fprintf(stderr, "n and e must be positive numbers\n");
        return -3;
    } else if (*x_n <= *x_0) {
        fprintf(stderr, "x_n must be righter than x_0\n");
        return -4;
    }
    fclose(in);
    return 0;
}

int printAnswer(char *file, double *X, double *Y, int n_opt, int scale) {
    FILE *out = fopen(file, "w");
    if (out == NULL) {
        fprintf(stderr, "Cannot open output file\n");
        return -1;
    }
    fprintf(out, "%d\n", getIterations());
    for (int i = 0; i <= n_opt; i += scale)
        fprintf(out, "%lf %lf\n", X[i], Y[i]);
    fclose(out);
    return 0;
}

int main(int argc, char *argv[]) {
    double x_0, x_n;  // границы интервала
    double y_0, y_n;  // значения функции на границах интервала
    int n;            // число интервалов деления
    double e;         // допустимая погрешность вычисления

    char *input = "input.txt", *output = "output.txt";
    if (argc > 1) input = argv[1];
    if (argc > 2) output = argv[2];

    // Чтение данных
    int err = readInitData(input, &x_0, &x_n, &y_0, &y_n, &n, &e);
    if (err != 0) return err;

    double z_0 = getOptimalZ0(x_0, x_n, y_0, y_n, n, e);
    int n_opt = getOptimalN(x_0, x_n, y_0, z_0, n, e);
    double h_opt = (x_n - x_0) / n_opt;

    double *Y = (double *) malloc((n_opt + 1) * sizeof(double));
    double *X = (double *) malloc((n_opt + 1) * sizeof(double));
    double *Y_real = (double *) malloc((n_opt + 1) * sizeof(double));

    solveSystem(Y, x_0, x_n, y_0, z_0, n_opt);
    for (int i = 0; i <= n_opt; i++) {
        X[i] = x_0 + i * h_opt;
        Y_real[i] = y_real(X[i]);
    }

    // Печать ответа
    err = printAnswer(output, X, Y, n_opt, n_opt / n);
    if (err != 0) return err;

    draw_graph(X, Y, Y_real, n_opt + 1);

    free(X);
    free(Y);
    free(Y_real);
}
