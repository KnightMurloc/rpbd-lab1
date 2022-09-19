//
// Created by victor on 08.09.2022.
//

#ifndef LAB1_ORDERGATEWAY_H
#define LAB1_ORDERGATEWAY_H

#include "Order.h"
#include "../Gateway.h"
class OrderGateway : public IGateway<Order> {
public:
    void save(Order &data) override;

    Order create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post);

    std::shared_ptr<Order> get(int id) override;

    void remove(Order &data) override;

    std::list<Order> get_all() override;
};


#endif //LAB1_ORDERGATEWAY_H
