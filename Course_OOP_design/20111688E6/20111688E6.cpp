#include <iostream>
using namespace std;

class CMember
{
	char *m_name;
protected:
	char m_gender;
	double m_tall;
	double m_weight;
public:
	CMember(char *name, char gender, double tall, double weight);
	void Output();
};
CMember::CMember(char *name, char gender, double tall, double weight)
{
	m_name = name;
	m_gender = gender;
	m_tall = tall;
	m_weight = weight;
}
void CMember::Output()
{
	cout.setf(ios::fixed);
	cout.precision(1);
	cout<<"= = = = [고객 기본 정보] = = = =\n";
	cout<<"이름 : "<<m_name<<endl;
	cout<<"성별 : "<<m_gender<<endl;
	cout<<"신장 : "<<m_tall<<endl;
	cout<<"체중 : "<<m_weight<<endl;
}

class CIndiHealth : public CMember
{
	double m_std_weight; // 표준 체중
	double m_condition_factor; // 비만도
public:
	CIndiHealth(char* name, char gender, double tall, double weight);
	void Calculate_Condition_Factor();
	char* Result_Condition_Factor();
	void Output();
};
CIndiHealth::CIndiHealth(char* name, char gender, double tall, double weight) : CMember(name,gender,tall,weight)
{
	m_std_weight = 0.0;
	m_condition_factor = 0.0;
}
void CIndiHealth::Calculate_Condition_Factor()
{
	if(m_gender=='M' || m_gender=='m')
	{
		m_std_weight = (CMember::m_tall - 100) * 0.9;
	}
	else if(m_gender=='F' || m_gender=='f')
	{
		m_std_weight = (CMember::m_tall - 100) * 0.85;
	}

	m_condition_factor = CMember::m_weight / m_std_weight * 100;
}
char* CIndiHealth::Result_Condition_Factor()
{
	if(m_condition_factor < 90)
	{
		return "저체중";
	}
	else if(m_condition_factor >= 91 && m_condition_factor <= 100)
	{
		return "정상 체중";
	}
	else if(m_condition_factor >= 111 && m_condition_factor <= 120)
	{
		return "과체중";
	}
	else if(m_condition_factor >= 121 && m_condition_factor <= 130)
	{
		return "경도비만";
	}
	else if(m_condition_factor >= 131 && m_condition_factor <= 150)
	{
		return "중도비만";
	}
	else if(m_condition_factor >= 150)
	{
		return "고도비만";
	}
}
void CIndiHealth::Output()
{
	CMember::Output();
	cout<<endl;

	cout<<"= = = = [고객 건강 결과] = = = =\n";
	cout.setf(ios::fixed);
	cout.precision(1);
	cout<<"당신의 비만도는 "<<m_condition_factor<<"이고 "<<Result_Condition_Factor()<<"입니다"<<endl<<endl;

	if(strcmp(Result_Condition_Factor(),"저체중") == 0)
	{
		cout<<"당신의 표준체중은 "<<m_std_weight<<"kg 이므로 "<<m_std_weight - m_weight<<"kg 증량이 필요합니다.\n"<<endl;
	}
	else if(strcmp(Result_Condition_Factor(),"정상 체중") == 0)
	{
		cout<<"당신의 표준체중은 "<<m_std_weight<<"kg 이고 현재 정상 체중입니다.\n"<<endl;
	}
	else
	{
		cout<<"당신의 표준체중은 "<<m_std_weight<<"kg 이므로 "<<m_weight - m_std_weight<<"kg 감량이 필요합니다.\n"<<endl;
	}
}

void Input_Data(char* name, char &gender, double &tall, double &weight);

void main()
{
	char selection;
	char name[30];
	char gender;
	double tall;
	double weight;
	CIndiHealth* client;

	do
	{

	Input_Data(name,gender,tall,weight);

	client = new CIndiHealth(name, gender, tall, weight); 
	client->Calculate_Condition_Factor();
	client->Output();

		cout<<"정보를 다시 입력하려면 Y나 y를, 종료하려면 N 또는 n을 입력하세요 : ";
		cin>>selection;
		while(selection !='Y' && selection !='y' && selection !='N' && selection !='n')
		{
			cout<<"오직 Y,y,N,n만 입력하시오 : ";
			cin>>selection;
		}
	}while(selection =='Y' || selection == 'y');

	system("PAUSE");
}

void Input_Data(char* name, char &gender, double &tall, double &weight)
{
	cout<<"= = = = [고객 정보 입력] = = = =\n";
	cout<<"이름 : ";
	cin>>name;

	cout<<"성별 : ";
	cin>>gender;

	while(gender !='F' && gender !='f' && gender !='M' && gender !='m')
	{
		cout<<"오직 M, m, F, f만 입력하시오 : ";
		cin>>gender;
	}

	cout<<"신장 : ";
	cin>>tall;

	cout<<"체중 : ";
	cin>>weight;

}