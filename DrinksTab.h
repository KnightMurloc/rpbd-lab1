//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINKSTAB_H
#define LAB1_DRINKSTAB_H

#include "Tab.h"
#include "gateways/Drinks/Drink.h"
#include "gateways/Drinks/Drinkgateway.h"
#include "gtkmm/entry.h"

class DrinksTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<Drink> drink;
    public:

        Gtk::Label* name_label;
        Gtk::Label* strength_label;
        Gtk::Label* size_label;
        Gtk::Label* container_label;

        Entry(std::shared_ptr<Drink> drink);

        std::shared_ptr<Drink> get_drink();

        int get_id() override;
    };

//     Drinkgateway gateway;

    EntityList<Drink,Entry>* list;


    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::Entry* strength_entry;
    Gtk::Entry* size_entry;
    Gtk::Entry* container_entry;
    Gtk::ListBox* ing_list;
    Gtk::Button* add_button;
    Gtk::Button* remove_button;

    void select(Gtk::ListBoxRow* row);

    static void add_ingredient(Gtk::ListBox* list, TabManager* tab_manager);
    static void remove_ingredient(Gtk::ListBox* list);

    void save_current();

    void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void create();

    void remove_entry();
protected:
    IList* create_list() override;
public:
    DrinksTab(TabManager* manager);
};


#endif //LAB1_DRINKSTAB_H
