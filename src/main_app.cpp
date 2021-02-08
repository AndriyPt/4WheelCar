/*
 * main_app.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Bogdan
 */
#include "main_app.h"
#include "Motor.h"
#include "BusinessLogicAO/BusinessLogic.h"
#include "BusinessLogicAO/VirtualComPort.h"
#include "orion_protocol/orion_frame_transport.h"
#include "orion_protocol/orion_cobs_framer.h"
#include "orion_protocol/orion_header.h"
#include "orion_protocol/orion_minor.h"

#include "qpcpp.hpp"
#include <new>

#include "TB6612FNG.h"
#include "RPMEncoderOptical.h"
#include "tim.h"
#include "i2c.h"
#include "cmsis_os.h"
#include "MPU9250.h"
#include "MPU9250HALSTM32HALI2C.h"
#include "imu.h"
#include "Communication.h"
//#include <ros.h>

#include "orion_protocol/orion_minor.h"

using namespace std;
using namespace QP;

TB6612FNG *driver;
RPMEncoderOptical *enc1;
RPMEncoderOptical *enc2;
MPU9250FIFO *mpu;
MPU9250HALSTM32HALI2C *mpuHal;

carmen_hardware::VirtualComPort *p_com_port;
orion::COBSFramer *p_cobs_framer;
orion::FrameTransport *p_frame_transport;
orion::Minor *p_minor;

IMU *imu;
motor::Motor *motorp;
Communication *communication;
business_logic::BusinessLogic *p_business_logic;

volatile static float time = 0;

extern "C" void send_new_command_event(void)
{
  if (NULL != p_business_logic)
  {
    p_business_logic->sendNewCommandEvent();
  }
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin) {
	case (GPIO_PIN_5) : {
		if (imu != nullptr)
			imu->data_ready();
		break;
	}
	}
}

extern "C" int __io_putchar(int ch) {
	uint8_t c = (char)ch;
	HAL_UART_Transmit(&huart1, &c, 1, 1000);
}

extern "C" int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 1000);

	return len;
}

extern "C" Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    exit(-1);
}
void QF::onStartup(void) {}
void QF::onCleanup(void) {}

uint8_t poolStor2[1024];
uint8_t poolStor3[1024];

uint8_t mmm[sizeof(MPU9250FIFO)] = {0};

//class NewHardware : public ArduinoHardware
//{
//  public:
//  NewHardware():ArduinoHardware(&huart6){};
//};
//
//ros::NodeHandle_<NewHardware>  nh;

void main_cpp(void) {
    QF::init(); // initialize the framework

	QF::poolInit(poolStor2,
		sizeof(poolStor2),
		50);
	QF::poolInit(poolStor3,
		sizeof(poolStor3),
		100);


//	auto clock = HAL_RCC_GetPCLK1Freq()/htim2.Init.Prescaler;
//	auto pulses = 0.04/(1.0/(float)clock);
//	enc1 = new RPMEncoderOptical(&htim2, TIM_CHANNEL_1, TIM_CHANNEL_2, pulses);

	driver = new TB6612FNG();
	driver->init(GPIOA,
		  LINE_TB66_AIN2_Pin, LINE_TB66_AIN1_Pin,
		  LINE_TB66_BIN2_Pin, LINE_TB66_BIN1_Pin,
		  LINE_TB66_STBY_Pin,
		  &htim3, TIM_CHANNEL_2,
		  &htim3, TIM_CHANNEL_1);

	p_com_port = new carmen_hardware::VirtualComPort();
	p_cobs_framer = new orion::COBSFramer();
	p_frame_transport = new orion::FrameTransport(p_com_port, p_cobs_framer);
	p_minor = new orion::Minor(p_frame_transport);

	p_business_logic = new business_logic::BusinessLogic(p_minor);

	motorp = new motor::Motor(driver, enc1, nullptr);
	p_business_logic->setMotor(motorp);

    // communication = new Communication(&huart6, motorp);

	mpuHal = new MPU9250HALSTM32HALI2C(&hi2c1, 0x68);
	mpu = new (mmm) MPU9250FIFO(mpuHal);
	imu = new IMU(mpu, p_business_logic);


	/// Start QP

	auto t = xTimerCreate("QPRoootTimer",
	                     (10 / portTICK_PERIOD_MS), ///< 10ms tick
	                     pdTRUE,
	                     ( void * ) 0,
	                     [](TimerHandle_t xTimer){
								configASSERT( xTimer );
								QF::TICK_X(0U, nullptr);  // perform the QF clock tick processing
						}
	                   );
	xTimerStart( t, 0 );

	QF::run();
}


void DefaultTask(void const * argument)
{
	  for(;;)
	  {
		  osDelay(1000);
	  }
}
