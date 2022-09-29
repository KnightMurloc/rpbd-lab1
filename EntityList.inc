//#include "EntityList.h"

template<class T, class E>
EntityList<T, E>::EntityList(IGateway<T>* gateway) : gateway(gateway) {
    box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_VERTICAL);

    header = Gtk::make_managed<Gtk::Box>();
    header->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    header->set_homogeneous(true);

    scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_vexpand(true);
    scroll->signal_edge_reached().connect(sigc::mem_fun(this,&EntityList::scroll_event));

    list = Gtk::make_managed<Gtk::ListBox>();
    list->set_vexpand(true);
    scroll->add(*list);

    search_entry = Gtk::make_managed<Gtk::SearchEntry>();
    search_entry->set_vexpand(false);
    search_entry->set_hexpand(true);


    box->add(*header);
    box->add(*scroll);
    box->add(*search_entry);

    current_search = std::make_unique<DefaultSearch>(gateway);

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
//        fmt::print("name: {}\n", entry->get_ingredient()->get_name());
        list->insert(*entry,0);
    }

    auto rows = list->get_children();
    if(rows.size() > 40){
        for(size_t i = rows.size() - 1; i >= 40; i--){
//            fmt::print("removed\n");
            list->remove(*rows[i]);
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
}

template<class T, class E>
EntityList<T, E>::DefaultSearch::DefaultSearch(IGateway<T>* gateway) : gateway(gateway){}


template<class T, class E>
std::list<std::shared_ptr<T>> EntityList<T,E>::DefaultSearch::get_great_then(int id, int count){
    return gateway->get_great_then_by_id(id,count);
}

template<class T, class E>
std::list<std::shared_ptr<T>> EntityList<T,E>::DefaultSearch::get_less_then(int id, int count){
    return gateway->get_less_then_by_id(id,count);
}

template<class T, class E>
void EntityList<T,E>::set_search(std::unique_ptr<ISearch<T>> search){
    current_search = std::move(search);
    fill_list();
    list->show_all();
}

template<class T, class E>
void EntityList<T,E>::reset_search(){
    current_search = std::make_unique<DefaultSearch>(gateway);
}

template<class T, class E>
std::string EntityList<T,E>::get_search_text(){
    return search_entry->get_text();
}

template<class T, class E>
IGateway<T>* EntityList<T,E>::get_gateway(){
    return gateway;
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
}


