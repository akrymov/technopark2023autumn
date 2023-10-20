#include <iostream>
#include <vector>
#include <algorithm>

template<typename T, typename Comp>
int partition(T* arr, int l, int r, Comp comp) {
    int pivot_idx = l + (r - l) / 2;
    int pivot = arr[pivot_idx];

    std::swap(arr[r - 1], arr[pivot_idx]);

    int i = l;
    int j = r - 1;

    while (i < j) {
        for(; comp(arr[i], pivot); i++) {
        }
        // arr[i] >= pivot

        while (!comp(arr[j], pivot)) {
            j--;
        }
        //arr[j] < pivot
        if (i < j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    std::swap(arr[r - 1], arr[i]);
    return i;
}


template<class T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <class T, class Comp=DefaultComparator<T>>
void kth_element(T* arr, int size, int k, Comp comp=Comp()) {
    int l = 0;
    int r = size;
    while ( l < r ) {
        int m = partition(arr, l, r, comp);
        if (k == m) {
            return;
        } else if (k < m) {
            r = m;
        } else { //k > m
            l = m + 1;
        }
    }
}



void unstable_count_sort(int* arr, int size, int R) {
    R = R + 1;
    int* counters = new int[R];
    for (int i = 0; i < R; ++i) {
        counters[i] = 0;
    }
    for (int i = 0; i < size; ++i) {
        counters[ arr[i] ]++;
    }

    int output_idx = 0;
    for (int counters_idx = 0; counters_idx < R; ++counters_idx) {
        int counter = counters[counters_idx];
        while (counter > 0) {
            arr[ output_idx++ ] = counters_idx;
            counter--;
        }
    }
    delete[] counters;
}

struct Person {
    std::string name;
    int year;

    operator int() const {
        return year;
    }
};

template<typename T>
struct DefaultKey {
    int operator()(const T& value) const {
        return value;
    }
};

template<typename T, typename Key=DefaultKey<T>>
void stable_count_sort(T* arr, int size, int R, Key key=Key()) {
    R = R + 1;
    int* counters = new int[R];
    for (int i = 0; i < R; ++i) {
        counters[i] = 0;
    }
    for (int i = 0; i < size; ++i) {
        counters[ key(arr[i]) ]++;
    }

    for (int i = 0; i < R - 1; i++) {
        counters[i + 1] += counters[i];
    }

    T* output = new T[size];
    for (int i = 0; i < size; ++i) {
        output[ --counters[ key(arr[i]) ] ] = arr[i];
    }

    for (int i = 0; i < size; ++i) {
        arr[i] = output[i];
    }

    delete[] output;
    delete[] counters;
}


int main() {
    std::vector<int> arr = {1,3,5,7,3,4,6,12,2,2};

    std::vector<int> expected(arr);
    std::sort(expected.begin(), expected.end());

    int k = 2;
    kth_element(&arr[0], arr.size(), k);
    assert(arr[k] == 2);

    for (int i = 0; i < arr.size(); ++i) {
        kth_element(&arr[0], arr.size(), i);
        assert(arr[i] == expected[i]);
    }

    {
        std::vector<int> arr = {1,3,5,7,3,4,6,12,2,2};

        std::vector<int> expected(arr);
        std::sort(expected.begin(), expected.end());

        unstable_count_sort(&arr[0], arr.size(), 12);

        for (int i = 0; i < arr.size(); ++i) {
            assert(arr[i] == expected[i]);
        }
    }

    {
        std::vector<int> arr = {1,3,5,7,3,4,6,12,2,2};

        std::vector<int> expected(arr);
        std::sort(expected.begin(), expected.end());

        stable_count_sort(&arr[0], arr.size(), 12);

        for (int i = 0; i < arr.size(); ++i) {
            assert(arr[i] == expected[i]);
        }
    }

    std::vector<Person> persons = {{"99999", 9}, {"11111", 1}, {"333333", 3}};

    stable_count_sort(&persons[0], persons.size(), 12/*, [](const Person& value){
        return value.year;
    }*/);
    for(auto& p : persons) {
        std:: cout << p.name << ", " << p.year << std::endl;
    }
    std::cout << "OK" << std::endl;
}
