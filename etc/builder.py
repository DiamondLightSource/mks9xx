from iocbuilder import AutoSubstitution, Device
from iocbuilder.arginfo import *
from iocbuilder.modules.asyn import _AsynOctetInterface
from iocbuilder.modules.asynaid import Asynaid

class _mks9xxTemplate(AutoSubstitution):
    TemplateFile = "mks9xx.template"

class mks9xx(_mks9xxTemplate, Device):
    Dependencies = (Asynaid,)
    UniqueName = "PORT"
    LibFileList = ['mks9xx']
    DbdFileList = ['mks9xx']
    def __init__(self, serialport, serialaddr=0, **args):    
        self.__super.__init__(**args)
        self.PORT = args["PORT"]
        self.serialport = serialport
        self.serialaddr = serialaddr

    def Initialise(self):
        print '# mks9xxConfig(port, serialport, serialaddr)'
        print 'mks9xxConfig("%(PORT)s", "%(serialport)s", %(serialaddr)d)' % self.__dict__
        
    ArgInfo = _mks9xxTemplate.ArgInfo + makeArgInfo(__init__,
        serialport = Ident("Asyn IP port or serial port name", _AsynOctetInterface),
        serialaddr = Simple("Address of serial port, normally 0", int))
        

