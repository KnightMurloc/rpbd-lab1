//
// Created by victor on 30.09.2022.
//

#ifndef LAB1_SNACKORDER_H
#define LAB1_SNACKORDER_H

#include "../entity.h"
#include <memory>
#include "../Employeer/Employeer.h"
#include "../Snacks/Snacks.h"
class SnackOrder : public IEntity {
private:
    int id;
    int snack;
    int waiter;
    int table_;

public:
    [[nodiscard]] int get_id() const override;

    explicit SnackOrder(int id);

    [[nodiscard]] int get_snack_id() const;

    [[nodiscard]] int get_waiter_id() const;

    [[nodiscard]] int get_table() const;

    [[nodiscard]] std::shared_ptr<Snack> get_snack();

    [[nodiscard]] std::shared_ptr<Employeer> get_waiter();

    void set_snack_id(int product);

    void set_waiter_id(int waiter);

    void set_table(int table);
};


#endif //LAB1_CLIENTORDERTAB_H
