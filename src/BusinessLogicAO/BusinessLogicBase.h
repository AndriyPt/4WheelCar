#ifndef BUSINESS_LOGIC_BASE_H_
#define BUSINESS_LOGIC_BASE_H_

#include "qpcpp.hpp"

namespace business_logic
{

typedef struct
{
    int32_t alpha;
    int32_t beta;
    int32_t gamma;
} ImuData;

typedef struct
{
    int32_t alpha;
    int32_t beta;
    int32_t gamma;
} EncodersData;

enum Signal
{
   BL_SET_IMU_SIG = QP::Q_USER_SIG,
   BL_SET_ENCODERS_SIG,
   BL_COMMAND_SIG
};

struct Event : public QP::QEvt {
    union
    {
        ImuData imu;
        EncodersData encoders;
    } data;

    Event(QP::QSignal const s) : QEvt(s) {};
};

};

//.$declare${application::BusinessLogicBase} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

namespace business_logic
{

//.${application::BusinessLogicBase} .........................................
class BusinessLogicBase : public QP::QActive {
protected:
    virtual void setImuHandler(Event const* event) = 0;
    virtual void setEncodersHandler(Event const* event) = 0;
    virtual void commandHandler(Event const* event) = 0;

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
};
}

//.$enddecl${application::BusinessLogicBase} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#endif /* BUSINESS_LOGIC_BASE_H_ */
