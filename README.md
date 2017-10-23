# Linux Shell

By: Thomas Bowlin  
For EKU CSC400, Fall 2017  
Dr. Zhang  

## System Overview

For Operating Systems class, we were tasked with creating a simple command line shell that works in Linux Ubuntu. The system was developed in a VirtualBox Ubuntu 17.10 installation, completely in the C programming language.  

Technologies: Ubuntu, Vim, C

## System Requirements

The shell had to support the following commands:
* cd <directory>
* clr
* dir <directory>
* echo <comment>
* quit

Additionally, the shell had to interpret all other commands as program invocation, which is done by forking and executing the new program as a child process. 
