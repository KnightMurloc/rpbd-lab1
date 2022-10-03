//
// Created by victor on 05.09.2022.
//

#include "Tab.h"
#include "EntityList.h"
#include "gateways/entity.h"
#include "gtkmm/box.h"
#include "gtkmm/dialog.h"
#include "gtkmm/enums.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/label.h"
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "gtkmm/searchbar.h"
#include "gtkmm/searchentry.h"
#include "gtkmm/stockid.h"
#include "sigc++/functors/mem_fun.h"
#include <iostream>
#include <memory>

Tab::Tab(TabManager* tab_manager) : tab_manager(tab_manager) {
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    this->add(*create_top_panel());
}

Gtk::ButtonBox *Tab::create_top_panel() {

    auto top_panel = Gtk::make_managed<Gtk::ButtonBox>();
    top_panel->set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    add_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-add"));
    top_panel->add(*add_button);
    remove_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-delete"));
    top_panel->add(*remove_button);

    return top_panel;
}

Gtk::Button *Tab::getAddButton() const {
    return add_button;
}


Gtk::Button *Tab::getRemoveButton() const {
    return remove_button;
}

TabManager* Tab::get_tab_manager() const {
    return tab_manager;
}

int Tab::select_dialog() {

    Gtk::Dialog dialog;
    dialog.set_size_request(500,500);
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);

    auto list = create_list();

    auto box = dynamic_cast<Gtk::Box*>(list);

    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(*box);

    box->show_all();

    if(dialog.run() == Gtk::RESPONSE_OK){
        auto entry = list->get_selected();
        return entry->get_id();
    }

    return -1;
}

void Tab::select_by_id(int entry_id){

    list->fill_list();

   do{
       for(auto child : list->get_children()){
           auto entry = dynamic_cast<IEntry*>(child);
           if(entry->get_id() == entry_id){
               list->select_row(dynamic_cast<Gtk::ListBoxRow*>(entry));
               return;
           }
       }
   }while(list->scroll_down());
}

void Tab::remove_entry_by_id(int id){
   for(auto child : list->get_children()){
       auto entry = dynamic_cast<IEntry*>(child);
       if(entry->get_id() == id){
           list->remove_row(child);
           break;
       }
   }
}

IList* Tab::get_list() const {
    return list;
}

void Tab::set_list(IList* list) {
    Tab::list = list;
    auto box = dynamic_cast<Gtk::Box*>(list);
    add(*box);
}

sigc::signal<void(std::shared_ptr<IEntity>)>& Tab::signal_remove(){
    return on_remove;
}
