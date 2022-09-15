//
// Created by victor on 08.09.2022.
//

#ifndef LAB1_ORDERTAB_H
#define LAB1_ORDERTAB_H

#include "Tab.h"
#include "gateways/Orders/OrederGateway.h"
#include "gateways/Orders/Order.h"

class OrderTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        Order order;
    public:
        Gtk::Label* reason_label;
        Gtk::Label* order_number_label;
        Gtk::Label* order_date_label;
        Gtk::Label* post_label;

        explicit Entry(const Order &order);

        Order& get_order();

        int get_id() override;
    };

    OrederGateway gateway;
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Box* info_box;
    Gtk::Entry* reason_entry;
    Gtk::Entry* order_number_entry;
    Gtk::ComboBoxText* post_combobox;
    Gtk::Entry* day_entry;
    Gtk::Entry* month_entry;
    Gtk::Entry* year_entry;

    void select(Gtk::ListBoxRow* row);

    void create();

    void setup_menu(const Glib::RefPtr<Gtk::Builder>& builder);

    void remove();

protected:
    void fill_list(Gtk::ListBox* list) override;

public:
    explicit OrderTab(TabManager* tab_manager);

    void save_current();

    void cancel_current();

    void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_ORDERTAB_H
