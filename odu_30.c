#include <math.h>
#include <stdlib.h>
#include "funkf_30.h"
#include "funkp_30.h"
#include "funkq_30.h"
#include "odu_30.h"

// Вспомогательные функции

double scalarProd(const double *first, const double *second, int size) {
    double prod = 0.0;
    for (int i = 0; i < size; i++)
        prod += first[i] * second[i];
    return prod;
}

double shortLongDist(const double *shorter, const double *longer, int shortSize) {
    double dist = 0.0;
    for (int i = 0; i < shortSize; i++)
        dist += pow((shorter[i] - longer[2 * i]), 2);
    return sqrt(dist) / shortSize;
}

// Константы метода Нюстрёма

const double A[4][3] = {{},
                        {0.5},
                        {0.0, 0.5},
                        {0.0, 0.0, 1.0}};
const double A_[4][3] = {{},
                         {0.125},
                         {0.125, 0.0},
                         {0.0,   0.0, 0.5}};
const double B[4] = {1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0};
const double B_[4] = {1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 0.0};
const double C[4] = {0.0, 0.5, 0.5, 0.0};

// Решение системы дифференциальных уравнений методом Нюстрема.

double solveSystem(double *Y, double x_0, double x_n, double y_0, double z_0, int n) {

    double h = (x_n - x_0) / n;
    double x = x_0, y = y_0, z = z_0;
    if (Y != NULL)
        Y[0] = y_0;

    for (int i = 1; i <= n; i++) {
        double K[4] = {0.0};

        for (int j = 0; j < 4; j++) {
            double dx = x + h * C[j];
            double dy = y + h * (z * C[j] + h * scalarProd(K, A_[j], j));
            double dz = z + h * scalarProd(K, A[j], j);
            K[j] = -p(dx, dy) * dz - q(dx, dy) * dy + f(dx, dy);
        }

        x += h;
        y += h * (z + h * scalarProd(K, B_, 4));
        z += h * scalarProd(K, B, 4);
        if (Y != NULL)
            Y[i] = y;
    }
    return y;
}

// Подбор оптимального числа интервалов, на которые делится исходный

int getOptimalN(double x_0, double x_n, double y_0, double z_0, int n, double eps) {
    double *Y = (double *) malloc((n + 1) * sizeof(double));
    solveSystem(Y, x_0, x_n, y_0, z_0, n);

    double error = eps * 2;

    while (error > eps) {
        double *Y_new = (double *) malloc((2 * n + 1) * sizeof(double));
        solveSystem(Y_new, x_0, x_n, y_0, z_0, n);
        error = shortLongDist(Y, Y_new, n + 1);

        free(Y);
        Y = Y_new;
        n *= 2;
    }
    free(Y);
    return n;
}


// Реализация метода пристрелки

int iterations = 0;

int getIterations() {
    return iterations;
}

double getOptimalZ0(double x_0, double x_n, double y_0, double y_n, int n, double eps) {
    double z_0_left = 1.0, z_0_right = -1.0;
    double F_left = 1.0, F_right = 1.0;

    while (F_left * F_right > 0) {
        z_0_left -= 1.0;
        z_0_right += 1.0;

        int n_left = getOptimalN(x_0, x_n, y_0, z_0_left, n, eps);
        F_left = solveSystem(NULL, x_0, x_n, y_0, z_0_left, n_left) - y_n;
        if (fabs(F_left) < eps) {
            iterations++;
            return z_0_left;
        }

        int n_right = getOptimalN(x_0, x_n, y_0, z_0_right, n, eps);
        F_right = solveSystem(NULL, x_0, x_n, y_0, z_0_right, n_right) - y_n;
        if (fabs(F_right) < eps) {
            iterations++;
            return z_0_right;
        }
    }

    while (1) {
        iterations++;
        double z_0_mid = (z_0_right + z_0_left) / 2;
        double n_mid = getOptimalN(x_0, x_n, y_0, z_0_mid, n, eps);
        double F_mid = solveSystem(NULL, x_0, x_n, y_0, z_0_mid, n_mid) - y_n;

        if (fabs(F_mid) < eps) {
            return z_0_mid;
        } else if (F_mid * F_left > 0) {
            F_left = F_mid;
            z_0_left = z_0_mid;
        } else if (F_mid * F_right > 0) {
            F_right = F_mid;
            z_0_right = z_0_mid;
        }
    }
}
