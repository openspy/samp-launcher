///////////////////////////////////////////////////////////////////////////////
// GameSpy ATLAS Competition System Source File
//
// NOTE: This is an auto-generated file, do not edit this file directly.
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "atlas_taprace_v2.h"

int atlas_rule_set_version = 2;

int ATLAS_GET_KEY(char* keyName)
{
   if(!keyName)
      return 0;
   
   if(!strcmp("RACE_TIME", keyName))
      return RACE_TIME;   
   if(!strcmp("KEY_NICK", keyName))
      return KEY_NICK;   
   if(!strcmp("KEY_LATITUDE", keyName))
      return KEY_LATITUDE;   
   if(!strcmp("KEY_LONGITUDE", keyName))
      return KEY_LONGITUDE;   
   if(!strcmp("KEY_MULTI_PLAY", keyName))
      return KEY_MULTI_PLAY;   
   if(!strcmp("KEY_SINGLE_PLAY", keyName))
      return KEY_SINGLE_PLAY;   
   if(!strcmp("SP_RACE_TIME", keyName))
      return SP_RACE_TIME;   
   if(!strcmp("KEY_RACE_DATE_TIME", keyName))
      return KEY_RACE_DATE_TIME;   
   
   return 0;
}

char* ATLAS_GET_KEY_NAME(int keyId)
{
   if(keyId <= 0)
      return "";
   
   if(keyId == RACE_TIME)
      return "RACE_TIME";   
   if(keyId == KEY_NICK)
      return "KEY_NICK";   
   if(keyId == KEY_LATITUDE)
      return "KEY_LATITUDE";   
   if(keyId == KEY_LONGITUDE)
      return "KEY_LONGITUDE";   
   if(keyId == KEY_MULTI_PLAY)
      return "KEY_MULTI_PLAY";   
   if(keyId == KEY_SINGLE_PLAY)
      return "KEY_SINGLE_PLAY";   
   if(keyId == SP_RACE_TIME)
      return "SP_RACE_TIME";   
   if(keyId == KEY_RACE_DATE_TIME)
      return "KEY_RACE_DATE_TIME";   
   
   return "";
}

int ATLAS_GET_STAT(char* statName)
{
   if(!statName)
      return 0;
   
   if(!strcmp("AVERAGE_RACE_TIME", statName))
      return AVERAGE_RACE_TIME;   
   if(!strcmp("BEST_RACE_TIME", statName))
      return BEST_RACE_TIME;   
   if(!strcmp("CAREER_DISCONNECTS", statName))
      return CAREER_DISCONNECTS;   
   if(!strcmp("CAREER_DRAWS", statName))
      return CAREER_DRAWS;   
   if(!strcmp("CAREER_LONGEST_DRAW_STREAK", statName))
      return CAREER_LONGEST_DRAW_STREAK;   
   if(!strcmp("CAREER_LONGEST_LOSS_STREAK", statName))
      return CAREER_LONGEST_LOSS_STREAK;   
   if(!strcmp("CAREER_LONGEST_WIN_STREAK", statName))
      return CAREER_LONGEST_WIN_STREAK;   
   if(!strcmp("CAREER_LOSSES", statName))
      return CAREER_LOSSES;   
   if(!strcmp("CAREER_WINS", statName))
      return CAREER_WINS;   
   if(!strcmp("CURRENT_DRAW_STREAK", statName))
      return CURRENT_DRAW_STREAK;   
   if(!strcmp("CURRENT_LOSS_STREAK", statName))
      return CURRENT_LOSS_STREAK;   
   if(!strcmp("CURRENT_WIN_STREAK", statName))
      return CURRENT_WIN_STREAK;   
   if(!strcmp("DISCONNECT_RATE", statName))
      return DISCONNECT_RATE;   
   if(!strcmp("LAST_TIME_PLAYED", statName))
      return LAST_TIME_PLAYED;   
   if(!strcmp("LATITUDE", statName))
      return LATITUDE;   
   if(!strcmp("LONGITUDE", statName))
      return LONGITUDE;   
   if(!strcmp("NICK", statName))
      return NICK;   
   if(!strcmp("SP_AVERAGE_RACE_TIME", statName))
      return SP_AVERAGE_RACE_TIME;   
   if(!strcmp("SP_BEST_RACE_TIME", statName))
      return SP_BEST_RACE_TIME;   
   if(!strcmp("SP_TOTAL_PLAYS", statName))
      return SP_TOTAL_PLAYS;   
   if(!strcmp("SP_TOTAL_RACE_TIME", statName))
      return SP_TOTAL_RACE_TIME;   
   if(!strcmp("SP_WORST_RACE_TIME", statName))
      return SP_WORST_RACE_TIME;   
   if(!strcmp("TOTAL_COMPLETE_MATCHES", statName))
      return TOTAL_COMPLETE_MATCHES;   
   if(!strcmp("TOTAL_MATCHES", statName))
      return TOTAL_MATCHES;   
   if(!strcmp("TOTAL_RACE_TIME", statName))
      return TOTAL_RACE_TIME;   
   if(!strcmp("WORST_RACE_TIME", statName))
      return WORST_RACE_TIME;   
   
   return 0;
}
char* ATLAS_GET_STAT_NAME(int statId)
{
   if(statId <= 0)
      return "";
   
   if(statId == AVERAGE_RACE_TIME)
      return "AVERAGE_RACE_TIME";   
   if(statId == BEST_RACE_TIME)
      return "BEST_RACE_TIME";   
   if(statId == CAREER_DISCONNECTS)
      return "CAREER_DISCONNECTS";   
   if(statId == CAREER_DRAWS)
      return "CAREER_DRAWS";   
   if(statId == CAREER_LONGEST_DRAW_STREAK)
      return "CAREER_LONGEST_DRAW_STREAK";   
   if(statId == CAREER_LONGEST_LOSS_STREAK)
      return "CAREER_LONGEST_LOSS_STREAK";   
   if(statId == CAREER_LONGEST_WIN_STREAK)
      return "CAREER_LONGEST_WIN_STREAK";   
   if(statId == CAREER_LOSSES)
      return "CAREER_LOSSES";   
   if(statId == CAREER_WINS)
      return "CAREER_WINS";   
   if(statId == CURRENT_DRAW_STREAK)
      return "CURRENT_DRAW_STREAK";   
   if(statId == CURRENT_LOSS_STREAK)
      return "CURRENT_LOSS_STREAK";   
   if(statId == CURRENT_WIN_STREAK)
      return "CURRENT_WIN_STREAK";   
   if(statId == DISCONNECT_RATE)
      return "DISCONNECT_RATE";   
   if(statId == LAST_TIME_PLAYED)
      return "LAST_TIME_PLAYED";   
   if(statId == LATITUDE)
      return "LATITUDE";   
   if(statId == LONGITUDE)
      return "LONGITUDE";   
   if(statId == NICK)
      return "NICK";   
   if(statId == SP_AVERAGE_RACE_TIME)
      return "SP_AVERAGE_RACE_TIME";   
   if(statId == SP_BEST_RACE_TIME)
      return "SP_BEST_RACE_TIME";   
   if(statId == SP_TOTAL_PLAYS)
      return "SP_TOTAL_PLAYS";   
   if(statId == SP_TOTAL_RACE_TIME)
      return "SP_TOTAL_RACE_TIME";   
   if(statId == SP_WORST_RACE_TIME)
      return "SP_WORST_RACE_TIME";   
   if(statId == TOTAL_COMPLETE_MATCHES)
      return "TOTAL_COMPLETE_MATCHES";   
   if(statId == TOTAL_MATCHES)
      return "TOTAL_MATCHES";   
   if(statId == TOTAL_RACE_TIME)
      return "TOTAL_RACE_TIME";   
   if(statId == WORST_RACE_TIME)
      return "WORST_RACE_TIME";   
   
   return "";
}

int ATLAS_GET_STAT_PAGE_BY_ID(int statId)
{
   if(statId <= 0)
      return 0;
   

   //PlayerStats
   if(statId == 1 || statId == 2 || statId == 3 || statId == 4 || statId == 5 || statId == 6 || statId == 7 ||
      statId == 8 || statId == 9 || statId == 10 || statId == 11 || statId == 12 || statId == 13 || statId == 14 ||
      statId == 15 || statId == 16 || statId == 17 || statId == 18 || statId == 19 || statId == 20 || statId == 21 ||
      statId == 22 || statId == 23 || statId == 24 || statId == 25 || statId == 26)
      return 1;   
      
   
   return 0;
}

int ATLAS_GET_STAT_PAGE_BY_NAME(char* statName)
{
   if(!statName)
      return 0;
   

   //PlayerStats
   if(!strcmp("CAREER_WINS", statName) || !strcmp("CAREER_LOSSES", statName) ||
      !strcmp("BEST_RACE_TIME", statName) || !strcmp("WORST_RACE_TIME", statName) ||
      !strcmp("TOTAL_MATCHES", statName) || !strcmp("AVERAGE_RACE_TIME", statName) ||
      !strcmp("CURRENT_WIN_STREAK", statName) || !strcmp("CURRENT_LOSS_STREAK", statName) ||
      !strcmp("TOTAL_RACE_TIME", statName) || !strcmp("CAREER_DISCONNECTS", statName) ||
      !strcmp("DISCONNECT_RATE", statName) || !strcmp("CAREER_DRAWS", statName) ||
      !strcmp("CURRENT_DRAW_STREAK", statName) || !strcmp("CAREER_LONGEST_WIN_STREAK", statName) ||
      !strcmp("CAREER_LONGEST_LOSS_STREAK", statName) || !strcmp("CAREER_LONGEST_DRAW_STREAK", statName) ||
      !strcmp("TOTAL_COMPLETE_MATCHES", statName) || !strcmp("NICK", statName) ||
      !strcmp("LATITUDE", statName) || !strcmp("LONGITUDE", statName) ||
      !strcmp("SP_BEST_RACE_TIME", statName) || !strcmp("SP_WORST_RACE_TIME", statName) ||
      !strcmp("SP_TOTAL_PLAYS", statName) || !strcmp("SP_AVERAGE_RACE_TIME", statName) ||
      !strcmp("SP_TOTAL_RACE_TIME", statName) || !strcmp("LAST_TIME_PLAYED", statName))
      return 1;   
      
   
   return 0;
}
