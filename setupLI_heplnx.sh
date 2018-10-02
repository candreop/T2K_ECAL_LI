#!/bin/bash

export ECALLI=${PWD}

export DAQ_BACKEND_HOST=localhost
export DAQ_EXPERIMENT=

export MIDAS_DIR=/opt/ppd/t2k/softw/LI/midas/
export MIDASSYS=$MIDAS_DIR/svn_head/
export MIDAS_EXPTAB=$MIDAS_DIR/exp/
export EXPTAB=$MIDAS_DIR/exp/

export LD_LIBRARY_PATH=${ECALLI}:${LD_LIBRARY_PATH}:${MIDASSYS}/linux/lib/
export PATH=.:${ECALLI}:${PATH}:${MIDASSYS}/linux/bin/

