#include "StartOfCycleSubscriber.h"
using namespace std;
StartOfCycleSubscriberSubscribers* StartOfCycleSubscriberSubscribers::instance = 0;

StartOfCycleSubscriber::StartOfCycleSubscriber()
{
    StartOfCycleSubscriberSubscribers::GetInstance().AddListener(this);
}

StartOfCycleSubscriberSubscribers::StartOfCycleSubscriberSubscribers()
    : subscribers()
{


}

void StartOfCycleSubscriberSubscribers::AddListener(StartOfCycleSubscriber* newSubscriber)
{
    subscribers.push_back(newSubscriber);
}

void StartOfCycleSubscriberSubscribers::NotifySubscribers()
{
    for(unsigned int i = 0; i < subscribers.size(); i++)
        subscribers[i]->NewCycle();
}

