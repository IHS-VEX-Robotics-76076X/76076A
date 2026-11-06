#include "main.h"
#include "robotConfigs.h" // Ensure this matches your config file name exactly

//-----------------------------------
// STARTUP / INITIALIZATION
//-----------------------------------
void initialize() {
    // 1. Start up the brain screen
    pros::lcd::initialize();
    pros::lcd::print(0, "Robot Initializing...");

    // 2. Set brake modes for the drivetrain
    left_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    // 3. Calibrate the Chassis (This automatically handles the IMU calibration)
    // Make sure the robot is completely still on the field when turning it on!
    chassis.calibrate(); 

    // 4. Create a background task to constantly print your live GPS coordinates
    pros::Task screen_task([&]() {
        while (true) {
            pros::lcd::print(1, "X: %.2f", chassis.getPose().x); 
            pros::lcd::print(2, "Y: %.2f", chassis.getPose().y); 
            pros::lcd::print(3, "Heading: %.2f", chassis.getPose().theta); 
            pros::delay(20);
        }
    });
}

//-----------------------------------
// AUTONOMOUS ROUTINE
//-----------------------------------
void autonomous() {
    
}

//-----------------------------------
// DRIVER MANUAL CONTROL
//-----------------------------------
void opcontrol() {
    // Create a controller object so you can drive
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    // 1. Tell the robot it is starting at the origin (0,0) facing straight up (0 degrees)
    chassis.setPose(0, 0, 0);
    
    // 2. Drive straight forward 24 inches (X stays 0, Y goes to 24)
    chassis.moveToPoint(0, 36, 2000);
    
    // 3. Turn exactly 90 degrees clockwise (To face the right side of the field)
    chassis.turnToHeading(90, 1500);
    
    // 4. Drive straight forward another 24 inches from the new position
    // (Since we are facing 90 degrees, moving "forward" means X increases to 24, Y stays 24)
    chassis.moveToPoint(36, 36, 2000);

    while (true) {
        // Read stick axes for Arcade Drive
        int power = master.get_analog(ANALOG_LEFT_Y);
        int steering = master.get_analog(ANALOG_RIGHT_X);

        // Send power directly to your motor groups
        left_motor_group.move_voltage((power + steering) * 120);
        right_motor_group.move_voltage((power - steering) * 120);

        pros::delay(20); // Save CPU resources
    }
}

// These empty functions must exist so PROS doesn't throw errors
void disabled() {}
void competition_initialize() {}