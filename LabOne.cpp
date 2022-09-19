//
// Created by victor on 05.09.2022.
//

#include "LabOne.h"

#include <utility>
#include "Form.h"
#include <iostream>
#include "EmployeesTab.h"
#include "ProductTab.h"
#include "OrderTab.h"

LabOne::LabOne(Glib::RefPtr<Gtk::Application> gtk_app): gtk_app(std::move(gtk_app)) {
    Gtk::Window* tmp_main_window = nullptr;
    Form::getInstance().getBuilder()->get_widget("main_window", tmp_main_window);
    if(!tmp_main_window){
        return;
    }
    main_window = static_cast<std::unique_ptr<Gtk::Window>>(tmp_main_window);

    Form::getInstance().getBuilder()->get_widget("tabs_container", tabs_container);
    if(!tabs_container){
        std::cout << "tabs_container get error" << std::endl;
        return;
    }

    tab_manager = std::make_unique<TabManager>(tabs_container);

//    tabs[0] = std::make_unique<EmployeesTab>();
//    tabs[1] = std::make_unique<ProductTab>();
//    tabs[2] = std::make_unique<OrderTab>();
//
//    tabs_container->append_page(*tabs[0], "сотрудники");
//    tabs_container->append_page(*tabs[1], "продукты");
//    tabs_container->append_page(*tabs[2], "приказы");

    tabs_container->show_all();
}

int LabOne::run() {
    if(main_window == nullptr){
        return 1;
    }
    return gtk_app->run(*main_window);
}
