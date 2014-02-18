PP      = wpp386
PFLAGS  = -xs -oneatxrl+ -bt=dos -I..\midas\src\midas\ -zq

all: jaa.exe

jaa.exe: jaa.obj
        wlink @keuhko.lnk

jaa.obj: jaa.cpp
        $(PP) $(PFLAGS) jaa.cpp

