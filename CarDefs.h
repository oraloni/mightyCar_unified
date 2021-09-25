
const byte motor1_A = 2;
const byte motor1_B = 14;
const byte motor1_EN = 0;

const byte motor2_A = 12;
const byte motor2_B = 13;
const byte motor2_EN = 15;

const byte servoPin = 4;

bool onBreakRight = false;
bool onBreakLeft = false;

enum carDirection{
    Stop,
    Forward,
    Backward
};

carDirection direction = Stop;

void setMotorSpeed(const byte motor_en_pin, byte set_speed)
{
    analogWrite(motor_en_pin, set_speed);
}

void setSpeedToAll(byte set_speed)
{
    byte en_pins[2] = {motor1_EN, motor2_EN};
    for (int i = 0; i < sizeof(en_pins); i++)
    {
        setMotorSpeed(en_pins[i], set_speed);
    }
}

void move_backward(byte speed_val)
{
    digitalWrite(motor1_A, HIGH);
    digitalWrite(motor1_B, LOW);

    digitalWrite(motor2_A, HIGH);
    digitalWrite(motor2_B, LOW);

    setSpeedToAll(speed_val);
}

void move_forward(byte speed_val)
{
    digitalWrite(motor1_A, LOW);
    digitalWrite(motor1_B, HIGH);

    digitalWrite(motor2_A, LOW);
    digitalWrite(motor2_B, HIGH);

    setSpeedToAll(speed_val);
}

void stop_car()
{
    digitalWrite(motor1_A, LOW);
    digitalWrite(motor1_B, LOW);

    digitalWrite(motor2_A, LOW);
    digitalWrite(motor2_B, LOW);
}

void break_left(byte speed_val)
{
    onBreakLeft = true;
    onBreakRight = false;
    Serial.println("break left!");
    digitalWrite(motor1_A, LOW);
    digitalWrite(motor1_B, HIGH);
    setMotorSpeed(motor1_EN, speed_val);
    
    digitalWrite(motor2_A, HIGH);
    digitalWrite(motor2_B, HIGH);
    setMotorSpeed(motor2_EN, 0);
}


void break_right(byte speed_val)
{
    onBreakRight = true;
    onBreakLeft = false;
    Serial.println("break right!");
    digitalWrite(motor1_A, HIGH);
    digitalWrite(motor1_B, HIGH);
    setMotorSpeed(motor1_EN, 0);
    
    digitalWrite(motor2_A, LOW);
    digitalWrite(motor2_B, HIGH);
    setMotorSpeed(motor2_EN, speed_val);
}
