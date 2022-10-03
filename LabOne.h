//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_LABONE_H
#define LAB1_LABONE_H

#include <gtkmm.h>
#include <memory>
#include "Tab.h"
#include "TabManager.h"

class LabOne {
private:
    Glib::RefPtr<Gtk::Application> gtk_app;
    std::unique_ptr<Gtk::Window> main_window;

    Gtk::Notebook* tabs_container;

    std::unique_ptr<TabManager> tab_manager;

public:
    explicit LabOne(Glib::RefPtr<Gtk::Application> gtk_app);

    int run();
};

#endif //LAB1_LABONE_H
