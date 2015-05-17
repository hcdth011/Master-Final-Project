#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/flann.hpp>
#include <fstream>

using namespace cv;
using namespace std;

vector<string> readFile(string fname) {
    vector<string> vec;

    ifstream ifs(fname.c_str());
    string line;
    while (true) {
        ifs >> line;
        vec.push_back(line);
        if (ifs.eof()) {
            break;
        }
    }
    return vec;
}

typedef cvflann::L2<float> Distance;

int main(int argc, const char * argv[]) {
    vector<string> images = readFile(string(argv[1]));

    Ptr<FeatureDetector> detector = xfeatures2d::SURF::create();
    Ptr<DescriptorExtractor> extractor = xfeatures2d::SURF::create();

    double t_d = (double)getTickCount();
    Mat descriptors;
    int numDesc = 0;
    for (size_t i=0; i<(size_t)images.size(); i++) {
        Mat image = imread(images[i], IMREAD_GRAYSCALE);
        if (image.empty()) {
            continue;
        }
        vector<KeyPoint> kp;
        Mat desc;
        detector->detect(image, kp);
        extractor->compute(image, kp, desc);
        if (desc.empty()) {
            continue;
        }
        descriptors.push_back(desc);
        numDesc += desc.rows;
        if (i > 0 && i % 1000 == 0) {
        	printf("LOADED_IMAGES=%lu, NUM_DESC=%d\n", i, numDesc);
        }
    }
    cout << "TIME_EXTRACT_FEATURES = " << ((double)getTickCount() - t_d)*1000/getTickFrequency() << endl;

    t_d = (double)getTickCount();

    int k = 3000;
    cvflann::Matrix<float> features((float*)descriptors.data, descriptors.rows, descriptors.cols);
    cvflann::Matrix<float> centers(new float[k*descriptors.cols], k, descriptors.cols);
    cvflann::KMeansIndexParams indexParams;
    cvflann::hierarchicalClustering< Distance >(features, centers, indexParams);

    cout << "TIME_CLUSTERING = " << ((double)getTickCount() - t_d)*1000/getTickFrequency() << endl;

    return 0;
}
