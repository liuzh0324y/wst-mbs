#ifndef HANDLER_HH
#define HANDLER_HH

#include "agorarecord.hh"

class Handler 
{
public:
    Handler();
    virtual ~Handler();

    Record recorder;
};

#endif // HANDLER_HH