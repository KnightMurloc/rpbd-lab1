//
// Created by victor on 11.09.2022.
//

#include "SnackTab.h"

#include <memory>
#include <string>
#include <utility>
#include <iostream>
#include "Form.h"
#include "TabManager.h"
#include "fmt/core.h"
#include "gateways/Ingredients/IngredientGateway.h"
#include "gateways/Snacks/Snacks.h"
#include "gtkmm/container.h"
#include "gtkmm/listbox.h"
#include "gtkmm/object.h"
#include "sigc++/adaptors/bind.h"
#include "sigc++/functors/mem_fun.h"
#include <tuple>
#include <algorithm>

SnackTab::SnackTab(TabManager* tab_manager) : Tab(tab_manager) {

    list = dynamic_cast<EntityList<Snack,Entry>*>(create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../snack_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("name_entry",name_entry);
    builder->get_widget("size_entry",size_entry);
    builder->get_widget("add_button",add_button);
    builder->get_widget("remove_button",remove_button);
    builder->get_widget("ing_list",ing_list);

    setup_menu(builder);
    ing_list->set_vexpand(true);

    add_button->signal_clicked().connect(sigc::bind<Gtk::ListBox*,TabManager*>(
        &SnackTab::add_ingredient,
        ing_list,
        get_tab_manager()
    ));

    remove_button->signal_clicked().connect(sigc::bind<Gtk::ListBox*>(
        &SnackTab::remove_ingredient,
        ing_list
    ));

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&SnackTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&SnackTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&SnackTab::remove_entry));

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&SnackTab::select));

}

void SnackTab::save_current(){
//    auto entry = dynamic_cast<Entry*>(list->get_selected());
//    if(entry == nullptr){
//        return;
//    }
//
//    auto snack = entry->get_snack();

    auto snack = std::dynamic_pointer_cast<Snack>(current);
    if(!snack){
        return;
    }

   if(name_entry->get_text().empty()){
       Gtk::MessageDialog message("???? ?????????????? ??????");
       message.run();
       return;
   }

   if(size_entry->get_text().empty()){
       Gtk::MessageDialog message("???? ???????????? ????????????");
       message.run();
       return;
   }

   std::vector<std::pair<int,int>> old_list;
   std::vector<std::pair<int,int>> new_list;

//    for(auto& ing : gateway.get_ingredients(snack)){
//        std::cout << std::get<0>(ing) << " ";
//        old_list.push_back(ing);
//
//    }

    for(auto& ing : Snack::get_ingredients(snack)){
       std::cout << std::get<0>(ing) << " ";
       old_list.push_back(ing);

   }

   for(auto row : ing_list->get_children()){

       auto box = dynamic_cast<Gtk::Container*>(row)->get_children()[0];

       int* id = static_cast<int*>(box->get_data("id"));
       int* count = static_cast<int*>(box->get_data("count"));
       new_list.push_back(std::make_pair(*id, *count));
   }

   std::sort(old_list.begin(), old_list.end(),[](std::pair<int,int>& a, std::pair<int,int>& b){
       return a.first < b.first;
   });

   std::sort(new_list.begin(), new_list.end(),[](std::pair<int,int>& a, std::pair<int,int>& b){
       return a.first < b.first;
   });

   std::vector<std::pair<int,int>> created;
   std::vector<std::pair<int,int>> removed;

   std::set_difference(new_list.begin(), new_list.end(), old_list.begin(), old_list.end(),
                       std::inserter(created, created.begin()),
       [](std::pair<int,int>& a, std::pair<int,int>& b){
           return a.first < b.first;
       }

   );

   std::set_difference(old_list.begin(), old_list.end(), new_list.begin(), new_list.end(),
                       std::inserter(removed, removed.begin()),
       [](std::pair<int,int>& a,std::pair<int,int>& b){
           return a.first < b.first;
       }
   );

   for(auto a : created){
       snack->get_recipe().add_ingridient(Ingredient::get(a.first),a.second);
   }

   for(auto a : removed){
       snack->get_recipe().remove_ingridient(Ingredient::get(a.first));
   }

   snack->set_name(name_entry->get_text());
   snack->set_size(std::stoi(size_entry->get_text()));

    auto entry = dynamic_cast<Entry*>(list->get_selected());

    if(entry && entry->get_snack()->get_id() == snack->get_id()){
        entry->name_label->set_text(name_entry->get_text());
        entry->size_label->set_text(size_entry->get_text());
    }
//    gateway.save(snack);
   Snack::save(snack);
}


void SnackTab::select(Gtk::ListBoxRow* entry_row) {

    auto entry = dynamic_cast<Entry*>(entry_row);
    if(entry == nullptr){
        return;
    }

    current = entry->get_snack();

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "snack_info"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    IngredientGateway ingredientGateway;

    for(auto child : ing_list->get_children()){
        ing_list->remove(*child);
    }

//     for(auto& ing : gateway.get_ingredients(entry->get_snack())){
//         std::shared_ptr<Ingredient> ingredient = ingredientGateway.get(std::get<0>(ing));
//         auto row = Gtk::make_managed<Gtk::Box>();
//         row->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
//         row->set_homogeneous(true);
//
//         auto name_ing = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
//         auto count_ing = Gtk::make_managed<Gtk::Label>(std::to_string(std::get<1>(ing)));
//         row->add(*name_ing);
//         row->add(*count_ing);
//
//         row->set_data("id", new int(ingredient->get_id()),[](void* data){delete (int*) data;});
//         row->set_data("count", new int(std::get<1>(ing)), [](void* data){delete (int*) data;});
//
//         ing_list->add(*row);
//     }


    for(auto& ing : Snack::get_ingredients(entry->get_snack())){
        std::shared_ptr<Ingredient> ingredient = ingredientGateway.get(std::get<0>(ing));
        auto row = Gtk::make_managed<Gtk::Box>();
        row->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        row->set_homogeneous(true);

        auto name_ing = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
        auto count_ing = Gtk::make_managed<Gtk::Label>(std::to_string(std::get<1>(ing)));
        row->add(*name_ing);
        row->add(*count_ing);

        row->set_data("id", new int(ingredient->get_id()),[](void* data){delete (int*) data;});
        row->set_data("count", new int(std::get<1>(ing)), [](void* data){delete (int*) data;});

        ing_list->add(*row);
    }

    name_entry->set_text(entry->get_snack()->get_name());
    size_entry->set_text(std::to_string(entry->get_snack()->get_size()));
    ing_list->show_all();
}

void SnackTab::add_ingredient(Gtk::ListBox* list, TabManager* tab_manager) {
    int ing_id = tab_manager->select_dialog(TabName::INGREDIENTS);
    if(ing_id == -1){
        return;
    }

    Gtk::Dialog dialog;
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("????????????", Gtk::RESPONSE_CANCEL);

    Gtk::Box box;
    Gtk::Label label("????????????????????");

    Gtk::Entry size_entry_dialog;
    size_entry_dialog.signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,&size_entry_dialog));

    box.add(size_entry_dialog);
    box.show_all();
    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(box);

    if(dialog.run() == Gtk::RESPONSE_OK){

        if(size_entry_dialog.get_text().empty()){
            return;
        }

        IngredientGateway ingredientGateway;
        std::shared_ptr<Ingredient> ingredient = ingredientGateway.get(ing_id);

        auto row = Gtk::make_managed<Gtk::Box>();
        row->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        row->set_homogeneous(true);

        auto name_ing = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
        auto count_ing = Gtk::make_managed<Gtk::Label>(size_entry_dialog.get_text());

        row->set_data("id", new int(ingredient->get_id()),[](void* data){delete (int*) data;});
        row->set_data("count", new int(std::stoi(size_entry_dialog.get_text())), [](void* data){delete (int*) data;});

        row->add(*name_ing);
        row->add(*count_ing);
        list->add(*row);
        list->show_all();
    }
}

void SnackTab::remove_ingredient(Gtk::ListBox* list) {
    auto row = list->get_selected_row();
    if(row == nullptr){
        return;
    }
    list->remove(*row);
}

void SnackTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder){
    Gtk::Entry* size_entry = nullptr;
    builder->get_widget("size_entry",size_entry);
    size_entry->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,size_entry));
}

void SnackTab::create(){
    auto builder = Gtk::Builder::create_from_file("../snack_menu.glade");
    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("????????????", Gtk::RESPONSE_CANCEL);


    Gtk::Button* add_button_dialog;
    Gtk::Button* remove_button_dialog;
    Gtk::ListBox* ing_list_dialog;
    Gtk::Box* box;
    builder->get_widget("box", box);
    builder->get_widget("add_button",add_button_dialog);
    builder->get_widget("remove_button",remove_button_dialog);
    builder->get_widget("ing_list",ing_list_dialog);

    add_button_dialog->signal_clicked().connect(sigc::bind<Gtk::ListBox*,TabManager*>(
        &SnackTab::add_ingredient,
        ing_list_dialog,
        get_tab_manager()
    ));

    remove_button_dialog->signal_clicked().connect(sigc::bind<Gtk::ListBox*>(
        &SnackTab::remove_ingredient,
        ing_list_dialog
    ));

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK) {
            Gtk::ListBox* ing_list_dialog;
            Gtk::Entry* name_entry_dialog;
            Gtk::Entry* size_entry_dialog;
            builder->get_widget("ing_list",ing_list_dialog);
            builder->get_widget("name_entry",name_entry_dialog);
            builder->get_widget("size_entry",size_entry_dialog);


            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("???? ?????????????? ??????");
                message.run();
                return;
            }

            if(size_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("???? ???????????? ????????????");
                message.run();
                return;
            }


            std::vector<std::pair<int,int>> ings;

            for(auto row : ing_list_dialog->get_children()){
                auto box = dynamic_cast<Gtk::Container*>(row)->get_children()[0];
                int* id = static_cast<int*>(box->get_data("id"));
                int* count = static_cast<int*>(box->get_data("count"));

                ings.push_back(std::make_pair(*id, *count));
            }

//             auto snack = gateway.create(name_entry_dialog->get_text(),std::stoi(size_entry_dialog->get_text()),ings);
            auto snack = Snack::create(name_entry_dialog->get_text(),std::stoi(size_entry_dialog->get_text()),ings);

            auto entry = Gtk::make_managed<Entry>(snack);

            list->add_entity(entry);

            dialog->close();
            delete dialog;
        }else{
            dialog->close();
            delete dialog;
        }
    });

    dialog->show_all();
}

void SnackTab::remove_entry() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

//    gateway.remove(entry->get_snack());
    on_remove.emit(entry->get_snack());
    Snack::remove(entry->get_snack());



   Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);
    current = std::shared_ptr<Snack>();
}

SnackTab::Entry::Entry(std::shared_ptr<Snack> snack) : snack(snack) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>();
    size_label = Gtk::make_managed<Gtk::Label>();

//     std::cout << this->snack->get_name() << std::endl;
    name_label->set_text(this->snack->get_name());
    size_label->set_text(std::to_string(this->snack->get_size()));

    box->add(*name_label);
    box->add(*size_label);

    this->add(*box);
    this->show_all();
}

std::shared_ptr<Snack> SnackTab::Entry::get_snack() {
    return snack;
}

int SnackTab::Entry::get_id() {
    return snack->get_id();
}

IList* SnackTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Snack,Entry>>();

    list->add_column_title("????????????????");
    list->add_column_title("????????????");

    return list;
}
