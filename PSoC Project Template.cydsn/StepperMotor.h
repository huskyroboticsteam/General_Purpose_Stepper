#include <stdint.h>
#include <project.h>

// Constants
#define MAX_STEPS 4 // 4-step sequence (AB, BC, CD, DA)
#define STEP_DELAY_MS 5 // Default delay between steps (adjustable)

// Step Commands: IN4 IN3 IN2 IN1
extern uint8_t stepSequence[MAX_STEPS][4];

// Stepper Motor State
typedef struct {
    int currentStep;  // Current step in the sequence
    int stepDelay;    // Delay between steps (ms)
    int direction;    // 1 for forward, -1 for reverse
    int stepsPerRevolution; // Steps for 360° rotation (4096)
} StepperMotor;

// Function Prototypes
void setStepPins(int stepIndex);
void stepMotor(StepperMotor *motor, int steps);
void rotateDegrees(StepperMotor *motor, int degrees);
void processCANPacket(CANPacket *packet);
void delayMs(uint16_t ms);
