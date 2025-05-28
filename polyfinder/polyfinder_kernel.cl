int getWeight(int value);
int check_d(long candidate, int m, int d);
long codeword(long data, long generator, int m);
int check_reminder(long candidate, int n);
long reminder(long value1, long value2);
int getBitsLength(int value);
long power(int a, int b);

__kernel void polyfinder_kernel(
    __global const long* candidates,  // Буфер для кандидатов
    __global int* stop,              // Флаг остановки
    __global long* result_candidate, // Результат вычислений
    const int count,
    const int m,                     
    const int t,                     
    const int d,                     
    const int n                      
) {
    int gid = get_global_id(0);
    if (gid >= count) return;
    if (*stop == 1) return;
    long candidate = candidates[gid];
    if (check_d(candidate, m, d)) {
        if (!check_reminder(candidate, n)) {
            *result_candidate = candidate;
            atomic_xchg(stop, 1);
        }
    }
}

int getWeight(int value) {
    int weight = 0;
    printf("getWeight: start, value = %d\n", value); // Отладочный вывод
    while (value >= 1) {
        if (value & 1) weight++;
        value >>= 1;
        printf("getWeight: value = %d, weight = %d\n", value, weight); // Отладочный вывод
    }
    printf("getWeight: end, result weight = %d\n", weight); // Отладочный вывод
    return weight;
}

int check_d(long candidate, int m, int d) {
    long data = 1;
    do {
        printf("check_d: data = %d\n", data); // Отладочный вывод
        long first = codeword(data, candidate, m);
        int distance = getWeight(first);
        printf("check_d: first = %d, distance = %d\n", first, distance);
        if (distance < d) {
            return 0; // false
        }
        data++;
    } while (data < power(2, m)); // ?
    return 1; // true
}

long codeword(long data, long generator, int m) {
    printf("codeword: start, data = %d, generator = %d, m = %d\n", data, generator, m); // Отладочный вывод
    long _data = data << (getBitsLength(generator) - 1);
    printf("codeword: _data after shift = %d\n", _data); // Отладочный вывод
    long result = _data + reminder(_data, generator);
    printf("codeword: end, result = %d\n", result); // Отладочный вывод
    return result;
}

long power(int a, int b) {
    long result = 1;
    for (int i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

long reminder(long value1, long value2) {
    printf("\nvalue1: %d", value1);
        printf("\nvalue2: %d", value2);

    int count1 = getBitsLength(value1);
    int count2 = getBitsLength(value2);
    int delta = count1 - count2;
    long result = value1;
    printf("reminder: start count1 = %d, count2 = %d\n", count1, count2); // Отладочный вывод
    while (count1 >= count2) {
        result = result ^ (value2 << delta);
        count1 = getBitsLength(result);  // Изменено для правильного обновления count1
        delta = count1 - count2;
        printf("reminder: result = %d, count1 = %d, delta = %d\n", result, count1, delta);
    }
    return result;
}

int check_reminder(long candidate, int n) {
    long poly = power(2, n) + 1; // pow(2, n) + 1
    int _reminder = reminder(poly, candidate);
    return _reminder != 0 ? 1 : 0;
}

int getBitsLength(int value) {
    int iters = 0;
    while (value != 0) {
        value >>= 1;
        iters++;
    }
    return iters;
}