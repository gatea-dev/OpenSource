#ifndef __BROADCAST_MESSAGE_TYPES_C_H__
#define __BROADCAST_MESSAGE_TYPES_C_H__

#include "fdsfe_Api_C.h"

#ifdef __cplusplus
extern "C" {
#endif

    const char BC_MSG_TYPE_QUOTE = 'Q';
    const char BC_MSG_TYPE_TRADE = 'T';
    const char BC_MSG_TYPE_CANCEL = 'X';
    const char BC_MSG_TYPE_CORRECTION = 'R';
    const char BC_MSG_TYPE_OTHER = 'O';
    const char BC_MSG_TYPE_BOND_SESSION_CLOSED = 'E';
    const char BC_MSG_TYPE_BOND_CLOSING_SPIN = 'I';
    const char BC_MSG_TYPE_RESERVED = 'V';
    const char BC_MSG_TYPE_DELETE = 'D';
    const char BC_MSG_TYPE_ROLLOVER = 'L';
    const char BC_MSG_TYPE_UPDATE = 'U';
    const char BC_MSG_TYPE_SNAPSHOT = 'S';

#ifdef __cplusplus
}
#endif

#endif // __BROADCAST_MESSAGE_TYPES_C_H__
