//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_TAB_H
#define LAB1_TAB_H

#include <gtkmm.h>
#include <memory>
class Tab;
#include "TabManager.h"

class Tab : public Gtk::Box {
private:
    TabManager* tab_manager;

    Gtk::Button* add_button;
    Gtk::Button* remove_button;

    Gtk::ListBox* listBox;

    Gtk::ButtonBox* create_top_panel();

protected:
    TabManager* get_tab_manager() const;

    Gtk::Button *getAddButton() const;

    Gtk::Button *getRemoveButton() const;

    Gtk::ListBox *getListBox() const;

    class IEntry {
    public:
        virtual int get_id() = 0;
    };

    virtual void fill_list(Gtk::ListBox* list) = 0;
public:
    Tab(TabManager* tab_manager);

    void select_by_id(int entry_id);

//    virtual int select_dialog() = 0;
    int select_dialog();


};

inline bool check_date(int day, int month, int year)
{
    if ((month == 1 || month == 3 || month == 5 || month == 7 ||
         month == 8 || month == 10 || month == 12) && ( day>31 || day<1) ){
        return false;
    }
    else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day>30 || day<1) ){
        return false;
    }
    else if ((month == 2) && (year % 4 == 0) && (day>29 || day<1)){
        return false;
    }
    else if ((month == 2) && (year % 4 != 0) && (day>28 || day<1) ){
        return false;
    }
    else if ((month < 1) || (month > 12)){
        return false;;
    }
    return true;
}

#endif //LAB1_TAB_H
