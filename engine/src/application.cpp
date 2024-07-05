#include "application.h"
#include <chrono>

namespace mars {

IApplication::IApplication(i32 width, i32 height, const std::string& title) :
    m_Window(width, height, title) {
}

IApplication::IApplication(const std::vector<std::pair<i32, i32>>& hints, i32 width, i32 height, const std::string& title) :
    m_Window(width, height, title) {
}

void IApplication::Run(void) {
    using namespace std::chrono;
        
    steady_clock::time_point previousTime = steady_clock::now();
        
    while (!m_Window.ShouldClose()) {
        steady_clock::time_point currentTime = steady_clock::now();
        f32 dt = duration_cast<duration<float>>(currentTime - previousTime).count();
        previousTime = currentTime;
        m_Window.Poll();
            
        Update(dt);
        Render();        
    }
}

void IApplication::Render(void) {
    m_Window.SwapBuffers();
}
    
} // namespace mars
