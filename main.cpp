#include <iostream>
#include <gtkmm.h>
#include "LabOne.h"

int main(int argc, char *argv[]) {

    auto app = Gtk::Application::create(argc, argv,"local.lab.one");

    LabOne lab(app);

    return lab.run();
}
