//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_EMPLOYEESTAB_H
#define LAB1_EMPLOYEESTAB_H

#include <gtkmm.h>
#include "Tab.h"
#include "gateways/Employeer/EmployeerGateway.h"

class EmployeesTab : public Tab {
private:
class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        std::shared_ptr<Employeer> emp;

    public:
        Gtk::Label* first_name;
        Gtk::Label* last_name;
        Gtk::Label* patronymic;
        Gtk::Label* address;
        Gtk::Label* birth_date;
        Gtk::Label* salary;
        Gtk::Label* post;

    public:
        explicit Entry(std::shared_ptr<Employeer> emp);

        std::shared_ptr<Employeer> get_emp();

    int get_id() override;
};

    Glib::RefPtr<Gtk::Builder> builder;

    EmployeerGateway gateway;

    Gtk::Box* info_box;

    Gtk::Entry* first_name_entry;
    Gtk::Entry* last_name_entry;
    Gtk::Label* order_link;
    Gtk::Button* find_button;
    Gtk::Button* select_button;
    Gtk::Entry* patronymic_entry;
    Gtk::Entry* address_entry;
    Gtk::Entry* day_entry;
    Gtk::Entry* month_entry;
    Gtk::Entry* year_entry;
    Gtk::Entry* salary_entry;
    Gtk::ComboBoxText* post_combobox;

    static void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void select(Gtk::ListBoxRow* row);

    void find_order();

    static void select_order(Gtk::Label* label, TabManager* manager);

    void create();

    void remove();

protected:
    void fill_list(Gtk::ListBox* list) override;

public:
    explicit EmployeesTab(TabManager* tab_manager);

    void save_current();

    void cancel_current();

//     void select_by_id(int entry_id) override;

//    int select_dialog() override;


};


#endif //LAB1_EMPLOYEESTAB_H
