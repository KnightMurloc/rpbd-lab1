//
// Created by victor on 12.09.2022.
//

#include "IngredientTab.h"

IngredientTab::IngredientTab(TabManager* tab_manager) : Tab(tab_manager) {
    this->fill_list(getListBox());

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::remove_entry));
}

void IngredientTab::select_by_id(int entry_id) {

}

void IngredientTab::fill_list(Gtk::ListBox* list) {
    for(const auto& ing : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(ing);
        list->add(*entry);
    }
}

//int IngredientTab::select_dialog() {
//    Gtk::Dialog dialog;
//
//    Gtk::ListBox list;
//
//    auto data = gateway.get_all();
//    for(auto& ing : data){
//        auto entry = Gtk::make_managed<Entry>(ing);
//        list.append(*entry);
//    }
//
//    dialog.add_button("OK", Gtk::RESPONSE_OK);
//    dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);
//
//    Gtk::ScrolledWindow scroll;
//    scroll.set_vexpand(true);
//
//    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(scroll);
//
//    scroll.show_all();
//    if(dialog.run() == Gtk::RESPONSE_OK){
//        auto entry = dynamic_cast<Entry*>(list.get_selected_row());
//        return entry->get_order().get_id();
//    }
//
//    return 0;
//}

void IngredientTab::create(){

}

void IngredientTab::remove_entry(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());

    if(entry == nullptr){
        return;
    }

    gateway.remove(entry->get_ingredient());

    getListBox()->remove(*entry);
}

IngredientTab::Entry::Entry(const Ingredient &ingredient) : ingredient(ingredient) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(ingredient.get_name());
    unit_label = Gtk::make_managed<Gtk::Label>(unit_to_string(ingredient.get_unit()));

    box->add(*name_label);
    box->add(*unit_label);

    this->add(*box);
}

Ingredient &IngredientTab::Entry::get_ingredient() {
    return ingredient;
}

int IngredientTab::Entry::get_id() {
    return ingredient.get_id();
}
