#pragma once
#include "defines.h"

#include "../deps/glfw-3.4/include/GLFW/glfw3.h"
#include "window_keys.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>
#include <functional>
#include <unordered_set>

namespace mars {
    
struct MARS_API KeyPressEvent {
    i32 key;    i32 scanode;
    i32 actin;
    i32 modiiers;  
};

struct MARS_API TextEvent {
    u32 codepoint;
};

struct MARS_API MouseMoveEvent {
    f64 mouseX;
    f64 mouseY;
};

struct MARS_API MouseEnteredEvent {
    i32 entered;
};

struct MARS_API MouseButtonEvent {
    i32 button;
    i32 action;
    i32 modifiers;  
};

struct MARS_API MouseScrollEvent {
    f64 mouseWheelX;
    f64 mouseWheelY;
};

struct MARS_API ResizeEvent {
    i32 width;
    i32 height;  
};

struct MARS_API WindowPositionEvent {
    i32 x;
    i32 y;      
};

class MARS_API Event {
public:
    static constexpr size_t NUMBER_OF_EVENTS = 8;
    
    enum EventType {
        KEY_PRESS_EVENT = 0,
        TEXT_EVENT,
        MOUSE_MOVE_EVENT,
        MOUSE_ENTERED_EVENT,
        MOUSE_BUTTON_EVENT,
        MOUSE_SCROLL_EVENT,
        RESIZE_EVENT,
        WINDOW_POSITION_EVENT,
    };

    union EventUnion {
        KeyPressEvent keyPressEvent;
        TextEvent textEvent;
        MouseMoveEvent mouseMoveEvent;
        MouseButtonEvent mouseButtonEvent;
        MouseEnteredEvent mouseEnteredEvent;
        MouseScrollEvent mouseScrollEvent;
        ResizeEvent resizeEvent;
        WindowPositionEvent windowPositionEvent;
    };
    
public:
    Event(void);
    Event(const Event&) = delete;
    Event(const Event&&) = delete;
        
    template<typename T>
    T Get(void) const;

    template<typename T>
    void Set(T event);

    EventType GetEventType(void) const;
    
private:
    EventUnion m_Event;
    EventType m_EventType;
    bool m_IsSet;
};

class Window;

class MARS_API GlfwManager {
public:
    using WindowToEventRegister = std::array<std::unordered_set<GLFWwindow*>, Event::NUMBER_OF_EVENTS>;
    
public:
    GlfwManager(const GlfwManager&) = delete;
    GlfwManager(const GlfwManager&&) = delete;
    ~GlfwManager(void);
        
    static void InsertWindow(GLFWwindow* window, Window* windowInstance);
    static void RemoveWindow(GLFWwindow* window);
    static void InsertEventCallback(GLFWwindow* window, Event::EventType eventType); 
    static void Init(void);
    static void Uninit(void);
    
private:
    static GlfwManager s_Instance;
    static std::unordered_map<GLFWwindow*, Window*> s_Windows; 
    static WindowToEventRegister s_WindowsRegistered;
    static bool s_Initialised;
        
private:        
    GlfwManager(void); 
        
    static void KeyEventCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 modifiers);
    static void TextEventCallback(GLFWwindow* window, u32 codepoint);
    static void MouseMoveEventCallback(GLFWwindow* window, f64 mouseX, f64 mouseY);
    static void MouseButtonEventCallback(GLFWwindow* window, i32 button, i32 action, i32 modifiers);
    static void MouseEnteredEventCallback(GLFWwindow* window, i32 entered);
    static void MouseScrollEventCallback(GLFWwindow* window, f64 mouseWheelX, f64 mouseWheelY);
    static void ResizeEventCallback(GLFWwindow* window, i32 width, i32 height);
    static void WindowPositionEventCallback(GLFWwindow* window, i32 x, i32 y);
};
    
class MARS_API Window {
public:
    using EventCallbackArray = std::array<std::vector<std::function<void(const Event&)>>, Event::NUMBER_OF_EVENTS>; 
        
    struct WindowProperties {
        i32 x;
        i32 y;
        i32 width;
        i32 height;
        std::string title;
    };

public:
    Window(void) = delete;
    Window(const Window& other) = delete;
    Window(const Window&& other) = delete;

    Window(i32 width, i32 height, const std::string& title);
    Window(const std::vector<std::pair<i32, i32>>& windowHints, i32 width, i32 height, const std::string& title);

    ~Window(void);
    
    WindowProperties GetWindowProperties(void) const;
    void InsertEventCallback(const std::function<void(const Event&)>&, Event::EventType eventType);
    void NotifyEventCallbacks(const Event& event, Event::EventType eventType);
    bool ShouldClose(void) const;
    void Close(void);
    void Poll(void);
    void SwapBuffers(void);
    
private:    
    GLFWwindow* m_Window;
    std::string m_Title;
    EventCallbackArray m_EventCallbacks;
};
    
} // namespace mars
