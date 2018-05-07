#ifndef PID_H_
#define PID_H_


#define PID_TRUE  0xff
#define PID_FALSE 0x00
#define DEFAULT_PID_INTEGRATION_LIMIT  100.0
struct Robot_PID
{
	float target;  // Ŀ��ֵ
	float current; // ��ǰֵ
	float merror;   //�������
	float last_error;  //������
	float Integrator;	//��ǰ����ֵ
	float deriv;       //΢��ֵ
	float last_deriv;
	float iLimit;
	float Kp;	   //���� 
	float Ki;	   //����
	float Kd;	   //΢��

	unsigned char Lowpass_EN;
	float outP;         //< proportional output (debugging)�������
  float outI;         //< integral output (debugging)�������
  float outD;         //< derivative output (debugging)΢�����
	float PID_out;   //��ǰPID �����
};

void pidInit(struct Robot_PID* pid, const float kp, const float ki, const float kd);    //��ʼ��PID����
float pidUpdate(struct Robot_PID* pid, float measured,float dt);     //���º���

float pidUpdate_err(struct Robot_PID* pid,float err, float dt);                    
void pidSetIntegralLimit(struct Robot_PID* pid, float limit);
void pidSetError(struct Robot_PID* pid, float err);
void pidReset(struct Robot_PID* pid);
void pidSetTarget(struct Robot_PID* pid, float target);
void pidSetKp(struct Robot_PID* pid, float kp);
void pidSetKi(struct Robot_PID* pid, float ki);
void pidSetKd(struct Robot_PID* pid, float kd);
void pidSetMeasured(struct Robot_PID* pid, float measured);
void pidSetTarget_Measure(struct Robot_PID* pid, float target, float measured);
void pidSetLowPassEnable(struct Robot_PID* pid);

#endif
