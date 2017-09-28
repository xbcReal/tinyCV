#include "../core/image.hpp"
#include "../core/im_io.hpp"
using namespace tinycv;

int main()
{
    
    //Image src(200,200,3,128);
    //imsave(src,"out.jpg");

     std::vector<Matrix<uchar> > _dvec;
     for (int i = 0; i<255; i++)
     {
        Image src1(4000, 4000, 3, 0);
        Image src2(4000, 4000, 3, 0);
        Scalar  s(0, i, i);
        Scalar  ss(0,2*i,2*i);

        src1.at(1, 0) = s;
        src1.at(1, 0) = ss;
        //std::cout << s[3] <<std::endl;
        //src2.at(1, 1) = src1.at(1, 0);
        Point_ pt(10,5);
        Point_ pt1(pt);
        Point_ pt2;
        pt2 = pt;

        //std::cout << pt.x() << std::ends << pt.y() << std::endl;
        std::cout<< pt2[0] << pt2[1] << std::endl;

        //std::cout << src2.at(1, 1) << std::endl;
        //_dvec.push_back(Matrix<uchar>(10000, 30000, (float)i));
     }
    return 0;
}
