#include <app.hpp>
#include <apps/systemapp.hpp>
#include <gfx/gfx.hpp>
#include <data/string.hpp>
#include <system.hpp>
#include <apps/applist.hpp>
#include <gfx/font.hpp>
#include <gfx/psf2.hpp>
#include <data/time.hpp>
#include <resources/systemfont.hpp>

SystemApp::SystemApp() {
    this->Init = (App::InitFunction)&SystemApp::_Init;
    this->UpdateCore = (App::UpdateFunction)&SystemApp::_UpdateCore;
    this->Render = (App::RenderFunction)&SystemApp::_Render;
}

void SystemApp::_Init() {
    this->name = String("System");
}

void SystemApp::_UpdateCore(int core) {
    if (this->currentApp != NULL) {
        this->currentApp->UpdateCore(this->currentApp, core);
        if (this->currentApp->refreshScreen) {
            this->refreshScreen = true;
            this->currentApp->refreshScreen = false;
        }
    } else {
        this->refreshScreen = true;
    }
}

void SystemApp::_Render(Graphics gfx) {
    /* Draw App List */
    gfx.FillRectangle(Rectangle(0, 40, gfx.bufferWidth, gfx.bufferHeight-40), Color(233,233,233));

    int appcount = 5;
    int selected = 0;
    int spacing = 30;
    int startX = gfx.bufferWidth / 2 - (gfx.bufferWidth / 220) * 110;
    
    int x = startX;
    int y = 40 + spacing - scrollY;
    for (int i = 0; i < appcount; i++) {
        gfx.FillRectangle(Rectangle(x, y, 200, 200), Color(219, 219, 219));

        // Draw image
        // yet to be implemented.

        if (selected == i) {
            // Draw border
            Color borderColor = Color(128, 128, 128);
            gfx.FillRectangle(Rectangle(x, y, 4, 200), borderColor);
            gfx.FillRectangle(Rectangle(x, y, 200, 4), borderColor);
            gfx.FillRectangle(Rectangle(x + 196, y, 4, 200), borderColor);
            gfx.FillRectangle(Rectangle(x, y + 196, 200, 4), borderColor);
        }

        x += 200 + spacing;
        if (x + 200 >= gfx.bufferWidth - spacing) {
            x = startX;
            y += 200 + spacing;
        }
    }

    /* Draw Status Bar */
    gfx.FillRectangle(Rectangle(0, 0, gfx.bufferWidth, 40), Color(200, 200, 200));

    /* Time */
    TimeSpan currentTime = TimeSpan::GetCurrentTime();

    String hourTime = String::ParseInt(currentTime.GetHourComponent());
    if (hourTime.GetLength() < 2)
    {
        hourTime = String("0") + hourTime;
    }

    String minuteTime = String::ParseInt(currentTime.GetMinuteComponent());
    if (minuteTime.GetLength() < 2) {
        minuteTime = String("0") + minuteTime;
    }

    String secondTime = String::ParseInt(currentTime.GetSecondComponent());
    if (secondTime.GetLength() < 2) {
        secondTime = String("0") + secondTime;
    }

    String time = hourTime + ":" + minuteTime + ":" + secondTime;
    PSF2Font font = getSystemFont();
    gfx.DrawString(&font, Point(7, 12), time, Color(0, 0, 0));
}