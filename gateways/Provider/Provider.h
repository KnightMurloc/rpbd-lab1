//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDER_H
#define LAB1_PROVIDER_H

#include <string>

class Provider {
private:
    int id;
    std::string name;
    std::string post_address;
    std::string phone_number;
    std::string fax;
    std::string email;
    int bank_detail;
public:

    explicit Provider(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] const std::string &get_post_address() const;

    [[nodiscard]] const std::string &get_phone_number() const;

    [[nodiscard]] const std::string &get_fax() const;

    [[nodiscard]] const std::string &get_email() const;

    [[nodiscard]] int get_bank_detail() const;

    void set_name(const std::string &name);

    void set_post_address(const std::string &postAddress);

    void set_phone_number(const std::string &phoneNumber);

    void set_fax(const std::string &fax);

    void set_email(const std::string &email);

    void set_bank_detail(int bankDetail);
};


#endif //LAB1_PROVIDER_H
