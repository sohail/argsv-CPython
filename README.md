**argsv-cpython**    
_README, last updated on :- 26th of November, 2019_

argsv for CPython(a CPython module). An effort to process the command line arguments with ease and style.  
---

"Command-line options represent the \"face\" of the program, and tools for options parsing should be convenient and easy to use.". C/C++ Users Journal November, 2004.

This argsv-cpython is capable of handling short and long commands. It is capable of handling multiple instances of same short or long command. argsv-cpython allows many command names for the same command e.g "?,/?,h,-h,help,--help"(This comma delimited list represents few names of the one single command). argsv-cpython module allows the use of funtions print, str and len on its type instances. 

Please Note:- Use this module with the versions of Python earlier than 3.0. **Folder argsv-cpython-Python3.9 has the working implementation of this program much recent version of Python. I tested it with 3.8 and 3.9.0-alpha1**.  


Example Application.
-----------------------
Few \"example applications\"(**test_argsv.py**, **regedit.py**) are part of the committed source code, use it as a usage guide and to test the smarts of argsv-cpython module.


```python

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

```