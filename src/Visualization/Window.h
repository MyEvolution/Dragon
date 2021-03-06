#ifndef DRAGON_WINDOW_H
#define DRAGON_WINDOW_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "IO/ConsoleColor.h"
#include "Geometry/Structure/BoundingBox.h"
#include <iostream>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include "GLFW/glfw3.h"
#include "Geometry/BasicGeometry.h"
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace dragon
{
namespace visualization
{
namespace window
{
    extern GLFWwindow* window;  
    extern geometry::Matrix4 projection_matrix;
    extern geometry::Matrix4 model_view_matrix;    
    extern geometry::BoundingBox bb;
    static int mouse_buttons[7];
    extern double last_x;
    extern double last_y;
    extern int w_height;
    extern int w_width;
    extern double near, far, fovy;
    extern double aspect;
    extern double up;
    extern double right;
    extern bool last_point_valid;
    // extern geometry::Point2List pressed_points;
    extern geometry::Point3 last_point_3d;
    bool Initialize(int width = 800, int height = 800, const std::string &window_name = "Dragon 3D");
    void Cleanup();
    // the function that can be rewritten
    void RenderGuiComponents();
    void RegisterMouseAndKeyboard();
    void RegisterMouseAndKeyboard2D();
    void Translate(double x, double y);
    void Translate(const geometry::Vector3 &translation);
    void Rotate(const geometry::Matrix3 &rotation);
    void Rotate(const geometry::Vector3 &axis, double angle);
    void Rotate(double x, double y, int type);
    void Zoom(double y);
    void CursorPos(double& x, double& y);
    void Movement(double xpos, double ypos);  
    bool inline right_mouse_pressed()
    {
        return mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT];
    }
    bool inline left_mouse_pressed()
    {
        return mouse_buttons[GLFW_MOUSE_BUTTON_LEFT];
    }
    bool inline middle_mouse_pressed()
    {
        return mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE];
    }

    void SetProjectionMatrix();
    void SetModelViewMatrix();

    void DrawCircle(const geometry::Point2 &p, double r, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2, int n = 100);    
    void DrawCircleFilled(const geometry::Point2 &p, double r, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), int n = 100); 
    void DrawPoint(const geometry::Point2 &p, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 5);
    void DrawPoints(const geometry::Point2List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 5);
    void DrawLine(const geometry::Point2 &p1, const geometry::Point2 &p2, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0),  double thickness = 2);
    void DrawLines(const geometry::Point2List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawLineStrip(const geometry::Point2List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawPolygon(const geometry::Point2List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawPolygonFilled(const geometry::Point2List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0));

    void DrawPoint(const geometry::Point3 &p, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 5);
    void DrawPoints(const geometry::Point3List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 5);
    void DrawLine(const geometry::Point3 &p1, const geometry::Point3 &p2, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0),  double thickness = 2);
    void DrawLines(const geometry::Point3List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawLineStrip(const geometry::Point3List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawPolygon(const geometry::Point3List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0), double thickness = 2);
    void DrawPolygonFilled(const geometry::Point3List &points, const Eigen::Vector3f &color = Eigen::Vector3f(0, 0, 0));
}
}
}


#endif