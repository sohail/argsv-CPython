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

    for arg in args:
      # The mapped key can be ?, -?, /?, h, -h, help, or --help
      # The h has same type as arg or args has but it refernces a new type 
      # instance
      #print ("hello")
      h=arg[b"/?"] # h=None when mapping fails, else h is a type instance 
      if h:   # If h is given(found) then next statement must be break 
	            # otherwise at next iteration h will be None(assigned None to it)
          print("h is given")
          break

    del args

    if h != None:
      # This will put out all the names of the command, the str(h) also 
      # works and returns same names as a python string(possibly for assignment)
       print(h)
    else:
       print ("!help")

    # Because internally arg and args both reference the same type instance
    args = arg

    for arg in args:
      d=arg[b"dir"]
      if d:         	      
         break

    if d:
       i=1 # Assigned 1 and not 0, command instances origin at 1
           # Arguments common to all commands(and all of their instances) 
	         # origin at 0(I know it is confusing just keep on reading and most 
	         # probably things will get bit more clear)
           # argsv module can handle multiple instances of the same short or long 
           # command, each instance is returned as a list
       while i <= len(d):

         #print (len(d))
         # l will be None if i is more than len(d)	      
         l=d(i)
         for arg in l:
            print (arg)
         del l # l is merely a python list, del'ing it here is most probably unnecessary
         i = i + 1
    else:
       print ("No dir")   


    # The list of all common command line arguments 
    i=0
    # This can never be None when i is 0, because the list atleast will have
    # a single item and that will be the name of this script(this statement is
    # is relative)
    l=args(i)
    for arg in l:
        print (arg)
    del l 

    del arg
    del args
 
except (ImportError, MemoryError, TypeError) as e:
   print (e)        


