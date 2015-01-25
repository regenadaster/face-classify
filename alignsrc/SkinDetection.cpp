#include "SkinDetection.h"

//这方法对光照太敏感了
double skin_rgb_cpp(Mat source,Mat& _dst)
{
	assert(source.channels()==3/*&&_dst.channels()==3*/);

	static const int R=2;
	static const int G=1;
	static const int B=0;

	double total_pixel_count = 0, skin_pixel_count = 0;
	Mat dst(source.rows,source.cols,CV_8UC3,Scalar::all(0));

	MatIterator_<Vec3b> it, end, dst_iterator;
	dst_iterator = dst.begin<Vec3b>();
	for( it = source.begin<Vec3b>(), end = source.end<Vec3b>(); it != end; ++it ,++dst_iterator)  
	{ 
		total_pixel_count++;
		if (((*it)[R]>80 && (*it)[G]>35 && (*it)[B]>15 &&
				(*it)[R]-(*it)[B]>15 && (*it)[R]-(*it)[G]>15/*&&
				!(prgb[R]>170&&prgb[G]>170&&prgb[B]>170)*/)||//uniform illumination 
				((*it)[R]>200 && (*it)[G]>210 && (*it)[B]>170 &&
				abs((*it)[R]-(*it)[B])<=15 && (*it)[R]>(*it)[B]&& (*it)[G]>(*it)[B])//lateral illumination
				)
		//if (((*it)[R]>95 && (*it)[G]>40 && (*it)[B]>20 &&
		//		(*it)[R]-(*it)[B]>15 && (*it)[R]-(*it)[G]>15/*&&
		//		!(prgb[R]>170&&prgb[G]>170&&prgb[B]>170)*/)||//uniform illumination 
		//		((*it)[R]>200 && (*it)[G]>210 && (*it)[B]>170 &&
		//		abs((*it)[R]-(*it)[B])<=15 && (*it)[R]>(*it)[B]&& (*it)[G]>(*it)[B])//lateral illumination
		//		)
		{
			skin_pixel_count++;
			(*dst_iterator)[B] = (*it)[B];  
			(*dst_iterator)[G] = (*it)[G];  
			(*dst_iterator)[R] = (*it)[R];
		}
	}
	dst.copyTo(_dst);
	return skin_pixel_count/total_pixel_count;
}