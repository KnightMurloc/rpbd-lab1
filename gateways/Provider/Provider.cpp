//
// Created by victor on 16.09.2022.
//

#include "Provider.h"
#include "../BankDetail/BankDetailgateway.h"
#include "ProviderGateway.h"

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

// int Provider::get_bank_detail_id() const {
//     return bank_detail_id;
// }

// void Provider::set_bank_detail_id(int bankDetail_id) {
//     bank_detail_id = bankDetail_id;
// }

// void Provider::set_bank_detail(std::shared_ptr<BankDetail> bank_detail){
//     this->bank_detail = bank_detail;
// }

// std::shared_ptr<BankDetail> Provider::get_bank_detail(){
// //     if(bank_detail_id == -1){
// //         throw GatewayException("not found");
// //     }
// //
// //     BankDetailgateway gateway;
// //     try{
// //         return gateway.get(bank_detail_id);
// //     }catch(GatewayException& e){
// //         bank_detail_id = -1;
// //         throw GatewayException("not found");
// //     }
//
//     if(auto ptr = bank_detail.lock()){
//         return ptr;
//     }else{
//         throw GatewayException("not found");
//     }
//
// }

std::shared_ptr<Provider> Provider::get(int id){
    ProviderGateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Provider>> Provider::get_great_than_by_id(int id, int count) {
    ProviderGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Provider>> Provider::get_less_than_by_id(int id, int count) {
    ProviderGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void Provider::save(std::shared_ptr<Provider> provider){
    ProviderGateway gateway;
    gateway.save(provider);
}

std::shared_ptr<Provider> Provider::create(
        std::string name,
        std::string post_address,
        std::string phone_number,
        std::string fax,
        std::string email
//         int bank_detail
    ){
    ProviderGateway gateway;
    return gateway.create(name,post_address,phone_number,fax,email);
}

void Provider::remove(std::shared_ptr<Provider> provider){
    ProviderGateway gateway;
    gateway.remove(provider);
}
