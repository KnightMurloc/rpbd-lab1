//
// Created by victor on 11.09.2022.
//

#ifndef LAB1_SNACKTAB_H
#define LAB1_SNACKTAB_H

#include "Tab.h"
#include "gateways/Snacks/Snacks.h"
#include "gateways/Snacks/SnackGateway.h"

class SnackTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        Snack snack;

    public:
        Gtk::Label* name_label;
        Gtk::Label* size_label;

        explicit Entry(Snack snack);

        Snack &get_snack();

        int get_id() override;
    };

    SnackGateway gateway;
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::Entry* size_entry;
    Gtk::Button* add_button;
    Gtk::Button* remove_button;
    Gtk::ListBox* ing_list;

    void select(Gtk::ListBoxRow* row);

    static void add_ingredient(Gtk::ListBox* list, TabManager* tab_manager);

    static void remove_ingredient(Gtk::ListBox* list);

    void save_current();

    void create();

    void remove_entry();

    void setup_menu(Glib::RefPtr<Gtk::Builder> builder);
protected:
    void fill_list(Gtk::ListBox* list) override;

public:
    explicit SnackTab(TabManager* tab_manager);

    void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_SNACKTAB_H
