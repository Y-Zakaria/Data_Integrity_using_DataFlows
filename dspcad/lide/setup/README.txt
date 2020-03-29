This directory provides files for bootstrapping of the LIDE setup.
===============================================================================
Here is the instructions for LIDE setup:

To set up (install) the LIDE software, please follow the following
steps in a Bash shell. The setup process will install LIDE from the
lidepub repository.

1. Change your working directory to lide directory in the lidepub repository.

2. Execute the following command to install the software:

source setup/setup

3. The command above will by default create a dspcad_user directory directly
within your home directory. The lide_user directory are in the dspcad_user 
directory. 

4. If you have your customized lide_user directory for LIDE setup. Please set 
the following variable before you execute the command for setting up LIDE. 

export LIDEUSER=<path to lide_user directory>