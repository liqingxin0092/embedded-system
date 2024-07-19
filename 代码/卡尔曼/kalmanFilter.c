#include"kalmanFilter.h"
//���忨�����ṹ�岢��ʼ������
KFP KFP_Init;
void kalman_Init()
{
    KFP_Init.Kg = 0;//���������� ��ʼ��ֵΪ0
    KFP_Init.Last_Out = 0;//�������˲����ϴ���� ��ʼ��ֵΪ0
    KFP_Init.Last_P = 0;//�ϴι���Э���� ��ʼ��ֵΪ0
    KFP_Init.Now_Out = 0;//�������˲���������� ��ʼ��ֵΪ0
    KFP_Init.Now_P = 0;//��ǰ����Э���� ��ʼ��ֵΪ0
    KFP_Init.Out_Mid = 0;//�������˲���״̬Ԥ��ֵ ��ʼ��ֵΪ0
    KFP_Init.P_Mid = 0;//�������˲���Э����Ԥ��ֵ ��ʼ��ֵΪ0
    KFP_Init.Q = 0.001;//��ʼ��ֵΪ0.001,��������Э����,Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
    KFP_Init.R = 0.534;//��ʼ��ֵΪ0.543,��������Э����,R���󣬶�̬��Ӧ�����������ȶ��Ա��
}
/**
 *�������˲���
 *@param KFP *KFP �������ṹ�����
 *@float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float kalmanFilter(KFP* KFP, float input)
{
    //�����ϴε����Ź���ֵ��Ԥ��Kʱ�̵�״ֵ̬(1)------------�޿�������
    KFP->Out_Mid= KFP->Last_Out
    //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����(2)
    KFP->P_Mid = KFP->Last_P + KFP->Q;
    //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���(3)
    KFP->Kg = KFP->P_Mid / (KFP->P_Mid + KFP->R);
    //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��(4)
    KFP->Now_Out = KFP->Out_Mid + KFP->Kg * (input - KFP->Out_Mid);
    //����Э�����(5)
    KFP->Now_P = (1 - KFP->Kg) * KFP->P_Mid;
    //���ε�ϵͳЭ�����KFP->LastP Ϊ��һ������׼��
    KFP->Last_P = KFP->Now_P;
    //���ε�����ֵ����KFP->Last_Out Ϊ��һ������׼��
    KFP->Last_Out = KFP->Now_Out;
    return KFP->Now_Out;
}

/**
 *���ÿ������˲��� ʵ��
 */
float value;
int kalman_value = 0;
kalman_value = kalmanFilter(&KFP_Init, value);
