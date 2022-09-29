//
// Created by victor on 05.09.2022.
//

#include "Tab.h"
#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/label.h"
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "gtkmm/searchbar.h"
#include "gtkmm/searchentry.h"
#include "gtkmm/stockid.h"
#include "sigc++/functors/mem_fun.h"
#include <iostream>

Tab::Tab(TabManager* tab_manager) : tab_manager(tab_manager) {
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    this->add(*create_top_panel());

//    header = Gtk::make_managed<Gtk::Box>();
//    header->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
//    header->set_homogeneous(true);

//    auto cursor = Gdk::Cursor::create(Gdk::CursorType::HAND1);

//    auto event_box = Gtk::make_managed<Gtk::EventBox>();
//    event_box->set_hexpand(true);

//     auto label1 = Gtk::make_managed<Gtk::Label>("test1");
//     label1->add_events(Gdk::POINTER_MOTION_MASK |
//                            Gdk::POINTER_MOTION_HINT_MASK);
//     event_box->signal_enter_notify_event().connect([cursor, label1](GdkEventCrossing* event){
//         label1->get_window()->set_cursor(cursor);
//        return false;
//     });
//
//     event_box->signal_button_press_event().connect([](GdkEventButton* event){
//         std::cout << "test" << std::endl;
//         return false;
//     });

//     event_box->add(*label1);
//     auto label2 = Gtk::make_managed<Gtk::Label>("test2");
//     auto label3 = Gtk::make_managed<Gtk::Label>("test3");
//     auto label4 = Gtk::make_managed<Gtk::Label>("test4");
//     auto label5 = Gtk::make_managed<Gtk::Label>("test5");
//     auto label6 = Gtk::make_managed<Gtk::Label>("test6");
//     auto label7 = Gtk::make_managed<Gtk::Label>("test7");

//     header->add(*event_box);
//     header->add(*label2);
//     header->add(*label3);
//     header->add(*label4);
//     header->add(*label5);
//     header->add(*label6);
//     header->add(*label7);



//    scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
//    scroll->set_vexpand(true);
//    scroll->signal_edge_reached().connect(sigc::mem_fun(this,&Tab::scroll_event));

//     scroll->signal_edge_reached().connect([](Gtk::PositionType type){
//         std::cout << (int) type << std::endl;
//     });

//    this->add(*scroll);

//    listBox = Gtk::make_managed<Gtk::ListBox>();
//    listBox->set_vexpand(true);

//    auto box = Gtk::make_managed<Gtk::Box>();
//    box->set_orientation(Gtk::ORIENTATION_VERTICAL);
//    scroll->add(*box);

//    auto bottom_box = Gtk::make_managed<Gtk::Box>();
//    bottom_box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);

//    search_entry = Gtk::make_managed<Gtk::SearchEntry>();
//    search_entry->set_vexpand(false);
//    search_entry->set_hexpand(true);

//    stop_search = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-cancel"));
//     stop_search->set_label("");


//    box->add(*header);
//    box->add(*listBox);
//    bottom_box->add(*search_entry);
//    bottom_box->add(*stop_search);

//    this->add(*bottom_box);
}

Gtk::ButtonBox *Tab::create_top_panel() {

    auto top_panel = Gtk::make_managed<Gtk::ButtonBox>();
    top_panel->set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    add_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-add"));
    top_panel->add(*add_button);
    remove_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-delete"));
    top_panel->add(*remove_button);

    return top_panel;
}

Gtk::Button *Tab::getAddButton() const {
    return add_button;
}


Gtk::Button *Tab::getRemoveButton() const {
    return remove_button;
}

//Gtk::ListBox *Tab::getListBox() const {
//    return listBox;
//}

TabManager* Tab::get_tab_manager() const {
    return tab_manager;
}

int Tab::select_dialog() {

    Gtk::Dialog dialog;
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);

    auto list = create_list();

    auto box = dynamic_cast<Gtk::Box*>(list);

    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(*box);

    box->show_all();

    if(dialog.run()){
        auto entry = list->get_selected();
        return entry->get_id();
    }

//     Gtk::ListBox list;
//
//     fill_list(&list);
//
//     list.show_all();
//
//     Gtk::Dialog dialog;
//     dialog.add_button("OK", Gtk::RESPONSE_OK);
//     dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);
//
//     Gtk::ScrolledWindow scroll;
//     scroll.set_vexpand(true);
//     scroll.add(list);
//
//     dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(scroll);
//
//     scroll.show_all();
//     if(dialog.run() == Gtk::RESPONSE_OK){
//         auto entry = dynamic_cast<IEntry*>(list.get_selected_row());
//         return entry->get_id();
//     }

    return -1;
}

void Tab::select_by_id(int entry_id){
//    fill_list(getListBox());

    list->fill_list();

//    bool find = true;
   do{
       for(auto child : list->get_children()){
           auto entry = dynamic_cast<IEntry*>(child);
           if(entry->get_id() == entry_id){
               list->select_row(dynamic_cast<Gtk::ListBoxRow*>(entry));
               return;
           }
       }
   }while(list->scroll_down());
}

// void Tab::add_clumn_lable(std::string title){
//     auto lable = Gtk::make_managed<Gtk::Label>(title);
// //    header->add(*lable);
// }


void Tab::remove_entry_by_id(int id){
   for(auto child : list->get_children()){
       auto entry = dynamic_cast<IEntry*>(child);
       if(entry->get_id() == id){
           list->remove_row(child);
           break;
       }
   }
}

//void Tab::scroll_event(Gtk::PositionType type){
//    if(type == Gtk::PositionType::POS_BOTTOM){
//        scroll_down();
//    }else if(type == Gtk::PositionType::POS_TOP){
//        scroll_up();
//    }
//}

IList* Tab::get_list() const {
    return list;
}

void Tab::set_list(IList* list) {
    Tab::list = list;
    auto box = dynamic_cast<Gtk::Box*>(list);
    add(*box);
}
