//
// Created by victor on 05.09.2022.
//

#include <string>
#include "Form.h"
#include <iostream>
#include <regex>
const static std::string ui_file = "../main.glade";

Form::Form() {
    builder = Gtk::Builder::create_from_file(Glib::canonicalize_filename(ui_file,std::getenv("APPDIR")));
    setup_entry();
}


Form &Form::getInstance() {
    static Form form;
    return form;
}

const Glib::RefPtr<Gtk::Builder> &Form::getBuilder() const {
    return builder;
}

void Form::number_only(const Glib::ustring& text,int* position, Gtk::Entry* self){
    for(auto c : text){
        if(!std::isdigit(c)){
            std::string str = self->get_text();
            if(str.empty()){
                return;
            }
            str.pop_back();
            self->set_text(str);
        }
    }
}

void Form::float_only(const Glib::ustring &text, int* position, Gtk::Entry* self) {
    static std::regex expr(R"(^\d+((\.|,)\d{0,2})?$)");
    std::string str = self->get_text();
    if(!std::regex_match(str,expr)){
        self->set_text(str.substr(0,str.length() - text.length()));
    }
}


void Form::setup_entry() {
    Gtk::Dialog* order_create_dialog;
    builder->get_widget("order_create_dialog", order_create_dialog);
    order_create_dialog->add_button("OK", Gtk::RESPONSE_OK);
    order_create_dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Entry* order_number_entry_dialog;
    builder->get_widget("order_number_entry_dialog", order_number_entry_dialog);
    order_number_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&number_only,order_number_entry_dialog));

    Gtk::Entry* order_day_entry_dialog;
    builder->get_widget("order_day_entry_dialog", order_day_entry_dialog);
    order_day_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&number_only,order_number_entry_dialog));
    Gtk::Entry* order_month_entry_dialog;
    builder->get_widget("order_month_entry_dialog", order_month_entry_dialog);
    order_month_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&number_only,order_number_entry_dialog));
    Gtk::Entry* order_year_entry_dialog;
    builder->get_widget("order_year_entry_dialog", order_year_entry_dialog);
    order_year_entry_dialog->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&number_only,order_number_entry_dialog));

}