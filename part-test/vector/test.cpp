#include <boost/numeric/ublas/vector.hpp> //零向量，不用解释
#include <boost/numeric/ublas/io.hpp>

int main () {
    using namespace boost::numeric::ublas;
    zero_vector<double> v (5);
    std::cout << v << std::endl;
}
