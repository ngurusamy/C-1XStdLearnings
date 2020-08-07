#include <iostream>

using namespace std;

void printInt(int & i) { cout << "lvalreference" << endl;}
void printInt(int && i) { cout << "rvalreference" << endl;}
//void printInt(int  i) { cout << "Normal " << endl;}

class boVector
{
	private:
		int m_size;
		double * m_arr;
	public:
		boVector(int size = 10) : m_size(size)
		{
			cout << "Ctor \n";
			m_arr = new double[m_size];
		} 

		boVector(const boVector & rhs)
		{
			cout << "Copy Ctor \n";
			m_size = rhs.m_size;
			m_arr = new double[rhs.m_size];
			for(int i = 0; i < rhs.m_size; i++)
			{
				m_arr[i] = rhs.m_arr[i];
			}
		}

		boVector(boVector && rhs)
		{
			cout << "Move Ctor \n";
			m_size = rhs.m_size;
			m_arr = rhs.m_arr;
			m_arr = nullptr;
		}

		~boVector()
		{
			delete m_arr;
		}
};


void foo(boVector v) {}
//void foo(boVector & v) {}
//void foo(boVector && v) {cout << "foo rvalref \n";}
void foo_by_ref(boVector & v) {}
boVector createBoVector() { return boVector();}

int main()
{
	int i = 10;
	//printInt(i);

	boVector resuable = createBoVector();

	//foo(createBoVector());
	foo(resuable);
	cout << "Calling foo_by_ref \n";
	//foo_by_ref(resuable);
	foo(std::move(resuable));
	//foo(createBoVector());
	return 0;
}
