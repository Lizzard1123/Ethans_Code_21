/*
void autonLineUpBall()
    {
        int val = 0;
        int count = 3;
        int tolerance = 3;
        while (true)
        {
            val = lineUpBall(true);
            if (val < tolerance && val > -tolerance)
            {
                count++;
            }

            if (count > 6)
            {
                break;
            }
            delay(10);
        }
    }

    void autonLineUpTower()
    {
        int val = 0;
        int count = 3;
        int tolerance = 3;
        while (true)
        {
            val = lineUpTower(true);
            if (val < tolerance && val > -tolerance)
            {
                count++;
            }

            if (count > 6)
            {
                break;
            }
            delay(10);
        }
    }

    
    void AutonomousOne()
    {
        Movement.flywheel.flywheelset(true);
        Movement.flywheel.setSpeed(100);
        // deploy da boi
        Movement.intake.activate(true);
        Movement.intake.open(true);
        delay(1000);
        //inward
        Movement.intake.open(false);
    }

    void AutonomousTwo()
    {
        Movement.flywheel.flywheelset(true);
        Movement.flywheel.setSpeed(50);
        Movement.uptake.setToggle(true);
        // deploy da boi
        Movement.intake.activate(true);
        Movement.intake.open(true);
        delay(400);
        Movement.intake.open(false);
        //delay(400);
        setPos(93, 12);
        //Movement.intake.activate(false);
        //Movement.intake.holdPos(true);
        //Movement.intake.keepAtPos(Movement.intake.middle);
        //go to right tower
        PIDMove(114, 30);
        PIDTurn(135);
        Movement.autonLineUpBall();
        //line up
        //Movement.autonLineUpTower();
        //go forward with stiff arms
        //new power cycle
        //wait until ball inside
        //Movement.moveTimed(75, 1200);
        Movement.moveLeft(30);
        Movement.moveRight(30);
        while (untilUptakePower())
        {
            delay(5);
        }
        Movement.moveLeft(0);
        Movement.moveRight(0);
        //Movement.intake.open(true);
        while (untilFlyhweelPower())
        {
            delay(5);
            Movement.lineUpTower(true);
        }
        //ball shoots
        //delay(100);
        //backout
        Movement.moveTimed(-100, 900);
        //go to opposite side and turn
        //PIDTurn(180);
        PIDMove(40, 30);
        Movement.intake.open(false);
        PIDTurn(225);
        //line up
        //Movement.autonLineUpTower();
        Movement.autonLineUpBall();
        //go forward and uptake
        //Movement.moveTimed(75, 1375);
        //uptake

        Movement.flywheel.setSpeed(50);
        Movement.moveLeft(20);
        Movement.moveRight(20);
        threshold = 7;
        while (untilUptakePower())
        {
            delay(5);
        }
        Movement.moveLeft(0);
        Movement.moveRight(0);
        //Movement.intake.open(true);
        while (untilFlyhweelPower())
        {
            delay(5);
            Movement.lineUpTower(true);
        }

        Movement.flywheel.setSpeed(45);
        //backout and turn to path
        Movement.moveTimed(-100, 900);
        Movement.intake.activate(true);
        Movement.intake.holdPos(false);
        PIDTurn(360);
    }

    void AutonomousThree()
    {
        Movement.flywheel.flywheelset(true);
        Movement.flywheel.setSpeed(50);
        Movement.uptake.setToggle(true);
        // deploy da boi
        Movement.intake.activate(true);
        Movement.intake.open(true);
        delay(400);
        Movement.intake.open(false);
        //delay(400);
        setPos(93, 12);
        //Movement.intake.activate(false);
        //Movement.intake.holdPos(true);
        //Movement.intake.keepAtPos(Movement.intake.middle);
        //go to right tower
        PIDMove(114, 30);
        PIDTurn(135);
        Movement.autonLineUpBall();
        //line up
        //Movement.autonLineUpTower();
        //go forward with stiff arms
        //new power cycle
        //wait until ball inside
        //Movement.moveTimed(75, 1200);
        Movement.moveLeft(30);
        Movement.moveRight(30);
        while (untilUptakePower())
        {
            delay(5);
        }
        Movement.moveLeft(0);
        Movement.moveRight(0);
        //Movement.intake.open(true);
        while (untilFlyhweelPower())
        {
            delay(5);
            Movement.lineUpTower(true);
        }
        //ball shoots
        //delay(100);
        //backout
        Movement.moveTimed(-100, 900);
        //go to opposite side and turn
        //PIDTurn(180);

        //middle tower
        PIDMove(74, 40);
        //PIDTurn(135);
        PIDMove(74, 54);

        PIDMove(40, 30);
        Movement.intake.open(false);
        PIDTurn(225);
        //line up
        //Movement.autonLineUpTower();
        Movement.autonLineUpBall();
        //go forward and uptake
        //Movement.moveTimed(75, 1375);
        //uptake

        Movement.flywheel.setSpeed(50);
        Movement.moveLeft(30);
        Movement.moveRight(30);
        threshold = 7;
        while (untilUptakePower())
        {
            delay(5);
        }
        Movement.moveLeft(0);
        Movement.moveRight(0);
        //Movement.intake.open(true);
        while (untilFlyhweelPower())
        {
            delay(5);
            Movement.lineUpTower(true);
        }

        Movement.flywheel.setSpeed(45);
        //backout and turn to path
        Movement.moveTimed(-100, 900);
        Movement.intake.activate(true);
        Movement.intake.holdPos(false);
        PIDTurn(360);
    }

    void Autonomous()
    {
        switch (autonCodeNum)
        {
        case 1:
            AutonomousOne();
            break;

        case 2:
            AutonomousTwo();
            break;

        case 3:
            AutonomousThree();
            break;
        }
    }
*/