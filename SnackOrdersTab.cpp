//
// Created by victor on 30.09.2022.
//

#include "SnackOrdersTab.h"
#include "TabManager.h"
#include "fmt/core.h"
#include "gateways/Snacks/SnackGateway.h"
#include "gateways/Employeer/EmployeerGateway.h"
#include "gateways/Snacks/Snacks.h"
#include <fmt/format.h>
#include <memory>
#include <string>
#include "Form.h"
#include "gtkmm/entry.h"
#include "gtkmm/object.h"
#include "sigc++/functors/mem_fun.h"

SnackOrdersTab::SnackOrdersTab(TabManager* tab_manager) : Tab(tab_manager) {
    list = dynamic_cast<EntityList<SnackOrder,Entry>*>(this->create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../snack_order_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("snack_link",snack_link);
    builder->get_widget("snack_find",snack_find);
    builder->get_widget("snack_select",snack_select);
    builder->get_widget("waiter_link",waiter_link);
    builder->get_widget("waiter_find",waiter_find);
    builder->get_widget("waiter_select",waiter_select);
    builder->get_widget("table_entry",table_entry);

    setup_menu(builder);

    snack_find->signal_clicked().connect(sigc::mem_fun(this,&SnackOrdersTab::find_snack));
    waiter_find->signal_clicked().connect(sigc::mem_fun(this,&SnackOrdersTab::find_waiter));

    snack_select->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&SnackOrdersTab::select_snack,snack_link,get_tab_manager()));
    waiter_select->signal_clicked().connect(sigc::bind< Gtk::Label*, TabManager*>(&SnackOrdersTab::select_waiter,waiter_link,get_tab_manager()));

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&SnackOrdersTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&SnackOrdersTab::select));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&SnackOrdersTab::create));

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&SnackOrdersTab::remove));

    get_tab_manager()->get_tab(TabName::SNACK)->signal_remove().connect(sigc::mem_fun(this,&SnackOrdersTab::remove_snack_callback));

    get_tab_manager()->get_tab(TabName::EMPLOYEES)->signal_remove().connect(sigc::mem_fun(this,&SnackOrdersTab::remove_waiter_callback));
//     get_tab_manager()->get_tab(TabName::SNACK)->signal_select().connect([](int a){
//        fmt::print("test: {}\n",a);
//     });
}

IList* SnackOrdersTab::create_list() {
    auto list = Gtk::make_managed<EntityList<SnackOrder,Entry>>(&gateway);

    return list;
}

void SnackOrdersTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder) {
    Gtk::Entry* table_entry_menu = nullptr;
    builder->get_widget("table_entry",table_entry_menu);
    table_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,table_entry_menu));
}

void SnackOrdersTab::select(Gtk::ListBoxRow* row) {
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

    snack_link->set_text(order->get_snack()->get_name());
    snack_link->set_data("id", new int(order->get_snack_id()), [](void* data){delete (int*) data;});

    waiter_link->set_text(order->get_waiter()->getLastName());
    waiter_link->set_data("id", new int(order->get_waiter_id()),[](void* data){delete (int*) data;});

    table_entry->set_text(std::to_string(order->get_table()));
}

void SnackOrdersTab::select_snack(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(TabName::SNACK);
    if(id == -1){
        return;
    }

    label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    SnackGateway snack_gateway;
    label->set_text(snack_gateway.get(id)->get_name());
}

void SnackOrdersTab::select_waiter(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(TabName::EMPLOYEES);
    if(id == -1){
        return;
    }

    label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    EmployeerGateway emp_gateway;
    label->set_text(emp_gateway.get(id)->getLastName());
}

void SnackOrdersTab::save_current(){
    auto entry = dynamic_cast<Entry*>(list->get_selected());
    if(entry == nullptr){
        return;
    }

    auto order = entry->get_order();

    int* snack_id = static_cast<int*>(snack_link->get_data("id"));
    if(snack_id == nullptr){
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

    order->set_snack_id(*snack_id);
    order->set_waiter_id(*waiter_id);
    order->set_table(std::stoi(table_entry->get_text()));

    gateway.save(order);

    entry->snack_label->set_text(order->get_snack()->get_name());

    std::unique_ptr<gchar, decltype(&g_free)> first_name_ptr(g_utf8_substring(order->get_waiter()->getFirstName().c_str(),0,1), g_free);

    std::string waiter_name = fmt::format("{} {}.", order->get_waiter()->getLastName(), first_name_ptr.get());

    entry->waiter_label->set_text(waiter_name);
    entry->table_label->set_text(table_entry->get_text());
}


void SnackOrdersTab::create() {
    auto builder = Gtk::Builder::create_from_file("../snack_order_menu.glade");
    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("box", box);

    Gtk::Label* snack_link_dialog;
    Gtk::Button* snack_find_dialog;
    Gtk::Button* snack_select_dialog;

    Gtk::Label* waiter_link_dialog;
    Gtk::Button* waiter_find_dialog;
    Gtk::Button* waiter_select_dialog;

    builder->get_widget("snack_link",snack_link_dialog);
    builder->get_widget("snack_find",snack_find_dialog);
    builder->get_widget("snack_select",snack_select_dialog);

    builder->get_widget("waiter_link",waiter_link_dialog);
    builder->get_widget("waiter_find",waiter_find_dialog);
    builder->get_widget("waiter_select",waiter_select_dialog);

    snack_find_dialog->set_sensitive(false);
    waiter_find_dialog->set_sensitive(false);

    snack_select_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&SnackOrdersTab::select_snack,snack_link_dialog,get_tab_manager()));
    waiter_select_dialog->signal_clicked().connect(sigc::bind< Gtk::Label*, TabManager*>(&SnackOrdersTab::select_waiter,waiter_link_dialog,get_tab_manager()));

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Label* snack_link_dialog;
            Gtk::Button* snack_find_dialog;
            Gtk::Button* snack_select_dialog;

            Gtk::Label* waiter_link_dialog;
            Gtk::Button* waiter_find_dialog;
            Gtk::Button* waiter_select_dialog;

            Gtk::Entry* table_entry_dialog;

            builder->get_widget("snack_link",snack_link_dialog);
            builder->get_widget("snack_find",snack_find_dialog);
            builder->get_widget("snack_select",snack_select_dialog);
            builder->get_widget("waiter_link",waiter_link_dialog);
            builder->get_widget("waiter_find",waiter_find_dialog);
            builder->get_widget("waiter_select",waiter_select_dialog);
            builder->get_widget("table_entry",table_entry_dialog);


            int* snack_id = static_cast<int*>(snack_link_dialog->get_data("id"));
            if(snack_id == nullptr){
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

            auto order = gateway.create(*snack_id, *waiter_id, std::stoi(table_entry_dialog->get_text()));

            auto entry = Gtk::make_managed<Entry>(order);
            list->add_entity(entry);
            list->show_all();
        }
        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void SnackOrdersTab::remove_snack_callback(std::shared_ptr<IEntity> entity){
    auto snack = std::dynamic_pointer_cast<Snack>(entity);
    if(snack == nullptr){
        return;
    }

    for(auto child : list->get_list_box()->get_children()){
        auto entry = dynamic_cast<Entry*>(child);
        if(entry->get_order()->get_snack_id() == snack->get_id()){
            list->remove_entity(entry);
        }
    }
}

void SnackOrdersTab::remove_waiter_callback(std::shared_ptr<IEntity> entity){
    auto waiter = std::dynamic_pointer_cast<Employeer>(entity);
    if(waiter == nullptr){
        return;
    }

    for(auto child : list->get_list_box()->get_children()){
        auto entry = dynamic_cast<Entry*>(child);
        if(entry->get_order()->get_waiter_id() == waiter->get_id()){
            list->remove_entity(entry);
        }
    }
}

void SnackOrdersTab::remove() {
    auto entry = dynamic_cast<Entry*>(list->get_selected());
    if(entry == nullptr){
        return;
    }

    gateway.remove(entry->get_order());

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    box->remove(*box->get_children()[0]);

    list->remove_entity(entry);
}

void SnackOrdersTab::find_snack(){
    int* id = static_cast<int*>(snack_link->get_data("id"));
    if(id == nullptr){
        return;
    }

    get_tab_manager()->select_on_tab(TabName::SNACK,*id);
}

void SnackOrdersTab::find_waiter(){
    int* id = static_cast<int*>(waiter_link->get_data("id"));
    if(id == nullptr){
        return;
    }

    get_tab_manager()->select_on_tab(TabName::EMPLOYEES,*id);
}

SnackOrdersTab::Entry::Entry(std::shared_ptr<SnackOrder> order) : order(order) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    snack_label = Gtk::make_managed<Gtk::Label>(order->get_snack()->get_name());
    auto waiter = order->get_waiter();
//     gchar* first_name_ptr = g_utf8_substring(waiter->getFirstName().c_str(),0,1);
    std::unique_ptr<gchar, decltype(&g_free)> first_name_ptr(g_utf8_substring(waiter->getFirstName().c_str(),0,1), g_free);

    std::string waiter_name = fmt::format("{} {}.", waiter->getLastName(), first_name_ptr.get());
//     g_free(first_name_ptr);

    waiter_label = Gtk::make_managed<Gtk::Label>(waiter_name);


    table_label = Gtk::make_managed<Gtk::Label>(std::to_string(order->get_table()));

    box->add(*snack_label);
    box->add(*waiter_label);
    box->add(*table_label);

    this->add(*box);
}

std::shared_ptr<SnackOrder> SnackOrdersTab::Entry::get_order() {
    return order;
}

int SnackOrdersTab::Entry::get_id() {
    return order->get_id();
}
