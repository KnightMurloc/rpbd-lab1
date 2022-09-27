//
// Created by victor on 12.09.2022.
//

#include "IngredientTab.h"
#include "Form.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "gateways/Ingredients/Ingredient.h"
#include "gateways/units.h"
#include "gtkmm/combobox.h"
#include "gtkmm/dialog.h"
#include "sigc++/functors/mem_fun.h"

IngredientTab::IngredientTab(TabManager* tab_manager) : Tab(tab_manager) {

    current_search = std::make_unique<DefaultSearch>(&gateway);

    builder = Gtk::Builder::create_from_file("../ingridient_menu.glade");

    builder->get_widget("box", info_box);
    builder->get_widget("name_entry", name_entry);
    builder->get_widget("unit_combo", unit_combo);

    auto save_button = Gtk::make_managed<Gtk::Button>("сохранить");
    save_button->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::save));

    info_box->add(*save_button);
    info_box->show_all();

//     first_id = gateway.get_min();
//     last_id = gateway.get_max();

    this->fill_list(getListBox());
//     scroll->signal_edge_reached().connect(sigc::mem_fun(this,&IngredientTab::scroll_event));

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::remove_entry));
    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::create));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&IngredientTab::select));

    add_clumn_lable("название");
    add_clumn_lable("еденицы");

    search_entry->signal_activate().connect(sigc::mem_fun(this,&IngredientTab::search));
    stop_search->signal_clicked().connect(sigc::mem_fun(this,&IngredientTab::search_stop));
//     fmt::print("{} : {}\n",first_id,last_id);
}

// void IngredientTab::select_by_id(int entry_id) {
//
// }

void IngredientTab::search(){
    current_search = std::make_unique<NameSearch>(&gateway, search_entry->get_text());
    fill_list(getListBox());
    getListBox()->show_all();
}

void IngredientTab::search_stop(){
    current_search = std::make_unique<DefaultSearch>(&gateway);
    fill_list(getListBox());
    getListBox()->show_all();
}

void IngredientTab::fill_list(Gtk::ListBox* list) {

    for(auto child : getListBox()->get_children()){
        getListBox()->remove(*child);
    }

    first_id = 0;
    last_id = -1;
    for(const auto& ing : current_search->get_great_then(0,20)){
        if(ing->get_id() > last_id){
            last_id = ing->get_id();
        }
        auto entry = Gtk::make_managed<Entry>(ing);
        list->add(*entry);
    }
    fmt::print("{}\n", last_id);
}

// void IngredientTab::scroll_event(Gtk::PositionType type){
//     if(type == Gtk::PositionType::POS_BOTTOM){
//         scroll_down();
// //         first_id = last_id;
// //         auto data = current_search->get_great_then(last_id,20);
// //         if(data.empty()){
// // //             goto end;
// // //             return false;
// //         }
// //         for(const auto& ing : data){
// //             if(ing->get_id() > last_id){
// //                 last_id = ing->get_id();
// //             }
// //             auto entry = Gtk::make_managed<Entry>(ing);
// //             getListBox()->add(*entry);
// //         }
// //
// //         auto rows = getListBox()->get_children();
// //         if(rows.size() > 40){
// //
// //             for(int i = 0; i < rows.size() - 40; i++){
// //                 fmt::print("removed\n");
// //                 getListBox()->remove(*rows[i]);
// //             }
// //         }
// //         getListBox()->show_all();
// //         scroll->get_vadjustment()->set_value(500);
//
// //     fmt::print("{} - {}\n",scroll->get_vadjustment()->get_lower(),scroll->get_vadjustment()->get_upper());
//
//     }else if(type == Gtk::PositionType::POS_TOP){
//         scroll_up();
// //         last_id = first_id;
// //         auto data = current_search->get_less_then(first_id,20);
// //         if(data.empty()){
// // //             goto end;
// // //             return false;
// // //             return;
// //         }
// //         for(const auto& ing : data){
// //             if(ing->get_id() < first_id){
// //                 first_id = ing->get_id();
// //             }
// //
// //             auto entry = Gtk::make_managed<Entry>(ing);
// //             fmt::print("name: {}\n", entry->get_ingredient()->get_name());
// // //             getListBox()->add(*entry);
// //             getListBox()->insert(*entry,0);
// //         }
// //
// //         auto rows = getListBox()->get_children();
// //         if(rows.size() > 40){
// //             for(int i = rows.size() - 1; i >= 40; i--){
// //                 fmt::print("removed\n");
// //                 getListBox()->remove(*rows[i]);
// //             }
// //         }
// //         getListBox()->show_all();
// //
// //         scroll->get_vadjustment()->set_value(100);
//
// //         fmt::print("{} - {}\n",scroll->get_vadjustment()->get_lower(),scroll->get_vadjustment()->get_upper());
//
// //         scroll->get_vadjustment()->set
//     }
//     //TODO убрать
// // end:
// //     auto rows = getListBox()->get_children();
// //     fmt::print("rows: {}\n", rows.size());
// //     return true;
// }

bool IngredientTab::scroll_up(){
        //test
        last_id = first_id;
        auto data = current_search->get_less_then(first_id,20);
        if(data.empty()){
            return false;
        }
        for(const auto& ing : data){
            if(ing->get_id() < first_id){
                first_id = ing->get_id();
            }

            auto entry = Gtk::make_managed<Entry>(ing);
            fmt::print("name: {}\n", entry->get_ingredient()->get_name());
            getListBox()->insert(*entry,0);
        }

        auto rows = getListBox()->get_children();
        if(rows.size() > 40){
            for(int i = rows.size() - 1; i >= 40; i--){
                fmt::print("removed\n");
                getListBox()->remove(*rows[i]);
            }
        }
        getListBox()->show_all();

        scroll->get_vadjustment()->set_value(100);
        return true;
}

bool IngredientTab::scroll_down(){
        first_id = last_id;
        auto data = current_search->get_great_then(last_id,20);
        if(data.empty()){
//             goto end;
            return false;
        }
        for(const auto& ing : data){
            if(ing->get_id() > last_id){
                last_id = ing->get_id();
            }
            auto entry = Gtk::make_managed<Entry>(ing);
            getListBox()->add(*entry);
        }

        auto rows = getListBox()->get_children();
        if(rows.size() > 40){

            for(int i = 0; i < rows.size() - 40; i++){
                fmt::print("removed\n");
                getListBox()->remove(*rows[i]);
            }
        }
        getListBox()->show_all();
        scroll->get_vadjustment()->set_value(500);

        return true;
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

    name_entry->set_text(entry->get_ingredient()->get_name());
    unit_combo->set_active_id(unit_to_string(entry->get_ingredient()->get_unit()));
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

    auto ing = entry->get_ingredient();

    ing->set_name(name_entry->get_text());
    ing->set_unit(string_to_unit(unit_combo->get_active_id()));

    gateway.save(ing);

    entry->name_label->set_text(ing->get_name());
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

            auto ing = gateway.create(name_entry_dialog->get_text(), string_to_unit(unit_combo_dialog->get_active_id()));

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

IngredientTab::DefaultSearch::DefaultSearch(IngredientGateway* gateway) : gateway(gateway) {};

std::list<std::shared_ptr<Ingredient>> IngredientTab::DefaultSearch::get_great_then(int id, int count){
    return gateway->get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Ingredient>> IngredientTab::DefaultSearch::get_less_then(int id, int count){
    return gateway->get_less_then_by_id(id,count);
}

IngredientTab::NameSearch::NameSearch(IngredientGateway* gateway, std::string name) : gateway(gateway), name(name){}

std::list<std::shared_ptr<Ingredient>> IngredientTab::NameSearch::get_great_then(int id, int count){
    return gateway->get_great_then_by_name(name,id,count);
}

std::list<std::shared_ptr<Ingredient>> IngredientTab::NameSearch::get_less_then(int id, int count){
    return gateway->get_less_then_by_name(name,id,count);
}
