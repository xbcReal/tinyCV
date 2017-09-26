#include "../core/matrix.hpp"
typedef tinycv::Matrix<float> Mat;
int main()
{
	std::vector<std::vector<float> >  dataVec({ {1,2},{2,3} });
	Mat  m1(1, 2), m2(2, 2, 0.2f) , m3(dataVec);
	std::cout << "m1:\n" << m1 << std::endl;
	std::cout << "m2:\n" << m2 << std::endl;
	std::cout << "m3:\n" << m3 << std::endl;

	m1 = m2;
	std::cout << "m1:\n" << m1 << std::endl;

	Mat m4(m3);
	Mat m5 = m2;
	std::cout << "m4:\n" << m4 << std::endl;
	std::cout << "m5:\n" << m5 << std::endl;

	m5 = m4.clone();
	m3.copyTo(m5);

	m5[0][0] = 88;
	std::cout << "m5:\n" << m5 << std::endl;


	std::cout << m3 + m2 << std::endl;
	std::cout << m1 + 0.1f << std::endl;
	std::cout << -m1 << std::endl;

	return 0;
}