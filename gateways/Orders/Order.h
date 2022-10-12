//
// Created by victor on 08.09.2022.
//

#ifndef LAB1_ORDER_H
#define LAB1_ORDER_H

#include <string>
#include "../posts.h"
#include <memory>
#include "../entity.h"
#include <list>

class Order : IEntity {
private:
    int id;
    std::string reason;
    int order_number;
    std::string order_date; //TODO конвертировать
    Post post;
public:
    static std::list<std::shared_ptr<Order>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Order>> get_less_than_by_id(int id, int count);

    static std::shared_ptr<Order> get(int id);

    static void save(std::shared_ptr<Order> order);

    static std::shared_ptr<Order> create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post);

    static void remove(std::shared_ptr<Order> order);

    explicit Order(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_reason() const;

    void set_reason(const std::string &reason);

    [[nodiscard]] int get_order_number() const;

    void set_order_number(int orderNumber);

    [[nodiscard]] const std::string &get_order_date() const;

    void set_order_date(const std::string &orderDate);

    [[nodiscard]] Post get_post() const;

    void set_post(Post post);

    std::string get_post_as_string() const;
};


#endif //LAB1_ORDER_H
