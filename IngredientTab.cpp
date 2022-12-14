//
// Created by victor on 12.09.2022.
//

#include "IngredientTab.h"
#include "Form.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "gateways/Ingredients/Ingredient.h"
#include "gateways/entity.h"
#include "gateways/units.h"
#include "gtkmm/combobox.h"
#include "gtkmm/dialog.h"
#include "sigc++/functors/mem_fun.h"
#include <memory>

IngredientTab::IngredientTab(TabManager* tab_manager) : Tab(tab_manager) {

//    current_search = std::make_unique<DefaultSearch>(&gateway);
    list = dynamic_cast<EntityList<Ingredient,Entry>*>(create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../ingridient_menu.glade");

    builder->get_widget("box", info_box);
    builder->get_widget("name_entry", name_entry);
    builder->get_widget("unit_combo", unit_combo);

    auto save_button = Gtk::make_managed<Gtk::Button>("сохранить");
    save_button->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::save));

    info_box->add(*save_button);
    info_box->show_all();

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::remove_entry));
    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::create));

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&IngredientTab::select));
}

void IngredientTab::search(EntityList<Ingredient,Entry>* list){
   std::string str = list->get_search_text();
   std:: transform(str.begin(), str.end(), str.begin(), ::tolower);
//    auto gateway = dynamic_cast<IngredientGateway*>(list->get_gateway());
//    if(gateway == nullptr){
//        return;
//     }
   auto ptr = std::make_unique<NameSearch>(str );
   list->set_search(std::move(ptr));
}

void IngredientTab::select(Gtk::ListBoxRow* row){
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    current = entry->get_ingredient();

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    if(box->get_children().empty() || box->get_children()[0]->get_name() != "ingrident_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    name_entry->set_text(entry->get_ingredient()->get_name());
    unit_combo->set_active_id(unit_to_string(entry->get_ingredient()->get_unit()));
}

void IngredientTab::save(){
//    auto entry = dynamic_cast<Entry*>(list->get_selected());
//    if(entry == nullptr){
//        return;
//    }

   if(name_entry->get_text().empty()){
       Gtk::MessageDialog message("имя не указано");
       message.run();
       return;
   }

//    auto ing = entry->get_ingredient();

    auto ing = std::dynamic_pointer_cast<Ingredient>(current);
    if(!ing){
        return;
    }

   ing->set_name(name_entry->get_text());
   ing->set_unit(string_to_unit(unit_combo->get_active_id()));

//    gateway.save(ing);
   Ingredient::save(ing);

   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry && entry->get_ingredient()->get_id() == ing->get_id()){
    entry->name_label->set_text(ing->get_name());
    entry->unit_label->set_text(unit_combo->get_active_id());
   }
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

//             auto ing = gateway.create(name_entry_dialog->get_text(), string_to_unit(unit_combo_dialog->get_active_id()));

            auto ing = Ingredient::create(name_entry_dialog->get_text(), string_to_unit(unit_combo_dialog->get_active_id()));

            auto entry = Gtk::make_managed<Entry>(ing);

            list->add_entity(entry);
            list->show_all();

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
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

//    gateway.remove(entry->get_ingredient());
    on_remove.emit(entry->get_ingredient());
    Ingredient::remove(entry->get_ingredient());

    Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);
   current = std::shared_ptr<Ingredient>();
}

IngredientTab::Entry::Entry(std::shared_ptr<Ingredient> ingredient) : ingredient(ingredient) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
    unit_label = Gtk::make_managed<Gtk::Label>(unit_to_string(ingredient->get_unit()));

    box->add(*name_label);
    box->add(*unit_label);

    this->add(*box);
}

std::shared_ptr<Ingredient> IngredientTab::Entry::get_ingredient() {
    return ingredient;
}

int IngredientTab::Entry::get_id() {
    return ingredient->get_id();
}

// IngredientTab::DefaultSearch::DefaultSearch(IngredientGateway* gateway) : gateway(gateway) {};

// std::list<std::shared_ptr<Ingredient>> IngredientTab::DefaultSearch::get_great_then(int id, int count){
//     return gateway->get_great_then_by_id(id,count);
// }

// std::list<std::shared_ptr<Ingredient>> IngredientTab::DefaultSearch::get_less_then(int id, int count){
//     return gateway->get_less_then_by_id(id,count);
// }

IngredientTab::NameSearch::NameSearch(std::string name) : name(name){}

std::list<std::shared_ptr<Ingredient>> IngredientTab::NameSearch::get_great_then(int id, int count){
//     return gateway->get_great_then_by_name(name,id,count);
    return Ingredient::get_great_then_by_id_filtred_by_name(name,id,count);
}

std::list<std::shared_ptr<Ingredient>> IngredientTab::NameSearch::get_less_then(int id, int count){
//     return gateway->get_less_then_by_name(name,id,count);
    return Ingredient::get_less_then_by_id_filtred_by_name(name,id,count);

}

IList* IngredientTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Ingredient,Entry>>(true);
    list->get_search_entry()->signal_activate().connect(sigc::bind<EntityList<Ingredient,Entry>*>(&IngredientTab::search,list));

    list->add_column_title("название");
    list->add_column_title("еденицы");

    return list;
}
