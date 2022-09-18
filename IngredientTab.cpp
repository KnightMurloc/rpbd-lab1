//
// Created by victor on 12.09.2022.
//

#include "IngredientTab.h"
#include "Form.h"
#include "gateways/Ingredients/Ingredient.h"
#include "gateways/units.h"
#include "gtkmm/combobox.h"
#include "gtkmm/dialog.h"

IngredientTab::IngredientTab(TabManager* tab_manager) : Tab(tab_manager) {

    builder = Gtk::Builder::create_from_file("../ingridient_menu.glade");

    builder->get_widget("box", info_box);
    builder->get_widget("name_entry", name_entry);
    builder->get_widget("unit_combo", unit_combo);

    auto save_button = Gtk::make_managed<Gtk::Button>("сохранить");
    save_button->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::save));

    info_box->add(*save_button);
    info_box->show_all();

    this->fill_list(getListBox());

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::remove_entry));
    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::create));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&IngredientTab::select));

    add_clumn_lable("название");
    add_clumn_lable("еденицы");
}

// void IngredientTab::select_by_id(int entry_id) {
//
// }

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

void IngredientTab::select(Gtk::ListBoxRow* row){
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    if(box->get_children().empty() || box->get_children()[0]->get_name() != "ingrident_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    name_entry->set_text(entry->get_ingredient().get_name());
    unit_combo->set_active_id(unit_to_string(entry->get_ingredient().get_unit()));
}

void IngredientTab::save(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    if(name_entry->get_text().empty()){
        Gtk::MessageDialog message("имя не указано");
        message.run();
        return;
    }

    auto& ing = entry->get_ingredient();

    ing.set_name(name_entry->get_text());
    ing.set_unit(string_to_unit(unit_combo->get_active_id()));

    gateway.save(ing);

    entry->name_label->set_text(ing.get_name());
    entry->unit_label->set_text(unit_combo->get_active_id());
}

void IngredientTab::create(){
    auto builder = Gtk::Builder::create_from_file("../ingridient_menu.glade");

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("box", box);

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this,dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){

            Gtk::Entry* name_entry_dialog;
            Gtk::ComboBox* unit_combo_dialog;

            builder->get_widget("name_entry", name_entry_dialog);
            builder->get_widget("unit_combo", unit_combo_dialog);

            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("имя не указано");
                message.run();
                return;
            }

            Ingredient ing = gateway.create(name_entry_dialog->get_text(), string_to_unit(unit_combo_dialog->get_active_id()));

            auto entry = Gtk::make_managed<Entry>(ing);

            getListBox()->append(*entry);
            getListBox()->show_all();

            dialog->close();
            delete dialog;
        }else{
            dialog->close();
            delete dialog;
        }
    });

    dialog->show_all();
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
