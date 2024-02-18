#include <app.hpp>
#include <apps/applist.hpp>
#include <data/list.hpp>
#include <apps/systeminfoapp.hpp>

template class List<App*>;

static List<App*> *getSystemAppList() {
    List<App*> *applist = new List<App*>();
    applist->Add(new SystemInfoApp());
    return applist;
};