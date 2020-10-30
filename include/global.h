// GL/GLFW Stuff
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Stuff
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// My stuff
#include "Camera.hpp"

#ifndef GLOBAL
#define GLOBAL

extern Camera camera(glm::vec3(0.0f, 45.0f, 0.0f));

#endif
