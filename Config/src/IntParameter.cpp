#include <IntParameter.h>
#include "Log.h"

using namespace Config;

//-----------------------------------------------------------------------------

IntParameter::IntParameter(const std::string& theName)
    : nameM(theName)
    , checkRangeM(false)
    , minValueM(0)
    , maxValueM(0)
    , valueM(0)
{
}

//-----------------------------------------------------------------------------

IntParameter::~IntParameter()
{}

//-----------------------------------------------------------------------------

int IntParameter::get()
{
    boost::shared_lock<boost::shared_mutex> lock(valueMutexM);
    return valueM;
}

//-----------------------------------------------------------------------------

int IntParameter::set(const std::string& theValue)
{
    int value = 0;
    if (0 != scanf("%d", &value))
    {
        ERROR(nameM << "'s value " << theValue << " is invalid.");
        return -1;
    }
    return set(value);
}

//-----------------------------------------------------------------------------

int IntParameter::set(const int theValue)
{
    if (theValue == valueM)
        return 0;

    bool notifyWatcher = false;

    {
        boost::upgrade_lock<boost::shared_mutex> lock(valueMutexM);
        if (!checkRangeM 
                || (checkRangeM && theValue >= minValueM && theValue <= maxValueM))
        {
            {
                boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
                valueM = theValue;
            }
            notifyWatcher = true;
        }
        else
        {
            ERROR("invalid value: "<< theValue
                    << " for " << nameM 
                    << "(" << minValueM << "-" << maxValueM << ")");        
            return -1;
        }
    }
    if (notifyWatcher)
    {
        for (WatcherList::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (*it)(theValue);
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const std::string& theRange);
{
    if (theRange.empty())
        return;

    int minValue = 0;
    int maxValue = 0;
    if (0 != scanf("%d-%d", &minValueM, &maxValueM))
    {
        ERROR(nameM << "'s range " << theRange << " is invalid.");
        return -1;
    }
    if (minValueM < maxValueM)
    {
        return setRange(minValueM, maxValueM);
    }
    else
    {
        return setRange(maxValueM, minValueM);
    }

}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const int theMin, const int theMax)
{
    boost::unique_lock<boost::shared_mutex> lock(valueMutexM);
    checkRangeM = true;
    minValueM = theMin;
    maxValueM = theMax;
}

//-----------------------------------------------------------------------------

void IntParameter::registerWatcher(Watcher& theWatcher)
{
    boost::unique_lock<boost::mutex> lock(watcherMutexM);
    changesWatchersM.push_back(theWatcher);
}

//-----------------------------------------------------------------------------



