#ifndef BUSINESS_LOGIC_H_
#define BUSINESS_LOGIC_H_

#include "BusinessLogicBase.h"
#include "IMUInterface.h"
#include "orion_protocol/orion_minor.h"
#include <stdint.h>
#include <stdbool.h>
#include <cstddef>
#include "qpcpp.h"

#define COMMAND_BUFFER_SIZE (1024)

namespace business_logic
{

class BusinessLogic : public BusinessLogicBase, public IMUInterface
{
public:
    BusinessLogic(orion::Minor *minor);
    BusinessLogic() = delete;
    BusinessLogic& operator=(const BusinessLogic& object) = delete;
    BusinessLogic(const BusinessLogic& object) = delete;

    virtual void update_data(float alpha, float beta, float gamma);

    bool setEncoders(int32_t left, int32_t right);
    bool setImu(int32_t alpha, int32_t beta, int32_t gamma);

    void sendNewCommandEvent();

protected:
    virtual void setImuHandler(Event const* event);
    virtual void setEncodersHandler(Event const* event);
    virtual void commandHandler();

    void process_handshake_receive(void);
    void process_set_commands_receive(void);
    void process_set_pid_receive(void);

private:
    orion::Minor *minor_ = NULL;

    int32_t encoder_left_ = 0;
    int32_t encoder_right_ = 0;

    int32_t imu_angle_alpha_ = 0;
    int32_t imu_angle_beta_ = 0;
    int32_t imu_angle_gamma_ = 0;

    uint8_t command_buffer_[COMMAND_BUFFER_SIZE];
    std::size_t command_size_ = 0;
};

} // namespace business_logic

#endif /* BUSINESS_LOGIC_H_ */
