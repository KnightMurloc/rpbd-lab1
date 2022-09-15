//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAILTAB_H
#define LAB1_BANKDETAILTAB_H

#include "Tab.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gateways/BankDetail/BankDetailgateway.h"

class BankDetailTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        BankDetail detail;
    public:

        Gtk::Label* name_label;
        Gtk::Label* city_label;
        Gtk::Label* tin_label;
        Gtk::Label* account_label;

        explicit Entry(BankDetail detail);

        int get_id() override;
    };

    BankDetailgateway gateway;
public:
    explicit BankDetailTab(TabManager* manager);

    void select_by_id(int entry_id) override;

protected:
    void fill_list(Gtk::ListBox *list) override;
};


#endif //LAB1_BANKDETAILTAB_H
