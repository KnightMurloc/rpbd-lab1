//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_EMPLOYEER_H
#define LAB1_EMPLOYEER_H

#include <string>
#include <ctime>
#include "../posts.h"
#include "../Orders/Order.h"

class Employeer {
private:
    int id;
    std::string first_name;
    std::string last_name;
    std::string patronymic;
    std::string address;
    std::string birth_date; //TODO конвертировать
    float salary;
    int movement_id = -1;
    Post post;
public:
    explicit Employeer(int id);

    //TODO поменять названия методов
    [[nodiscard]] int getId() const;

    [[nodiscard]] const std::string &getFirstName() const;

    void setFirstName(const std::string &firstName);

    [[nodiscard]] const std::string &getLastName() const;

    void setLastName(const std::string &lastName);

    [[nodiscard]] const std::string &getPatronymic() const;

    void setPatronymic(const std::string &patronymic);

    [[nodiscard]] const std::string &getAddress() const;

    void setAddress(const std::string &address);

    [[nodiscard]] std::string getBirthDate() const;

    void setBirthDate(std::string birthDate);

    [[nodiscard]] float getSalary() const;

    void setSalary(float salary);

    [[nodiscard]] int get_movement_id() const;

    void set_movement_id(int movementId);

    [[nodiscard]] Post getPost() const;

    void setPost(Post post);

    [[nodiscard]] std::string getPostAsString() const;

    [[nodiscard]] Order get_movement();
};


#endif //LAB1_EMPLOYEER_H
