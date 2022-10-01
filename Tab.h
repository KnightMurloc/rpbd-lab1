//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_TAB_H
#define LAB1_TAB_H

#include "gateways/entity.h"
#include <gtkmm.h>
#include <memory>
class Tab;
#include "TabManager.h"
#include "EntityList.h"

class Tab : public Gtk::Box {
private:

    TabManager* tab_manager;

    Gtk::Button* add_button;
    Gtk::Button* remove_button;

    IList* list;
//    Gtk::ListBox* listBox;

//    Gtk::Box* header;

    Gtk::ButtonBox* create_top_panel();

//    void scroll_event(Gtk::PositionType type);


protected:
    sigc::signal<void(std::shared_ptr<IEntity>)> on_remove;

    TabManager* get_tab_manager() const;

    Gtk::Button *getAddButton() const;

    Gtk::Button *getRemoveButton() const;
    virtual IList* create_list() = 0;
public:
    IList* get_list() const;

    void set_list(IList* list);

    Tab(TabManager* tab_manager);

    void select_by_id(int entry_id);

//    virtual int select_dialog() = 0;
    int select_dialog();

    void remove_entry_by_id(int id);

    sigc::signal<void(std::shared_ptr<IEntity>)>& signal_remove();
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
