#!/bin/zsh
lldb -- ./build/PlanetaryObservatory <<'LLDB'
process launch --stop-at-entry 0
run
bt all
quit
LLDB
