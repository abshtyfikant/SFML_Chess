#ifndef STOCKFISHCONNECTOR_H
#define STOCKFISHCONNECTOR_H

#include "ChessGame.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <atlconv.h>

STARTUPINFO sti = { 0 };
SECURITY_ATTRIBUTES sats = { 0 };
PROCESS_INFORMATION pi = { 0 };
HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
BYTE buffer[2048];
DWORD writ, excode, read, available;


void ConnectToEngine();

std::string getNextMove(std::string position);

void CloseConnection();

#endif STOCKFISHCONNECTOR_H
