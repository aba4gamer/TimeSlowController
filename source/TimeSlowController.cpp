#include "TimeSlowController.h"

TimeSlowController::TimeSlowController(const char *pName) : LiveActor(pName)
{
    executeOn = false;
    wasOnSwitchFlag = false;
    willKill = false;
}
void TimeSlowController::initAfterPlacement()
{
    MR::createTimeStopScreenEffect();
}
void TimeSlowController::init(const JMapInfoIter &rIter)
{
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::getJMapInfoArg0NoInit(rIter, &effectTimer);
    MR::getJMapInfoArg1NoInit(rIter, &willKill);
    MR::getJMapInfoArg2NoInit(rIter, &cooldownTimer);
    countableEffectTimer = effectTimer;
    makeActorAppeared();
}
void TimeSlowController::control()
{
    if (!countableCooldownTimer)
    {
        if (MR::isOnSwitchA(this))
            wasOnSwitchFlag = true;
            // If I don't use a flag the object can be broken if SW_A is disabled in the middle of the action
        if (effectTimer != -1 && wasOnSwitchFlag)
        {
            if (!executeOn)
            {
                MR::onTimeStopScreenEffect();
                MR::onSwitchB(this);
                executeOn = true;
            }
            else if (!countableEffectTimer)
            {
                MR::offTimeStopScreenEffect();
                MR::offSwitchB(this);
                MR::offSwitchA(this);

                countableCooldownTimer = cooldownTimer;
                wasOnSwitchFlag = false;
                executeOn = false;
                if (willKill)
                    kill();
                countableEffectTimer = effectTimer;
            }
            else
                countableEffectTimer--;
        }
        else
        {
            if (MR::isOnSwitchA(this) && !executeOn)
            {
                MR::onTimeStopScreenEffect();
                MR::onSwitchA(this);
                executeOn = true;
            }
            if (!MR::isOnSwitchA(this) && executeOn)
            {
                MR::offTimeStopScreenEffect();
                MR::offSwitchB(this);

                countableCooldownTimer = cooldownTimer;
                executeOn = false;
                if (willKill)
                    kill();
            }
        }
    }
    else
    {
        countableCooldownTimer--;
        MR::offSwitchA(this);
    }
}
