export ECALLI=$PWD

export DAQ_BACKEND_HOST=t2krallogger
export DAQ_EXPERIMENT=VSLICE_DAQ

export MIDASSYS=/home/t2kdaq/midas/vslice
export EXP_DIR=$MIDASSYS/backend
export MIDAS_EXPTAB=$EXP_DIR/exptab
export MIDAS_BINDIR=$MIDASSYS/bin
export MIDAS_LIBDIR=$MIDASSYS/lib
export MIDAS_INCDIR=$MIDASSYS/include

export PATH=.:$MIDAS_BINDIR:$PATH
export LD_LIBRARY_PATH=$MIDAS_LIBDIR:$LD_LIBRARY_PATH
