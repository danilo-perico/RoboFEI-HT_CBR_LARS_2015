/*--------------------------------------------------------------------

******************************************************************************
* @file main.cpp
* @author Isaac Jesus da Silva - ROBOFEI-HT - FEI
* @version V0.0.2
* @created 15/05/2014
* @Modified 15/05/2014
* @e-mail isaac25silva@yahoo.com.br
* @brief serial imu
****************************************************************************

Arquivo fonte contendo o programa que lê os dados da IMU via serial e escreve
na memória compartilhada os valores do girocópio, acelerometro e magnetômetro
estes valores são publicados na memoria a cada 5ms, ou seja, uma frequência de
200Hz


Para usar este processo deverá ser instalado o BOOST usando o comando abaixo

    sudo apt-get install libboost-all-dev

Para instalar o serial.h entrar na pasta serial, apagar a pasta build e executar os seguintes comandos:

mkdir build
cd build
cmake ..
make
sudo make install


/--------------------------------------------------------------------*/

/**
 *
 *  \file
 *  \brief      Main entry point for UM6 driver. Handles serial connection
 *              details, as well as all ROS message stuffing, parameters,
 *              topics, etc.
 *  \author     Mike Purvis <mpurvis@clearpathrobotics.com>
 *  \copyright  Copyright (c) 2013, Clearpath Robotics, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clearpath Robotics, Inc. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLEARPATH ROBOTICS, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Please send comments, questions, or patches to code@clearpathrobotics.com
 *
 */
#include <string>
#include <iostream>
#include <stdlib.h>
#include <blackboard.h>

//#include "geometry_msgs/Vector3Stamped.h"
//#include "ros/ros.h"
//#include "sensor_msgs/Imu.h"
#include <serial.h>
//#include "std_msgs/Float32.h"
//#include "std_msgs/Header.h"
#include <comms.h>
#include <registers.h>
//#include "um6/Reset.h"

double accelz;

// Don't try to be too clever. Arrival of this message triggers
// us to publish everything we have.
const uint8_t TRIGGER_PACKET = UM6_TEMPERATURE;

/**
 * Function generalizes the process of writing an XYZ vector into consecutive
 * fields in UM6 registers.
 */
template<typename RegT>
void configureVector3(um6::Comms* sensor, const um6::Accessor<RegT>& reg,
                      std::string param, std::string human_name)
{
  if (reg.length != 3)
  {
    throw std::logic_error("configureVector3 may only be used with 3-field registers!");
  }

  /*if (ros::param::has(param))
  {
    double x, y, z;
    ros::param::get(param + "/x", x);
    ros::param::get(param + "/y", y);
    ros::param::get(param + "/z", z);
    ROS_INFO_STREAM("Configuring " << human_name << " to ("
                    << x << ", " << y << ", " << z << ")");
    reg.set_scaled(0, x);
    reg.set_scaled(1, y);
    reg.set_scaled(2, z);
    if (sensor->sendWaitAck(reg))
    {
      throw std::runtime_error("Unable to configure vector.");
    }
  }*/
}

/**
 * Function generalizes the process of commanding the UM6 via one of its command
 * registers.
 */
template<typename RegT>
void sendCommand(um6::Comms* sensor, const um6::Accessor<RegT>& reg, std::string human_name)
{
  //ROS_INFO_STREAM("Sending command: " << human_name);
  std::cout<<"Sending command: " << human_name<<std::endl;
  if (!sensor->sendWaitAck(reg))
  {
    throw std::runtime_error("Command to device failed.");
  }
}


/**
 * Send configuration messages to the UM6, critically, to turn on the value outputs
 * which we require, and inject necessary configuration parameters.
 */
void configureSensor(um6::Comms* sensor)
{
  um6::Registers r;

  // Enable outputs we need.
  const uint8_t UM6_BAUD_115200 = 0x5;
  uint32_t comm_reg = UM6_BROADCAST_ENABLED |
                      UM6_GYROS_PROC_ENABLED | UM6_ACCELS_PROC_ENABLED | UM6_MAG_PROC_ENABLED |
                      UM6_QUAT_ENABLED | UM6_EULER_ENABLED | UM6_COV_ENABLED | UM6_TEMPERATURE_ENABLED |
                      UM6_BAUD_115200 << UM6_BAUD_START_BIT;
  r.communication.set(0, comm_reg);
  if (!sensor->sendWaitAck(r.communication))
  {
    throw std::runtime_error("Unable to set communication register.");
  }

  // Optionally disable mag and accel updates in the sensor's EKF.
  bool mag_updates, accel_updates;
  //ros::param::param<bool>("~mag_updates", mag_updates, true);
  //ros::param::param<bool>("~accel_updates", accel_updates, true);
  uint32_t misc_config_reg = UM6_QUAT_ESTIMATE_ENABLED;
  if (mag_updates)
  {
    misc_config_reg |= UM6_MAG_UPDATE_ENABLED;
  }
  else
  {
    std::cout<<"Excluding magnetometer updates from EKF."<<std::endl;
  }
  if (accel_updates)
  {
    misc_config_reg |= UM6_ACCEL_UPDATE_ENABLED;
  }
  else
  {
    std::cout<<"Excluding accelerometer updates from EKF."<<std::endl;
  }
  r.misc_config.set(0, misc_config_reg);
  if (!sensor->sendWaitAck(r.misc_config))
  {
    throw std::runtime_error("Unable to set misc config register.");
  }

  // Optionally disable the gyro reset on startup. A user might choose to do this
  // if there's an external process which can ascertain when the vehicle is stationary
  // and periodically call the /reset service.
  bool zero_gyros;
  //ros::param::param<bool>("~zero_gyros", zero_gyros, true);
  if (zero_gyros) sendCommand(sensor, r.cmd_zero_gyros, "zero gyroscopes");

  // Configurable vectors.
  configureVector3(sensor, r.mag_ref, "~mag_ref", "magnetic reference vector");
  configureVector3(sensor, r.accel_ref, "~accel_ref", "accelerometer reference vector");
  configureVector3(sensor, r.mag_bias, "~mag_bias", "magnetic bias vector");
  configureVector3(sensor, r.accel_bias, "~accel_bias", "accelerometer bias vector");
  configureVector3(sensor, r.gyro_bias, "~gyro_bias", "gyroscope bias vector");
}


bool handleResetService(um6::Comms* sensor,
                        bool zero_gyros = true, bool reset_ekf = true,
                        bool set_mag_ref = true, bool set_accel_ref = true)
{
  um6::Registers r;
  if (zero_gyros) sendCommand(sensor, r.cmd_zero_gyros, "zero gyroscopes");
  if (reset_ekf) sendCommand(sensor, r.cmd_reset_ekf, "reset EKF");
  if (set_mag_ref) sendCommand(sensor, r.cmd_set_mag_ref, "set magnetometer reference");
  if (set_accel_ref) sendCommand(sensor, r.cmd_set_accel_ref, "set accelerometer reference");
  return true;
}

/**
 * Uses the register accessors to grab data from the IMU, and populate
 * the ROS messages which are output.
 */
//void publishMsgs(um6::Registers& r, ros::NodeHandle* n, const std_msgs::Header& header)
void publishMsgs(um6::Registers& r)
{
  //static ros::Publisher imu_pub = n->advertise<sensor_msgs::Imu>("imu/data", 1, false);
  //static ros::Publisher mag_pub = n->advertise<geometry_msgs::Vector3Stamped>("imu/mag", 1, false);
  //static ros::Publisher rpy_pub = n->advertise<geometry_msgs::Vector3Stamped>("imu/rpy", 1, false);
  //static ros::Publisher temp_pub = n->advertise<std_msgs::Float32>("imu/temperature", 1, false);

//std::cout<<"accel"<< r.accel.get_scaled(2)<<std::endl;
//accelz = r.accel.get_scaled(2);
/*
  //if (imu_pub.getNumSubscribers() > 0)
  {
    sensor_msgs::Imu imu_msg;
    imu_msg.header = header;

    // IMU outputs [w,x,y,z] NED, convert to [x,y,z,w] ENU
    imu_msg.orientation.x = r.quat.get_scaled(2);
    imu_msg.orientation.y = r.quat.get_scaled(1);
    imu_msg.orientation.z = -r.quat.get_scaled(3);
    imu_msg.orientation.w = r.quat.get_scaled(0);

    // IMU reports a 4x4 wxyz covariance, ROS requires only 3x3 xyz.
    // NED -> ENU conversion req'd?
    imu_msg.orientation_covariance[0] = r.covariance.get_scaled(5);
    imu_msg.orientation_covariance[1] = r.covariance.get_scaled(6);
    imu_msg.orientation_covariance[2] = r.covariance.get_scaled(7);
    imu_msg.orientation_covariance[3] = r.covariance.get_scaled(9);
    imu_msg.orientation_covariance[4] = r.covariance.get_scaled(10);
    imu_msg.orientation_covariance[5] = r.covariance.get_scaled(11);
    imu_msg.orientation_covariance[6] = r.covariance.get_scaled(13);
    imu_msg.orientation_covariance[7] = r.covariance.get_scaled(14);
    imu_msg.orientation_covariance[8] = r.covariance.get_scaled(15);

    // NED -> ENU conversion.
    imu_msg.angular_velocity.x = r.gyro.get_scaled(1);
    imu_msg.angular_velocity.y = r.gyro.get_scaled(0);
    imu_msg.angular_velocity.z = -r.gyro.get_scaled(2);

    // NED -> ENU conversion.
    imu_msg.linear_acceleration.x = r.accel.get_scaled(1);
    imu_msg.linear_acceleration.y = r.accel.get_scaled(0);
    imu_msg.linear_acceleration.z = -r.accel.get_scaled(2);

    imu_pub.publish(imu_msg);
  }

  if (mag_pub.getNumSubscribers() > 0)
  {
    geometry_msgs::Vector3Stamped mag_msg;
    mag_msg.header = header;
    mag_msg.vector.x = r.mag.get_scaled(1);
    mag_msg.vector.y = r.mag.get_scaled(0);
    mag_msg.vector.z = -r.mag.get_scaled(2);
    mag_pub.publish(mag_msg);
  }

  if (rpy_pub.getNumSubscribers() > 0)
  {
    geometry_msgs::Vector3Stamped rpy_msg;
    rpy_msg.header = header;
    rpy_msg.vector.x = r.euler.get_scaled(1);
    rpy_msg.vector.y = r.euler.get_scaled(0);
    rpy_msg.vector.z = -r.euler.get_scaled(2);
    rpy_pub.publish(rpy_msg);
  }

  if (temp_pub.getNumSubscribers() > 0)
  {
    std_msgs::Float32 temp_msg;
    temp_msg.data = r.temperature.get_scaled(0);
    temp_pub.publish(temp_msg);
  }
  */

    IMU_GYRO_X = r.gyro.get_scaled(1);
    IMU_GYRO_Y = r.gyro.get_scaled(0);
    IMU_GYRO_Z = -r.gyro.get_scaled(2);

    IMU_ACCEL_X = r.accel.get_scaled(1);
    IMU_ACCEL_Y = r.accel.get_scaled(0);
    IMU_ACCEL_Z = -r.accel.get_scaled(2);

    IMU_COMPASS_X = r.mag.get_scaled(1);
    IMU_COMPASS_Y = r.mag.get_scaled(0);
    IMU_COMPASS_Z = -r.mag.get_scaled(2);

    IMU_EULER_X = r.euler.get_scaled(1);
    IMU_EULER_Y = r.euler.get_scaled(0);
    IMU_EULER_Z = -r.euler.get_scaled(2);

    IMU_QUAT_X = r.quat.get_scaled(2);
    IMU_QUAT_Y = r.quat.get_scaled(1);
    IMU_QUAT_Z = -r.quat.get_scaled(3);

//std::cout<<"accel "<< r.quat.get_scaled(2)<<std::endl;

}


/**
 * Node entry-point. Handles ROS setup, and serial port connection/reconnection.
 */
int main(int argc, char **argv)
{

    system("echo 123456 | sudo -S chmod 777 /dev/ttyUSB*");//libera USB
  //ros::init(argc, argv, "um6_driver");

    using_shared_memory();

  // Load parameters from private node handle.
  std::string port("/dev/ttyUSB0");
  int32_t baud = 115200;;
  //ros::param::param<std::string>("~port", port, "/dev/ttyUSB0");
  //ros::param::param<int32_t>("~baud", baud, 115200);

  serial::Serial ser;
  ser.setPort(port);
  ser.setBaudrate(baud);
  serial::Timeout to = serial::Timeout(50, 50, 0, 50, 0);
  ser.setTimeout(to);

  //ros::NodeHandle n;
  //std_msgs::Header header;
  //ros::param::param<std::string>("~frame_id", header.frame_id, "imu_link");

  bool first_failure = true;
  while (1)
  {
    try
    {
      ser.open();
    }
    catch(const serial::IOException& e)
    {
      std::cout<<"Unable to connect to port."<<std::endl;
    }
    if (ser.isOpen())
    {
      std::cout<<"Successfully connected to serial port."<<std::endl;
      first_failure = true;
      try
      {
        um6::Comms sensor(&ser);
        configureSensor(&sensor);
        um6::Registers registers;
        //ros::ServiceServer srv = n.advertiseService<um6::Reset::Request, um6::Reset::Response>(
        //                           "reset", boost::bind(handleResetService, &sensor, _1, _2));
        handleResetService(&sensor);
        int t=0;
        int contador = 0;
        float med_accel_z = 0, ac_med_accel_z = 0;
        while (1)
        {

            //--------- calcula a média do accel em Z-----------------------------
            if(contador>=40)
            {
                med_accel_z = ac_med_accel_z/40; // calcula a média do accel em Z
                contador = 0;
                ac_med_accel_z = 0;
            }
            ac_med_accel_z = ac_med_accel_z + IMU_ACCEL_Y;
            contador++;
            //--------------------------------------------------------------------

            if(med_accel_z<0.5) // Identifica se o robô esta caido ou em pé
                IMU_STATE = 1; // Robo caido
            else
                IMU_STATE = 0; // Robo em pé

            if(t>20)
            {

            std::cout<<"Robo caido = "<<std::fixed<<IMU_STATE<<std::endl;
            std::cout<<"med_acc_z = "<<std::fixed<<med_accel_z<<std::endl;
            std::cout<<"giros_x = "<<std::fixed<<IMU_GYRO_X<<std::endl;
            std::cout<<"giros_y = "<<std::fixed<<IMU_GYRO_Y<<std::endl;
            std::cout<<"giros_z = "<<std::fixed<<IMU_GYRO_Z<<std::endl;

            std::cout<<"accel_x = "<<std::fixed<<IMU_ACCEL_X<<std::endl;
            std::cout<<"accel_y = "<<std::fixed<<IMU_ACCEL_Y<<std::endl;
            std::cout<<"accel_z = "<<std::fixed<<IMU_ACCEL_Z<<std::endl;

            std::cout<<"magne_x = "<<std::fixed<<IMU_COMPASS_X<<std::endl;
            std::cout<<"magne_y = "<<std::fixed<<IMU_COMPASS_Y<<std::endl;
            std::cout<<"magne_z = "<<std::fixed<<IMU_COMPASS_Z<<std::endl;
            std::cout<<"Quat_x = "<<std::fixed<<IMU_QUAT_X<<std::endl;
            std::cout<<"Quat_y = "<<std::fixed<<IMU_QUAT_Y<<std::endl;
            std::cout<<"Quat_z = "<<std::fixed<<IMU_QUAT_Z<<std::endl;
            std::cout<<"Euler_x = "<<std::fixed<<IMU_EULER_X<<std::endl;
            std::cout<<"Euler_y = "<<std::fixed<<IMU_EULER_Y<<std::endl;
            std::cout<<"Euler_z = "<<std::fixed<<IMU_EULER_Z<<std::endl<<std::endl;
            t=0;
            }
            t++;
          if (sensor.receive(&registers) == TRIGGER_PACKET)
          {
            // Triggered by arrival of final message in group.
            //header.stamp = ros::Time::now();
            //publishMsgs(registers, &n, header);
            publishMsgs(registers);
            //ros::spinOnce();
          }
        }
      }
      catch(const std::exception& e)
      {
        if (ser.isOpen()) ser.close();
        //ROS_ERROR_STREAM(e.what());
        //ROS_INFO("Attempting reconnection after error.");
        std::cout<<"Attempting reconnection after error."<<std::endl;
        //ros::Duration(1.0).sleep();
      }
    }
    else
    {
      //ROS_WARN_STREAM_COND(first_failure, "Could not connect to serial device "
       //                    << port << ". Trying again every 1 second.");
      std::cout<< "Could not connect to serial device "
                << port << ". Trying again every 1 second."<< std::endl;
      first_failure = false;
      //ros::Duration(1.0).sleep();
    }
  }
}
