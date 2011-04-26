#ifndef START_OF_CYCLE_SUBSCRIBER_H
#define START_OF_CYCLE_SUBSCRIBER_H

#include <vector>

class StartOfCycleSubscriber
{
public:
    StartOfCycleSubscriber();
    virtual ~StartOfCycleSubscriber() {}
    virtual void NewCycle() = 0;
};

class StartOfCycleSubscriberSubscribers
{
private:
    std::vector<StartOfCycleSubscriber*> subscribers;
    StartOfCycleSubscriberSubscribers();
    static StartOfCycleSubscriberSubscribers* instance;
public:
    static StartOfCycleSubscriberSubscribers& GetInstance()
    {
        if(!instance)
            instance = new StartOfCycleSubscriberSubscribers();
        return *instance;
    }
    void AddListener(StartOfCycleSubscriber* newSubscriber);

    void NotifySubscribers();
};


#endif
