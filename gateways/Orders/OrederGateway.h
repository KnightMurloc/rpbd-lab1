//
// Created by victor on 08.09.2022.
//

#ifndef LAB1_OREDERGATEWAY_H
#define LAB1_OREDERGATEWAY_H

#include "Order.h"
#include "../Gateway.h"
class OrederGateway : public IGateway<Order> {
public:
    void save(Order &data) override;

    Order create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post);

    Order get(int id) override;

    void remove(Order &data) override;

    std::list<Order> get_all() override;
};


#endif //LAB1_OREDERGATEWAY_H
