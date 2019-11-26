# May Allah help us all.

import sys

try:
    import argsv

    # argsv module allows several names of one single command
    # Here we are asking argsv to process three different commands
    args=argsv.argsv(sys.argv, "?,-?,/?,h,-h,help,--help#d,-d,dir,--dir#v,version")
    
    # str(args) throws TypeError exception when no command line arguments
    # are given at command line of this application
    print ("All commands at CL are --> " + str(args))

except (ImportError, MemoryError, TypeError) as e:
   print (e)        


