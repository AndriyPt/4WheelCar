#include "BusinessLogic.h"
#include "qpcpp.h"
#include <assert.h>
#include "protocol.h"
#include "qpcpp.h"

namespace business_logic {

BusinessLogic::BusinessLogic(orion::Minor *minor): BusinessLogicBase(), minor_(minor)
{
}

bool BusinessLogic::setImu(int32_t alpha, int32_t beta, int32_t gamma_)
{

}

bool setEncoders(int32_t left, int32_t right)
{

}

void BusinessLogic::sendNewCommandEvent()
{
    return Q_NEW(SetImuEvt, signal);    
}

void BusinessLogic::process_handshake_receive(void)
{
    assert(NULL != this->command_buffer_);
    assert(this->command_size_ >= sizeof(orion::CommandHeader));
    orion::CommandHeader * command_header = reinterpret_cast<orion::CommandHeader*>(this->command_buffer_);
    assert(carmen_hardware::MessageType::Handshake ==
            (carmen_hardware::MessageType)command_header->common.message_id);
    carmen_hardware::HandshakeResult handshake_result;
    handshake_result.header.common.sequence_id = command_header->common.sequence_id;
    // TODO: Add code to validate that protocol versions coincide else send error code e.g. minor.validate method
    this->minor_->sendResult((uint8_t*)&handshake_result, sizeof(handshake_result));
}

void BusinessLogic::setImuHandler(Event const* event)
{
    assert(NULL != event);
    this->imu_.velocity_x = event->data.velocity_x; 
    this->imu_.acceleration_y = event->data.acceleration_y; 
}

void BusinessLogic::setEncodersHandler(Event const* event)
{

}

void BusinessLogic::process_set_commands_receive(void)
{
    assert(NULL != this->command_buffer_);
    assert(this->command_size_ >= sizeof(carmen_hardware::SetCommandsCommand));
    carmen_hardware::SetCommandsCommand * command =
            reinterpret_cast<carmen_hardware::SetCommandsCommand*>(this->command_buffer_);
    assert(carmen_hardware::MessageType::SetCommands ==
            (carmen_hardware::MessageType)command->header.common.message_id);

    // TODO: Implement
    // this->motor_->setCommand(command->left_cmd, command->right_cmd);

    carmen_hardware::SetCommandsResult reply;
    reply.header.common.sequence_id = command->header.common.sequence_id;
    reply.encoder_left = this->encoder_left_;
    reply.encoder_right = this->encoder_right_;
    reply.imu_angle_alpha = this->imu_angle_alpha_;
    reply.imu_angle_beta = this->imu_angle_beta_;
    reply.imu_angle_gamma = this->imu_angle_gamma_;
    // TODO: Add code to validate that protocol versions coincide else send error code e.g. minor.validate method
    this->minor_->sendResult((uint8_t*)&reply, sizeof(reply));
}

void BusinessLogic::process_set_pid_receive(void)
{
    assert(NULL != this->command_buffer_);
    assert(this->command_size_ >= sizeof(carmen_hardware::SetPIDCommand));
    carmen_hardware::SetPIDCommand * command =
            reinterpret_cast<carmen_hardware::SetPIDCommand*>(this->command_buffer_);
    assert(carmen_hardware::MessageType::SetPID ==
            (carmen_hardware::MessageType)command->header.common.message_id);

    carmen_hardware::SetPIDResult reply;
    reply.header.common.sequence_id = command->header.common.sequence_id;

    // TODO: Implement
    // reply.result = this->motor_->setPid(command->left_p, command->left_i, command->left_d, command->right_p, command->right_i, command->right_d);

    // TODO: Add code to validate that protocol versions coincide else send error code e.g. minor.validate method
    this->minor_->sendResult((uint8_t*)&reply, sizeof(reply));
}

void BusinessLogic::commandHandler()
{
    if (this->minor_->receiveCommand(this->command_buffer_, COMMAND_BUFFER_SIZE, this->command_size_))
    {
        assert(NULL != this->command_buffer_);
        assert(this->command_size_ >= sizeof(orion::CommandHeader));
        orion::CommandHeader * command_header = reinterpret_cast<orion::CommandHeader*>(this->command_buffer_);
        switch (static_cast<carmen_hardware::MessageType>(command_header->common.message_id))
        {
            case carmen_hardware::MessageType::Handshake:
                process_handshake_receive();
                break;
            case carmen_hardware::MessageType::SetCommands:
                process_set_commands_receive();
                break;
            case carmen_hardware::MessageType::SetPID:
                process_set_pid_receive();
                break;
            default:
                assert(false);
        }
    }
}

} // namespace business_logic
