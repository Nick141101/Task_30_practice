#ifndef TASK_30_PRACTICE_ODU_30_H
#define TASK_30_PRACTICE_ODU_30_H

double solveSystem(double *Y, double x_0, double x_n, double y_0, double z_0, int n);

int getOptimalN(double x_0, double x_n, double y_0, double z_0, int n, double eps);

double getOptimalZ0(double x_0, double x_n, double y_0, double y_n, int n, double eps);

int getIterations();

#endif
