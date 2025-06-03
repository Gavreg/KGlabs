#include "Render.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>

// Библиотека для разгрузки изображений
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Внутренняя логика "движка"
#include "MyOGL.h"
extern OpenGL gl;

#include "Camera.h"
Camera camera;


void initRender()
{
	//================НАСТРОЙКА КАМЕРЫ======================
	camera.caclulateCameraPos();

	// Привязываем камеру к событиям "движка"
	gl.WheelEvent.reaction(&camera, &Camera::Zoom); //рекакия на колесико мыши
	gl.MouseMovieEvent.reaction(&camera, &Camera::MouseMovie);//рекакия на движение мыши
	gl.MouseLeaveEvent.reaction(&camera, &Camera::MouseLeave); //мышь покидает окно
	gl.MouseLdownEvent.reaction(&camera, &Camera::MouseStartDrag); //Левая кнопка мыши нажата
	gl.MouseLupEvent.reaction(&camera, &Camera::MouseStopDrag); //Левая кнопка мыши отпущена

	camera.setPosition(2, 1.5, 1.5); //начальная позиция камеры
}

void Render(double delta_time) {
    camera.SetUpCamera(); // Применяем настройки камеры. Эту функцию удалять нельзя
    gl.DrawAxes(); // Рисуем оси координат

    // Программировать тут

}
