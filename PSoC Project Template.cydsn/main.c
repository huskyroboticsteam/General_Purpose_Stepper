#include <project.h>
#include "CANPacket.h"
#include "StepperMotor.h"

// Stepper Motor State
StepperMotor motor = {0, STEP_DELAY_MS, 1, 4096};

int main(void) {
    CyGlobalIntEnable; // Enable global interrupts
    CAN_Start();
    CANPacket packet;

    for (;;) {
        // Check for CAN packets
        int err = PollAndReceiveCANPacket(&packet);
        if (!err) {
            processCANPacket(&packet);
        }
    }
}
