#include "../core/matrix.hpp"
typedef tinycv::Matrix<double> Mat;
int main()
{
    std::vector<std::vector<double> >  dataVec = { {1,2},{2,3} };
    Mat  m1(1, 2), m2(2, 2, 0.2f) , m3(dataVec);
    std::cout << "m1:\n" << m1 << std::endl;
    std::cout << "m2:\n" << m2 << std::endl;
    std::cout << "m3:\n" << m3 << std::endl;
    Mat m6 = m1;
    m1 = m2;

    std::cout << "m1:\n" << m1 << std::endl;
    std::cout << "m6:\n" << m6 << std::endl;    

    Mat m4(m3);
    Mat m5 = m2;
    std::cout << "m4:\n" << m4 << std::endl;
    std::cout << "m5:\n" << m5 << std::endl;

    m5 = m4.clone();
    m3.copyTo(m5);

    m5[0][0] = 88;
    std::cout << "m5:\n" << m5 << std::endl;
    std::cout << "m3 + m2\n" << m3 + m2 << std::endl;
    std::cout << "m1+0.1f\n" << m1 + 0.1f << std::endl;
    std::cout << "-m1\n" << -m1 << std::endl;
    //Mat x = m1&;
    Mat m7 = m1 + 1;
    Mat m8 = m1 - 1;
    Mat m9 = m1 - 1.0;
    Mat m10 = m1 / 2.0;
    Mat m11 = m1 * 2.0;
    Mat m12 = m10 - m11;
    std::cout << "m7:\n" << m7 << std::endl;
    std::cout << "m8:\n" << m8 << std::endl;
    std::cout << "m9:\n" << m9 << std::endl;
    std::cout << "m10:\n" << m10 << std::endl;
    std::cout << "m11:\n" << m11 << std::endl;
    std::cout << "m12:\n" << m12 << std::endl;
    return 0;
}