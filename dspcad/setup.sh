#!/usr/bin/env bash
set -a
export UXARCH=lin
export REPODSPCAD="/home/hntran/Dev/project/UBO/2019/m2lse2019/dspcad"
export UXDICE="$REPODSPCAD/dice"
export UXLIDE="$REPODSPCAD/lide"
export DICEUSER="$REPODSPCAD/dice_user"
export LIDEUSER="$REPODSPCAD/lide_user"

source "$UXDICE"/setup/runme

pushd $DICEUSER
source startup/dice_startup
popd

pushd $UXLIDE
echo $LIDEUSER
source setup/setup
popd

pushd $LIDEUSER
source startup/lide_startup
popd
