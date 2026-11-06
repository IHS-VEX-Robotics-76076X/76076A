#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "robotConfigs.h"

// Left motor group (Port 20 was conflicting with an encoder, moved to 14)
pros::MotorGroup left_motor_group({-19, 20}, pros::MotorGears::blue);
// Right motor group
pros::MotorGroup right_motor_group({11, -12}, pros::MotorGears::blue);

// Drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, 
                              &right_motor_group, 
                              11, // 11 inch track width (Measure your own wheel middle-to-middle!)
                              lemlib::Omniwheel::NEW_325, // 3.25" omnis
                              450, // Drivetrain RPM
                              2 // Horizontal drift
);

// IMU (Inertial Sensor)
pros::Imu imu(13);

// Odometry settings (CLEANED UP: Set all tracking wheels to nullptr)
lemlib::OdomSensors sensors(nullptr, // No vertical tracking wheel 1
                            nullptr, // No vertical tracking wheel 2
                            nullptr, // No horizontal tracking wheel 1
                            nullptr, // No horizontal tracking wheel 2
                            &imu // LemLib will use your motors + this IMU for position tracking!
);

// Lateral PID controller (Handles straight line driving)
lemlib::ControllerSettings lateral_controller(2, 0, 2, 3, 1, 100, 3, 500, 20);

// Angular PID controller (Handles turning)
lemlib::ControllerSettings angular_controller(2, 0, 10, 3, 1, 100, 3, 500, 0);

// Create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

// Initialize function
void initialize() {
    pros::lcd::initialize(); 
    chassis.calibrate(); // This will calibrate your IMU on startup. Keep the robot still!
    
    pros::Task screen_task([&]() {
        while (true) {
            pros::lcd::print(0, "X: %f", chassis.getPose().x); 
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); 
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); 
            pros::delay(20);
        }
    });
}
