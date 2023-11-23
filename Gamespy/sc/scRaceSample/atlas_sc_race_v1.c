///////////////////////////////////////////////////////////////////////////////
// File:	atlas_sc_race_v1.c
// SDK:		GameSpy ATLAS Competition SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2008-2009 GameSpy Industries, Inc.

#include <string.h>
#include "atlas_sc_race_v1.h"

int atlas_rule_set_version = 1;

int ATLAS_GET_KEY(char* keyName)
{
   if(!keyName)
      return 0;
   
   if(!strcmp("RACE_TIME", keyName))
      return RACE_TIME;   
   
   return 0;
}

char* ATLAS_GET_KEY_NAME(int keyId)
{
   if(keyId <= 0)
      return "";
   
   if(keyId == RACE_TIME)
      return "RACE_TIME";   
   
   return "";
}

int ATLAS_GET_STAT(char* statName)
{
   if(!statName)
      return 0;
   
   if(!strcmp("CAREER_WINS", statName))
      return CAREER_WINS;   
   else if(!strcmp("CAREER_LOSSES", statName))
      return CAREER_LOSSES;   
   else if(!strcmp("BEST_RACE_TIME", statName))
      return BEST_RACE_TIME;   
   else if(!strcmp("WORST_RACE_TIME", statName))
      return WORST_RACE_TIME;   
   else if(!strcmp("TOTAL_MATCHES", statName))
      return TOTAL_MATCHES;   
   else if(!strcmp("AVERAGE_RACE_TIME", statName))
      return AVERAGE_RACE_TIME;   
   else if(!strcmp("CURRENT_WIN_STREAK", statName))
      return CURRENT_WIN_STREAK;   
   else if(!strcmp("CURRENT_LOSS_STREAK", statName))
      return CURRENT_LOSS_STREAK;   
   else if(!strcmp("TOTAL_RACE_TIME", statName))
      return TOTAL_RACE_TIME;   
   else if(!strcmp("CAREER_DISCONNECTS", statName))
      return CAREER_DISCONNECTS;   
   else if(!strcmp("DISCONNECT_RATE", statName))
      return DISCONNECT_RATE;   
   else if(!strcmp("CAREER_DRAWS", statName))
      return CAREER_DRAWS;   
   else if(!strcmp("CURRENT_DRAW_STREAK", statName))
      return CURRENT_DRAW_STREAK;   
   else if(!strcmp("CAREER_LONGEST_WIN_STREAK", statName))
      return CAREER_LONGEST_WIN_STREAK;   
   else if(!strcmp("CAREER_LONGEST_LOSS_STREAK", statName))
      return CAREER_LONGEST_LOSS_STREAK;   
   else if(!strcmp("CAREER_LONGEST_DRAW_STREAK", statName))
      return CAREER_LONGEST_DRAW_STREAK;   
   else if(!strcmp("TOTAL_COMPLETE_MATCHES", statName))
      return TOTAL_COMPLETE_MATCHES;   
   else if(!strcmp("RICHARD_TEST1", statName))
      return RICHARD_TEST1;   
   else if(!strcmp("RICHARD_TEST2", statName))
      return RICHARD_TEST2;   
   else if(!strcmp("RICHARD_TEST3", statName))
      return RICHARD_TEST3;   
   
   return 0;
}
char* ATLAS_GET_STAT_NAME(int statId)
{
   if(statId <= 0)
      return "";
   
   if(statId == CAREER_WINS)
      return "CAREER_WINS";   
   else if(statId == CAREER_LOSSES)
      return "CAREER_LOSSES";   
   else if(statId == BEST_RACE_TIME)
      return "BEST_RACE_TIME";   
   else if(statId == WORST_RACE_TIME)
      return "WORST_RACE_TIME";   
   else if(statId == TOTAL_MATCHES)
      return "TOTAL_MATCHES";   
   else if(statId == AVERAGE_RACE_TIME)
      return "AVERAGE_RACE_TIME";   
   else if(statId == CURRENT_WIN_STREAK)
      return "CURRENT_WIN_STREAK";   
   else if(statId == CURRENT_LOSS_STREAK)
      return "CURRENT_LOSS_STREAK";   
   else if(statId == TOTAL_RACE_TIME)
      return "TOTAL_RACE_TIME";   
   else if(statId == CAREER_DISCONNECTS)
      return "CAREER_DISCONNECTS";   
   else if(statId == DISCONNECT_RATE)
      return "DISCONNECT_RATE";   
   else if(statId == CAREER_DRAWS)
      return "CAREER_DRAWS";   
   else if(statId == CURRENT_DRAW_STREAK)
      return "CURRENT_DRAW_STREAK";   
   else if(statId == CAREER_LONGEST_WIN_STREAK)
      return "CAREER_LONGEST_WIN_STREAK";   
   else if(statId == CAREER_LONGEST_LOSS_STREAK)
      return "CAREER_LONGEST_LOSS_STREAK";   
   else if(statId == CAREER_LONGEST_DRAW_STREAK)
      return "CAREER_LONGEST_DRAW_STREAK";   
   else if(statId == TOTAL_COMPLETE_MATCHES)
      return "TOTAL_COMPLETE_MATCHES";   
   else if(statId == RICHARD_TEST1)
      return "RICHARD_TEST1";   
   else if(statId == RICHARD_TEST2)
      return "RICHARD_TEST2";   
   else if(statId == RICHARD_TEST3)
      return "RICHARD_TEST3";   
   
   return "";
}
