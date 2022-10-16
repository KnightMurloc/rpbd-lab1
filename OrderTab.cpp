//
// Created by victor on 08.09.2022.
//

#include "OrderTab.h"
#include <iostream>
#include "EntityList.h"
#include "Form.h"
#include "fmt/core.h"
#include "gateways/Employeer/Employeer.h"
#include "gateways/Employeer/EmployeerGateway.h"
#include "gateways/Orders/Order.h"
#include "gateways/entity.h"
#include <fmt/format.h>

OrderTab::OrderTab(TabManager* tab_manager) : Tab(tab_manager) {

    list = dynamic_cast<EntityList<Order,Entry>*>(create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../order_menu.glade");

    setup_menu(builder);

    builder->get_widget("box", info_box);
    builder->get_widget("order_reason_entry_dialog", reason_entry);
    builder->get_widget("order_number_entry_dialog", order_number_entry);
    builder->get_widget("order_post_combobox_dialog", post_combobox);
    builder->get_widget("order_day_entry_dialog", day_entry);
    builder->get_widget("order_month_entry_dialog", month_entry);
    builder->get_widget("order_year_entry_dialog", year_entry);
    builder->get_widget("empl_link", empl_link);
    builder->get_widget("find_button_empl", find_button);
    builder->get_widget("select_button_empl", select_button);

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&OrderTab::select));
    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::remove));

    find_button->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::find_empl));

    select_button->signal_clicked().connect(
        sigc::bind<Gtk::Label*,TabManager*>(
            &OrderTab::select_empl,
            empl_link,
            get_tab_manager()
        )
    );

//     select_button->signal_clicked()
}

OrderTab::Entry::Entry(std::shared_ptr<Order> order) : order(order) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    reason_label = Gtk::make_managed<Gtk::Label>(order->get_reason());
    order_number_label = Gtk::make_managed<Gtk::Label>(std::to_string(order->get_order_number()));
    order_date_label = Gtk::make_managed<Gtk::Label>(order->get_order_date());
    post_label = Gtk::make_managed<Gtk::Label>(order->get_post_as_string());

    box->add(*reason_label);
    box->add(*order_number_label);
    box->add(*order_date_label);
    box->add(*post_label);

    this->add(*box);
}

std::shared_ptr<Order> OrderTab::Entry::get_order() {
    return order;
}

int OrderTab::Entry::get_id() {
    return order->get_id();
}

void OrderTab::select(Gtk::ListBoxRow *entry_row) {
    auto entry = dynamic_cast<Entry*>(entry_row);

    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "order_info"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }
    reason_entry->set_text(entry->get_order()->get_reason());
    order_number_entry->set_text(std::to_string(entry->get_order()->get_order_number()));
    post_combobox->set_active_id(entry->get_order()->get_post_as_string());

    tm date{};
    strptime(entry->get_order()->get_order_date().c_str(), "%Y-%m-%d", &date);
    day_entry->set_text(std::to_string(date.tm_mday));
    month_entry->set_text(std::to_string(date.tm_mon + 1));
    year_entry->set_text(std::to_string(date.tm_year + 1900));

    if(entry->get_order()->get_employer()){
        auto empl = entry->get_order()->get_employer();
        std::unique_ptr<gchar, decltype(&g_free)> first_name_ptr(g_utf8_substring(empl->getFirstName().c_str(),0,1), g_free);

        std::string empl_name = fmt::format("{} {}.", empl->getLastName(), first_name_ptr.get());

        empl_link->set_text(empl_name);
        find_button->set_sensitive(true);
    }else{
        find_button->set_sensitive(false);
        empl_link->set_text("none");
    }
}

void OrderTab::save_current() {

   auto entry = dynamic_cast<Entry*>(list->get_selected());

   if(entry == nullptr){
       return;
   }

   if (reason_entry->get_text().empty()) {
       Gtk::MessageDialog message("не указана причина");
       message.run();
       return;
   }

   if (order_number_entry->get_text().empty()) {
       Gtk::MessageDialog message("не указан номер приказа");
       message.run();
       return;
   }
   int day;
   int month;
   int year;
   try {
       day = std::stoi(day_entry->get_text());
       month = std::stoi(month_entry->get_text());
       year = std::stoi(year_entry->get_text());
   } catch (std::exception&) {
       Gtk::MessageDialog message("некоректная дата");
       message.run();
       return;
   }
   if (!check_date(day, month, year)) {
       Gtk::MessageDialog message("некоректная дата");
       message.run();
       return;
   }

   auto order = entry->get_order();

   order->set_reason(reason_entry->get_text());
   order->set_order_number(std::stoi(order_number_entry->get_text()));
   order->set_post(string_to_post(post_combobox->get_active_id()));
   order->set_order_date(fmt::format("{}-{}-{}",year,month, day));

   if(empl_link->get_data("id")){
        auto ptr = static_cast<int*>(empl_link->get_data("id"));
        auto empl = Employeer::get(*ptr);
        order->set_employer(empl);
    }

//    gateway.save(order);
   Order::save(order);

   entry->reason_label->set_text(order->get_reason());
   entry->order_number_label->set_text(std::to_string(order->get_order_number()));
   entry->order_date_label->set_text(order->get_order_date());
   entry->post_label->set_text(order->get_post_as_string());
}

void OrderTab::create(){

    auto builder = Gtk::Builder::create_from_file("../order_menu.glade");

    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("box",box);

    Gtk::Label* empl_link_dialog;
    Gtk::Button* find_button_dialog;
    Gtk::Button* select_button_dialog;

    builder->get_widget("empl_link", empl_link_dialog);
    builder->get_widget("find_button_empl", find_button_dialog);
    builder->get_widget("select_button_empl", select_button_dialog);

    find_button_dialog->set_sensitive(false);
    select_button_dialog->signal_clicked().connect(
        sigc::bind<Gtk::Label*,TabManager*>(
            &OrderTab::select_empl,
            empl_link_dialog,
            get_tab_manager()
        )
    );

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
            if(response == Gtk::RESPONSE_OK) {
                Gtk::Entry *reason_entry_dialog;
                Gtk::Entry *order_number_entry_dialog;
                Gtk::ComboBoxText *order_post_combobox;
                Gtk::Entry *order_day_entry;
                Gtk::Entry *order_month_entry;
                Gtk::Entry *order_year_entry;
                Gtk::Label* empl_link_dialog;

                builder->get_widget("order_reason_entry_dialog", reason_entry_dialog);
                builder->get_widget("order_number_entry_dialog", order_number_entry_dialog);
                builder->get_widget("order_post_combobox_dialog", order_post_combobox);
                builder->get_widget("order_day_entry_dialog", order_day_entry);
                builder->get_widget("order_month_entry_dialog", order_month_entry);
                builder->get_widget("order_year_entry_dialog", order_year_entry);

                builder->get_widget("empl_link", empl_link_dialog);


                if (reason_entry_dialog->get_text().empty()) {
                    Gtk::MessageDialog message("не указана причина");
                    message.run();
                    return;
                }

                if (order_number_entry_dialog->get_text().empty()) {
                    Gtk::MessageDialog message("не указан номер приказа");
                    message.run();
                    return;
                }

                int day;
                int month;
                int year;
                try {
                    day = std::stoi(order_day_entry->get_text());
                    month = std::stoi(order_month_entry->get_text());
                    year = std::stoi(order_year_entry->get_text());
                } catch (std::exception&) {
                    Gtk::MessageDialog message("некоректная дата");
                    message.run();
                    return;
                }
                fmt::print("{} {} {}\n",
                           (std::string) order_day_entry->get_text(),
                     (std::string) order_month_entry->get_text(),
                           (std::string) order_year_entry->get_text());
                fmt::print("{} {} {}\n", day,month, year);
                if (!check_date(day, month, year)) {
                    Gtk::MessageDialog message("некоректная дата");
                    message.run();
                    return;
                }

                if(empl_link_dialog->get_data("id") == nullptr){
                    Gtk::MessageDialog message("не указан сотрудник");
                    message.run();
                    return;
                }

                std::shared_ptr<Employeer> empl;

                if(empl_link_dialog->get_data("id")){
                    auto ptr = static_cast<int*>(empl_link_dialog->get_data("id"));
                    empl = Employeer::get(*ptr);
                }

//                 auto order = gateway.create(
//                         reason_entry_dialog->get_text(),
//                         std::stoi(order_number_entry_dialog->get_text()),
//                         fmt::format("{}-{}-{}",year, month, day),
//                         order_post_combobox->get_active_id()
//                         );

                auto order = Order::create(
                        reason_entry_dialog->get_text(),
                        std::stoi(order_number_entry_dialog->get_text()),
                        fmt::format("{}-{}-{}",year, month, day),
                        order_post_combobox->get_active_id(),
                                           empl);

                auto entry = Gtk::make_managed<Entry>(order);
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

void OrderTab::setup_menu(const Glib::RefPtr<Gtk::Builder>& builder) {
    Gtk::Entry* order_number_entry_dialog;
    builder->get_widget("order_number_entry_dialog", order_number_entry_dialog);
    order_number_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_number_entry_dialog));

    Gtk::Entry* order_day_entry_dialog;
    builder->get_widget("order_day_entry_dialog", order_day_entry_dialog);
    order_day_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_day_entry_dialog));
    Gtk::Entry* order_month_entry_dialog;
    builder->get_widget("order_month_entry_dialog", order_month_entry_dialog);
    order_month_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_month_entry_dialog));
    Gtk::Entry* order_year_entry_dialog;
    builder->get_widget("order_year_entry_dialog", order_year_entry_dialog);
    order_year_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_year_entry_dialog));
}

void OrderTab::remove() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   on_remove.emit(entry->get_order());

//    gateway.remove(entry->get_order());
    Order::remove(entry->get_order());

   Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);
}

IList* OrderTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Order,Entry>>();

    list->add_column_title("причина");
    list->add_column_title("номер приказа");
    list->add_column_title("дата приказа");
    list->add_column_title("должность");

    return list;
}

void OrderTab::find_empl() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   get_tab_manager()->select_on_tab(TabName::EMPLOYEES,entry->get_order()->get_employer()->get_id());

//    get_tab_manager()->select_on_tab(TabName::ORDER, entry->get_emp()->get_movement()->get_id());
}

void OrderTab::select_empl(Gtk::Label* label, TabManager* manager) {
    int id = manager->select_dialog(TabName::EMPLOYEES);
    if(id == -1){
        return;
    }
    try {
        label->set_text(Employeer::get(id)->getLastName());
        label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    }catch(std::exception&){}
}
