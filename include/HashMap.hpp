#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <list>
#include <string>
class TreeNode;
template <typename K, typename V>
class HashMap{
private:
    int capacity;
    int size;
    std::list<std::pair<K, V>> *table;
    int hashFunction(const K& key) {
    if constexpr (std::is_same<K, std::string>::value) {
        const int p = 31;
        const int m = 1000000009;
        long long hashValue = 0, p_pow = 1;
        for (unsigned char c : key) {
            hashValue = (hashValue + (c + 1) * p_pow) % m;  
            p_pow = (p_pow * p) % m;
        }
        return static_cast<int>(hashValue % capacity);
    } else {
        return key % capacity;
    }
}
    void rehash(){
        int oldCapacity = capacity;
        capacity *= 2;
        auto* newTable = new std::list<std::pair<K, V>>[capacity];
        for (int i = 0; i < oldCapacity; i++) {
            for (auto &p : table[i]) {
                int newIndex = hashFunction(p.first);
                newTable[newIndex].push_back(p);
            }
        }

        delete[] table;
        table = newTable;
    }
public:
    HashMap(int initialCapacity = 10){
        capacity = initialCapacity;
        size = 0;
        table = new std::list<std::pair<K, V>>[capacity];
    }
    ~HashMap(){delete[] table;}
    void insertItem(K key, V value){
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        for (auto &p : cell) {
            if (p.first == key) {  
                p.second = value;
                return;
            }
        }
        cell.push_back({key, value});
        size++;
        double loadFactor = (1.0 * size) / capacity;
        if (loadFactor > 0.7) {
            rehash();
        }
    }
    void removeItem(K key){
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        for (auto it = cell.begin(); it != cell.end(); ++it) {
            if (it->first == key) {
                cell.erase(it);
                size--;
                return;
            }
        }
    }
    V searchTable(K key){
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        for (auto &p : cell) {
            if (p.first == key) {
                return p.second;
            }
        }
        return V{};
    }
    int getSize(){return size;}
    int getCapacity(){return capacity;}
    bool contains(K key){
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        for (auto &p : cell){
            if (p.first == key){
                return true;
            }
        }
        return false;
    }
    std::vector<std::pair<K, V>> getAllItems() const{
        std::vector<std::pair<K, V>> items;
        for (int i = 0; i < capacity; i++) {
            for (auto &p : table[i]) {
                items.push_back(p);
            }
        }
        return items;
    }
};

#endif