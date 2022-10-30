# "${workspaceFolder}\\tgaimage.cpp",
#                 "${workspaceFolder}\\geometry.cpp",
#                 "${workspaceFolder}\\model.cpp",
#                 "${workspaceFolder}\\camera.cpp",
#                 "${workspaceFolder}\\doji_gl.cpp",
# 				"${workspaceFolder}\\main.cpp", //指定要编译的是当前文件

SHELL=cmd.exe

objects = main.o doji_gl.o camera.o \
		model.o geometry.o tgaimage.o

main : $(objects)
	g++ $(objects) -o bin\main -g 

main.o : main.cpp tgaimage.h model.h geometry.h camera.h doji_gl.h
	g++ -c main.cpp -o main.o

doji_gl.o : doji_gl.cpp doji_gl.h
	g++ -c doji_gl.cpp -o doji.o

camera.o : geometry.h camera.h camera.cpp
	g++ -c camera.cpp -o camera.o

model.o : model.h geometry.h model.cpp
	g++ -c model.cpp -o model.o

geometry.o : geometry.h geometry.cpp
	g++ -c geometry.cpp -o geometry.o

tgaimage.o : tgaimage.cpp tgaimage.h
	g++ -c tgaimage.cpp -o tgaimage.o

clean:
	del bin\main.exe, $(objects)