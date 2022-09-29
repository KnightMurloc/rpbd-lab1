//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_EMPLOYEERGATEWAY_H
#define LAB1_EMPLOYEERGATEWAY_H

#include "../Gateway.h"
#include "Employeer.h"


class EmployeerGateway : public IGateway<Employeer> {
public:

    void save(std::shared_ptr<Employeer> data) override;

    std::shared_ptr<Employeer> create(
            std::string first_name,
            std::string last_name,
            std::string patronymic,
            std::string address,
            std::string birth_date,
            float salary,
            int movement_id,
            Post post);

    void remove(std::shared_ptr<Employeer> data) override;

    std::shared_ptr<Employeer> get(int id) override;

    std::list<std::shared_ptr<Employeer>> get_all() override;

    std::list<std::shared_ptr<Employeer>> get_great_then_by_name(std::string name,int id, int count);
    std::list<std::shared_ptr<Employeer>> get_less_then_by_name(std::string name,int id, int count);

    std::list<std::shared_ptr<Employeer>> get_great_then_by_id(int min, int count) override;
    std::list<std::shared_ptr<Employeer>> get_less_then_by_id(int max, int count) override;
};


#endif //LAB1_EMPLOYEERGATEWAY_H
