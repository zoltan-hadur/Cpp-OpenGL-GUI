# Cpp-OpenGL-GUI
Graphical user interface written in C++ using FreeGLUT.

Quick overview of the features:<br/>
[![youtube vid_0](https://img.youtube.com/vi/PVw1Eoe4wKE/0.jpg)](https://www.youtube.com/watch?v=PVw1Eoe4wKE)

## Getting started
### MainWindow
The main window wraps ```glutCreateWindow```, handles the keyboard/mouse input events, and limits the fps of the gui.

To create the main window with the title "MainWindow" at (100, 100) point on screen with a width of 960 pixels and a height of 540 pixels:
```
GLUI::MainWindow* main_window = new GLUI::MainWindow("MainWindow", 100, 100, 960, 540);
```

For proper operation, you must forward the events of glut to the main window:
```
glutKeyboardFunc(on_keyboard_down);
glutKeyboardUpFunc(on_keyboard_up);
glutSpecialFunc(on_special_down);
glutSpecialUpFunc(on_special_up);
glutMouseFunc(on_mouse);
glutMotionFunc(on_mouse_motion);
glutPassiveMotionFunc(on_mouse_motion_passive);

void on_keyboard_down(unsigned char key, int x, int y) {
	main_window->on_keyboard_down(key, x, y);
}
void on_keyboard_up(unsigned char key, int x, int y) {
	main_window->on_keyboard_up(key, x, y);
}
void on_special_down(int key, int x, int y) {
	main_window->on_special_down(key, x, y);
}
void on_special_up(int key, int x, int y) {
	main_window->on_special_up(key, x, y);
}
void on_mouse(int button, int state, int x, int y) {
	main_window->on_mouse(button, state, x, y);
}
void on_mouse_motion(int x, int y) {
	main_window->on_mouse_motion(x, y);
}
void on_mouse_motion_passive(int x, int y) {
	main_window->on_mouse_motion_passive(x, y);
}
```

To render the gui components:
```
glutDisplayFunc(on_display);

void on_display() {
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	main_window->render();
	glutSwapBuffers();
}
```

It does not matter how many times you call the ```render``` method, it will draw itself only as many times what it's configured fps defines.

### Window
Windows are almost behave like windows in Windows (the OS), you can move/resize/collapse them.

To create a window at (10, 10) in the parent component, with a width of 800 pixels and a height of 500 pixels:
```
GLUI::Window* window = new GLUI::Window("Window 0", 10, 10, 800, 500);
main_window->add_component(window);
```
