#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL\freeglut.h>
#include <GL\glext.h>
#include <string>
#include "Component.h"

namespace GLUI {

	// A top level window, created by glut
	class MainWindow : public Component {
	private:
		PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;				// OpenGL function pointers that needs to be loaded at runtime
		PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
		PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
		PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;

		int id;															// Window id created by glut

		GLuint tex_id;													// Texture id (the gui renders to this texture)
		GLuint rbo_id;													// Render buffer object id
		GLuint fbo_id;													// Frame buffer object id

		Event event;													// The last event
		int last_x;														// For dx
		int last_y;														// For dy

		Stopwatch watch;												// For controlling the fps
		float wait_time;												// The time to wait until the next frame can be rendered
		float max_fps;													// The maximum fps
	protected:
		virtual float get_width() override;
		virtual float get_height() override;
	public:
		// Title, position, size
		MainWindow(std::string title = "MainWindow", float x = 100, float y = 100, float width = 960, float height = 540);
		// Destroys the window
		void destroy();
		// Sets the max fps (min 30 or 0=unlimited)
		void set_max_fps(float max_fps);
		// Gets the max fps
		float get_max_fps();
		// Event handlers
		void on_keyboard_down(unsigned char key, int x, int y);
		void on_keyboard_up(unsigned char key, int x, int y);
		void on_special_down(int key, int x, int y);
		void on_special_up(int key, int x, int y);
		void on_mouse(int button, int state, int x, int y);
		void on_mouse_motion(int x, int y);
		void on_mouse_motion_passive(int x, int y);
		// Renders the gui
		virtual void render() override;
		// Handles the events
		virtual void event_handler(Event& e) override;
	};

	float MainWindow::get_width() {
		return glutGet(GLUT_WINDOW_WIDTH);
	}

	float MainWindow::get_height() {
		return glutGet(GLUT_WINDOW_HEIGHT);
	}

	// Title, position, size
	MainWindow::MainWindow(std::string title, float x, float y, float width, float height) : Component(0, 0, 0, 0, 0) {
		glutInitWindowSize(width, height);																						// Set size
		glutInitWindowPosition(x, y);																							// Set position
		this->id = glutCreateWindow(title.c_str());																				// Create the system window


		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");									// Load the OpenGL functions
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");


		glGenTextures(1, &tex_id);																								// Create the texture where the gui will be rendering
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->get_width(), this->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenRenderbuffers(1, &rbo_id);																							// Create a renderbuffer object to store depth info
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		glGenFramebuffers(1, &fbo_id);																							// Create a framebuffer object
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

																																// Attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER,																					// 1. fbo target: GL_FRAMEBUFFER 
							   GL_COLOR_ATTACHMENT0,																			// 2. attachment point
							   GL_TEXTURE_2D,																					// 3. tex target: GL_TEXTURE_2D
							   tex_id,																							// 4. tex ID
							   0);																								// 5. mipmap level: 0(base)

																																// Attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,																				// 1. fbo target: GL_FRAMEBUFFER
								  GL_DEPTH_ATTACHMENT,																			// 2. attachment point
								  GL_RENDERBUFFER,																				// 3. rbo target: GL_RENDERBUFFER
								  rbo_id);																						// 4. rbo ID


		glBindFramebuffer(GL_FRAMEBUFFER, 0);																					// Switch back to window-system-provided framebuffer

		this->event = Event();
		this->last_x = 0;
		this->last_y = 0;

		this->watch = Stopwatch();
		this->wait_time = 0;
		this->max_fps = 60;

		this->watch.start();
	}

	void MainWindow::destroy() {
		throw "Unimplemented method!";
	}

	// Sets the max fps (min 30 or 0=unlimited)
	void MainWindow::set_max_fps(float max_fps) {
		this->max_fps = max_fps;
	}

	// Gets the max fps
	float MainWindow::get_max_fps() {
		return this->max_fps;
	}

	void MainWindow::on_keyboard_down(unsigned char key, int x, int y) {
		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.keys_down[key] = true;
		this->event.key_code = key;
		this->event.key_pressed = true;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = false;
		this->event.x = x;
		this->event.y = y;
		this->event_handler(event);
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = false;
	}

	void MainWindow::on_keyboard_up(unsigned char key, int x, int y) {
		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.keys_down[key] = false;
		this->event.key_code = key;
		this->event.key_pressed = false;
		this->event.key_released = true;
		this->event.special_pressed = false;
		this->event.special_released = false;
		this->event.x = x;
		this->event.y = y;
		this->event_handler(event);
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = false;
	}

	void MainWindow::on_special_down(int key, int x, int y) {
		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.special_key_code = key;
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = true;
		this->event.special_released = false;
		this->event.x = x;
		this->event.y = y;
		this->event_handler(event);
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = false;
	}

	void MainWindow::on_special_up(int key, int x, int y) {
		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.special_key_code = key;
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = true;
		this->event.x = x;
		this->event.y = y;
		this->event_handler(event);
		this->event.key_pressed = false;
		this->event.key_released = false;
		this->event.special_pressed = false;
		this->event.special_released = false;
	}

	void MainWindow::on_mouse(int button, int state, int x, int y) {
		this->last_x = x;
		this->last_y = y;

		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.mouse_left = (button == GLUT_LEFT_BUTTON);
		this->event.mouse_middle = (button == GLUT_MIDDLE_BUTTON);
		this->event.mouse_right = (button == GLUT_RIGHT_BUTTON);
		this->event.mouse_scroll_up = (button == 3);
		this->event.mouse_scroll_down = (button == 4);
		this->event.mouse_pressed = (state == GLUT_DOWN);
		this->event.mouse_released = (state == GLUT_UP);
		this->event.mouse_left_down = (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON);
		this->event.mouse_middle_down = (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON);
		this->event.mouse_right_down = (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON);
		this->event.x = x;
		this->event.y = y;
		this->event_handler(event);
		this->event.mouse_pressed = false;
		this->event.mouse_released = false;
	}

	void MainWindow::on_mouse_motion(int x, int y) {
		int dx = x - last_x;
		int dy = y - last_y;
		this->last_x = x;
		this->last_y = y;

		this->event.active_alt = (glutGetModifiers() & GLUT_ACTIVE_ALT);
		this->event.active_ctrl = (glutGetModifiers() & GLUT_ACTIVE_CTRL);
		this->event.active_shift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
		this->event.mouse_moved = true;
		this->event.x = x;
		this->event.y = y;
		this->event.dx = dx;
		this->event.dy = dy;
		this->event_handler(event);
		this->event.dx = 0;
		this->event.dy = 0;
		this->event.mouse_moved = false;
	}

	void MainWindow::on_mouse_motion_passive(int x, int y) {
		this->on_mouse_motion(x, y);
	}

	void MainWindow::render() {
		float time_elapsed = this->watch.get_elapsed_time();
		bool allowed = false;

		if (this->max_fps == 0) {																												// Maxmimum fps is unlimited
			allowed = true;
		} else if (time_elapsed >= this->wait_time) {																							// Control the fps
			this->max_fps = std::max(this->max_fps, 30.0f);																						// For practical reasons
			this->wait_time = std::max((1.0f - (time_elapsed * this->max_fps / 1 - this->max_fps * this->wait_time)) / this->max_fps, 0.0f);	// Calculate the dt between frames
			this->watch.reset();
			allowed = true;
		}

		if (allowed) {																															// Only redraw the gui when needed (according to the max_fps)
			glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();																		// Save current projection matrix
			glOrtho(0.0f, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);											// Transform it to able to draw in pixel coordinates
			glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();																		// Save current modelview matrix
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);																	// Enable transparency

			glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_id);																					// Set rendering destination to FBO
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);																					// Clear buffers

			glBindTexture(GL_TEXTURE_2D, this->tex_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->get_width(), this->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);					// Change texture size in case if window was resized
			glBindTexture(GL_TEXTURE_2D, 0);

			glScissor(0, 0, this->get_width(), this->get_height());																				// Don't allow to draw anything outside of the window
			glEnable(GL_SCISSOR_TEST);

			this->event_handler(this->event);
			Component::render();

			glDisable(GL_SCISSOR_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_BLEND);
			glPopMatrix(); glMatrixMode(GL_PROJECTION);
			glPopMatrix(); glMatrixMode(GL_MODELVIEW);
		}

		// Drawing a simple quad is much faster than drawing the whole gui all the time
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->tex_id);
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-1, -1);
		glTexCoord2f(1, 0); glVertex2f(1, -1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(0, 1); glVertex2f(-1, 1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}

	// Handles input events (keyboard, mouse)
	void MainWindow::event_handler(Event& e) {
		if (e.mouse_left && e.mouse_pressed) {
			this->bring_front(e);
		}
		Component::event_handler(e);
	}

}