#include <string>
#include <vector>
#include <utility>

#include <opencv/cv.h>
using namespace std;
using namespace cv;

class LBPFeatureExtractor{
public:
   LBPFeatureExtractor(const vector<int> scales, const int patchSize, const int numCellX, const int numCellY, const bool uniform);
   void setParameters(const vector<int> scales, const int patchSize, const int numCellX, const int numCellY, const bool uniform);
   vector<int> extractAt(const Mat &image, const vector< /*pair<double, double>*/cv::Point2d >points/*, int *outputFeature*/);
   void lbpImage(const Mat &image, Mat &lbpImage);
private:
   int lbpCode (unsigned char seq[9]);
   int* lbpHist(Mat &img, int* lbpHist);
   bool uniform;
   vector<int> scale;
   int patchSize;
   int numCellX;
   int numCellY;
   int mapping[256] ;
};

