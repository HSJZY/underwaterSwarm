#ifndef PID_H_
#define PID_H_


#define PID_TRUE  0xff
#define PID_FALSE 0x00
#define DEFAULT_PID_INTEGRATION_LIMIT  100.0
struct Robot_PID
{
	float target;  // 目标值
	float current; // 当前值
	float merror;   //测量误差
	float last_error;  //最后误差
	float Integrator;	//当前积分值
	float deriv;       //微分值
	float last_deriv;
	float iLimit;
	float Kp;	   //比例 
	float Ki;	   //积分
	float Kd;	   //微分

	unsigned char Lowpass_EN;
	float outP;         //< proportional output (debugging)比例输出
  float outI;         //< integral output (debugging)积分输出
  float outD;         //< derivative output (debugging)微分输出
	float PID_out;   //当前PID 的输出
};

void pidInit(struct Robot_PID* pid, const float kp, const float ki, const float kd);    //初始化PID参数
float pidUpdate(struct Robot_PID* pid, float measured,float dt);     //更新函数

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
