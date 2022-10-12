//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAILTAB_H
#define LAB1_BANKDETAILTAB_H

#include "Tab.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gateways/BankDetail/BankDetailgateway.h"
#include "glibmm/refptr.h"
#include "gtkmm/listboxrow.h"
#include <iostream>

class BankDetailTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<BankDetail> detail;
    public:

        Gtk::Label* name_label;
        Gtk::Label* city_label;
        Gtk::Label* tin_label;
        Gtk::Label* account_label;

        explicit Entry(std::shared_ptr<BankDetail> detail);

        std::shared_ptr<BankDetail> get_bank_detail();

        int get_id() override;
    };

//     BankDetailgateway gateway;

    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::Entry* city_entry;
    Gtk::Entry* tin_entry;
    Gtk::Entry* settlement_entry;

    EntityList<BankDetail,Entry>* list;

    void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void select(Gtk::ListBoxRow* row);

    void save_current();

    void create();

    void remove_entry();

protected:

    IList* create_list() override;
public:
    explicit BankDetailTab(TabManager* manager);

};


#endif //LAB1_BANKDETAILTAB_H
