#include "obj_date.h"
#include <iomanip>  /*! std::put_time */
#include <sstream>  /*! std::ostringstream */
#include <chrono>   /*! std::chrono::system_clock */
#include <ctime>    /*! std::time_t
                      * std::localtime */

void ObjectDate::Update() {
    auto timepoint = std::chrono::system_clock::now();
    std::time_t time_struct = std::chrono::system_clock::to_time_t(timepoint);
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&time_struct), "%F %T");
    SetText(stream.str());
}
