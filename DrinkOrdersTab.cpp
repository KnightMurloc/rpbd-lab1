//
// Created by victor on 02.10.2022.
//

#include "DrinkOrdersTab.h"
#include "TabManager.h"
#include "gateways/Drinks/Drinkgateway.h"
#include "gateways/Employeer/Employeer.h"
#include "gateways/Employeer/EmployeerGateway.h"
#include "gateways/Snacks/Snacks.h"
#include <fmt/format.h>
#include <memory>
#include <string>
#include "Form.h"

DrinkOrdersTab::DrinkOrdersTab(TabManager* tab_manager) : Tab(tab_manager) {
    list = dynamic_cast<EntityList<DrinkOrder,Entry>*>(this->create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../drink_order_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("drink_link", drink_link);
    builder->get_widget("drink_find", drink_find);
    builder->get_widget("drink_select", drink_select);
    builder->get_widget("waiter_link",waiter_link);
    builder->get_widget("waiter_find",waiter_find);
    builder->get_widget("waiter_select",waiter_select);
    builder->get_widget("table_entry",table_entry);

    setup_menu(builder);

    drink_find->signal_clicked().connect(sigc::mem_fun(this, &DrinkOrdersTab::find_drink));
    waiter_find->signal_clicked().connect(sigc::mem_fun(this,&DrinkOrdersTab::find_waiter));

    drink_select->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&DrinkOrdersTab::select_drink, drink_link, get_tab_manager()));
    waiter_select->signal_clicked().connect(sigc::bind< Gtk::Label*, TabManager*>(&DrinkOrdersTab::select_waiter,waiter_link,get_tab_manager()));

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&DrinkOrdersTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&DrinkOrdersTab::select));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&DrinkOrdersTab::create));

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&DrinkOrdersTab::remove));

    get_tab_manager()->get_tab(TabName::DRINKS)->signal_remove().connect(sigc::mem_fun(this,&DrinkOrdersTab::remove_drink_callback));

    get_tab_manager()->get_tab(TabName::EMPLOYEES)->signal_remove().connect(sigc::mem_fun(this,&DrinkOrdersTab::remove_waiter_callback));
}

IList* DrinkOrdersTab::create_list() {
    auto list = Gtk::make_managed<EntityList<DrinkOrder,Entry>>();
    list->add_column_title("напиток");
    list->add_column_title("офицант");
    list->add_column_title("столик");
    return list;
}

void DrinkOrdersTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder) {
    Gtk::Entry* table_entry_menu = nullptr;
    builder->get_widget("table_entry",table_entry_menu);
    table_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,table_entry_menu));
}

void DrinkOrdersTab::select(Gtk::ListBoxRow* row) {
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "snack_order_info"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    auto order = entry->get_order();

    drink_link->set_text(order->get_drink()->getName());
    drink_link->set_data("id", new int(order->get_drink()->get_id()), [](void* data){delete (int*) data;});

    waiter_link->set_text(order->get_waiter()->getLastName());
    waiter_link->set_data("id", new int(order->get_waiter()->get_id()),[](void* data){delete (int*) data;});

    table_entry->set_text(std::to_string(order->get_table()));
}

void DrinkOrdersTab::select_drink(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(TabName::DRINKS);
    if(id == -1){
        return;
    }

    label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    Drinkgateway drink_gateway;
    label->set_text(drink_gateway.get(id)->getName());
}

void DrinkOrdersTab::select_waiter(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(TabName::EMPLOYEES);
    if(id == -1){
        return;
    }

    label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    EmployeerGateway emp_gateway;
    label->set_text(emp_gateway.get(id)->getLastName());
}

void DrinkOrdersTab::save_current(){
    auto entry = dynamic_cast<Entry*>(list->get_selected());
    if(entry == nullptr){
        return;
    }

    auto order = entry->get_order();

    int* drink_id = static_cast<int*>(drink_link->get_data("id"));
    if(drink_id == nullptr){
       Gtk::MessageDialog message("продукт не указан");
       message.run();
       return;
    }

    int* waiter_id = static_cast<int*>(waiter_link->get_data("id"));
    if(waiter_id == nullptr){
       Gtk::MessageDialog message("офицант не указан");
       message.run();
       return;
    }

    if(table_entry->get_text().empty()){
       Gtk::MessageDialog message("не указан номер столика");
       message.run();
       return;
    }

//     order->set_drink_id(*snack_id);
//     order->set_waiter_id(*waiter_id);
    order->set_drink(Drink::get(*drink_id));
    order->set_waiter(Employeer::get(*waiter_id));
    order->set_table(std::stoi(table_entry->get_text()));

//     gateway.save(order);
    DrinkOrder::save(order);

    entry->drink_label->set_text(order->get_drink()->getName());

    std::unique_ptr<gchar, decltype(&g_free)> first_name_ptr(g_utf8_substring(order->get_waiter()->getFirstName().c_str(),0,1), g_free);

    std::string waiter_name = fmt::format("{} {}.", order->get_waiter()->getLastName(), first_name_ptr.get());

    entry->waiter_label->set_text(waiter_name);
    entry->table_label->set_text(table_entry->get_text());
}


void DrinkOrdersTab::create() {
    auto builder = Gtk::Builder::create_from_file("../drink_order_menu.glade");
    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("box", box);

    Gtk::Label* drink_link_dialog;
    Gtk::Button* drink_find_dialog;
    Gtk::Button* drink_select_dialog;

    Gtk::Label* waiter_link_dialog;
    Gtk::Button* waiter_find_dialog;
    Gtk::Button* waiter_select_dialog;

    builder->get_widget("drink_link", drink_link_dialog);
    builder->get_widget("drink_find", drink_find_dialog);
    builder->get_widget("drink_select", drink_select_dialog);

    builder->get_widget("waiter_link",waiter_link_dialog);
    builder->get_widget("waiter_find",waiter_find_dialog);
    builder->get_widget("waiter_select",waiter_select_dialog);

    drink_find_dialog->set_sensitive(false);
    waiter_find_dialog->set_sensitive(false);

    drink_select_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&DrinkOrdersTab::select_drink, drink_link_dialog, get_tab_manager()));
    waiter_select_dialog->signal_clicked().connect(sigc::bind< Gtk::Label*, TabManager*>(&DrinkOrdersTab::select_waiter,waiter_link_dialog,get_tab_manager()));

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Label* drink_link_dialog;
            Gtk::Label* waiter_link_dialog;

            Gtk::Entry* table_entry_dialog;

            builder->get_widget("drink_link", drink_link_dialog);
            builder->get_widget("waiter_link",waiter_link_dialog);
            builder->get_widget("table_entry",table_entry_dialog);


            int* drink_id = static_cast<int*>(drink_link_dialog->get_data("id"));
            if(drink_id == nullptr){
                Gtk::MessageDialog message("продукт не указан");
                message.run();
                return;
            }

            int* waiter_id = static_cast<int*>(waiter_link_dialog->get_data("id"));
            if(waiter_id == nullptr){
                Gtk::MessageDialog message("офицант не указан");
                message.run();
                return;
            }

            if(table_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("номер столика не указан");
                message.run();
                return;
            }

//             auto order = gateway.create(*drink_id, *waiter_id, std::stoi(table_entry_dialog->get_text()));
            auto order = DrinkOrder::create(Drink::get(*drink_id),Employeer::get(*waiter_id),std::stoi(table_entry_dialog->get_text()));

            auto entry = Gtk::make_managed<Entry>(order);
            list->add_entity(entry);
            list->show_all();
        }
        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void DrinkOrdersTab::remove_drink_callback(std::shared_ptr<IEntity> entity){
    for(auto child : list->get_list_box()->get_children()){
        auto entry = dynamic_cast<Entry*>(child);
        if(entry->get_order()->get_drink()->get_id() == entity->get_id()){
            list->remove_entity(entry);
        }
    }
}

void DrinkOrdersTab::remove_waiter_callback(std::shared_ptr<IEntity> entity){
    auto waiter = std::dynamic_pointer_cast<Employeer>(entity);
    if(waiter == nullptr){
        return;
    }

    for(auto child : list->get_list_box()->get_children()){
        auto entry = dynamic_cast<Entry*>(child);
        if(entry->get_order()->get_waiter()->get_id() == waiter->get_id()){
            list->remove_entity(entry);
        }
    }
}

void DrinkOrdersTab::remove() {
    auto entry = dynamic_cast<Entry*>(list->get_selected());
    if(entry == nullptr){
        return;
    }

    on_remove.emit(entry->get_order());
//     gateway.remove(entry->get_order());
    DrinkOrder::remove(entry->get_order());

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    box->remove(*box->get_children()[0]);

    list->remove_entity(entry);
}

void DrinkOrdersTab::find_drink(){
    int* id = static_cast<int*>(drink_link->get_data("id"));
    if(id == nullptr){
        return;
    }

    get_tab_manager()->select_on_tab(TabName::DRINKS,*id);
}

void DrinkOrdersTab::find_waiter(){
    int* id = static_cast<int*>(waiter_link->get_data("id"));
    if(id == nullptr){
        return;
    }

    get_tab_manager()->select_on_tab(TabName::EMPLOYEES,*id);
}

DrinkOrdersTab::Entry::Entry(std::shared_ptr<DrinkOrder> order) : order(order) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    drink_label = Gtk::make_managed<Gtk::Label>(order->get_drink()->getName());
    auto waiter = order->get_waiter();
    std::unique_ptr<gchar, decltype(&g_free)> first_name_ptr(g_utf8_substring(waiter->getFirstName().c_str(),0,1), g_free);

    std::string waiter_name = fmt::format("{} {}.", waiter->getLastName(), first_name_ptr.get());

    waiter_label = Gtk::make_managed<Gtk::Label>(waiter_name);


    table_label = Gtk::make_managed<Gtk::Label>(std::to_string(order->get_table()));

    box->add(*drink_label);
    box->add(*waiter_label);
    box->add(*table_label);

    this->add(*box);
}

std::shared_ptr<DrinkOrder> DrinkOrdersTab::Entry::get_order() {
    return order;
}

int DrinkOrdersTab::Entry::get_id() {
    return order->get_id();
}
