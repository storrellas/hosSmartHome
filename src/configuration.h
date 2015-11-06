#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#define RPI_APP
//#define X86_EM_APP

#ifdef RPI_APP
    #define SET_UNDECORATED
    #define REMOVE_CURSOR
    #define TRANSLATE_FONTS
#endif

#ifdef X86_EM_APP
    #define SET_UNDECORATED
    #define REMOVE_CURSOR
#endif



#endif // CONFIGURATION_H
