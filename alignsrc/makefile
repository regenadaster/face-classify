preProcess:multi_thread.o align.o linreg.o commonUtil.o Illumination.o liblbp.o flandmark_detector.o
	g++ -g `pkg-config --libs opencv` -o preProcess multi_thread.o align.o commonUtil.o Illumination.o liblbp.o linreg.o flandmark_detector.o -I /usr/local/include/opencv -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_ts -lopencv_video -lopencv_objdetect -lopencv_ml -lpthread
multi_thread.o:multi_thread.cpp align.h commonUtil.h flandmark_detector.h
	g++ -g `pkg-config --cflags opencv` -c multi_thread.cpp  -I /usr/local/include/opencv -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_ts -lopencv_video -lopencv_objdetect -lopencv_ml -lpthread
align.o:align.cpp linreg.h align.h Illumination.h flandmark_detector.h
	g++ -g `pkg-config --cflags opencv` -c align.cpp -I /usr/local/include/opencv -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_ts -lopencv_video -lopencv_objdetect -lopencv_ml -lpthread
Illumination.o:Illumination.cpp Illumination.h
	g++ -g `pkg-config --cflags opencv` -c Illumination.cpp  -I /usr/local/include/opencv -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_ts -lopencv_video -lopencv_objdetect -lopencv_ml -lpthread
flandmark_detector.o:flandmark_detector.cpp flandmark_detector.h liblbp.h
	g++ -g -c flandmark_detector.cpp
liblbp.o:liblbp.cpp liblbp.h msvc-compat.h
	g++ -g -c liblbp.cpp
linreg.o:linreg.cpp linreg.h
	g++ -g -c linreg.cpp
commonUtil.o:commonUtil.cpp commonUtil.h
	g++ -g -c commonUtil.cpp
clean: 
	rm commonUtil.o linreg.o Illumination.o align.o multi_thread.o liblbp.o flandmark_detector.o
