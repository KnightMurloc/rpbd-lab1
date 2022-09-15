//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_GATEWAY_H
#define LAB1_GATEWAY_H

#include <list>
#include <string>

template<class T>
class IGateway{
public:
    virtual void save(T& data) = 0;

    virtual T get(int id) = 0;

    virtual void remove(T& data) = 0;

    virtual std::list<T> get_all() = 0;
};

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
