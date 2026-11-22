#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

double calculate_Y(double x);
double calculate_V(double x);
void tabulate(double (*func)(double), double a, double b, double h, const char* name);
void print_graph(double (*func)(double), double a, double b, double h, const char* name);
void calculate_set(double (*func)(double), const char* name);
void calculate_fixed_step(double (*func)(double), const char* name);
void calculate_random(double (*func)(double), const char* name);
void clear_input();

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int action;
    double x, a, b, h;
    int interval_type;

    do {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Вычислить Y(x) в точке\n");
        printf("2. Вычислить V(x) в точке\n");
        printf("3. Табулировать Y(x)\n");
        printf("4. Табулировать V(x)\n");
        printf("5. Построить график Y(x)\n");
        printf("6. Построить график V(x)\n");
        printf("7. Расчет Y(x) по набору значений\n");
        printf("8. Расчет V(x) по набору значений\n");
        printf("9. Расчет Y(x) с фиксированным шагом (n=10)\n");
        printf("10. Расчет V(x) с фиксированным шагом (n=10)\n");
        printf("11. Расчет Y(x) для N случайных значений\n");
        printf("12. Расчет V(x) для N случайных значений\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");

        if (scanf("%d", &action) != 1) {
            printf("Ошибка ввода. Введите число.\n");
            clear_input();
            continue;
        }

        if (action == 0) break;
        if (action < 1 || action > 12) {
            printf("Неверный пункт меню.\n");
            continue;
        }

        if (action == 1 || action == 2) {
            printf("Введите x: ");
            if (scanf("%lf", &x) != 1) {
                printf("Ошибка ввода.\n");
                clear_input();
                continue;
            }
            if (action == 1) {
                printf("Y(%.4lf) = %.4lf\n", x, calculate_Y(x));
            }
            else {
                printf("V(%.4lf) = %.4lf\n", x, calculate_V(x));
            }
        }
        else if (action >= 3 && action <= 6) {
            printf("\nВыберите тип промежутка:\n");
            printf("1. Отрезок [a, b]\n");
            printf("2. Интервал (a, b)\n");
            printf("3. Полуинтервал (a, b]\n");
            printf("4. Полуинтервал [a, b)\n");
            printf("Ваш выбор: ");

            if (scanf("%d", &interval_type) != 1 || interval_type < 1 || interval_type > 4) {
                printf("Ошибка выбора промежутка.\n");
                clear_input();
                continue;
            }

            printf("Введите начало (a): ");
            if (scanf("%lf", &a) != 1) { clear_input(); continue; }
            printf("Введите конец (b): ");
            if (scanf("%lf", &b) != 1) { clear_input(); continue; }
            printf("Введите шаг (h): ");
            if (scanf("%lf", &h) != 1) { clear_input(); continue; }

            if (h == 0) {
                printf("Ошибка: шаг не может быть равен нулю.\n");
                continue;
            }
            if ((h > 0 && a > b) || (h < 0 && a < b)) {
                printf("Ошибка: направление шага не соответствует интервалу.\n");
                continue;
            }
            if (a == b) {
                printf("Ошибка: начало и конец совпадают.\n");
                continue;
            }

            double a_adj = a, b_adj = b;
            if (interval_type == 2 || interval_type == 3) a_adj += 1e-9;
            if (interval_type == 2 || interval_type == 4) b_adj -= 1e-9;

            if (action == 3) tabulate(calculate_Y, a_adj, b_adj, h, "Y(x)");
            else if (action == 4) tabulate(calculate_V, a_adj, b_adj, h, "V(x)");
            else if (action == 5) print_graph(calculate_Y, a_adj, b_adj, h, "Y(x)");
            else if (action == 6) print_graph(calculate_V, a_adj, b_adj, h, "V(x)");
        }
        else if (action == 7 || action == 8) {
            if (action == 7) calculate_set(calculate_Y, "Y(x)");
            else calculate_set(calculate_V, "V(x)");
        }
        else if (action == 9 || action == 10) {
            if (action == 9) calculate_fixed_step(calculate_Y, "Y(x)");
            else calculate_fixed_step(calculate_V, "V(x)");
        }
        else if (action == 11 || action == 12) {
            if (action == 11) calculate_random(calculate_Y, "Y(x)");
            else calculate_random(calculate_V, "V(x)");
        }
    } while (1);

    return 0;
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double calculate_Y(double x) {
    return pow(x, 3) + 2 * x + sin(x);
}

double calculate_V(double x) {
    if (x < -1) {
        return log(1 + pow(x, 2)) - 2;
    }
    else if (x >= -1 && x <= 1) {
        return pow(x, 2) - 1;
    }
    else {
        return sqrt(x - 1) / (1 + pow(x, 2));
    }
}

void tabulate(double (*func)(double), double a, double b, double h, const char* name) {
    printf("\nТаблица значений для %s:\n", name);
    printf(" x\t\t%s\n", name);
    printf("------------------------\n");
    for (double i = a; (h > 0) ? (i <= b + 1e-9) : (i >= b - 1e-9); i += h) {
        printf("%.4lf\t\t%.4lf\n", i, func(i));
    }
}

void print_graph(double (*func)(double), double a, double b, double h, const char* name) {
    printf("\nПсевдографик для %s:\n", name);
    double max_val = 0;
    for (double i = a; (h > 0) ? (i <= b + 1e-9) : (i >= b - 1e-9); i += h) {
        double val = fabs(func(i));
        if (val > max_val) max_val = val;
    }
    if (max_val == 0) max_val = 1;
    int rows = 10;
    for (int r = rows; r >= -rows; r--) {
        double level = max_val * r / rows;
        printf("%7.2lf | ", level);
        for (double x = a; (h > 0) ? (x <= b + 1e-9) : (x >= b - 1e-9); x += h) {
            double val = func(x);
            if (fabs(val - level) <= (max_val / rows) / 2.0) printf("*");
            else printf(" ");
        }
        printf("\n");
    }
    printf("        +");
    for (double x = a; (h > 0) ? (x <= b + 1e-9) : (x >= b - 1e-9); x += h) printf("-");
    printf("\n         %.1lf", a);
    for (int i = 0; i < 5; i++) printf(" ");
    printf("%.1lf\n", b);
}

void calculate_set(double (*func)(double), const char* name) {
    int n;
    printf("Введите количество значений: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Ошибка ввода количества.\n");
        clear_input();
        return;
    }

    printf("Введите %d значений для x:\n", n);
    for (int i = 0; i < n; i++) {
        double x;
        printf("x[%d] = ", i + 1);
        if (scanf("%lf", &x) != 1) {
            printf("Ошибка ввода значения.\n");
            clear_input();
            return;
        }
        printf("  %s(%.4lf) = %.4lf\n", name, x, func(x));
    }
}

void calculate_fixed_step(double (*func)(double), const char* name) {
    double xmin, dx;
    printf("Введите начальное значение xmin: ");
    if (scanf("%lf", &xmin) != 1) { clear_input(); return; }

    printf("Введите шаг dx: ");
    if (scanf("%lf", &dx) != 1) { clear_input(); return; }

    printf("\nРасчет %s (10 значений, шаг %.4lf):\n", name, dx);
    printf(" x\t\t%s\n", name);
    printf("------------------------\n");

    for (int i = 0; i < 10; i++) {
        double x = xmin + i * dx;
        printf("%.4lf\t\t%.4lf\n", x, func(x));
    }
}

void calculate_random(double (*func)(double), const char* name) {
    int n;
    double xmin, xmax;

    printf("Введите количество случайных значений (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) { clear_input(); return; }

    printf("Введите левую границу интервала (xmin): ");
    if (scanf("%lf", &xmin) != 1) { clear_input(); return; }

    printf("Введите правую границу интервала (xmax): ");
    if (scanf("%lf", &xmax) != 1) { clear_input(); return; }

    if (xmin >= xmax) {
        printf("Ошибка: xmin должно быть меньше xmax.\n");
        return;
    }

    printf("\nРасчет %s для %d случайных значений на [%.4lf, %.4lf]:\n", name, n, xmin, xmax);
    printf(" x\t\t%s\n", name);
    printf("------------------------\n");

    for (int i = 0; i < n; i++) {
        double r = (double)rand() / RAND_MAX;
        double x = xmin + r * (xmax - xmin);
        printf("%.4lf\t\t%.4lf\n", x, func(x));
    }
}