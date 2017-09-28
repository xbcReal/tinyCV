#include "../core/image.hpp"
#include "../core/im_io.hpp"
using namespace tinycv;

int main()
{

//Image src(200,200,3,128);
//imsave(src,"out.jpg");

    std::vector<Matrix<uchar> > _dvec;
    
    Image src1(3, 3, 3, 22);
    Image src2(4000, 4000, 3, 0);
    Scalar  s(4, 4, 4);
    //Scalar  ss(0,2*i,2*i);
    Image roi = src1.roi(0,0,3,3);
    roi.at(0,0) = s;
    for(int i = 0;i < roi.rows();i++){
        for(int j = 0;j < roi.cols();j++){
            std::cout << roi.at(i,j) << " " << std::ends;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;    
    for(int i = 0;i < roi.rows();i++){
        for(int j = 0;j < roi.cols();j++){
            std::cout << src1.at(i,j) << " " << std::ends;
        }
        std::cout << std::endl;        
    }
    //std::cout <<roi.at(0,0) <<std::endl;
    //std::cout << src1.at(10,10) <<std::endl;
    //src1.at(1, 0) = s;
    //src1.at(1, 0) = ss;
    //std::cout << s[3] <<std::endl;
    //src2.at(1, 1) = src1.at(1, 0);
    Point_ pt(10,5);
    Point_ pt1(pt);
    Point_ pt2;
    pt2 = pt;

    //std::cout << pt.x() << std::ends << pt.y() << std::endl;
    //std::cout<< pt2[0] << pt2[1] << std::endl;

    //std::cout << src2.at(1, 1) << std::endl;
    //_dvec.push_back(Matrix<uchar>(10000, 30000, (float)i));
    
return 0;
}
