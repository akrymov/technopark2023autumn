#include <iostream>
#include <vector>

void sort_simple(int *arr, int l, int r) {
    for (int i = l; i < r; ++i) {
        for (int j = l; j < r - 1; ++j) {
            if (arr[j + 1] < arr[j]) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
    return;
}

bool is_less(const int& l, const int& r) {
    return l < r;
}

bool is_more(const int& l, const int& r) {
    return l > r;
}

void sort_cmp( int *arr, int l, int r, bool (*comp)(const int&, const int&) ) {
    for (int i = l; i < r; ++i) {
        for (int j = l; j < r - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
    return;
}



struct CompCounter {
    CompCounter(int* counter) : counter(counter) {
    }

    bool operator()(const int& l, const int& r) {
        (*counter)++;
        return l > r;
    }

    int* counter;
};

void sort_cmp_comparator( int *arr, int l, int r, CompCounter comp ) {
    for (int i = l; i < r; ++i) {
        for (int j = l; j < r - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
    return;
}

template <class T>
void sort_tmpl_bad( T *arr, int l, int r) {
    for (int i = l; i < r; ++i) {
        for (int j = l; j < r - 1; ++j) {
            if (arr[j + 1] <  arr[j]) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
    return;
}

template<class T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <class T, class Comp=DefaultComparator<T>>
void sort( T *arr, int l, int r, Comp comp=DefaultComparator<T>() ) {
    for (int i = l; i < r; ++i) {
        for (int j = l; j < r - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
    return;
}

struct Point {
    int x;
    int y;

    //bool operator<(const Point& r) const {
    //    return this->x < r.x;
    //}
};

struct PointComparator {
    PointComparator(const Point& b) : base(b) {

    }

    bool operator()(const Point& l, const Point& r) const {
        int l_dst = abs(base.x - l.x)*abs(base.x - l.x) + abs(base.y - l.y)*abs(base.y - l.y);
        int r_dst = abs(base.x - r.x)*abs(base.x - r.x) + abs(base.y - r.y)*abs(base.y - r.y);

        return l_dst < r_dst;
    }

    Point base;
};

bool operator<(const Point& l, const Point& r) {
    return l.x < r.x;
}

template <class T, class Comp=DefaultComparator<T>>
class Heap {
public:
    Heap(Comparator comp=DefaultComparator<T>());
    //....

    const T& top() const;
    void push(const &T val);
    void pop();

    size_t size();

private:
    void sift_up(size_t idx);
    void sift_down(size_t idx);

    void heapify();
    void grow();

    Comparator comp;

    T* buf;
    size_t capacity;
    size_t size;

};


int main2() {
    std::vector<int> arr = {5,4,3,2,1,1,23,4,5, -1, 0};
    std::vector<Point> arr_point = {
        {5,4},
        {3,2},
        {1,1},
        {23,4},
        {5, -1},
        {0,0}
    };

    //sort_cmp(&arr[0], 0, arr.size(), is_more);
    //int counter = 0;
    //sort_cmp_comparator(&arr[0], 0, arr.size(), CompCounter(&counter));
    PointComparator comp({0, 0});
    int counter = 0;
    sort<Point>(&arr_point[0], 0, arr_point.size(), [&counter](const Point& l, const Point& r) {
        counter++;
        return l.x < r.x;
    });

    sort(&arr[0], 0, arr.size());
    sort(&arr_point[0], 0, arr_point.size());



    //std::cout << "counter: " << counter << std::endl;
    for(int i =0; i < arr_point.size(); ++i) {
        std::cout << "{" << arr_point[i].x << ", "  << arr_point[i].y << "} ";
    }

    std::cout << std::endl;
    return 0;
}

struct Desc {
    int *buf;
    size_t size;
    size_t pos;

    bool operator<(const Desc& r) const {
        return buf[pos] < r.buf[r.pos];
    }

    int value() const {
        return buf[pos];
    }
};

int main() {
    std::vector<Desc> descrs;


    Heap<Desc> h;
    while (h.size() > 0) {
        Desc d = h.top();
        std::cout << d.value();
        h.pop();
        d.pos++;
        if (d.pos >= d.size) {
            continue;
        }
        h.push(d);
    }

}
