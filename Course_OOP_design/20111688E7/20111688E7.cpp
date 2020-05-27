#include <iostream>
#include <math.h>

using namespace std;

class CPoint
{
protected:
	double m_x,m_y;
public:
	CPoint(double x, double y)
	{
		m_x = x;
		m_y = y;
	}
	virtual double ComputeUnit()=0;
	virtual void ShowPoint()
	{
		cout.setf(ios::fixed);
		cout.precision(1);
		cout<<"x = "<<m_x<<" y = "<<m_y;
	}
};

class CLengthPoint : public CPoint
{
	double m_z;
public:
	CLengthPoint(double x, double y, double z) : CPoint(x,y)
	{
		m_z = z;
	}
	double ComputeUnit()
	{
		return sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z) );
	}
	void ShowPoint()
	{
		cout.setf(ios::fixed);
		cout.precision(1);

		CPoint::ShowPoint();
		cout<<" z : "<<m_z;
	}
};

class CVolumePoint : public CPoint
{
protected:
	double m_z;
public:
	CVolumePoint(double x, double y, double z) : CPoint(x,y)
	{
		m_z = z;
	}
	double ComputeUnit()
	{
		return m_x * m_y * m_z;
	}
	void ShowPoint()
	{
		cout.setf(ios::fixed);
		cout.precision(1);

		CPoint::ShowPoint();
		cout<<" z : "<<m_z;
	}
};

class CAreaPoint : public CVolumePoint
{
public:
	CAreaPoint(double x, double y, double z) : CVolumePoint(x,y,z)
	{
	}
	double ComputeUnit()
	{
		return 2.0 * ( (m_x * m_y) + (m_y * m_z) + (m_x * m_z) );
	}
};

int main()
{
	CPoint* p;
	CLengthPoint lp(5.2,12.5,3.3);
	CVolumePoint vp(10.6,4.2,5.7);
	CAreaPoint ap(5.3,6.9,4.1);

	p = &lp;
	cout<<"°´Ã¼ lengthPtÀÇ ÁÂÇ¥ : <";
	p->ShowPoint();
	cout<<">"<<endl;
	cout<<"°´Ã¼ lengthPtÀÇ ¿øÁ¡À¸·ÎºÎÅÍÀÇ °Å¸® : "<<p->ComputeUnit()<<endl;

	p = &vp;
	cout<<"°´Ã¼ lengthPtÀÇ ÁÂÇ¥ : <";
	p->ShowPoint();
	cout<<">"<<endl;
	cout<<"°´Ã¼ volumePtÀÇ À°¸éÃ¼ ºÎÇÇ : "<<p->ComputeUnit()<<endl;

	p = &ap;
	cout<<"°´Ã¼ areaaPtÀÇ ÁÂÇ¥ : <";
	p->ShowPoint();
	cout<<">"<<endl;
	cout<<"°´Ã¼ areaPtÀÇ À°¸éÃ¼ °Ñ³ÐÀÌ : "<<p->ComputeUnit()<<endl;

	system("PAUSE");
	return 0;
}