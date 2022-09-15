//
// Created by victor on 12.09.2022.
//

#ifndef LAB1_INGREDIENTTAB_H
#define LAB1_INGREDIENTTAB_H

#include "Tab.h"
#include "gateways/Ingredients/Ingredient.h"
#include "gateways/Ingredients/IngredientGateway.h"

class IngredientTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        Ingredient ingredient;
    public:
        Gtk::Label* name_label;
        Gtk::Label* unit_label;

        Entry(const Ingredient &ingredient);

        Ingredient &get_ingredient();

        int get_id() override;
    };

    IngredientGateway gateway;
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::ComboBoxText* unit_combo;

    void remove_entry();

    void create();

    void select(Gtk::ListBoxRow* row);

    void save();
protected:
    void fill_list(Gtk::ListBox* list) override;

public:




    explicit IngredientTab(TabManager* tab_manager);

    void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_INGREDIENTTAB_H
