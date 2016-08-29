#include "fwparser.h"
#include "state.h"
#include <QFile>

QString FWParser::get_hdl_custom_inc_h (QString str){
    QString retval;
    if (str.length() == 0){
        retval = State::CUSTOM_INCLUDE_BEGIN_TAG + State::CUSTOM_INCLUDE_END_TAG;
    } else {
        int idxb = str.indexOf(State::CUSTOM_INCLUDE_BEGIN_TAG);
        if (idxb != -1){
            int idxe = str.indexOf(State::CUSTOM_INCLUDE_END_TAG);
            if (idxe > idxb){
                retval = str.mid(idxb
                         , idxe - idxb + State::CUSTOM_INCLUDE_END_TAG.length());
            }
        }
        if (retval.isEmpty()){
            retval = State::CUSTOM_INCLUDE_BEGIN_TAG + State::CUSTOM_INCLUDE_END_TAG;
        }
    }
    return retval;
}
QString FWParser::get_hdl_custom_part_h (QString str){
    QString retval;
    if (str.length() == 0){
        retval = State::CUSTOM_PART_BEGIN_TAG + State::CUSTOM_PART_END_TAG;
    } else {
        int idxb = str.indexOf(State::CUSTOM_PART_BEGIN_TAG);
        if (idxb != -1){
            int idxe = str.indexOf(State::CUSTOM_PART_END_TAG);
            if (idxe > idxb){
                retval = str.mid(idxb
                         , idxe - idxb + State::CUSTOM_PART_END_TAG.length());
            }
        }
        if (retval.isEmpty()){
            retval = State::CUSTOM_PART_BEGIN_TAG + State::CUSTOM_PART_END_TAG;
        }
    }
    return retval;
}
QString FWParser::get_hdl_custom_inc_cpp (QString str){
    QString retval;
    if (str.length() == 0){
        retval = State::CUSTOM_INCLUDE_BEGIN_TAG + State::CUSTOM_INCLUDE_END_TAG;
    } else {
        int idxb = str.indexOf(State::CUSTOM_INCLUDE_BEGIN_TAG);
        if (idxb != -1){
            int idxe = str.indexOf(State::CUSTOM_INCLUDE_END_TAG);
            if (idxe > idxb){
                retval = str.mid(idxb
                         , idxe - idxb + State::CUSTOM_INCLUDE_END_TAG.length());
            }
        }
        if (retval.isEmpty()){
            retval = State::CUSTOM_INCLUDE_BEGIN_TAG + State::CUSTOM_INCLUDE_END_TAG;
        }
    }
    return retval;
}
QString FWParser::get_hdl_custom_part_cpp (QString str){
    QString retval;
    if (str.length() == 0){
        retval = State::CUSTOM_PART_BEGIN_TAG + State::CUSTOM_PART_END_TAG;
    } else {
        int idxb = str.indexOf(State::CUSTOM_PART_BEGIN_TAG);
        if (idxb != -1){
            int idxe = str.indexOf(State::CUSTOM_PART_END_TAG);
            if (idxe > idxb){
                retval = str.mid(idxb
                         , idxe - idxb + State::CUSTOM_PART_END_TAG.length());
            }
        }
        if (retval.isEmpty()){
            retval = State::CUSTOM_PART_BEGIN_TAG + State::CUSTOM_PART_END_TAG;
        }
    }
    return retval;
}
