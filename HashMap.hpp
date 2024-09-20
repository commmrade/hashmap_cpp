#include <forward_list>
#include <functional>
#include <ios>
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>
#include<unordered_map>

template<typename Key, typename Value>
class HashMap {
private:
    std::vector<std::list<std::pair<Key, Value>>> map;
    size_t bucket_count;
    size_t element_count;
    size_t capacity;

    
    size_t hashFunc(const Key& key) {
        return std::hash<Key>{}(key) % capacity;
    }
public:

    HashMap(size_t init_capacity = 20) : capacity(init_capacity), bucket_count(0), element_count(0) {
        map.resize(capacity);
    }

    void insert(const Key &key, const Value &val) {
        size_t index = hashFunc(key);
        for (auto &[k, v] : map[index]) {
            if (k == key) {
                v = val;
                return;
            }
        }

        if (map[index].empty()) {
            bucket_count++;
        }
        map[index].push_front({key, val});
        element_count++;

        if (static_cast<double>(element_count) / capacity > 0.8) {
            rehash(capacity * 2);
        }
    }
    void erase(const Key &key) {
        size_t index = hashFunc(key);

        for (auto b = map[index].begin(); b != map[index].end(); b++) {
            auto &[k, v] = *b;

            if (k == key) {
                if (map[index].size() == 1) {
                    map[index].clear();
                    element_count--;
                    bucket_count--;
                    break;
                }
                map[index].erase(b);
                element_count--;
                break;
            }
        }
    }

    void rehash(size_t new_capacity) {
        std::vector<std::list<std::pair<Key, Value>>> temp_map;
        temp_map.resize(new_capacity);
        capacity = new_capacity;

        for(auto &bucket : map) {
            for (const auto &[k, v] : bucket) {
                size_t new_index = hashFunc(k);
                temp_map[new_index].push_back({k, v});
            }
        }
        map.clear();
        map.shrink_to_fit();
        map = std::move(temp_map);
    }

    bool exists(const Key& key) {
        try {
            auto val = at(key);
            return true;
        } catch (std::out_of_range &ex) {
            return false;
        }
    }
    Value& at(const Key& key) {
        size_t index = hashFunc(key);
        for (auto &[k, v] : map[index]) {
            if (key == k) {
                return v;
            }
        }
        throw std::out_of_range("Value not found");
    }
    Value& operator[](const Key& key) {
        if (exists(key)) {
            return at(key);
        } else {
            insert(key, {});
            return at(key);
        }
    }

    size_t buck_count() {
        return bucket_count;
    }
    size_t buck_size(size_t bucket_pos) {
        return map[bucket_pos].size();
    }
    size_t elements_count() {
        return element_count;
    }
    double max_load_factor() {
        return 1.0;
    }
    double cur_load_factor() {
        return static_cast<double>(element_count) / bucket_count;
    }

};