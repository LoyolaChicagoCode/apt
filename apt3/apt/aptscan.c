/******************************************************************
**
** APTSCAN.C:
**
**    APT Scanner (partially generated by AST)
**
** This file is part of Apt Parsing Tool (APT) for LL(1) Parsing
** Copyright (c) 1990 -- Apt Technologies
** All rights reserved
**
******************************************************************/

/* ---------- C Headers */
#include "cheaders.h"

/* ---------- Headers */

#include "apt.h"
#include "aptscan.h"

#include "buffer.h"
#include "gprintf.h"
/* ---------- Imported Global Variable */

extern ScanInfo scanInfo;

/* ---------- Functions */

Token Scan(file)
FILE *file;
{
  static int ScanState = NotActionsSection;
  Token token;

  switch (ScanState) {
  case NotActionsSection:
    token = scan1(file);
    if (token->type == T_actions) ScanState = ActionsSection;
    break;
  case ActionsSection:
    token = scan2(file);
    switch(token->type) {
    case T_productions:
    case T_aliases:
    case T_fiducials:
    case T_phases:
      ScanState = NotActionsSection;
    }
  }
  return token;
}

Token scan1(file)
FILE *file;
{
  int CurrentChar = 0;

  SetScanFile(scanInfo,file);
  SetScanState(scanInfo,S_Start);
  SetScanAttribute(scanInfo,-1);
  SetScanBuffer(scanInfo,BufferNew());
  Ignore(scanInfo);
  for (;;) {
    CurrentChar = GetScanChar(scanInfo);
    switch(GetScanState(scanInfo)) {
    case S_Ident:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (WHITESPACE(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        Advance(scanInfo);
      }
      break;
    case S_Minus:
      if (CharInString(CurrentChar,">")) {
        Match(scanInfo);
        SetScanAttribute(scanInfo,T_arrow);
        return MakeToken(scanInfo);
      }
      else if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (WHITESPACE(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        Advance(scanInfo);
        SetScanState(scanInfo,S_Ident);
      }
      break;
    case S_NewLine:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_EoL);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"\n")) {
        Advance(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_EoL);
        return MakeToken(scanInfo);
      }
      break;
    case S_Start:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_EoF);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"\t ")) {
        Ignore(scanInfo);
      }
      else if (CharInString(CurrentChar,"-")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Minus);
      }
      else if (CharInString(CurrentChar,"\n")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_NewLine);
      }
      else if (LAMBDA(CurrentChar)) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Ident);
      }
      break;
    default:
      gprintf(stderr,"Illegal State Encountered.\n");
      exit(1);
    }
  }
}

Token scan2(file)
FILE *file;
{
  int CurrentChar = 0;

  SetScanFile(scanInfo,file);
  SetScanState(scanInfo,S_Start);
  SetScanAttribute(scanInfo,-1);
  SetScanBuffer(scanInfo,BufferNew());
  Ignore(scanInfo);
  for (;;) {
    CurrentChar = GetScanChar(scanInfo);
    switch(GetScanState(scanInfo)) {
    case S_StringEsc:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanState(scanInfo,S_String);
        SetScanAttribute(scanInfo,T_string);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        Advance(scanInfo);
        SetScanState(scanInfo,S_String);
      }
      break;
    case S_String:
      if (CharInString(CurrentChar,"\n")) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_string);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"\\")) {
        Advance(scanInfo);
        SetScanState(scanInfo,S_StringEsc);
      }
      else if (CharInString(CurrentChar,"\"")) {
        Match(scanInfo);
        SetScanAttribute(scanInfo,T_string);
        return MakeToken(scanInfo);
      }
      else if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_string);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_string);
        return MakeToken(scanInfo);
      }
      break;
    case S_Char1:
      if (CharInString(CurrentChar,"\'")) {
        Match(scanInfo);
        SetScanAttribute(scanInfo,T_character);
        return MakeToken(scanInfo);
      }
      else if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_character);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_character);
        return MakeToken(scanInfo);
      }
      break;
    case S_Char:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_character);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        Advance(scanInfo);
        SetScanState(scanInfo,S_Char1);
      }
      break;
    case S_Number:
      if (NUMERIC(CurrentChar)) {
        Advance(scanInfo);
      }
      else if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_number);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_number);
        return MakeToken(scanInfo);
      }
      break;
    case S_Ident:
      if (CharInString(CurrentChar,"():,\"\'")) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (WHITESPACE(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        Advance(scanInfo);
      }
      break;
    case S_Section:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      else if (ALPHANUMERIC(CurrentChar)) {
        Advance(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_ident);
        return MakeToken(scanInfo);
      }
      break;
    case S_NewLine:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_EoL);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"\n")) {
        Advance(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        PushBack(scanInfo);
        SetScanAttribute(scanInfo,T_EoL);
        return MakeToken(scanInfo);
      }
      break;
    case S_Start:
      if (ENDOFFILE(CurrentChar)) {
        Ignore(scanInfo);
        SetScanAttribute(scanInfo,T_EoF);
        return MakeToken(scanInfo);
      }
      else if (NUMERIC(CurrentChar)) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Number);
      }
      else if (CharInString(CurrentChar,"\t ")) {
        Ignore(scanInfo);
      }
      else if (CharInString(CurrentChar,"\n")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_NewLine);
      }
      else if (CharInString(CurrentChar,"\"")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_String);
      }
      else if (CharInString(CurrentChar,"\'")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Char);
      }
      else if (CharInString(CurrentChar,",")) {
        InitMatch(scanInfo);
        SetScanAttribute(scanInfo,T_comma);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,":")) {
        InitMatch(scanInfo);
        SetScanAttribute(scanInfo,T_colon);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,")")) {
        InitMatch(scanInfo);
        SetScanAttribute(scanInfo,T_rparen);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"(")) {
        InitMatch(scanInfo);
        SetScanAttribute(scanInfo,T_lparen);
        return MakeToken(scanInfo);
      }
      else if (CharInString(CurrentChar,"%")) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Section);
      }
      else if (WHITESPACE(CurrentChar)) {
        Ignore(scanInfo);
      }
      else if (LAMBDA(CurrentChar)) {
        InitAdvance(scanInfo);
        SetScanState(scanInfo,S_Ident);
      }
      break;
    default:
      gprintf(stderr,"Illegal State Encountered.\n");
      exit(1);
    }
  }
}
