//
// Created by victor on 16.09.2022.
//

#include "Provider.h"
#include "../BankDetail/BankDetailgateway.h"

int Provider::get_id() const {
    return id;
}

const std::string &Provider::get_name() const {
    return name;
}

const std::string &Provider::get_post_address() const {
    return post_address;
}

const std::string &Provider::get_phone_number() const {
    return phone_number;
}

const std::string &Provider::get_fax() const {
    return fax;
}

const std::string &Provider::get_email() const {
    return email;
}

void Provider::set_name(const std::string &name) {
    Provider::name = name;
}

void Provider::set_post_address(const std::string &postAddress) {
    post_address = postAddress;
}

void Provider::set_phone_number(const std::string &phoneNumber) {
    phone_number = phoneNumber;
}

void Provider::set_fax(const std::string &fax) {
    Provider::fax = fax;
}

void Provider::set_email(const std::string &email) {
    Provider::email = email;
}

Provider::Provider(int id) : id(id) {}

int Provider::get_bank_detail_id() const {
    return bank_detail_id;
}

void Provider::set_bank_detail_id(int bankDetail_id) {
    bank_detail_id = bankDetail_id;
}

std::shared_ptr<BankDetail> Provider::get_bank_detail(){
    if(bank_detail_id == -1){
        throw GatewayException("not found");
    }

    BankDetailgateway gateway;
    try{
        return gateway.get(bank_detail_id);
    }catch(GatewayException& e){
        bank_detail_id = -1;
        throw GatewayException("not found");
    }
}
