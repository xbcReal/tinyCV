#include "image.hpp"

using namespace tinycv;

int main()
{
    Image<float> src(5,5,3); 
   // std::cout<<src.at(0,0)<<std::endl;

    std::vector<Matrix<float>> _dvec;
    for(int i=0;i<3;i++)
    {
        _dvec.push_back(Matrix<float>(3,3,0.1f));
    }
    for(int i=0;i<3;i++)
    {
        std::cout<<_dvec[i]<<std::endl;
    }


    return 0;
}