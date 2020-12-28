#ifndef BUSINESS_LOGIC_BASE_H_
#define BUSINESS_LOGIC_BASE_H_

#include "qpcpp.hpp"

class SetImuEvt : public QP::QEvt
{
public:
    ImuData data;
};

//.$declare${application::BusinessLogicBase} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//.${application::BusinessLogicBase} .........................................
class BusinessLogicBase : public QP::QActive {
protected:
    virtual void setImuHandler(SetImuEvt const* event) = 0;
    virtual void setEncodersHandler() = 0;
    virtual void commandHandler() = 0;

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
};

//.$enddecl${application::BusinessLogicBase} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#endif /* BUSINESS_LOGIC_BASE_H_ */
