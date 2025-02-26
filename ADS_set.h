#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename Key, size_t N = 6>
class ADS_set {
public:
    class Iterator;
    using value_type = Key;
    using key_type = Key;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using const_iterator = Iterator;
    using iterator = const_iterator;
    using key_equal = std::equal_to<key_type>;
    using hasher = std::hash<key_type>;

private:
    struct bucket {
        key_type* arr {new key_type[N]};
        size_t size{0}; 
        bucket* overflow {nullptr};

        bool is_full() {
            return size == N;
        }
        friend std::ostream& operator<<(std::ostream& o, const bucket& obj) {
            for(size_type i{0}; i < obj.size; ++i) {
                o << "[" << obj.arr[i] << "] ";
            }
            for(size_type i{obj.size}; i < N; ++i) {
                o << "[-] ";
            }
            const bucket* last{&obj};
            while(last->overflow != nullptr) {
                o << "---> ";
                o << *last->overflow;
                last = last->overflow;
            }
            return o;
        }
    };
    //Inctance variables
    bucket** table {nullptr};
    size_type capacity {0}; // max size
    size_type size_of_table {0}; // real size_of_table, number of buckets
    size_type count_element{0}; //the amount of elements
    size_type nextToSplit {0};
    size_type round {0};

    //Private functions
    size_type hashing(const key_type& value) const { 
        size_type unique_value = hasher{}(value); 
        size_type index = unique_value % (1 << round);
        if (index < nextToSplit)
            index = unique_value % (1 << (round+1)); 
        return index;
    }

    void extend_table() {
        capacity *= 2;
        auto new_table = new bucket*[capacity];
        for (size_type i{0}; i < size_of_table; ++i) {
            new_table[i] = table[i];
        }
        delete [] table;
        table = new_table;
    }

    void split() {

        if (size_of_table == capacity)
            extend_table();

        size_of_table++;
        table[size_of_table-1] = new bucket;


        bucket* to_split{table[nextToSplit]};
        table[nextToSplit] = new bucket;
        nextToSplit++;

        rehash(to_split);
        delete_bucket(to_split);

        if(nextToSplit >= static_cast<size_type>((1 << round))) {
            round++;
            nextToSplit = 0;
        }
    }

    void rehash(bucket* to_split) {
        bool not_caused_by_splittig {false};
        for(size_type i{0}; i < to_split->size; ++i)
            insert_element(to_split->arr[i], not_caused_by_splittig);

        while(to_split->overflow != nullptr) {
            for(size_type i{0}; i < to_split->overflow->size; ++i)
                insert_element(to_split->overflow->arr[i], not_caused_by_splittig);
            to_split = to_split->overflow;

        }
    }

    bool identify(const key_type& key) const {
        size_type index {hashing(key)};
        bucket* current = table[index];
        bool found{false};
        for (size_type i{0}; i < current->size; ++i) {
            if (key_equal{}(current->arr[i], key)) {
                found = true;
                break;
            }
        }
        while(current->overflow != nullptr) {
            current = current->overflow;
            for (size_type j{0}; j < current->size; ++j) {
                if (key_equal{}(current->arr[j], key)) {
                    found = true;
                    break;
                }
            }
        }
        return found;
    }

    void insert_element(const key_type& value, bool not_caused_by_splitting = true) {
        size_type index = hashing(value);
        bucket* last = table[index];
        while(last->overflow != nullptr) {
            last = last->overflow;
        }
        if (last->is_full()) {
            last->overflow = new bucket;
            last = (last->overflow);
            last->arr[last->size] = value;
            last->size++;
            if (not_caused_by_splitting)
                split();
        }
        else {
            last->arr[last->size] = value;
            last->size++;
        }
        if (not_caused_by_splitting)
            count_element++;
    }
    void delete_bucket(bucket* actual_bucket) {
        if (actual_bucket->overflow != nullptr) {
            delete_bucket(actual_bucket->overflow);
        }
        delete[] actual_bucket->arr;
        delete actual_bucket;
    }
//Public functions
public:

    ADS_set() {
        capacity = 1 << round;
        table = new bucket*[capacity];
        table[0] = new bucket;
        size_of_table++;
    } 

    ADS_set(std::initializer_list<key_type> ilist) : ADS_set() {
        this->insert(ilist);
    }

    template<typename InputIt> ADS_set(InputIt first, InputIt last) : ADS_set() {
        this->insert(first, last);
    }

    ADS_set(const ADS_set &other) {
        capacity = other.capacity;
        size_of_table = other.size_of_table;
        count_element = other.count_element;
        nextToSplit = other.nextToSplit;
        round = other.round;
        table = new bucket *[capacity];
        for (size_type i{0}; i < other.size_of_table; ++i) {
            table[i] = new bucket;
            auto actual_bucket_copy{other.table[i]};

            do {
                size_type j{0};
                while (j < actual_bucket_copy->size) {
                    insert_element(actual_bucket_copy->arr[j], false);
                    j++;
                }

                actual_bucket_copy = actual_bucket_copy->overflow;
            } while (actual_bucket_copy != nullptr);

        }
    }

    ~ADS_set() {
        for (size_type i{0}; i < size_of_table; ++i) {
            bucket* actual_bucket {table[i]};
            delete_bucket(actual_bucket);
        }
        delete[] table;
    }

    ADS_set &operator=(const ADS_set &other) {
        ADS_set help(other);
        this->swap(help);
        return *this;
    }

    ADS_set &operator=(std::initializer_list<key_type> ilist) {
        ADS_set help(ilist);
        this->swap(help);
        return *this;
    }

    size_type size() const {
        return count_element;
    }

    bool empty() const {
        return this->size() == 0;
    }

    void insert(std::initializer_list<key_type> ilist) {
        for(const auto& el: ilist) {
            if(!count(el))
                insert_element(el);
        }
    }

    std::pair<iterator,bool> insert(const key_type &key) {
        if(!count(key)) {
            insert_element(key);
            return std::make_pair(find(key),true);
        }
        return std::make_pair(find(key), false);
    }

    template<typename InputIt> void insert(InputIt first, InputIt last) {
        for (auto i{first}; i != last; i++) {
            if(!count(*i))
                insert_element(*i);
        }
    }

    void clear() {
        ADS_set help;
        swap(help);
    }

    size_type erase(const key_type &key) { 
        size_type index = hashing(key);
        auto actual_bucket = table[index];
        bucket* previous_bucket = nullptr;
        do {
            for (size_type j{0}; j < actual_bucket->size; ++j) {
                if (key_equal{}(actual_bucket->arr[j], key)) {
                    if (j != actual_bucket->size-1)
                        actual_bucket->arr[j] = actual_bucket->arr[actual_bucket->size-1];
                    actual_bucket->size--;
                    count_element--;

                    if (actual_bucket->size == 0 && previous_bucket != nullptr) {
                        previous_bucket->overflow = actual_bucket->overflow;
                        delete[] actual_bucket->arr;
                        delete actual_bucket;
                    }
                    return 1;
                }
            }
            previous_bucket = actual_bucket;
            actual_bucket = actual_bucket->overflow;
        } while (actual_bucket != nullptr);
        return 0;
    }

    size_type count(const key_type &key) const {
        return static_cast<size_type>(identify(key));
    }

    iterator find(const key_type &key) const {
        size_type index = hashing(key);
        auto current_bucket = table[index];
        while (current_bucket != nullptr) {
            for (size_type j = 0; j < current_bucket->size; ++j) {
                if (key_equal{}(current_bucket->arr[j], key)) {
                    return iterator(table, current_bucket, index, &current_bucket->arr[j], j, size_of_table);
                }
            }
            current_bucket = current_bucket->overflow;
        }
        return end();
    }

    void swap(ADS_set &other) {
        std::swap(table, other.table);
        std::swap(size_of_table, other.size_of_table);
        std::swap(capacity, other.capacity);
        std::swap(count_element, other.count_element);
        std::swap(nextToSplit, other.nextToSplit);
        std::swap(round, other.round);
    }

    const_iterator begin() const {
        if(this->empty())
            return this->end();
        value_type* element;
        bucket* current_bucket;

        for (size_type i{0}; i < size_of_table; ++i) {
            current_bucket = table[i];
            while(current_bucket != nullptr) {
                if(current_bucket->size > 0) {
                    element = &current_bucket->arr[0];
                    return iterator{table, current_bucket, i, element, 0, size_of_table};

                }
                current_bucket= current_bucket->overflow;
            }
        }
        return end();
    }

    const_iterator end() const {
        return iterator{table,table[size_of_table], size_of_table, nullptr, 0, size_of_table};
    }

    void dump(std::ostream &o = std::cerr) const {
        o << "Table size: " << size_of_table << ", elements: " << count_element << std::endl;
        o << "d(round) = " << round << " Next to Split: " << nextToSplit << std::endl;
        for (size_type i{0}; i < size_of_table ; ++i) {
            o << i << "  ";
            o << *table[i]; 
            o << std::endl;
        }
    }

    friend bool operator==(const ADS_set &lhs, const ADS_set &rhs) {
        bool all{true};
        for(size_type i{0}; i < lhs.size_of_table; ++i) {
            auto actual_bucket{lhs.table[i]};
            do {
                for(size_type j{0}; j < actual_bucket->size; ++j)
                    if(!rhs.count(actual_bucket->arr[j]))
                        all = false;
                actual_bucket = actual_bucket->overflow;
            } while (actual_bucket != nullptr);
        }
        if (lhs.size() == rhs.size() && all)
            return true;
        return false;
    }
    friend bool operator!=(const ADS_set &lhs, const ADS_set &rhs) {
        return !(lhs == rhs);
    }

};

template <typename Key, size_t N>
class ADS_set<Key,N>:: Iterator {
public:
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type &;
    using pointer = const value_type *;
    using iterator_category = std::forward_iterator_tag;
private:
    bucket** table;
    bucket* current_bucket;
    size_type bucket_index;
    pointer element; 
    size_type index;
    size_type size_table;
public:
    explicit Iterator (bucket** table = nullptr, bucket* current_bucket = nullptr, size_type bucket_index = 0, pointer element = nullptr, size_type index = 0, size_type size_table = 0) : table{table},
                                                                                                                                                                                           current_bucket{current_bucket}, bucket_index{bucket_index}, element{element}, index{index}, size_table{size_table} {}
    reference operator*() const {
        if (element == nullptr)
            throw std::runtime_error("Iterator is out of range!");
        return *element;
    }
    pointer operator->() const {
        if (element == nullptr)
            throw std::runtime_error("Iterator is out of range!");
        return element;
    }

    iterator &operator++() {
        if (element == nullptr)
            return *this;

        if(index + 1 < current_bucket->size) {
            index++;
            element++;
            return *this;
        }

        while (current_bucket->overflow != nullptr) { 
            current_bucket = current_bucket->overflow;
            if (current_bucket->size > 0) {
                index = 0;
                element = &current_bucket->arr[index];
                return *this;
            }
        }
        while (bucket_index + 1 < size_table) {
            bucket_index++;
            current_bucket = table[bucket_index];
            if (current_bucket->size > 0) {
                index = 0;
                element = &current_bucket->arr[index];
                return *this;
            }

            while (current_bucket->overflow != nullptr) {
                current_bucket = current_bucket->overflow;
                if (current_bucket->size > 0) {
                    index = 0;
                    element = &current_bucket->arr[index];
                    return *this;
                }
            }

        }

        current_bucket = table[size_table];
        element = nullptr;

        return *this;
    }
    iterator operator++(int) {
        iterator help(table, current_bucket, bucket_index,element, index, size_table);
        ++(*this);
        return help;
    }

    friend bool operator==(const iterator &lhs, const iterator &rhs) {
        if(lhs.element == rhs.element)
            return true;
        return false;
    }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
        return !(lhs == rhs);
    }

};

template <typename Key, size_t N>
void swap(ADS_set<Key,N> &lhs, ADS_set<Key,N> &rhs) { lhs.swap(rhs); }


#endif //ADS_SET_H