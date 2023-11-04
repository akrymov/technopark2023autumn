#include <iostream>
#include <string>

size_t str_hash(const std::string& data) {
    size_t hash = 0;
    for (char i : data)
        hash = hash * 13 + i;
    return hash;
}

static constexpr size_t BUCKETS_SIZES_LIST[] = {
    7, 17, 37, 73, 149, 251, 509, 1021, 2027, 5003, 10837
};

template< class Key > struct Hash;

template<> struct Hash<int> {
    size_t operator()(int value) const {
        return value;
    }
};

template<> struct Hash<std::string> {
    size_t operator()(const std::string& value) const {
        return str_hash(value);
    }
};

template <typename Key, typename Value, typename Hasher=Hash<Key>>
class HashTable {
    struct Node {
        Node(const Key& key, const Value& value) : key(key), value(value), next(nullptr) {
        }

        Key key;
        Value value;

        Node* next;
    };

    static constexpr size_t MAX_LOAD_FACTOR = 7;
public:
    HashTable(Hasher hasher=Hasher()) : buckets(nullptr), buckets_size(0), buckets_sizes_list_idx(0), items_count(0), hasher(hasher) {
    }

    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;

    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    ~HashTable() {
        for (size_t i = 0; i < buckets_size; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    bool insert(const Key& key, const Value& value) {
        if (find(key)) {
            return false;
        }

        if (items_count >= MAX_LOAD_FACTOR * buckets_size) {
            rehash();
        }

        size_t bucket_idx = hasher(key) % buckets_size;
        Node* new_node = new Node(key, value);

        new_node->next = buckets[bucket_idx];
        buckets[bucket_idx] = new_node;
        items_count++;

        return true;
    }

    Value* find(const Key& key) {
        if (!buckets) {
            return nullptr;
        }

        size_t bucket_idx = hasher(key) % buckets_size;

        Node* cur = buckets[bucket_idx];
        while (cur) {
            if (cur->key == key) {
                return &cur->value;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool erase(const Key& key) {
        if (!find(key)) {
            return false;
        }

        size_t bucket_idx = hasher(key) % buckets_size;

        Node* cur = buckets[bucket_idx];
        Node* prev = nullptr;
        while (cur) {
            if (cur->key == key) {
                if (prev == nullptr) {
                    buckets[bucket_idx] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;

                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

private:
    void rehash() {
        size_t new_buckets_size = BUCKETS_SIZES_LIST[buckets_sizes_list_idx++];
        Node** new_buckets = new Node*[new_buckets_size];
        for (size_t i = 0; i < buckets_size; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;

                size_t bucket_idx = hasher(tmp->key) % new_buckets_size;
                tmp->next = new_buckets[bucket_idx];
                new_buckets[bucket_idx] = tmp;
            }
        }
        delete[] buckets;
        buckets = new_buckets;
        buckets_size = new_buckets_size;
    }

    Node** buckets;
    size_t buckets_size;
    size_t buckets_sizes_list_idx;

    size_t items_count;

    Hasher hasher;
};


int test() {
    HashTable<int, int> ht;
    int k = 10000;
    for (int i = 0; i < k; ++i) {
        assert(ht.insert(i, i));
    }
    for (int i = 0; i < k; ++i) {
        assert((*ht.find(i)) == i);
    }
    for (int i = 0; i < k; ++i) {
        assert(ht.erase(i));
        assert(ht.find(i) == nullptr);
    }
    std::cout << "ololo" << std::endl;
    return 0;
}


int main() {
    HashTable<std::string, int> ht;
    char command = '\0';
    std::string value;
    while(std::cin >> command >> value) {
        bool result = false;
        switch(command) {
            case '+':
                result = (ht.insert(value, 0) == true);
                break;
            case '-':
                result = (ht.erase(value) == true);
                break;
            case '?':
                result = (ht.find(value) != nullptr);
                break;
            default:
                return 1;
        }
        if (result) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }
    return 0;
}
