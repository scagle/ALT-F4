#include <stdio.h>

unsigned int getServoPosition(volatile unsigned long *ADC_Servo_Value);
// Servos
const unsigned int LEFT_MOST_VALUE = 8000;   // 5%  Duty
const unsigned int RIGHT_MOST_VALUE = 40000; // 10% Duty
const unsigned int RANGE = RIGHT_MOST_VALUE - LEFT_MOST_VALUE;
const unsigned int MAX_ADC = 4095;


int main(void)
{

    volatile unsigned long input;
    printf("Enter a value between 0 - 4095\n");
    scanf("%d", &input);
    printf("%d\n", getServoPosition(&input));
    return 0; 
}

// Returns a clock cycle count requirement in order to acheive PWM duty cycles 
// Converts (0-4095) to 5% - 10% duty cycle (or 8000-40000 clock cycles)
unsigned int getServoPosition(volatile unsigned long *ADC_Servo_Value)
{
    unsigned int Add_Value = 8;
    return ( Add_Value * (*ADC_Servo_Value) );
}
