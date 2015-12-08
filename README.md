INTRODUCTION
------------
This is the EPICS support module for the
MKS 9xx Gauge Controller. Providing support for the following gauges: MKS 901, 901P, 925, 971B, 972B, 974B

The conroller utilises a RS232 interface for serial communications. The default
framing is:

9600 baud
8 data bits
1 stop bits
No parity

The specification is based on MKS document:
P/N: 100017121
901P Loadlock Transducer
Operation and Installation Manual
Revision: F, January 2012

USAGE
-----


One template is provided here:

1) mks9xx.template
This instatiates all the records associated with the controller's common

   
The user interface is presently available via EDM screens. 
Starting with the root screen: 
mks9xx.edl
mks9xxDetail.edl
mks9xxGauge.edl
mks9xxInfo.edl
mks9xxSetup.edl

MAINTAINERS
-----------

Current maintainers:
* Ian Gillingham (Diamond Light Source) - ian.gillingham@diamond.ac.uk

First created December 2014.


