#include <iostream>
#include <sstream>

void func(int n) {

    int i;
    int* ptr = nullptr;

    if (i > 0) {
        //.....
    }
    i = 0;

    char *str = new char[1024];
    for (i; i < 1024; ++i) {
        std::cout << str[i];
    }
    delete[] str;
}

class Vec {
public:
    Vec() : buf(nullptr), size(10) {
        buf = new char[size];
    }

    Vec(const Vec& v):  buf(nullptr), size(v.size) {
        std::cout << "Vec copy ctr" << std::endl;

        buf = new char[size];
        for (int i = 0; i < size; ++i) {
            buf[i] = v.buf[i];
        }

    }

    Vec& operator= (const Vec& v) {
        delete[] buf;

        size = v.size;
        std::cout << "operartor copy ctr" << std::endl;

        buf = new char[size];
        for (int i = 0; i < size; ++i) {
            buf[i] = v.buf[i];
        }
        return *this;
    }

    ~Vec() {
        std::cout << "~Vec" << std::endl;
        delete[] buf;
    }
private:
    char* buf;
    int size;
};

char* create_func_bad() {
    char *str = new char[1024];
    return str;
}

void free_func_bad(char* s) {
    delete[] s;
}

template<class T>
class Queue {
    struct Node {
        Node(): next(nullptr) {
        }
        Node(Node* next, const T& v) : next(next), value(value){
        }

        Node* next;
        T value;
    };
public:
    //head->node->node.....->node<-tail
    Queue() : head(nullptr), tail(nullptr) {

    }

    bool isEmpty() const {
        return head == tail && head == nullptr;
    }

    void push(const T& value) {
        Node* node = new Node();
        node->value = value;
        if (isEmpty()) {
            head = node;
            tail = node;
            return;
        }
        tail->next = node;
        tail = node;
    }

    void pop() {
        Node* top = head;

        head = head->next;
        delete top;
        //head->node->nullptr
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    const T& top() {
        return head->value;
    }


private:
    Node* head;
    Node* tail;
};


void testQueue() {
    {
        Queue<int> q;
        assert( q.isEmpty() );
    }

    {
        Queue<int> q;
        q.push(1);
        assert(q.top() == 1);
        q.pop();
        assert( q.isEmpty() );

        q.push(2);
        assert(q.top() == 2);
        q.pop();
        assert( q.isEmpty() );
    }

     {
        Queue<int> q;
        for (int i = 0; i < 100; ++i) {
            q.push(i);
            assert(q.top() == i);
            q.pop();
            assert( q.isEmpty() );
        }
    }

     {
        Queue<int> q;
        for (int i = 0; i < 100; ++i) {
            q.push(i);

        }
        assert( !q.isEmpty() );
        for (int i = 0; i < 100; ++i) {
            assert(q.top() == i);
            q.pop();
        }
    }
    std::cout << "test Queue OK" << std::endl;
}

void run(std::istream& in, std::ostream& out) {
    Queue<int> q;

    int n = 0;
    in >> n;
    for (int i = 0; i < n; ++i) {
        int operation = 0;
        int value = 0;
        int top = -1;
        in >> operation >> value;
        switch(operation) {
            case 2: //pop
                if (!q.isEmpty()) {
                    top = q.top();
                    q.pop();
                }
                if (top != value) {
                    out << "NO" << std::endl;
                    return;
                }
                break;
            case 3: //push
                q.push(value);
                break;
            default:
                out << "NO" << std::endl;

                return;
        }
    }
    out << "YES" << std::endl;
}

void testLogic() {
    {
        std::stringstream in;
        in << "3" << std::endl;
        in << "3 44" << std::endl;
        in << "3 50" << std::endl;
        in << "2 44" << std::endl;

        std::stringstream out;

        run(in, out);
        //std::cout << "lolol>>>" << out.str() << std::endl;
        assert(out.str() == "YES\n");

    }
     {
        std::stringstream in;
        in << "2" << std::endl;
        in << "3 44" << std::endl;
        in << "2 66" << std::endl;

        std::stringstream out;

        run(in, out);
        //std::cout << "lolol>>>" << out.str() << std::endl;
        assert(out.str() == "NO\n");

    }
    {
        std::stringstream in;
        in << "2" << std::endl;
        in << "2 -1" << std::endl;
        in << "3 1" << std::endl;

        std::stringstream out;

        run(in, out);
        //std::cout << "lolol>>>" << out.str() << std::endl;
        assert(out.str() == "YES\n");

    }
    {
        std::stringstream in;
        in << "1" << std::endl;
        in << "1 -1" << std::endl;

        std::stringstream out;

        run(in, out);
        //std::cout << "lolol>>>" << out.str() << std::endl;
        assert(out.str() == "NO\n");

    }
    std::cout << "testLogic OK" << std::endl;

}



int main() {
    //testQueue();
    //testLogic();
    run(std::cin, std::cout);

    return 0;
}
