#include "base.h"

const float  lowpass_filter = 7.9577e-3;

/*PID������ʼ��*/
void pidInit(struct Robot_PID* pid, float kp,float ki, float kd)
{
  pid->merror = 0;     //�������
  pid->last_error = 0;
  pid->Integrator = 0;
  pid->deriv = 0;
  pid->target = 0;
  pid->last_deriv = 0;
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;
  pid->Lowpass_EN = PID_FALSE;                  //��ͨ�˲�	��ֹ  PID_FALSE=0x00
  pid->iLimit = DEFAULT_PID_INTEGRATION_LIMIT;	//���ּ���  100
}

//���ݲ���ֵ������ֵ��ֵ����PID���
float pidUpdate(struct Robot_PID* pid, float measured,float dt)
{
	float output;
	
	pid->current = measured;
	pid->merror = pid->target - measured;
	
	//I  ����
  if(pid->Ki != 0)
  {
   pid->Integrator += (pid->Ki * pid->merror) * dt;//�����㷨  ��������Ż�Ϊ�����㷨
   if (pid->Integrator > pid->iLimit)             //���ڻ��ּ���ʱ ����ΪiLimit   ����Ϊ�������Ӧ�������Ӳ���index
   {
    pid->Integrator = pid->iLimit;
   }
   else if (pid->Integrator < -pid->iLimit)
   {
    pid->Integrator = -pid->iLimit;
   }
  }
	 //D ΢��
  pid->deriv = (pid->merror - pid->last_error) / dt;
  if(pid->Lowpass_EN != PID_FALSE)	//��ͨ�˲�����ֹƵ��20hz
  { 
		pid->deriv = pid->last_deriv + (dt / (lowpass_filter + dt)) * (pid->deriv - pid->last_deriv);	
  }

  pid->outP = pid->Kp * pid->merror;
  pid->outI = pid->Ki*pid->Integrator;
  pid->outD = pid->Kd * pid->deriv;

  pid->PID_out = output =pid->outP +pid->outI +pid->outD;

  pid->last_error = pid->merror;
	
	 return output;
}

//����������PID���
float pidUpdate_err(struct Robot_PID* pid,float err, float dt)
{
  float output;
  pid->merror = err;
  //I ���� 
  pid->Integrator += (pid->Ki * pid->merror) * dt;
  if (pid->Integrator > pid->iLimit)
  {
    pid->Integrator = pid->iLimit;
  }
  else if (pid->Integrator < -pid->iLimit)
  {
    pid->Integrator = -pid->iLimit;
  }
  //D  ΢��
  pid->deriv = (pid->merror - pid->last_error) / dt;
  if(pid->Lowpass_EN != PID_FALSE) //��ͨ�˲�����ֹƵ��20hz
	{
		pid->deriv = pid->last_deriv + (dt / (lowpass_filter + dt)) * (pid->deriv - pid->last_deriv);	
  }

  pid->outP = pid->Kp * pid->merror;
  pid->outI = pid->Ki *pid->Integrator;
  pid->outD = pid->Kd * pid->deriv;

  pid->PID_out =  output = 	pid->outP +pid->outI +pid->outD;
  
  pid->last_error = pid->merror;
  return output;
}

void pidSetError(struct Robot_PID* pid, float err)
{
  pid->merror = err;
}

void pidSetIntegralLimit(struct Robot_PID* pid, float limit)
{
  pid->iLimit = limit;
}

void pidReset(struct Robot_PID* pid)
{
  pid->merror = 0;
  pid->last_error = 0;
  pid->Integrator = 0;
  pid->deriv = 0;
}


void pidSetTarget(struct Robot_PID* pid, float target)
{
  pid->target = target;
}

void pidSetTarget_Measure(struct Robot_PID* pid, float target, float measured)
{
  pid->target = target;
  pid->current = measured;
  pid->merror = pid->target - measured;
}

void pidSetKp(struct Robot_PID* pid, float kp)
{
  pid->Kp = kp;
}

void pidSetKi(struct Robot_PID* pid, float ki)
{
  pid->Ki = ki;
}

void pidSetKd(struct Robot_PID* pid, float kd)
{
  pid->Kd = kd;
}

void pidSetMeasured(struct Robot_PID* pid, float measured)
{
  pid->current = measured;
}

void pidSetLowPassEnable(struct Robot_PID* pid)
{
	pid->Lowpass_EN = PID_TRUE;	
}

