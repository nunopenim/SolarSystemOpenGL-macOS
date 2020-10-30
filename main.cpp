// General includes
#include <iostream>
#include <vector>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL

// My stuff (mostly)
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h" // not mine
#include "include/Camera.hpp"
#include "include/Sphere.hpp"
#include "include/Ring.hpp"
#include "include/Shader.hh" // not made by me
#include "include/global.h"

// Some functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
float timeToSeconds(float days, float hours, float minutes, float seconds);

// Timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Textures
const char* sunPath = "textures/sun.jpg";
const char* mercuryPath = "textures/mercury.jpg";
const char* venusPath = "textures/venus.jpg";
const char* earthPath = "textures/earth.jpg";
const char* moonPath = "textures/moon.jpg";
const char* marsPath = "textures/mars.jpg";
const char* jupiterPath = "textures/jupiter.jpg";
const char* saturnPath = "textures/saturn.jpg";
const char* uranusPath = "textures/uranus.jpg";
const char* neptunePath = "textures/neptune.jpg";
const char* saturnRingPath = "textures/ring.jpg";
const char* boxPath = "textures/box.jpg"; // SkyBox limitation, uses a milky way texture

// Shaders
const char* vertexPath = "shaders/vertex.glsl";
const char* fragPath = "shaders/fragment.glsl";
const char* lightFragPath = "shaders/illuminated_fragment.glsl";

// Misc
float scale = 1.0f;
float boxRadius = 700.0f * scale;
float skyLimit = sqrt((boxRadius * boxRadius) / 2) - 5;

int main()
{
	//************************** CONTEXT **********************************
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);  // anti-aliasing factor
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //resizable window = false
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy;
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // use modern openGL



	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System - Nuno Penim", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewInit();
	//************************************************************
	std::vector<Sphere> spheres;

	std::vector<Ring> rings; //planet rings (saturn)

	//sun goes counter clock wise and so does the orbit of all planets
	//planet orbit speed by order
	/*
	https://www.youtube.com/watch?v=Pn_kSjcfm44 km/s
	47.9 km/seg
	35.0
	29.8
	24.1    lua 24hrs
	13.1
	9.6
	6.8
	5.5

	planet rotation speed by order
	https://www.youtube.com/watch?v=oA4TjBhT9Os&list=PLX2gX-ftPVXXubDMNvWwPfrzd5vXtJkpu&index=18 days
	http://www.grandpapencil.net/projects/plansped.htm km/h
	58.65 days
	243 days
	24hrs
	24hrs 37min
	9hrs 50 min
	10hrs 14 min
	17.24hrs
	15.97hrs
	days and years of each planet
	https://www.universetoday.com/37507/years-of-the-planets/
	planet size
	https://www.education.com/science-fair/article/scale-model-planets-solar-system/

	could help
	https://www.exploratorium.edu/ronh/solar_system/



	important
	size
	https://www.education.com/science-fair/article/scale-model-planets-solar-system/
	years and days
	https://www.universetoday.com/37507/years-of-the-planets/
	distance from the sun
	https://phys.org/news/2014-04-planets-sun.html
	*/

	float PI = M_PI;

	

	float orbitSpeed = 100000.0f;

	float gasSpecial = 10.0f;

	//scale reduced to distanceSunScale * daySpeed times
	float distanceSunScale = 10.0f;
	float daySpeed = 1000000.0f;
	
	float sunRadius = 20.0f * scale;
	float mercuryRadius = 0.4f * scale;
	float venusRadius = 0.9f * scale;
	float earthRadius = 1.0f * scale;
	float moonRadius = 0.13f * scale;
	float marsRadius = 0.533f * scale;
	float jupiterRadius = 11.2f * scale;
	float saturnRadius = 9.46f * scale;
	float uranusRadius = 4.06f * scale;
	float neptuneRadius = 3.88f * scale;

	Sphere sun(sunRadius, 50, 50, sunPath);
	sun.useShader(vertexPath, fragPath); //The sun uses the old shader, with 100% lightning

	Sphere mercury(mercuryRadius, 50, 50, mercuryPath);
	mercury.useShader(vertexPath, lightFragPath);

	Sphere venus(venusRadius, 50, 50, venusPath);
	venus.useShader(vertexPath, lightFragPath);

	Sphere earth(earthRadius, 50, 50, earthPath);
	earth.useShader(vertexPath, lightFragPath);

	Sphere moon(moonRadius, 50, 50, moonPath);
	moon.useShader(vertexPath, lightFragPath);
	
	Sphere mars(marsRadius, 50, 50, marsPath);
	mars.useShader(vertexPath, lightFragPath);

	Sphere jupiter(jupiterRadius, 50, 50, jupiterPath);
	jupiter.useShader(vertexPath, lightFragPath);

	Sphere saturn(saturnRadius, 50, 50, saturnPath);
	saturn.useShader(vertexPath, lightFragPath);

	Sphere uranus(uranusRadius, 50, 50, uranusPath);
	uranus.useShader(vertexPath, lightFragPath);

	Sphere neptune(neptuneRadius, 50, 50, neptunePath);
	neptune.useShader(vertexPath, lightFragPath);

	Sphere box(boxRadius, 50, 50, boxPath);
	box.useShader(vertexPath, fragPath); // The skybox uses the older shader with 100% light
	box.shift(0.0f, 0.0f, 0.0f);
	box.rotate(0.0f);

	sun.shift(0.0f, 0.0f, 0.0f);
	sun.rotate(0.0f);
	sun.rotationSpeed(360 / timeToSeconds(27.0f, 0, 0, 0) * orbitSpeed);

	mercury.shift(0.0f, 0.0f, 57.0f / distanceSunScale + sunRadius + mercuryRadius);
	mercury.rotate(0.0);
	mercury.orbitSpeed(360 / timeToSeconds(88.969f, 0, 0, 0) * orbitSpeed);
	mercury.rotationSpeed(360 / timeToSeconds(58.0f, 15.5f, 0, 0) * orbitSpeed);

	venus.shift(0.0f, 0.0f, 108.0f / distanceSunScale + sunRadius + venusRadius);
	venus.rotate(177.3f);
	venus.orbitSpeed(360 / timeToSeconds(224.7f, 0, 0, 0) * orbitSpeed);
	venus.rotationSpeed(360 / timeToSeconds(243.0f, 0, 26.0f, 0) * orbitSpeed);

	earth.shift(0.0f, 0.0f, 150.0f / distanceSunScale + sunRadius + earthRadius);
	earth.rotate(23.4f);
	earth.orbitSpeed(360 / timeToSeconds(365.25f, 0, 26.0f, 0) * orbitSpeed);
	earth.rotationSpeed(360 / timeToSeconds(0.0f, 24.0f, 0, 0) * orbitSpeed);

	moon.shift(0.0f, 0.0f, 150.0f / distanceSunScale + sunRadius + earthRadius);
	moon.rotationSpeed(360 / timeToSeconds(27.0f, 0, 0, 0) * orbitSpeed);
	moon.rotate(1.5f);
	moon.orbitSpeed(360 / timeToSeconds(365.25f, 0, 26.0f, 0) * orbitSpeed);
	moon.secondRotate(0.0f, 0.0f, 1.0f + earthRadius + moonRadius);
	moon.secondOrbitSpeed(360 / timeToSeconds(27.0f, 0, 0, 0) * orbitSpeed);

	mars.shift(0.0f, 0.0f, 228.0f / distanceSunScale + sunRadius + marsRadius);
	mars.rotate(25.2f);
	mars.orbitSpeed(360 / timeToSeconds(687.0f, 0, 0, 0) * orbitSpeed);
	mars.rotationSpeed(360 / timeToSeconds(0.0f, 24.0f, 33.0f, 35.244f) * orbitSpeed);

	jupiter.shift(0.0f, 0.0f, 779.0f / distanceSunScale + sunRadius + jupiterRadius);
	jupiter.rotate(3.1f);
	jupiter.orbitSpeed(360 / timeToSeconds(4332.59f, 0, 0, 0) * orbitSpeed * gasSpecial);
	jupiter.rotationSpeed(360 / timeToSeconds(0.0f, 9.0f, 55.0f, 30.0f) * orbitSpeed);

	saturn.shift(0.0f, 0.0f, 1430.0f / distanceSunScale + sunRadius + saturnRadius);
	saturn.rotate(-26.7f);
	saturn.orbitSpeed(360 / timeToSeconds(10759.0f, 0, 0, 0) * orbitSpeed * gasSpecial);
	saturn.rotationSpeed(360 / timeToSeconds(0.0f, 10.0f, 33.0f, 0.0f) * orbitSpeed);

	uranus.shift(0.0f, 0.0f, 2880.0f / distanceSunScale + sunRadius + uranusRadius);
	uranus.rotate(97.8f);
	uranus.orbitSpeed(360 / timeToSeconds(30688.5f, 0, 0, 0) * orbitSpeed * gasSpecial);
	uranus.rotationSpeed(360 / timeToSeconds(0.0f, 17.0f, 14.0f, 24.0f) * orbitSpeed);

	neptune.shift(0.0f, 0.0f, 4500.0f / distanceSunScale + sunRadius + neptuneRadius);
	neptune.rotate(28.3f);
	neptune.orbitSpeed(360 / timeToSeconds(60182.0f, 0.0f, 0.0f, 0.0f) * orbitSpeed * gasSpecial);
	neptune.rotationSpeed(360 / timeToSeconds(0.0f, 16.0f, 6.0f, 36.0f) * orbitSpeed);

	spheres.push_back(sun);
	spheres.push_back(mercury);
	spheres.push_back(venus);
	spheres.push_back(earth);
	spheres.push_back(moon);
	spheres.push_back(mars);
	spheres.push_back(jupiter);
	spheres.push_back(saturn);
	spheres.push_back(uranus);
	spheres.push_back(neptune);
	spheres.push_back(box);

	// Saturn (and possibly uranus too) rings
	Ring satRings(saturnRadius + 9.8f, 50, 50, saturnRingPath); //plus 9.8 units, according to my math.
	satRings.useShader(vertexPath, lightFragPath);

	satRings.shift(0.0f, 0.0f, 1430.0f / distanceSunScale + sunRadius + saturnRadius);
	satRings.rotate(-26.7f - 60.0f);
	satRings.orbitSpeed(360 / timeToSeconds(10759.0f, 0, 0, 0) * orbitSpeed * gasSpecial);
	satRings.rotationSpeed(0.0f);

	rings.push_back(satRings);

	//rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (Sphere x : spheres) {
			x.draw();
		}

		for (Ring x : rings) {
			x.draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}

// input processor
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	int speed = 1;
	
	// Shift key for speeeeeed
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 10;
	}
	else {
		speed = 1;
	}
	
	// movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime * speed);

	//limits so it doesnt get out of the skybox
	if (camera.Position.x > skyLimit) {
		camera.Position.x = skyLimit;
	}
	if (camera.Position.x < -skyLimit) {
		camera.Position.x = -skyLimit;
	}
	if (camera.Position.y > skyLimit) {
		camera.Position.y = skyLimit;
	}
	if (camera.Position.y < -skyLimit) {
		camera.Position.y = -skyLimit;
	}
	if (camera.Position.z > skyLimit) {
		camera.Position.z = skyLimit;
	}
	if (camera.Position.z < -skyLimit) {
		camera.Position.z = -skyLimit;
	}
}

// glfw: window size stuff
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

// glfw: mouse stuff
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: scroll stuff (not used I think, but if it ain't broken...)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// auxiliary functions, if it gets messy, consider a separate file.
float timeToSeconds(float days, float hours, float minutes, float seconds) {
	hours = hours + days * 24;
	minutes = minutes + hours * 60;
	seconds = seconds + minutes * 60;
	return seconds;
}
