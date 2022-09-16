//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAILGATEWAY_H
#define LAB1_BANKDETAILGATEWAY_H

#include "../Gateway.h"
#include "BankDetail.h"

class BankDetailgateway : public IGateway<BankDetail> {
public:
    void save(BankDetail &data) override;

    BankDetail create(
        std::string bank_name,
        std::string city,
        std::string tin,
        std::string settlement_account
    );

    BankDetail get(int id) override;

    void remove(BankDetail &data) override;

    std::list<BankDetail> get_all() override;
};


#endif //LAB1_BANKDETAILGATEWAY_H
