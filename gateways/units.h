//
// Created by victor on 11.09.2022.
//

#ifndef LAB1_UNITS_H
#define LAB1_UNITS_H

#include <string>
#include <string_view>

enum class Unit{
    g,
    ml,
    undefined
};

inline Unit string_to_unit(std::string_view unit){
    if(unit == "g"){
        return Unit::g;
    }
    if(unit == "ml"){
        return Unit::ml;
    }

    return Unit::undefined;
}

inline std::string unit_to_string(Unit unit){
    switch(unit){
        case Unit::g:
            return "g";
        case Unit::ml:
            return "ml";
        default:
            return "undefined";
    }
}

#endif //LAB1_UNITS_H
