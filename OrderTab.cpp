//
// Created by victor on 08.09.2022.
//

#include "OrderTab.h"
#include <iostream>
#include "Form.h"
#include "fmt/core.h"
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

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&OrderTab::select));
    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&OrderTab::remove));

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

   gateway.save(order);

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

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
            if(response == Gtk::RESPONSE_OK) {
                Gtk::Entry *reason_entry_dialog;
                Gtk::Entry *order_number_entry_dialog;
                Gtk::ComboBoxText *order_post_combobox;
                Gtk::Entry *order_day_entry;
                Gtk::Entry *order_month_entry;
                Gtk::Entry *order_year_entry;

                builder->get_widget("order_reason_entry_dialog", reason_entry_dialog);
                builder->get_widget("order_number_entry_dialog", order_number_entry_dialog);
                builder->get_widget("order_post_combobox_dialog", order_post_combobox);
                builder->get_widget("order_day_entry_dialog", order_day_entry);
                builder->get_widget("order_month_entry_dialog", order_month_entry);
                builder->get_widget("order_year_entry_dialog", order_year_entry);

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

                auto order = gateway.create(
                        reason_entry_dialog->get_text(),
                        std::stoi(order_number_entry_dialog->get_text()),
                        fmt::format("{}-{}-{}",year, month, day),
                        order_post_combobox->get_active_id()
                        );

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
    order_day_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_number_entry_dialog));
    Gtk::Entry* order_month_entry_dialog;
    builder->get_widget("order_month_entry_dialog", order_month_entry_dialog);
    order_month_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_number_entry_dialog));
    Gtk::Entry* order_year_entry_dialog;
    builder->get_widget("order_year_entry_dialog", order_year_entry_dialog);
    order_year_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,order_number_entry_dialog));
}

void OrderTab::remove() {
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

IList* OrderTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Order,Entry>>(&gateway);

    list->add_column_title("причина");
    list->add_column_title("номер приказа");
    list->add_column_title("дата приказа");
    list->add_column_title("должность");

    return list;
}
