#ifndef PMD_EDU_EVENT_HPP__
#define PMD_EDU_EVENT_HPP__

#include "core.hpp"
enum pmdEDUEventTypes {
    PMD_EDU_EVENT_NONE = 0,
    PMD_EDU_EVENT_TERM,
    PMD_EDU_EVENT_RESUME,
    PMD_EDU_EVENT_ACTIVE,
    PMD_EDU_EVENT_DEACTIVE,
    PMD_EDU_EVENT_MSG,
    PMD_EDU_EVENT_TIMEOUT,
    PMD_EDU_EVENT_LOCKWAKEUP
};

class pmdEDUEvent {
public:
    pmdEDUEvent():
    _eventType(PMD_EDU_EVENT_NONE),
    _release(false),
    _Data(NULL)
    {}

    pmdEDUEvent(pmdEDUEventTypes type):
    _eventType(type),
    _release(false),
    _Data(NULL)
    {}

    pmdEDUEvent(pmdEDUEventTypes type, bool release, void *data):
    _eventType(type),
    _release(release),
    _Data(data)
    {}

    void reset() {
        _eventType = PMD_EDU_EVENT_NONE ;
        _release = false ;
        _Data = NULL ;    
    }

    pmdEDUEventTypes    _eventType;
    bool                _release;
    void                *_Data;
};

#endif