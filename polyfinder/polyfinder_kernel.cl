int getWeight(int value);
int check_d(long candidate, int m, int d);
long codeword(long data, long generator, int m);
int check_reminder(long candidate, int n);
long reminder(long value1, long value2);
int getBitsLength(int value);
long power(int a, int b);

__kernel void polyfinder_kernel(
    __global const long* candidates,  // candidate buffer
    __global int* stop,              // stop-flag buffer
    __global long* result_candidate, // result buffer
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
    while (value >= 1) {
        if (value & 1) weight++;
        value >>= 1;
    }
    return weight;
}

int check_d(long candidate, int m, int d) {
    long data = 1;
    do {
        long first = codeword(data, candidate, m);
        int distance = getWeight(first);
        if (distance < d) {
            return 0; // false
        }
        data++;
    } while (data < power(2, m)); // ?
    return 1; // true
}

long codeword(long data, long generator, int m) {
    long _data = data << (getBitsLength(generator) - 1);
    long result = _data + reminder(_data, generator);
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
    int count1 = getBitsLength(value1);
    int count2 = getBitsLength(value2);
    int delta = count1 - count2;
    long result = value1;
    while (count1 >= count2) {
        result = result ^ (value2 << delta);
        count1 = getBitsLength(result);
        delta = count1 - count2;
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