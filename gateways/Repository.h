//
// Created by victor on 03.10.2022.
//

#ifndef LAB1_REPOSITORY_H
#define LAB1_REPOSITORY_H

#include "fmt/core.h"
#include <map>
#include <memory>
#include <fmt/format.h>

template<typename T>
class Repository {
private:
    std::map<int,std::weak_ptr<T>> map;
public:
    void put(std::shared_ptr<T> data){
        for(auto entry : map){
            if(entry.second.expired()){
//                 fmt::print("free: {}\n", entry.first);
                map.erase(entry.first);
                break;
            }
        }
        map.insert(std::make_pair(data->get_id(),data));
    }

    bool exist(int key){
//         fmt::print("exist: {} - {}\n",key,  map.find(key) != map.end() && !map[key].expired());
        return map.find(key) != map.end() && !map[key].expired();
    }

    std::shared_ptr<T> get(int key){
        return map[key].lock();
    }

    void remove(int key){
        map.erase(key);
    }
};


#endif //LAB1_REPOSITORY_H
