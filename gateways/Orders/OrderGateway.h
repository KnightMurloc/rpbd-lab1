//
// Created by victor on 08.09.2022.
//

#ifndef LAB1_ORDERGATEWAY_H
#define LAB1_ORDERGATEWAY_H

#include "Order.h"
#include "../Gateway.h"
class OrderGateway : public IGateway<Order> {
public:
    void save(std::shared_ptr<Order> data) override;

    std::shared_ptr<Order> create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post);

    std::shared_ptr<Order> get(int id) override;

    void remove(std::shared_ptr<Order> data) override;

    std::list<std::shared_ptr<Order>> get_all() override;

    std::list<std::shared_ptr<Order>> get_great_then_by_id(int min, int count);
    std::list<std::shared_ptr<Order>> get_less_then_by_id(int max, int count);
};


#endif //LAB1_ORDERGATEWAY_H
