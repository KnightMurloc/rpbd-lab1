//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_EMPLOYEERGATEWAY_H
#define LAB1_EMPLOYEERGATEWAY_H

#include "../Gateway.h"
#include "Employeer.h"

class EmployeerGateway : public IGateway<Employeer> {
public:

    void save(Employeer &data) override;

    Employeer create(
            std::string first_name,
            std::string last_name,
            std::string patronymic,
            std::string address,
            std::string birth_date,
            float salary,
            int movement_id,
            Post post
            );

    void remove(Employeer &data) override;

    Employeer get(int id) override;

    std::list<Employeer> get_all() override;
};


#endif //LAB1_EMPLOYEERGATEWAY_H
