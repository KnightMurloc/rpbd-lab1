//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_EMPLOYEER_H
#define LAB1_EMPLOYEER_H

#include <string>
#include <ctime>
#include "../posts.h"
#include "../entity.h"
#include <memory>
#include <list>

class Employeer : public IEntity {
private:
    int id;
    std::string first_name;
    std::string last_name;
    std::string patronymic;
    std::string address;
    std::string birth_date;
    float salary;
//     std movement_id = -1;
//     std::weak_ptr<Order> movement;
    Post post;
public:
    static std::list<std::shared_ptr<Employeer>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Employeer>> get_less_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Employeer>> get_great_than_by_id_filtred_by_name(std::string name,int id, int count);

    static std::list<std::shared_ptr<Employeer>> get_less_than_by_id_filtred_by_name(std::string name,int id, int count);

    static std::shared_ptr<Employeer> get(int id);

    static std::shared_ptr<Employeer> create(std::string first_name,
            std::string last_name,
            std::string patronymic,
            std::string address,
            std::string birth_date,
            float salary,
//             std::shared_ptr<Order> movement,
            Post post);

    static void save(std::shared_ptr<Employeer> empl);

    static void remove(std::shared_ptr<Employeer> empl);

    explicit Employeer(int id);

    [[nodiscard]] int get_id() const override;

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

//     [[nodiscard]] std::shared_ptr<Order> get_movement() const;
//
//     void set_movement(std::shared_ptr<Order> movement);

//     [[nodiscard]] int get_movement_id() const;

//     void set_movement_id(int movementId);

    [[nodiscard]] Post getPost() const;

    void setPost(Post post);

    [[nodiscard]] std::string getPostAsString() const;
};


#endif //LAB1_EMPLOYEER_H
