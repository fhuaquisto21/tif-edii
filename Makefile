build_linux:
	g++ ./src/main.cpp -I /usr/include/opencv2 -L /usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -o ./Main.out

build_color:
	g++ ./src/color_img.cpp -I /usr/include/opencv2 -L /usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -o ./Main.out

build_ocu:
	g++ ./src/ocultar.cpp -I /usr/include/opencv2 -L /usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -o ./Ocultar.out

build_rev:
	g++ ./src/revelar.cpp -I /usr/include/opencv2 -L /usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -o ./Revelar.out

