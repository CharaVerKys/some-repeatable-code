#pragma once

#include <stdexcept>
#include <vector>
namespace cvk{
template <typename T>
concept key = true;

template <typename T>
concept value = true;

template<key K, value V>
class flat_map{
    std::vector<K> keys; // all implicit constructors looks ok 
    std::vector<V> vals;
    size_t binarySearch(const K& key){
        assert(not keys.empty());
        size_t lowId = 0;
        size_t highId = keys.size()-1;
        while(lowId <= highId){
            size_t mid = lowId + (highId-lowId)/2;
            if(keys[mid] <=> key == std::strong_ordering::equal){
                return mid;
            }

            if(keys[mid] <=> key == std::strong_ordering::less){
                lowId = mid+1;
            }else{
                highId = mid-1;
                if(mid == 0){break;}
            }
            if(lowId == highId){
                if(keys[lowId] <=> key == std::strong_ordering::equal){
                    return lowId;
                }
                break;
            }
        }
        throw std::out_of_range("no value with this key");
    }
public:
    flat_map(){}
    V& at(const K& key){
        return vals[binarySearch(key)];
    }
    void insert(const K& key, const V& value){
        size_t index = 0;
        while(index < keys.size()){
            if(key <=> keys[index] == std::strong_ordering::greater){
                ++index;
            }else if(key <=> keys[index] == std::strong_ordering::equal){
                throw std::runtime_error("this key already exist");
            }else if(key <=> keys[index] == std::strong_ordering::less){
                break;
            }
        }
        // ? insert BEFORE iterator
        vals.insert(vals.begin()+index, value);
        keys.insert(keys.begin()+index, key);
        // ? for 0 1 2 4 if want to insert key=3
        // ? it will place before 4, cuz 3 less that 4
        // ?                (index 3)
        // ?     0 1 2 3 4
        // ?            new index 3 is new value
    }
    void remove(const K& key){
        if(keys.empty()){throw std::runtime_error("flat_map is empty, nothing to remove");}
        int index = binarySearch(key);
        vals.erase(vals.begin()+index);
        keys.erase(keys.begin()+index);
    }
    
    V& at_tryFirst(const uint& tryFindFaster, const K& key){
        assert(tryFindFaster <= keys.size());
        for(uint i = 0 ; i < tryFindFaster; ++i){
            if(key <=> keys[i] == std::strong_ordering::equal){
                return vals[i];
            }
        }
        return vals[binarySearch(key)];
    }
};
}// namespace cvk