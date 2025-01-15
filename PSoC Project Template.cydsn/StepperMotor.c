#include <project.h>
#include "CANPacket.h"
#include "StepperMotor.h"
#include <math.h>
#include <stdlib.h>

// Step Commands: IN4 IN3 IN2 IN1
uint8_t stepSequence[MAX_STEPS][4] = {
    {0, 0, 1, 1}, // AB
    {0, 1, 1, 0}, // BC
    {1, 1, 0, 0}, // CD
    {1, 0, 0, 1}  // DA
};

// Rotate Motor by Degrees
void rotateDegrees(StepperMotor *motor, int degrees) {
    int steps = (degrees * motor->stepsPerRevolution) / 360;
    stepMotor(motor, steps);
}

// Rotate Motor by Steps
void stepMotor(StepperMotor *motor, int steps) {
    motor->direction = (steps > 0) ? 1 : -1;
    steps = abs(steps);

    for (int i = 0; i < steps; i++) {
        motor->currentStep += motor->direction;
        if (motor->currentStep >= MAX_STEPS) {
            motor->currentStep = 0; 
        } else if (motor->currentStep < 0) {
            motor->currentStep = MAX_STEPS - 1;
        }
        setStepPins(motor->currentStep);
        delayMs(motor->stepDelay);
    }
}

// Set Step Pins
void setStepPins(int stepIndex) {
    // Apply the step signal to the ULN2003 driver
    IN1_Write(stepSequence[stepIndex][3]);
    IN2_Write(stepSequence[stepIndex][2]);
    IN3_Write(stepSequence[stepIndex][1]);
    IN4_Write(stepSequence[stepIndex][0]);

    // Visual feedback through LEDs (optional, if LEDs are wired to motor states)
    LED1_Write(1);
}

// Process CAN Packets
void processCANPacket(CANPacket *packet, StepperMotor *motor) {
    int packetID = GetPacketID(packet);
    if (packetID == ID_GPIO_BOARD_PWM_SET) { // ID_GPIO_BOARD_PWM_SET == the degree ID
        int16_t degrees = DecodeBytesToIntMSBFirst(packet->data, 0, 1);
        rotateDegrees(motor, degrees);
    }
}

void delayMs(uint16_t ms) {
    CyDelay(ms);
}
