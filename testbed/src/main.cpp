#include <iostream>
#include "window.h"
#include "logger.h"
#include "application.h"

using namespace mars;

class testApp : public IApplication {
public:
    testApp(void) = delete;
    testApp(i32 width, i32 height, const std::string& title) :
        IApplication(width, height, title) {
        m_Window.InsertEventCallback(std::bind(&testApp::PrintTime, this, std::placeholders::_1), Event::MOUSE_BUTTON_EVENT);
    }
    
private:
    f32 m_Dt;
    
private:
    virtual void Update(f32 dt) override {
        m_Dt = dt;
    }

    void PrintTime(const Event& event) {
        MARS_FATAL("dt: %f\n", m_Dt);
    }
};

/*
TODO:
    consider optimising logging module (i.e. get rid of vector and string use)
        move logging instance out of function (creates global instance)

    start vulkan
*/

int main(void) {
    testApp test(1000, 1000, "test");
    test.Run();
}
