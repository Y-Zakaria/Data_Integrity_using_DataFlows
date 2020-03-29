#!/usr/bin/env bash

set -a
export UXARCH=lin
export REPODSPCAD="/home/zakaria2/Desktop/Fault_Data_Detection_RTOS/m2lse2019"
export REPHOME="$REPODSPCAD/elyahyoui"
export REPDRIV="$REPODSPCAD/elyahyoui/sum8_Integrity_drivers/sum8_Tiger_SimSched"
export REPSUM8="$REPODSPCAD/dspcad/lide/lang/c/src/gems/actors/sum8Tiger"
export REPDEMO="$REPODSPCAD/elyahyoui/sum8_Integrity_drivers/test_demo1"


cd $REPDRIV
rm -rf *.o *.d *.exe

cd $REPSUM8
rm -rf *.o *.d

cd $REPSUM8
./makeme

cd $REPDRIV
./makeme

#cd $REPDEMO 
#./runme