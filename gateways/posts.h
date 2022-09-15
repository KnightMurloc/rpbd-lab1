//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_POSTS_H
#define LAB1_POSTS_H

#include <string>

enum class Post{
    manager,
    bartender,
    waiter,
    undefined
};

inline Post string_to_post(std::string data){
    if(data == "waiter"){
        return Post::waiter;
    }else if(data == "bartender"){
        return Post::bartender;
    }else if(data == "manager"){
        return Post::manager;
    }
    return Post::undefined;
}

inline std::string post_to_string(Post post){
    switch (post) {
        case Post::waiter:
            return "waiter";
        case Post::bartender:
            return "bartender";
        case Post::manager:
            return "manager";
        default:
            return "undefined";
    }
}

#endif //LAB1_POSTS_H
