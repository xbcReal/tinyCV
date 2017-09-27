#include "../core/image.hpp"
#include "../core/im_io.hpp"
using namespace tinycv;

int main()
{
    
	Image src(200,200,3,128);
	imsave(src,"out.jpg");

	// std::vector<Matrix<uchar> > _dvec;
	// for (int i = 0; i<255; i++)
	// {
	// 	Image src1(4000, 4000, 3, 0);
	// 	Image src2(4000, 4000, 3, 0);
	// 	Scalar  s(0, i, i);

	// 	src1.at(1, 0) = s;
	// 	src2.at(1, 1) = src1.at(1, 0);

	// 	std::cout << src2.at(1, 1) << std::endl;
	// 	//_dvec.push_back(Matrix<uchar>(10000, 30000, (float)i));
	// }
    return 0;
}