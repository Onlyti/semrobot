int joystic_x_pin = 1;
int joystic_y_pin = 0;

const int MAX_OUTPUT = 255;
double FL_t = 0.0;
double FR_t = 0.0;
double RL_t = 0.0;
double RR_t = 0.0;

const int FLpwmPin = 5;
const int FRpwmPin = 6;
const int RLpwmPin = 7;
const int RRpwmPin = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  double x_joy = analogRead(joystic_x_pin);
  double y_joy = analogRead(joystic_y_pin);
  double r_fl, r_fr, r_rl, r_rr;
  double t_fl, t_fr, t_rl, t_rr;
  rotation_update(y_joy,r_fl, r_fr, r_rl, r_rr);
  forward_update(x_joy,t_fl, t_fr, t_rl, t_rr);
  // linear composition of rotation and translation
  FL_t = r_fl + t_fl;
  FR_t = r_fr + t_fr;
  RL_t = r_rl + t_rl;
  RR_t = r_rr + t_rr;

  update_motor(FLpwmPin, FL_t);
  update_motor(FRpwmPin, FR_t);
  update_motor(RLpwmPin, RL_t);
  update_motor(RRpwmPin, RR_t);

  Serial.print(" x: ");
  Serial.print(x_joy);
  Serial.print(" y: ");
  Serial.print(y_joy);
  Serial.print(" FL: ");
  Serial.print(FL_t);
  Serial.print(" FR: ");
  Serial.print(FR_t);
  Serial.print(" RL: ");
  Serial.print(RL_t);
  Serial.print(" RR: ");
  Serial.println(RR_t);
}

void rotation_update(double y_joystic, double& FL, double& FR,double& RL,double& RR)
{
  double joystic_ratio = (double)map(y_joystic,0,1023,-1000,1000)/1000;

  FL = -MAX_OUTPUT/2 * joystic_ratio;
  RL = -MAX_OUTPUT/2 * joystic_ratio;
  FR = MAX_OUTPUT/2 * joystic_ratio;
  RR = MAX_OUTPUT/2 * joystic_ratio;
}

void forward_update(double x_joystic, double& FL, double& FR,double& RL,double& RR)
{
  double joystic_ratio = (double)map(x_joystic,0,1023,-1000,1000)/1000;

  FL = MAX_OUTPUT/2 * joystic_ratio;
  RL = MAX_OUTPUT/2 * joystic_ratio;
  FR = MAX_OUTPUT/2 * joystic_ratio;
  RR = MAX_OUTPUT/2 * joystic_ratio;
}

void update_motor(int pin, double throtle)
{
  if(throtle >= 0) // forward
  {
    if(throtle > 255)
      throtle = 255;
    else if(throtle < 0)
      throtle = 0;
    
    int output = map(throtle, 0, 255, 127,255);
    analogWrite(pin,output);
  }
  else if(throtle < 0)//backward
  {
    throtle = -throtle; // inverce rotaion.
    if(throtle > 255)
      throtle = 255;
    else if(throtle < 0)
      throtle = 0;
    
    int output = map(throtle, 0, 255, 127,0);
    analogWrite(pin,output);
  }
}
