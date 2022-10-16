//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_GATEWAY_H
#define LAB1_GATEWAY_H

#include <list>
#include <string>
// #include <cache.hpp>
// #include <lru_cache_policy.hpp>
#include <memory>
#include <string_view>
#include "../Repository.h"
#include <sstream>

constexpr std::size_t CACHE_SIZE = 32;

// template <typename Key, typename Value>
// using lru_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;

template<class T>
class IGateway{
protected:

//     constexpr static std::size_t CACHE_SIZE = 256;
//     lru_cache_t<int, T> cache(32);
//     caches::fixed_sized_cache<int, T, caches::LRUCachePolicy> cache(CACHE_SIZE);
//     static std::unique_ptr<lru_cache_t<int,std::shared_ptr<T>>> cache;
//     static lru_cache_t<int,std::shared_ptr<T>> cache;

    static Repository<T> cache_new;

    static std::string escape_string(std::string_view str){
        std::stringstream stream;
        for(const auto& c : str){
            switch(c){
                case '\'':
                    stream << "''";
                    break;
                default:
                    stream << c;
            }
        }

        return stream.str();
    }
public:

    virtual void save(std::shared_ptr<T> data) = 0;

    virtual std::shared_ptr<T> get(int id) = 0;

    virtual void remove(std::shared_ptr<T> data) = 0;

    virtual std::list<std::shared_ptr<T>> get_all() = 0;

    void remove_from_cache(int id){
//         cache.Remove(id);
        cache_new.remove(id);
    }

    virtual std::list<std::shared_ptr<T>> get_great_then_by_id(int min, int count) = 0;
    virtual std::list<std::shared_ptr<T>> get_less_then_by_id(int max, int count) = 0;
};

template<class T>
Repository<T> IGateway<T>::cache_new{};

class GatewayException : public std::exception {
public:

    GatewayException(GatewayException& it) noexcept = default;

    explicit GatewayException(std::string msg) : msg_(msg) {}

    ~GatewayException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

#endif //LAB1_GATEWAY_H
