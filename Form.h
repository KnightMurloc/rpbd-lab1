//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_FORM_H
#define LAB1_FORM_H

#include <gtkmm.h>

class Form {
private:

    Glib::RefPtr<Gtk::Builder> builder;

    void setup_entry();

    Form();
public:
    static Form& getInstance();

    static void number_only(const Glib::ustring& text,int* position, Gtk::Entry* self);

    static void float_only(const Glib::ustring& text,int* position, Gtk::Entry* self);

    const Glib::RefPtr<Gtk::Builder> &getBuilder() const;
};



#endif //LAB1_FORM_H
