#include <iostream>
using namespace std;

class Time
{
	int m_hour;
	int m_min;
public:
	Time(int min = 0);
	Time(int hour, int min);

	Time operator+(int min);
	friend Time operator+(Time t1, Time t2);

	Time operator-(Time t);
	friend Time operator-(Time t, int min);

	Time operator*(double k);
	friend Time operator*(double k, Time t);

	Time operator=(Time t);

	int operator>(Time t);

	void Show_Time(char* name);
	friend int Compute_Tatal_Mins(Time t);
	friend Time Make_time_Object(int min);
};

Time::Time(int min)
{
	m_hour = 0; // 초기화
	m_min = 0;

	if(min>=0 && min<=59)
	{
		m_min = min;
	}
	else if(min<0)
	{
		m_min = 0;
		m_hour = 0;
	}
	else
	{
		m_hour = min / 60;
		m_min = min % 60;
	}
}
Time::Time(int hour, int min)
{
	m_hour = 0; // 초기화
	m_min = 0;

	if(min >= 60)
	{
		m_hour = hour;
		m_hour += (min/60);
		m_min = min%60;
	}
	else if(hour < 0 || min < 0)
	{
		m_hour = 0;
		m_min = 0;
	}
	else
	{
		m_hour = hour;
		m_min = min;
	}
}

Time Time::operator+(int min)
{
	Time t;
	int sumTime = m_hour*60+m_min;
	t.m_min = sumTime + min;

	if(t.m_min>=60)
	{
		t.m_hour += (t.m_min/60);
		t.m_min = (t.m_min%60);
	}
	else
	{
	}

	return t;
}
Time operator+(Time t1, Time t2)
{
	Time t;

	int t1Sum = t1.m_hour*60 + t1.m_min;
	int t2Sum = t2.m_hour*60 + t2.m_min;
	
	t.m_min = t1Sum + t2Sum;
	
	t.m_hour = t.m_min / 60;
	t.m_min = t.m_min % 60;

	return t;
}

Time Time::operator-(Time t)
{
	Time resTime;
	resTime.m_hour=0;
	resTime.m_min=0;

	int sumTime = m_hour*60 + m_min;
	int tSumTime = t.m_hour*60 + t.m_min;
	int subTime = sumTime - tSumTime;

	if(subTime < 0)
	{
		subTime = 0;
	}
	else if(subTime >= 60)
	{
		resTime.m_hour = subTime / 60;
		resTime.m_min = subTime % 60;
	}
	else
	{
		resTime.m_min = subTime;
	}

	return resTime;
}
Time operator-(Time t, int min)
{
	Time resTime;
	resTime.m_hour = 0;
	resTime.m_min = 0;

	int tTime = t.m_hour*60 + t.m_min;

	resTime.m_min = tTime - min;

	if(resTime.m_min < 0)
	{
		resTime.m_min = 0;
	}
	else if(resTime.m_min >= 60)
	{
		resTime.m_hour = resTime.m_min / 60;
		resTime.m_min = resTime.m_min % 60;
	}
	else
	{
	}

	return resTime;
}

Time Time::operator*(double k)
{
	Time t;
	t.m_hour = 0;
	t.m_min = 0;

	double sumTime = m_hour*60 + m_min;

	sumTime *= k;

	if(sumTime >= 60)
	{
		t.m_hour = sumTime / 60;
		t.m_min = fmod(sumTime,60);
	}
	else if(sumTime < 0)
	{
		t.m_hour = 0;
		t.m_min = 0;
	}
	else
	{
		t.m_min = sumTime;
	}

	return t;
}
Time operator*(double k, Time t)
{
	Time resTime;
	resTime.m_hour=0;
	resTime.m_min=0;
	
	int sumTime = k * (t.m_hour*60 + t.m_min);

	if(sumTime >= 60)
	{
		resTime.m_hour = sumTime / 60;
		resTime.m_min = sumTime % 60;
	}
	else if(sumTime < 0)
	{
		resTime.m_hour = 0;
		resTime.m_min = 0;
	}
	else
	{
		resTime.m_min = sumTime;
	}

	return resTime;
}

Time Time::operator=(Time t)
{
	m_hour = t.m_hour;
	m_min = t.m_min;
	return *this;
}

int Time::operator>(Time t)
{
	int sumTime = (m_hour*60) + m_min;
	int t_sumTime = (t.m_hour*60) + t.m_min;

	if(sumTime > t_sumTime)
	{
		return 1;
	}
	else if(sumTime == t_sumTime)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void Time::Show_Time(char* name) // 수정
{
	if(m_hour == 0)
	{
		cout<<name<<" : "<<m_min<<"분"<<endl;
	}
	else
	{
		cout<<name<<" : "<<m_hour<<"시간 "<<m_min<<"분"<<endl;
	}
}
int Compute_Tatal_Mins(Time t)
{
	int sumTime = (t.m_hour*60) + t.m_min;

	return sumTime;
}
Time Make_time_Object(int min)
{
	Time t;

	if(min>=60)
	{
		t.m_hour = min/60;
		t.m_min = min%60;
	}
	else if(min<60)
	{
		t.m_hour = 0;
		t.m_min = min;
	}
	else if(min < 0)
	{
		t.m_hour = 0;
		t.m_min = 0;
	}

	return t;
}

void main()
{
	Time ob1(115), ob2(2,80), ob3(3,-40), result;
	
	ob1.Show_Time("ob1");
	ob2.Show_Time("ob2");
	ob3.Show_Time("ob3");

	cout<<"ob1의 total mins : "<<Compute_Tatal_Mins(ob1)<<"분"<<endl;
	cout<<"ob2의 total mins : "<<Compute_Tatal_Mins(ob2)<<"분"<<endl;

	result=ob1+ob2;
	result.Show_Time("ob1 + ob2");

	result = ob1+70;
	result.Show_Time("ob1 + 70");

	result = ob1-ob2;
	result.Show_Time("ob1 - ob2");

	result = ob2-ob1;
	result.Show_Time("ob2 - ob1");

	result = ob1-40;
	result.Show_Time("ob1 - 40");

	result = ob2*10;
	result.Show_Time("ob2 * 10");

	result = 2.75*ob2;
	result.Show_Time("2.75 * ob2");

	if(ob1>ob2 == 1)
	{
		cout<<"ob1의 시간은 ob2의 시간보다 큽니다."<<endl;
	}
	else if(ob1>ob2 == 0)
	{
		cout<<"ob1의 시간은 ob2의 시간과 같습니다."<<endl;
	}
	else
	{
		cout<<"ob1의 시간은 ob2보다 작습니다."<<endl;
	}

	result=ob1=ob2;
	result.Show_Time("ob1 = ob2");

	if(ob1>ob2 == 1)
	{
		cout<<"ob1의 시간은 ob2의 시간보다 큽니다."<<endl;
	}
	else if(ob1>ob2 == 0)
	{
		cout<<"ob1의 시간은 ob2와 같습니다."<<endl;
	}
	else
	{
		cout<<"ob1의 시간은 ob2보다 작습니다."<<endl;
	}
	
	system("PAUSE");
}