
[![DOI](https://zenodo.org/badge/151296032.svg)](https://doi.org/10.5281/zenodo.15365309)


# T2K ECAL/LI Control Software

Software for controlling the Light Injection (LI) system of the T2K/ND280 Electromagnetic Calorimeter (ECAL).

## Author 

Prof. Costas Andreopoulos <c.andreopoulos \at cern.ch>

University of Liverpool

## LI system and flash sequence configuration via ODB 

The client reads the *LI system configuration* and the *flash sequence* from two ODB tables.
These tables live in **/Config/ECAL/LI/Controller/** and **/Config/ECAL/LI/Sequence/**
and they need to be created by the user (instructions are given at the end of this REAME).

The structure of the tables is the following:

<pre>
    /Config/ECAL/LI/Controller/
			|
			|-------> string IPAddrress [MAX_NCARDS]
			|-------> int    Port       [MAX_NCARDS]
</pre>

and

<pre>
    /Config/ECAL/LI/Sequence/
			|
			|-------> unsigned short int NFlashesPerSetting
			|-------> unsigned short int NTrigAppended 
			|-------> unsigned short int NSettings 
			|-------> unsigned char      Amplitute  [MAX_NSETTINGS]
			|-------> unsigned char      Pattern    [MAX_NSETTINGS]
			|-------> unsigned short int PulserMask [MAX_NSETTINGS]
</pre>

The definition and use of the controller and flash sequence variables is given below:

- **MAX_NCARDS**: The maximum number of LI cards in the system.   

The value should should match *kNumOfLIControlCards* defined in ECalLiDefs.h

- **MAX_NSETTINGS**: The maximum number of LI frash sequence settings.  

   The value should match *kMaxNumOfFlashSeqSet* defined in ECalLiDefs.h

   The value of **MAX_NSETTINGS** also controls the TCP/IP mesg size sent to each LI card -
   TCP/IP mesg size in bytes = (MAX_NSETTINGS+4)*2 
   
   If you modify **MAX_NSETTINGS** then the LI card firmware needs to be updated too so that the mesg sizes are in sync.

- **IPAddress** and **Port**: Define the address of a LI card.
  
  The **IPAddress** string size should match *kHostNameStrLen* defined in ECalLiDefs.h.

The **IPAddrress** array can include null addresses. The corresponding card slots will be disabled.

- **NFlashesPerSetting**: The number of triggers to flash for each setting. 
  
  It has the same value for all settings & cards.

- **NTrigAppended**: The number of triggers to append to the end of each setting to allow time for control card to reconfigure. 
  
  These triggers are not entered in the LI histograms.
  Therefore, total number of triggers at each setting is **NFlashesPerSetting** + **NTrigAppended**.
  
  **NTrigAppended** has the same value for all settings & cards.
  
- **NSettings**: The number of settings in the list to follow ( <= **MAX_NSETTINGS** )

- **Amplitute**: The LED intensity.
  
  Is a single-byte unsigned int which defines the light-level on a scale of 0-255

- **Pattern**: The LED group flashed.
  
  Is a bit-coded single-byte field.

  Pattern is to be interpreted by the LI card.

  Example for the down-stream ECAL:
  B0: North, B1: East, B2: South, B3: West, B4-7: unused.
  BO is the LSF bit. Any combination of bits can be set.

- **PulserMark**: A bit-mask which can be used to define which cards sees which settings.
  
  For example, to send setting k to cards 0, 1 and 5 only set:
  PulserMask[k] = (1<<0) + (1<<1) +  (1<<5) = 1 + 2 + 32 = 35.


## LI system state information in ODB 

The client periodically updates an ECAL LI state table in the ODB.
This table lives in **/State/ECAL/LI/** and it is created automatically by the controler.

Its structure is the following:

<pre>
    /State/ECAL/LI/
			|
			|-------> unsigned char       IsActive          [MAX_NCARDS]
			|-------> unsigned short int  CurrSetting       [MAX_NCARDS]
			|-------> unsigned short int  CurrAmplitude     [MAX_NCARDS]
			|-------> unsigned short int  CurrPattern       [MAX_NCARDS]
			|-------> unsigned short int  CurrNTrigInSetting[MAX_NCARDS]
			|-------> unsigned long  int  CurrNTrigTotal    [MAX_NCARDS]
			|-------> unsigned short int  GlobalCurrSetting       
			|-------> unsigned short int  GlobalCurrNTrigInSetting

</pre>

where:

- **IsActive**: Is 1 if the card responded last time it was pinged or was queried for its flash progress.

- **CurrSetting**: Is the current flashing setting for the given card.

- **CurrAmplitude**: Is the current flashing amplitude for the given card.

- **CurrPattern**: Is the current flashing pattern for the given card.

- **CurrNTrigInSetting**: Is the current number of triggers flashed in the current setting for the given card.

- **CurrNTrigTotal**: Is the total number of triggers flashed in the current run for the given card.

- **GlobCurrSetting**: Is the *global* current flashing setting in the input sequence.

  Typically, all cards should be at the same setting. If the program queried while cards
  were transitioning between settings then they can be different by 1. In this latter case
  the earliest setting is returned. 

- **GlobCurrNTrigInSetting**: Is the *global* number of triggers flashed in the current sequence.

  Typically, all cards report the same number.
  If all cards are at the same setting, then the contoller queries all cards and saves the
  smallest number of triggers. If cards are not at the same setting, then the contoller 
  considers only the cards at the earliest setting and saves the smallest number of triggers.
  If the spread in the number of triggers is > *kMaxNTrigSpread*  (defined in ECalLiDefs.h)
  then the controller goes on error.


# Creating the required ODB tables

To create the required input ODB tables (**/Config/ECAL/LI/Controller/** and **/Config/ECAL/LI/Sequence/**)
make sure that array sizes and variable type lengths match the ones decalared in ECalLiDefs.h

For example, to create the structure for:

- kHostNameStrLen      = 16
- kNumOfLIControlCards = 8
- kMaxNumOfFlashSeqSet = 50

type the following:

<pre>
    $ odbedit
    [local:Default:S]/>          mkdir Config
    [local:Default:S]/>          cd Config
    [local:Default:S]Config>     mkdir ECAL
    [local:Default:S]Config>     cd ECAL
    [local:Default:S]ECAL>       mkdir LI
    [local:Default:S]ECAL>       cd LI
    [local:Default:S]LI>         mkdir Controller
    [local:Default:S]LI>         cd Controller
    [local:Default:S]Controller> create string IPAddress[8]
    String length [32]: 16
    [local:Default:S]Controller> create int Port[8]    
    [local:Default:S]Controller> cd ..
    [local:Default:S]LI>         mkdir Sequence
    [local:Default:S]LI>         cd Sequence
    [local:Default:S]Sequence>   create WORD NFlashesPerSetting
    [local:Default:S]Sequence>   create WORD NTrigAppended            
    [local:Default:S]Sequence>   create WORD NSettings     
    [local:Default:S]Sequence>   create WORD PulserMask[50]
    [local:Default:S]Sequence>   create WORD Amplitude[50]  
    [local:Default:S]Sequence>   create WORD Pattern[50]  
    [local:Default:S]Sequence>   cd ..
    [local:Default:S]LI>         mkdir Sequence
    [local:Default:S]LI>         cd Sequence
</pre>


# Setting up the environment

*shell%* **source** /path/to/setup.sh

The following shows the contents of my setup.sh script. Set yours accordingly.

<pre>
!/bin/bash

export ECALLI=/home/hep/candreop/LI/controller/dev/
export MIDASSYS=/home/hep/candreop/LI/midas/svn_head/
export MIDAS_DIR=/home/hep/candreop/LI/midas/exp/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ECALLI}
export PATH=${PATH}:${MIDASSYS}/linux/bin/:${ECALLI}
</pre>


# Configuring and building the LI controller

To configure the LI controller software, run the 'configure' script.

For help, type:   

*shell%* **./configure --help**

You can run the configure script as follows:

*shell%* **./configure --arch=**linux **--with-midas-inc=**/some/path **--with-midas-lib=**/some/path

The MIDAS paths are auto-detected, so it may sufficient to type:

*shell%* **./configure --arch=**linux 


Once the installer is configured, build the LI controller by typing:

*shell%% **gmake**


# Running the LI controller

- Open a terminal and start-up the MIDAS http server.

<pre>
    shell% echo $HOSTNAME              || use that name in next step)
    shell% mhttp -p port_nu            || eg, shell$ mhttp -p 8080
</pre>
  
- Fire-up your web browser and connect to the MIDAS https server (at *hostname:port_nu*) 

- Open a terminal, start the ODB editor and cd to the ECALLI **Controller** and **Sequence** directories.
  Set the variables within these directories to control the LI client.

  Example - Set a card configuration:

<pre>
    $ odbedit
    [local:Default:R]/>cd /Config/ECAL/LI/Controller/
    [local:Default:S]Controller> set IPAddress[0] "168.120.10.101"
    [local:Default:S]Controller> set IPAddress[1] "168.120.10.102"
    [local:Default:S]Controller> set Port[0] 9090
    [local:Default:S]Controller> set Port[1] 9090
    ...
</pre>

  Example - Set a flash sequence:

<pre>
    $ odbedit
    [local:Default:R]/>cd /Config/ECAL/LI/Sequence/
    [local:Default:S]Sequence>set NFlashesPerSetting 8000
    [local:Default:S]Sequence>set NTrigAppended 1000
    [local:Default:S]Sequence>set NSettings 10  
    [local:Default:S]Sequence>set Amplitude[0] 100
    [local:Default:S]Sequence>set Amplitude[1] 120
    [local:Default:S]Sequence>set Amplitude[2] 220
    [local:Default:S]Sequence>set Amplitude[3]  30 
    [local:Default:S]Sequence>set Amplitude[4]  55
    ...
    ...
    [local:Default:S]Sequence>set Pattern[0] 1
    [local:Default:S]Sequence>set Pattern[1] 2
    [local:Default:S]Sequence>set Pattern[2] 12
    [local:Default:S]Sequence>set Pattern[3] 12
    [local:Default:S]Sequence>set Pattern[4] 10
    ...
    ...
</pre>

- Open a terminal and start up the ECAL LI controller.

<pre>
    shell$ ./pulser_controller
</pre>

- Start/stop runs, as needed, using the web browser


# Other notes and recent changes

- June 21, 2013:
  The Aplitude, Pattern and IsActive fields used to be of BYTE type. 
  They were changed to WORD on Giles' request.
  To toggle between the new/old behavious use the kODBNoBYTE flag in ECalLiDefs.h
