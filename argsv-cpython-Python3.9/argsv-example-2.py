"""
   args-example-2.py

   This script stress tests and demonstrates smarts(given if there are any) 
   of argsv module. argsv module is capable of handling short and long 
   commands. argsv module is capable of handling multiple instances of same
   short or long command. argsv module allows many command names for the
   same command e.g "?,/?,h,-h,help,--help"(this comma delimited list 
   represents few names of the one single command). argsv module allows
   the use of functions print, str and len on its type instances. 
  ---------------------------------------------------------------------------

  [Super glue]
  For me the Python programming language is a glue language. I divide the problem at hand in modules, then write each module in CPython and use the script written in Python to bind or glue the CPython modules together. This way I've the best of both worlds. The ease of using the C language, with the facilities which the C++ provides(The CPython's per object data and per object type), then the benefits of using the non statically type language.

"""

import sys

try:
   import argsv

   # argsv module allows several names of one single command
   # Here we are asking argsv to process three different commands
   args=argsv.argsv(sys.argv, "?,-?,/?,h,-h,help,--help#d,-d,dir,--dir#v,version")

   d=None 

   for arg in args:
      # The mapped key can be ?, -?, /?, h, -h, help, or --help
      # The h has same type as arg or args has but it refernces a new type 
      # instance
      #print ("hello")
      d=arg[b"d"] # h=None when mapping fails, else h is a type instance 
      if d:   # If h is given(found) then next statement must be break 
	            # otherwise at next iteration h will be None(assigned None to it)
         print("d was given")
         break

   del args

   # Now process the first instance of d, and all above detail is applied here
   # all over again
   if d:       
       l=d(1)
       args=argsv.argsv(l,"l,-l#s,-s")
       print (args)
       k=args(0)
       if k != None: # Optional, the list k will atleast have a single item
	                 # and that is... out of many names of d command, the one 
		             # name you put on the command line
	       for arg in k:
               print (arg)

except (ImportError, MemoryError, TypeError) as e:
    print (e)    