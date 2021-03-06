#if 1
#include "custom/robot.h"
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol()
{
    //space to run stuff once before begin driving
    master.print(0, 8, "Player 1");
    while (true)
    {
        //prints to screen the position and rotation of bongo
        Bongo.debugPos();

        // tyler control
        Bongo.tylerControl();
        // catie control
        //Bongo.catieControl();
        //testing for arm pos
        //if(master.get_digital(DIGITAL_Y)){
        //    Xarm.move_velocity(5);
        //} else if(master.get_digital(DIGITAL_A)){
        //    Xarm.move_velocity(-5);
        //} else {
        //    Bongo.Lift.stopXarm();
        //}
        if(master.get_digital(DIGITAL_Y)){
            Bongo.setPos(0,0);
            Bongo.setRotation(-90);
            Bongo.autonomous();
        }
        //locks
        if(master.get_digital(E_CONTROLLER_DIGITAL_LEFT)){
            Bongo.Wings.lockLeft();
        } else if (master.get_digital(E_CONTROLLER_DIGITAL_RIGHT)){
            Bongo.Wings.lockRight();
        }

        //odom testers
       //if(master.get_digital(DIGITAL_B)){
       //    Bongo.testOdom3();
       //} else if (master.get_digital(DIGITAL_X)){
       //    Bongo.testOdom();
       //} else if(master.get_digital(DIGITAL_Y)){
       //    Bongo.testOdom2();
       //}

        //lift controller updaters
        Bongo.Lift.setJoystickX(partner.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
        Bongo.Lift.setJoystickY(partner.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
        Bongo.Lift.handleVals();
        //Arm interface
        //emergency stop
        if(partner.get_digital(DIGITAL_L1) && partner.get_digital(DIGITAL_L2) && 
            partner.get_digital(DIGITAL_R1) && partner.get_digital(DIGITAL_R2)){
            Bongo.Lift.toggleEmergencyStop();
        }
        if(partner.get_digital_new_press(DIGITAL_A)){
            printf("confirming drop \n");
            Bongo.Lift.confirmDrop(); //unlocks ring piston and ends user drop command
        } else if (partner.get_digital_new_press(DIGITAL_B)){
            printf("toggle big side\n");
            Bongo.Lift.toggleBigSide(); //toggles the side that the big tower has been locked into
        } else if (partner.get_digital_new_press(DIGITAL_X)){
            printf("loading rings\n");
            Bongo.Lift.loadRings(); //starts auton load rings
        } else if (partner.get_digital_new_press(DIGITAL_Y)){
            printf("user drop\n");
            Bongo.Lift.userDrop(); //starts auton go to position
        }
        //manual powering 
        if (partner.get_digital(DIGITAL_DOWN)){
            Bongo.Lift.powerLiftDown(); //manually pushes arm down at max torque
        } else if (partner.get_digital(DIGITAL_UP)){
            Bongo.Lift.powerLiftUp(); //manually lifts arm with max torque
        } else {
            Bongo.Lift.stopPowerLifting(); //stops manual controll
        }


        //Spinning towers && controling wings 
        if(partner.get_digital(DIGITAL_R1) && Bongo.Wings.rightLockState()){
            //spin wing wheel right forward
            Bongo.Wings.spinRight(true);
        } else if (partner.get_digital(DIGITAL_R2) && Bongo.Wings.rightLockState()){
            //spin wing wheel right reverse
            Bongo.Wings.spinRight(false);
        } else if(master.get_digital(DIGITAL_R1) && !Bongo.Wings.rightLockState()){
            Bongo.Wings.tiltRightWing(true);
        } else if (master.get_digital(DIGITAL_R2) && !Bongo.Wings.rightLockState()){
            Bongo.Wings.tiltRightWing(false);
        } else {
            //dont spin wing wheel right
            Bongo.Wings.stopRight();
        }

        if(partner.get_digital(DIGITAL_L1) && Bongo.Wings.leftLockState()){
            //spin wing wheel left forward
            Bongo.Wings.spinLeft(true);
        } else if (partner.get_digital(DIGITAL_L2) && Bongo.Wings.leftLockState()){
            //spin wing wheel left reverse
            Bongo.Wings.spinLeft(false);
        } else if(master.get_digital(DIGITAL_L1) && !Bongo.Wings.leftLockState()){
            Bongo.Wings.tiltLeftWing(true);
        } else if (master.get_digital(DIGITAL_L2) && !Bongo.Wings.leftLockState()){
            Bongo.Wings.tiltLeftWing(false);
        } else {
            //dont spin wing wheel left
            Bongo.Wings.stopLeft();
        }
        //mogo line up
        if(master.get_digital(DIGITAL_A)){
            Bongo.lineUpMogo(LeftSideNum);
        } else if(master.get_digital(DIGITAL_Y)){
            Bongo.lineUpMogo(RightSideNum);
        }

        // starts the spin on motors or cuts power
        Bongo.Movement.move();
        //delay between updates
        delay(20);
    }
}
#endif