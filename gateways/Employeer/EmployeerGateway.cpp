//
// Created by victor on 07.09.2022.
//

#include "EmployeerGateway.h"
#include "../DbInstance.h"
#include <iostream>
#include <fmt/format.h>

Employeer EmployeerGateway::get(int id) {
    return Employeer(id);
}

std::list<Employeer> EmployeerGateway::get_all() {

    std::list<Employeer> result;

    auto db = DbInstance::getInstance();

    auto response = db.exec("select * from employees;");

    //TODO сдлеать установку id  не публичной
    while(response.next()){
        Employeer employeer(response.get<int>(0));
        employeer.setFirstName(response.get<std::string>(1));
        employeer.setLastName(response.get<std::string>(2));
        if(!response.is_null(3)) {
            employeer.setPatronymic(response.get<std::string>(3));
        }
        employeer.setAddress(response.get<std::string>(4));
        employeer.setBirthDate(response.get<std::string>(5));
//        std::cout << response.get<std::string>(5) << std::endl;
//        employeer.setBirthDate(response.get<st>(5));
        employeer.setSalary(response.get<float>(6));
        if(!response.is_null(7)) {
            employeer.set_movement_id(response.get<int>(7));
        }
//        if(response.get<std::string>(8) == "waiter"){
//            employeer.setPost(Post::waiter);
//        }else if(response.get<std::string>(8) == "bartender"){
//            employeer.setPost(Post::bartender);
//        }else if(response.get<std::string>(8) == "manager"){
//            employeer.setPost(Post::manager);
//        }
        employeer.setPost(string_to_post(response.get<std::string>(8)));
        result.push_back(employeer);
    }

    return result;
}

void EmployeerGateway::save(Employeer &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update employees set"
                                  " first_name = '{}',"
                                  " last_name = '{}',"
                                  " patronymic = '{}',"
                                  " address = '{}',"
                                  " birth_date = '{}',"
                                  " salary = {},"
                                  " movement = {},"
                                  " post = '{}'"
                                  " where id = {};",
                                  data.getFirstName(),
                                  data.getLastName(),
                                  data.getPatronymic(),
                                  data.getAddress(),
                                  data.getBirthDate(),
                                  data.getSalary(),
                                  data.get_movement_id(),
                                  post_to_string(data.getPost()),
                                  data.getId()
                                  );

    db.exec(sql);
}

Employeer
EmployeerGateway::create(std::string first_name, std::string last_name, std::string patronymic, std::string address,
                         std::string birth_date, float salary, int movement_id, Post post) {
    auto db = DbInstance::getInstance();
    std::string sql = fmt::format("insert into employees(first_name,last_name,patronymic,address,birth_date,salary,movement,post)"
                                  " values('{}','{}','{}', '{}', '{}', {}, {}, '{}')"
                                  " returning id;",
                                  first_name,
                                  last_name,
                                  patronymic,
                                  address,
                                  birth_date,
                                  salary,
                                  movement_id == -1 ? "NULL" : std::to_string(movement_id),
                                  post_to_string(post)
                                  );
    auto result = db.exec(sql);

    if(result.next()){
        Employeer empl(result.get<int>(0));
        empl.setFirstName(first_name);
        empl.setLastName(last_name);
        empl.setPatronymic(patronymic);
        empl.setAddress(address);
        empl.setBirthDate(birth_date);
        empl.setSalary(salary);
        empl.set_movement_id(movement_id);
        empl.setPost(post);
        return empl;
    }

    return Employeer(0);
}

void EmployeerGateway::remove(Employeer &data) {
    
}
