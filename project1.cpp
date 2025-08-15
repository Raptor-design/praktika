// practica-1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    int N;
    cout << "Введите размер массива: ";
    cin >> N;

    vector<int> A(N);
    cout << "Введите элементы массива: ";
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
    }

    if (N == 0) {
        cout << "Массив пуст." << endl;
        return 0;
    }

    int max_val = A[0], min_val = A[0];
    int max_idx = 0, min_idx = 0;

    // Находим индексы максимального и минимального элементов
    for (int i = 1; i < N; ++i) {
        if (A[i] > max_val) {
            max_val = A[i];
            max_idx = i;
        }
        if (A[i] < min_val) {
            min_val = A[i];
            min_idx = i;
        }
    }

    // Определяем границы диапазона
    int start = min(max_idx, min_idx);
    int end = max(max_idx, min_idx);

    // Суммируем отрицательные элементы в этом диапазоне
    int sum_negatives = 0;
    for (int i = start + 1; i < end; ++i) {
        if (A[i] < 0) {
            sum_negatives += A[i];
        }
    }

    cout << "Сумма отрицательных элементов между максимальным и минимальным: " << sum_negatives << endl;

    return 0;
}
