/**************************************
Exercise: dispatcher
Date: 10/12/2025
Developer: Baruch Haimson
Reviewer: 
Status:	waiting
**************************************/

#include <iostream>
#include <memory>
#include "dispatcher.hpp"

using namespace ilrd;

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"

void PrintTestResult(const std::string& name, bool pass)
{
    if (pass)
        std::cout << GREEN << "[PASS] " << name << RESET << std::endl;
    else
        std::cout << RED << "[FAIL] " << name << RESET << std::endl;
}

struct Event
{
    int value;
};

class ViewWindow
{
public:
    bool gotEvent = false;
    bool deathNotified = false;

    void OnEvent(Event e)
    {
        gotEvent = true;
    }

    void OnDeath()
    {
        deathNotified = true;
    }
};

class DataModel
{
public:
    bool gotEvent = false;
    bool deathNotified = false;

    void OnEvent(Event e)
    {
        gotEvent = true;
    }

    void OnDeath()
    {
        deathNotified = true;
    }
};

class ControlWindow
{
public:
    bool gotEvent = false;
    bool deathNotified = false;

    void OnEvent(Event e)
    {
        gotEvent = true;
    }

    void OnDeath()
    {
        deathNotified = true;
    }
};

int main()
{
    // ----------------------------------------------------------
    // Test 1: Broadcast to one observer
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        Callback<Event, ViewWindow> cb(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);

        disp.Subscribe(&cb);
        disp.Broadcast(Event{1});

        PrintTestResult("Broadcast to one observer", vw.gotEvent == true);
    }

    // ----------------------------------------------------------
    // Test 2: Broadcast to two observers
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);

        disp.Subscribe(&cb1);
        disp.Subscribe(&cb2);

        disp.Broadcast(Event{2});

        PrintTestResult("Broadcast to two observers",
                        vw.gotEvent && dm.gotEvent);
    }

    // ----------------------------------------------------------
    // Test 3: Broadcast to three observers
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;
        ControlWindow cw;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);
        Callback<Event, ControlWindow> cb3(cw, &ControlWindow::OnEvent, &ControlWindow::OnDeath);

        disp.Subscribe(&cb1);
        disp.Subscribe(&cb2);
        disp.Subscribe(&cb3);

        disp.Broadcast(Event{3});

        PrintTestResult("Broadcast to three observers",
                        vw.gotEvent && dm.gotEvent && cw.gotEvent);
    }

    // ----------------------------------------------------------
    // Test 4: Remove one observer during broadcast
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);

        disp.Subscribe(&cb1);
        disp.Subscribe(&cb2);

        class Remover
        {
        public:
            Remover(Dispatcher<Event>& d, BaseCallback<Event>* c) : disp(d), toRemove(c) {}
            void OnEvent(Event)
            {
                disp.Unsubscribe(toRemove);
            }
            void OnDeath() {}

            Dispatcher<Event>& disp;
            BaseCallback<Event>* toRemove;
        };

        Remover remover(disp, &cb2);
        Callback<Event, Remover> cbRem(remover, &Remover::OnEvent, &Remover::OnDeath);
        disp.Subscribe(&cbRem);

        disp.Broadcast(Event{4});

        vw.gotEvent = false;
        dm.gotEvent = false;

        disp.Broadcast(Event{1000});

        PrintTestResult("Remove one observer during broadcast",
                        dm.gotEvent == false);
            }

    // ----------------------------------------------------------
    // Test 5: Remove all observers during broadcast
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;
        ControlWindow cw;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);
        Callback<Event, ControlWindow> cb3(cw, &ControlWindow::OnEvent, &ControlWindow::OnDeath);

        disp.Subscribe(&cb1);
        disp.Subscribe(&cb2);
        disp.Subscribe(&cb3);

        std::unordered_set<BaseCallback<Event>*> snapshot = { &cb1, &cb2, &cb3 };

        class RemoveAll
        {
        public:
            RemoveAll(Dispatcher<Event>& d, const std::unordered_set<BaseCallback<Event>*>& s)
                : disp(d), snapshot(s) {}

            void OnEvent(Event)
            {
                for (auto cb : snapshot)
                {
                    disp.Unsubscribe(cb);
                }
            }

            void OnDeath() {}

            Dispatcher<Event>& disp;
            const std::unordered_set<BaseCallback<Event>*>& snapshot;
        };

        RemoveAll remover(disp, snapshot);
        Callback<Event, RemoveAll> cbRem(remover, &RemoveAll::OnEvent, &RemoveAll::OnDeath);
        disp.Subscribe(&cbRem);

        disp.Broadcast(Event{5});

        vw.gotEvent = false;
        dm.gotEvent = false;
        cw.gotEvent = false;

        disp.Broadcast(Event{6});

        PrintTestResult("Remove ALL observers during broadcast",
                        vw.gotEvent == false &&
                        dm.gotEvent == false &&
                        cw.gotEvent == false);
    }


    // ----------------------------------------------------------
    // Test 6: Add observer during broadcast
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);

        disp.Subscribe(&cb1);

        class Adder
        {
        public:
            Adder(Dispatcher<Event>& d, BaseCallback<Event>* c) : disp(d), cbNew(c) {}
            void OnEvent(Event) { disp.Subscribe(cbNew); }
            void OnDeath() {}
            Dispatcher<Event>& disp; BaseCallback<Event>* cbNew;
        };

        Adder adder(disp, &cb2);
        Callback<Event, Adder> cbAdder(adder, &Adder::OnEvent, &Adder::OnDeath);

        disp.Subscribe(&cbAdder);
        disp.Broadcast(Event{6});
        disp.Broadcast(Event{6});

        PrintTestResult("Add observer during broadcast", dm.gotEvent);
    }

    // ----------------------------------------------------------
    // Test 7: Add TWO observers during broadcast
    // ----------------------------------------------------------
    {
        Dispatcher<Event> disp;
        ViewWindow vw;
        DataModel dm;
        ControlWindow cw;

        Callback<Event, ViewWindow> cb1(vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
        Callback<Event, DataModel> cb2(dm, &DataModel::OnEvent, &DataModel::OnDeath);
        Callback<Event, ControlWindow> cb3(cw, &ControlWindow::OnEvent, &ControlWindow::OnDeath);

        disp.Subscribe(&cb1);

        class Adder2
        {
        public:
            Adder2(Dispatcher<Event>& d, BaseCallback<Event>* a, BaseCallback<Event>* b)
                : disp(d), A(a), B(b) {}

            void OnEvent(Event)
            {
                disp.Subscribe(A);
                disp.Subscribe(B);
            }

            void OnDeath() {}
            Dispatcher<Event>& disp; BaseCallback<Event>* A; BaseCallback<Event>* B;
        };

        Adder2 adder(disp, &cb2, &cb3);
        Callback<Event, Adder2> cbAdder(adder, &Adder2::OnEvent, &Adder2::OnDeath);
        disp.Subscribe(&cbAdder);

        disp.Broadcast(Event{7});
        disp.Broadcast(Event{7}); 

        PrintTestResult("Add TWO observers during broadcast", dm.gotEvent && cw.gotEvent);
    }

    std::cout << "\nAll tests completed.\n";
}




/*#include <iostream>
#include "dispatcher.hpp"

using namespace ilrd;

struct Event
{
    int value;
};

class Observer
{
public:
    Observer(const std::string& name) : m_name(name) {}

    void OnEvent1(Event e)
    {
        std::cout << m_name << "accept event with value = 1" << e.value << std::endl;
    }

    void OnEvent2(Event e)
    {
        std::cout << m_name << " accept event with value = 2" << e.value << std::endl;
    }

    void OnDispatcherDeath()
    {
        std::cout << m_name << ": accept Notifydeth from the despatcher " << std::endl;
    }

private:
    std::string m_name;
};

int main()
{
    std::cout << "---- start tests ----" << std::endl;

    Dispatcher<Event> disp;

    Observer obsA("Observer A");
    Observer obsB("Observer B");

    Callback<Event, Observer> cbA1(
        obsA,
        &Observer::OnEvent1,
        &Observer::OnDispatcherDeath);

    Callback<Event, Observer> cbA2(
        obsA,
        &Observer::OnEvent2,
        &Observer::OnDispatcherDeath);

    Callback<Event, Observer> cbB1(
        obsB,
        &Observer::OnEvent1,
        &Observer::OnDispatcherDeath);

    disp.Subscribe(&cbA1);
    disp.Subscribe(&cbA2);
    disp.Subscribe(&cbB1);

    std::cout << "\n---- first Broadcast----" << std::endl;
    disp.Broadcast(Event{10});

    std::cout << "\n-- cbA2 do SetDispatcher(NULL) Initiated disengagement --" << std::endl;
    cbA2.SetDispatcher(NULL);

    std::cout << "\n---- seconde Broadcast----" << std::endl;
    disp.Broadcast(Event{20});

    std::cout << "\n----destroye dispatcher (NotifyDeath for every callbacks) ----" << std::endl;


    std::cout << "----fainal tests----" << std::endl;
    return 0;
}*/
