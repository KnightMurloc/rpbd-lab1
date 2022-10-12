//#include "EntityList.h"

#include "EntityList.h"
#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "gtkmm/object.h"
#include "sigc++/functors/mem_fun.h"
template<class T, class E>
EntityList<T, E>::EntityList() {
    box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_VERTICAL);

    header = Gtk::make_managed<Gtk::Box>();
    header->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    header->set_homogeneous(true);

    scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_vexpand(true);
    scroll->signal_edge_reached().connect(sigc::mem_fun(this,&EntityList::scroll_event));

    auto scroll_box = Gtk::make_managed<Gtk::Box>();
    scroll_box->set_orientation(Gtk::ORIENTATION_VERTICAL);


    list = Gtk::make_managed<Gtk::ListBox>();
    list->set_vexpand(true);
    scroll_box->add(*header);
    scroll_box->add(*list);

    scroll->add(*scroll_box);

    auto bottom_box = Gtk::make_managed<Gtk::Box>();
    bottom_box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    search_entry = Gtk::make_managed<Gtk::SearchEntry>();
    search_entry->set_vexpand(false);
    search_entry->set_hexpand(true);

    auto stop_search_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-cancel"));

    stop_search_button->signal_clicked().connect(sigc::mem_fun(this,&EntityList::reset_search));

    bottom_box->add(*search_entry);
    bottom_box->add(*stop_search_button);

    box->add(*scroll);
    box->add(*bottom_box);

    current_search = std::make_unique<DefaultSearch>();

    fill_list();

    this->add(*box);
}

template<class T, class E>
void EntityList<T, E>::fill_list() {
    for(auto child : list->get_children()){
        list->remove(*child);
    }

    first_id = 0;
    last_id = -1;
    for(const auto& entity : current_search->get_great_then(0,20)){
        if(entity->get_id() > last_id){
            last_id = entity->get_id();
        }
        auto entry = Gtk::make_managed<E>(entity);
        list->add(*entry);
    }

    scroll->show_all();
}

template<class T, class E>
void EntityList<T, E>::scroll_event(Gtk::PositionType type) {
    if(type == Gtk::PositionType::POS_BOTTOM){
        scroll_down();
    }else if(type == Gtk::PositionType::POS_TOP){
        scroll_up();
    }
}

template<class T, class E>
bool EntityList<T, E>::scroll_down() {
    first_id = last_id;
    auto data = current_search->get_great_then(last_id,20);
    if(data.empty()){
//             goto end;
        return false;
    }
    for(const auto& ing : data){
        if(ing->get_id() > last_id){
            last_id = ing->get_id();
        }
        auto entry = Gtk::make_managed<E>(ing);
        list->add(*entry);
    }

    auto rows = list->get_children();
    if(rows.size() > 40){

        for(int i = 0; i < rows.size() - 40; i++){
//            fmt::print("removed\n");
            list->remove(*rows[i]);
            delete rows[i];
        }
    }
    list->show_all();
    scroll->get_vadjustment()->set_value(500);
    return true;
}

template<class T, class E>
bool EntityList<T, E>::scroll_up() {
    last_id = first_id;
    auto data = current_search->get_less_then(first_id,20);
    if(data.empty()){
        return false;
    }
    for(const auto& ing : data){
        if(ing->get_id() < first_id){
            first_id = ing->get_id();
        }

        auto entry = Gtk::make_managed<E>(ing);
        list->insert(*entry,0);
    }

    auto rows = list->get_children();
    if(rows.size() > 40){
        for(size_t i = rows.size() - 1; i >= 40; i--){
            list->remove(*rows[i]);
            delete rows[i];

        }
    }
    list->show_all();

    scroll->get_vadjustment()->set_value(100);
    return true;
}

template<class T, class E>
Gtk::ListBox* EntityList<T, E>::get_list_box(){
    return list;
}

template<class T, class E>
IEntry* EntityList<T, E>::get_selected() {
    return dynamic_cast<IEntry*>(list->get_selected_row());
}

template<class T, class E>
void EntityList<T, E>::add_entity(E* entity){
    list->add(*entity);
}

template<class T, class E>
void EntityList<T, E>::remove_entity(E* entity){
    list->remove(*entity);
//     gateway->remove_from_cache(entity->get_id());
    delete entity;
}

// template<class T, class E>
// EntityList<T, E>::DefaultSearch::DefaultSearch(IGateway<T>* gateway) : gateway(gateway){}


template<class T, class E>
std::list<std::shared_ptr<T>> EntityList<T,E>::DefaultSearch::get_great_then(int id, int count){
    return T::get_great_than_by_id(id,count);
//     return gateway->get_great_then_by_id(id,count);
}

template<class T, class E>
std::list<std::shared_ptr<T>> EntityList<T,E>::DefaultSearch::get_less_then(int id, int count){
    return T::get_less_than_by_id(id,count);
//     return gateway->get_less_then_by_id(id,count);
}

template<class T, class E>
void EntityList<T,E>::set_search(std::unique_ptr<ISearch<T>> search){
    current_search = std::move(search);
    fill_list();
    list->show_all();
}

template<class T, class E>
void EntityList<T,E>::reset_search(){
//     current_search = std::make_unique<DefaultSearch>(gateway);
    search_entry->set_text("");
    fill_list();
}

template<class T, class E>
std::string EntityList<T,E>::get_search_text(){
    return search_entry->get_text();
}

template<class T, class E>
IGateway<T>* EntityList<T,E>::get_gateway(){
//     return gateway;
    return nullptr;
}

template<class T, class E>
Gtk::SearchEntry* EntityList<T,E>::get_search_entry(){
    return search_entry;
}

template<class T, class E>
std::vector<Gtk::Widget*> EntityList<T,E>::get_children(){
    return list->get_children();
}

template<class T, class E>
void EntityList<T,E>::select_row(Gtk::ListBoxRow* widget){
    list->select_row(*widget);
}

template<class T, class E>
void EntityList<T,E>::remove_row(Gtk::Widget* widget){
    list->remove(*widget);
    auto entity = dynamic_cast<E*>(widget);
//     if(entity){
//         gateway->remove_from_cache(entity->get_id());
//     }
    delete widget;
}

template<class T, class E>
void EntityList<T,E>::add_column_title(std::string title){
    auto label = Gtk::make_managed<Gtk::Label>(title);
    header->add(*label);
}

