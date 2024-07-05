#pragma once

#include "defines.h"
#include "window.h"

#include <string>
#include <vector>
#include <utility>
#include <chrono>

namespace mars {

class MARS_API IApplication {
public:
    IApplication(const IApplication&) = delete;
    IApplication(const IApplication&&) = delete;
        
    IApplication(i32 width, i32 height, const std::string& title);
    IApplication(const std::vector<std::pair<i32, i32>>& hints, i32 width, i32 height, const std::string& title);
    ~IApplication(void) = default;
        
    void Run(void);

private:
    virtual void Update(f32 dt) = 0;
    void Render(void);

protected:  
    Window m_Window;
};

} // namespace mars
