#!/bin/sh

source ./setenv.sh

export PATH=${PATH}:${TAOX11_BASE_ROOT}/bin:${TAOX11_BASE_ROOT}/taox11/bin
export MPC_ROOT=${TAOX11_BASE_ROOT}/ACE/MPC
export TAOX11_ROOT=${TAOX11_BASE_ROOT}/taox11
export TAO_ROOT=${TAOX11_BASE_ROOT}/ACE/TAO
export ACE_ROOT=${TAOX11_BASE_ROOT}/ACE/ACE


mwc.pl -type gnuace
make -f GNUmakefile


