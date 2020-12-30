#include "BusinessLogicBase.h"

#include "BusinessLogic.h"

enum Signal
{
   BL_SET_IMU_SIG = Q_USER_SIG,
   BL_SET_ENCODERS_SIG,
   BL_COMMAND_SIG
};

//.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//. Check for the minimum required QP version
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.8.0 or higher required
#endif
//.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//.$define${application::BusinessLogicBase} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//.${application::BusinessLogicBase} .........................................
//.${application::BusinessLogicBas~::SM} .....................................
Q_STATE_DEF(BusinessLogicBase, initial) {
    //.${application::BusinessLogicBas~::SM::initial}
    return tran(&idle);
}
//.${application::BusinessLogicBas~::SM::idle} ...............................
Q_STATE_DEF(BusinessLogicBase, idle) {
    QP::QState status_;
    switch (e->sig) {
        //.${application::BusinessLogicBas~::SM::idle::BL_SET_IMU}
        case BL_SET_IMU_SIG: {
            this->setImuHandler(Q_EVT_CAST(SetImuEvt));
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${application::BusinessLogicBas~::SM::idle::BL_SET_ENCODERS}
        case BL_SET_ENCODERS_SIG: {
            this->setEncodersHandler();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${application::BusinessLogicBas~::SM::idle::BL_COMMAND}
        case BL_COMMAND_SIG: {
            this->commandHandler();
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
