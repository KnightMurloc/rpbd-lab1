//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDER_H
#define LAB1_PROVIDER_H

#include <string>
#include <memory>
#include <list>
#include "../entity.h"

class Provider : public IEntity {
private:
    int id;
    std::string name;
    std::string post_address;
    std::string phone_number;
    std::string fax;
    std::string email;
//     int bank_detail_id = -1;
//     std::weak_ptr<BankDetail> bank_detail;
public:
    static std::list<std::shared_ptr<Provider>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Provider>> get_less_than_by_id(int id, int count);

    static std::shared_ptr<Provider> get(int id);

    static void save(std::shared_ptr<Provider> provider);

    static std::shared_ptr<Provider> create(
        std::string name,
        std::string post_address,
        std::string phone_number,
        std::string fax,
        std::string email
//         int bank_detail
    );

    static void remove(std::shared_ptr<Provider> provider);

    explicit Provider(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] const std::string &get_post_address() const;

    [[nodiscard]] const std::string &get_phone_number() const;

    [[nodiscard]] const std::string &get_fax() const;

    [[nodiscard]] const std::string &get_email() const;

//     [[nodiscard]] int get_bank_detail_id() const;
//     [[nodiscard]] std::shared_ptr<BankDetail> get_bank_detail();

//     void set_bank_detail(std::shared_ptr<BankDetail> bank_detail);

    void set_name(const std::string &name);

    void set_post_address(const std::string &postAddress);

    void set_phone_number(const std::string &phoneNumber);

    void set_fax(const std::string &fax);

    void set_email(const std::string &email);

//     void set_bank_detail_id(int bankDetail_id);
};


#endif //LAB1_PROVIDER_H
