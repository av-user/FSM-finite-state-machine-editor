#ifndef FWPARSER_H
#define FWPARSER_H

#include <QString>

class FWParser{
public:
    static QString get_hdl_custom_inc_h (QString str);
    static QString get_hdl_custom_part_h (QString str);
    static QString get_hdl_custom_inc_cpp (QString str);
    static QString get_hdl_custom_part_cpp (QString str);
};

#endif // FWPARSER_H
