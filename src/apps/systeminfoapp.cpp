#include <app.hpp>
#include <apps/systeminfoapp.hpp>
#include <gfx/gfx.hpp>
#include <data/string.hpp>
#include <system.hpp>

SystemInfoApp::SystemInfoApp() {
    this->Init = (App::InitFunction)&SystemInfoApp::_Init;
    this->UpdateCore = (App::UpdateFunction)&SystemInfoApp::_UpdateCore;
    this->Render = (App::RenderFunction)&SystemInfoApp::_Render;
}

void SystemInfoApp::_Init() {
    this->name = String("System");
}

void SystemInfoApp::_UpdateCore(int core) {
    this->refreshScreen = true;
}

void SystemInfoApp::_Render(Graphics gfx) {
    gfx.FillRectangle(Rectangle(25, 25, 10, 10), Color(255, 255, 255));
}