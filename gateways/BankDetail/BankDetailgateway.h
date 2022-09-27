//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAILGATEWAY_H
#define LAB1_BANKDETAILGATEWAY_H

#include "../Gateway.h"
#include "BankDetail.h"

class BankDetailgateway : public IGateway<BankDetail> {
public:
    void save(std::shared_ptr<BankDetail> data) override;

    std::shared_ptr<BankDetail> create(
        std::string bank_name,
        std::string city,
        std::string tin,
        std::string settlement_account
    );

    std::shared_ptr<BankDetail> get(int id) override;

    void remove(std::shared_ptr<BankDetail> data) override;

    std::list<std::shared_ptr<BankDetail>> get_all() override;

    std::list<std::shared_ptr<BankDetail>> get_great_then_by_id(int min, int count);
    std::list<std::shared_ptr<BankDetail>> get_less_then_by_id(int max, int count);
};


#endif //LAB1_BANKDETAILGATEWAY_H
