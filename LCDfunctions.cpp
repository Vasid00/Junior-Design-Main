//Driver file for lcd funcitons
#include "LCDfunctions.h" //header file

//Select function
void dispSelect(int *selCur, int *state, bool *caseStartSel, bool *palletStartSel, bool *twoCubeMode, bool *showStats, bool *autoParameters)
{
  //State selection
  //State -> BOOTUP
  if(&state == BOOTUP_STATE)
  {
    //Select Skip
    &state = MENU_STATE;
  }

  //State -> MAIN MENU
  else if(&state == MENU_STATE)
  {
    //Select Run
    if(&selCur == )
    {
      &state = RUN_STATE
    }
    //Select Settings
    else if(&selCur == )
    {
      &state = SETTINGS_STATE
    }
    //Select ShutDown
    else if(&selCur == )
    {
      &state = SHUTDOWN_STATE
    }
  }

  //State -> SETTINGS
  else if(&state == SETTINGS_STATE)
  {
    //Select Back
    if(&selCur == )
    {
      &state = MENU_STATE;
    }

    //Select Bool_2CaseMode
    else if(&selCur == )
    {
      twoCubeMode = !twoCaseMode;
    }

    //Select Bool_ShowStats
    else if(&selCur == )
    {
      showStats = !showStats;
    }

    //Select Bool_AutoParameters
    else if(&selCur == )
    {
      autoParameters = !autoParameters;
    }

    //Select Adjust_CaseStart
    else if(&selCur == )
    {
      caseStartSel = !caseStartSel;
    }

    //Select Adjust_PalletStart
    else if(&selCur == )
    {
      palletStartSel = !palletStartSel;
    }
  }

  //State -> RUN
  else if(&state == RUN_STATE)
  {
    //Select Stop
    if(&selCur == )
    {
      &state = STATS_STATE;
    }
    //Select Show_CurrStats
    else if(&selCur == )
    {
      showStats = !showStats;
    }
  }

  //State -> ANALYTICS
  else if(&state == STATS_STATE)
  {
    //Select Return
    if(&selCur == )
    {
      &state = MENU_STATE;
    }
  }
}

