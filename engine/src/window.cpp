#include "window.h"
#include "logger.h"

#include <cassert>

namespace mars {

GlfwManager GlfwManager::s_Instance;
std::unordered_map<GLFWwindow*, Window*> GlfwManager::s_Windows;
GlfwManager::WindowToEventRegister GlfwManager::s_WindowsRegistered;
bool GlfwManager::s_Initialised = false;

Event::Event(void) :
    m_IsSet(false) {}
    
template<>
MARS_API KeyPressEvent Event::Get(void) const {
    assert(m_EventType == KEY_PRESS_EVENT);
    assert(m_IsSet);

    return m_Event.keyPressEvent;
}

template<>
MARS_API TextEvent Event::Get(void) const {
    assert(m_EventType == TEXT_EVENT);
    assert(m_IsSet);

    return m_Event.textEvent;
}

template<>
MARS_API MouseMoveEvent Event::Get(void) const {
    assert(m_EventType == MOUSE_MOVE_EVENT);
    assert(m_IsSet);

    return m_Event.mouseMoveEvent;
}

template<>
MARS_API MouseButtonEvent Event::Get(void) const {
    assert(m_EventType == MOUSE_BUTTON_EVENT);
    assert(m_IsSet);

    return m_Event.mouseButtonEvent;
}

template<>
MARS_API MouseEnteredEvent Event::Get(void) const {
    assert(m_EventType == MOUSE_ENTERED_EVENT);
    assert(m_IsSet);

    return m_Event.mouseEnteredEvent;
}

template<>
MARS_API MouseScrollEvent Event::Get(void) const {
    assert(m_EventType == MOUSE_SCROLL_EVENT);
    assert(m_IsSet);

    return m_Event.mouseScrollEvent;
}

template<>
MARS_API ResizeEvent Event::Get(void) const {
    assert(m_EventType == RESIZE_EVENT);
    assert(m_IsSet);

    return m_Event.resizeEvent;
}

template<>
MARS_API WindowPositionEvent Event::Get(void) const {
    assert(m_EventType == WINDOW_POSITION_EVENT);
    assert(m_IsSet);

    return m_Event.windowPositionEvent;
}

template<>
MARS_API void Event::Set(KeyPressEvent event) {
    m_EventType = KEY_PRESS_EVENT;
    m_Event.keyPressEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(TextEvent event) {
    m_EventType = TEXT_EVENT;
    m_Event.textEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(MouseMoveEvent event) {
    m_EventType = MOUSE_MOVE_EVENT;
    m_Event.mouseMoveEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(MouseButtonEvent event) {
    m_EventType = MOUSE_BUTTON_EVENT;
    m_Event.mouseButtonEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(MouseEnteredEvent event) {
    m_EventType = MOUSE_ENTERED_EVENT;
    m_Event.mouseEnteredEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(MouseScrollEvent event) {
    m_EventType = MOUSE_SCROLL_EVENT;
    m_Event.mouseScrollEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(ResizeEvent event) {
    m_EventType = RESIZE_EVENT;
    m_Event.resizeEvent = event;
    m_IsSet = true;
}

template<>
MARS_API void Event::Set(WindowPositionEvent event) {
    m_EventType = WINDOW_POSITION_EVENT;
    m_Event.windowPositionEvent = event;
    m_IsSet = true;    
}

Event::EventType Event::GetEventType(void) const {
    return m_EventType;
}

GlfwManager::GlfwManager(void) {
    Init();
}

GlfwManager::~GlfwManager(void) {
    Uninit();        
}

void GlfwManager::Init(void) {
    if (!s_Initialised) {
        if (!glfwInit()) {
            exit(EXIT_FAILURE);
        }

        if (!glfwVulkanSupported()) {
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        s_Initialised = true;
    }
}

void GlfwManager::Uninit(void) {
    for (auto const [window, _] : s_Windows) {
        if (window) {
            glfwDestroyWindow(window);        
        }        
    }        
        
    glfwTerminate();
    s_Initialised = false;
}

void GlfwManager::InsertWindow(GLFWwindow* window, Window* windowInstance) {
    assert(window && windowInstance);
    assert(!s_Windows.contains(window));

    s_Windows[window] = windowInstance;        
}

void GlfwManager::RemoveWindow(GLFWwindow* window) {
    assert(window);
    assert(s_Windows.contains(window));

    s_Windows.erase(window);       
}

void GlfwManager::InsertEventCallback(GLFWwindow* window,  Event::EventType eventType) {    
    if (!s_WindowsRegistered[eventType].contains(window)) {
        switch (eventType) {
        case Event::KEY_PRESS_EVENT:
            glfwSetKeyCallback(window, GlfwManager::KeyEventCallback);
        break;            
        case Event::TEXT_EVENT:
            glfwSetCharCallback(window, GlfwManager::TextEventCallback);
        break;            
        case Event::MOUSE_MOVE_EVENT:
            glfwSetCursorPosCallback(window, GlfwManager::MouseMoveEventCallback);
        break;            
        case Event::MOUSE_BUTTON_EVENT:
            glfwSetMouseButtonCallback(window, GlfwManager::MouseButtonEventCallback);
        break;            
        case Event::MOUSE_ENTERED_EVENT:
            glfwSetCursorEnterCallback(window, GlfwManager::MouseEnteredEventCallback);
        break;            
        case Event::MOUSE_SCROLL_EVENT:
            glfwSetScrollCallback(window, GlfwManager::MouseScrollEventCallback);
        break;            
        case Event::RESIZE_EVENT:
            glfwSetWindowSizeCallback(window, GlfwManager::ResizeEventCallback);
        break;            
        case Event::WINDOW_POSITION_EVENT:
            glfwSetWindowPosCallback(window, GlfwManager::WindowPositionEventCallback);
        break;            
        }
        s_WindowsRegistered[eventType].insert(window);
    }
}
    
void GlfwManager::KeyEventCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 modifiers) {
    assert(s_Windows.contains(window));
        
    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<KeyPressEvent>({ key, scancode, action, modifiers });
        windowInstance->NotifyEventCallbacks(event, Event::KEY_PRESS_EVENT);
    }
}

void GlfwManager::TextEventCallback(GLFWwindow* window, u32 codepoint) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<TextEvent>({ codepoint });
        windowInstance->NotifyEventCallbacks(event, Event::TEXT_EVENT);
    }
}

void GlfwManager::MouseMoveEventCallback(GLFWwindow* window, f64 mouseX, f64 mouseY) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<MouseMoveEvent>({ mouseX, mouseY });
        windowInstance->NotifyEventCallbacks(event, Event::MOUSE_MOVE_EVENT);
    }
}

void GlfwManager::MouseButtonEventCallback(GLFWwindow* window, i32 button, i32 action, i32 modifiers) {
    assert(s_Windows.contains(window));
        
    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<MouseButtonEvent>({ button, action, modifiers });
        windowInstance->NotifyEventCallbacks(event, Event::MOUSE_BUTTON_EVENT);
    }
}

void GlfwManager::MouseEnteredEventCallback(GLFWwindow* window, i32 entered) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<MouseEnteredEvent>({ entered });
        windowInstance->NotifyEventCallbacks(event, Event::MOUSE_ENTERED_EVENT);
    }
}

void GlfwManager::MouseScrollEventCallback(GLFWwindow* window, f64 mouseWheelX, f64 mouseWheelY) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<MouseScrollEvent>({ mouseWheelX, mouseWheelY });
        windowInstance->NotifyEventCallbacks(event, Event::MOUSE_SCROLL_EVENT);
    }
}

void GlfwManager::ResizeEventCallback(GLFWwindow* window, i32 width, i32 height) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<ResizeEvent>({ width, height });
        windowInstance->NotifyEventCallbacks(event, Event::RESIZE_EVENT);
    }
}

void GlfwManager::WindowPositionEventCallback(GLFWwindow* window, i32 x, i32 y) {
    assert(s_Windows.contains(window));

    if (Window* windowInstance = s_Windows.at(window)) { 
        Event event;
        event.Set<WindowPositionEvent>({ x, y });
        windowInstance->NotifyEventCallbacks(event, Event::WINDOW_POSITION_EVENT);
    }
}

Window::Window(i32 width, i32 height, const std::string& title) :
    m_Title(title) {
        
    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    GlfwManager::InsertWindow(m_Window, this);
}

Window::Window(const std::vector<std::pair<i32, i32>>& windowHints, i32 width, i32 height, const std::string& title) :
    m_Title(title) {
        
    for (auto const& [hint, value] : windowHints) {
        if (hint == GLFW_NO_API) {
            continue;
        }
        glfwWindowHint(hint, value);
    }
    
    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    GlfwManager::InsertWindow(m_Window, this);
}

Window::~Window(void) {
    GlfwManager::RemoveWindow(m_Window);
    Close();
}

void Window::InsertEventCallback(const std::function<void(const Event&)>& callback, Event::EventType eventType) {
    m_EventCallbacks[(u32)eventType].push_back(callback);
    GlfwManager::InsertEventCallback(m_Window, eventType);
}

void Window::NotifyEventCallbacks(const Event& event, Event::EventType eventType) {
    for (auto const& callback : m_EventCallbacks[(u32)eventType]) {
        callback(event);
    }    
}

bool Window::ShouldClose(void) const {
    return glfwWindowShouldClose(m_Window);    
}

void Window::Close(void) {
    glfwDestroyWindow(m_Window);
}

Window::WindowProperties Window::GetWindowProperties(void) const {
    WindowProperties properties;
    glfwGetWindowSize(m_Window, &properties.width, &properties.height);    
    glfwGetWindowPos(m_Window, &properties.x, &properties.y);
    properties.title = m_Title;
    return properties;
} 

void Window::Poll(void) {
    glfwPollEvents();
}

void Window::SwapBuffers(void) {
    glfwSwapBuffers(m_Window);
}

} // namespace mars
