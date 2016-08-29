#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T12:06:21
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = CodeGen
TEMPLATE = lib

DEFINES += CODEGEN_LIBRARY

SOURCES += codegenerator.cpp \
    fsm.cpp \
    state.cpp \
    event.cpp \
    transition.cpp \
    stateparser.cpp \
    fwparser.cpp

HEADERS += codegenerator.h\
        codegen_global.h \
    fsm.h \
    state.h \
    event.h \
    transition.h \
    stateparser.h \
    fwparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    templates/template_constructor_cpp.tplt \
    templates/template_event_hdl_cpp.tplt \
    templates/template_event_hdl_h.tplt \
    templates/template_fwhdl_clear_cpp.tplt \
    templates/template_fwhdl_cpp.tplt \
    templates/template_fwhdl_h.tplt \
    templates/template_fwstate_h.tplt \
    templates/template_fwstate_event_hdl_h.tplt \
    templates/template_fwstate_event_exception_hdl_cpp.tplt \
    templates/template_fwstate_event_case_cpp.tplt \
    templates/template_fwstate_cpp.tplt \
    templates/template_fwhdl_initialize_cpp.tplt \
    templates/template_fwhdl_constructor_cpp.tplt \
    templates/template_fwhdl_sibl_class_h.tplt \
    templates/template_fwhdl_sibl_h.tplt \
    templates/template_fwhdl_sibl_inc_cpp.tplt \
    templates/template_fwhdl_sibl_interf_cpp.tplt \
    templates/template_fwhdl_sibl_interf_h.tplt \
    templates/template_fwstate_h.tplt \
    templates/template_include_cpp.tplt \
    templates/template_name_cpp.tplt \
    templates/template_state_initialize_cpp.tplt \
    templates/template_state_allowed_return_cpp.tplt \
    templates/template_state_fwhdl_include_cpp.tplt \
    templates/template_state_cpp.tplt \
    templates/template_state_h.tplt \
    templates/template_tostring_cpp.tplt \
    templates/template_model_msg_h.tplt \
    templates/template_model_msg_cpp.tplt \
    templates/template_model_handler_h.tplt \
    templates/template_model_handler_cpp.tplt \
    templates/template_model_state_h.tplt \
    templates/template_model_state_cpp.tplt \
    templates/template_model_timer_h.tplt \
    templates/template_model_timer_cpp.tplt \
    templates/template_model_timerthread_h.tplt \
    templates/template_model_timerthread_cpp.tplt \
    templates/template_model_msg_enum_event_case.tplt \
    templates/template_model_msg_enum_event.tplt
