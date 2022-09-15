//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINKSTAB_H
#define LAB1_DRINKSTAB_H

#include "Tab.h"
#include "gateways/Drinks/Drink.h"
#include "gateways/Drinks/Drinkgateway.h"

class DrinksTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        Drink drink;
    public:

        Gtk::Label* name_label;
        Gtk::Label* strength_label;
        Gtk::Label* size_label;
        Gtk::Label* container_label;

        Entry(const Drink &drink);

        const Drink &getDrink() const;

        int get_id() override;
    };

    Drinkgateway gateway;

protected:
    void fill_list(Gtk::ListBox *list) override;

public:

    DrinksTab(TabManager* manager);

    void select_by_id(int entry_id) override;


};


#endif //LAB1_DRINKSTAB_H
