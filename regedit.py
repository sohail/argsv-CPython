#!/usr/bin/python -O

""" Allah, The ONE and The ONLY ONE 
    Who created Hydrogen and Hellium
    and made Energy and Matter commutative(E=MC^2) """ 
    

""" 
          regexpr, edit a file with regular expression[s] 
                Written by, Sohail Qayum Malik
  --------------------------------------------------------------
    ----------------------------------------------------------
      ------------------------------------------------------

    A general purpose script to clean text files from what ever qualifies as
    nasty. 
    The nastiness of text is determined by one or more regular expressions

    Example usage:- ...
          $ regedit file [file ...] reg exp [exp ...]
          $ regedit foo bar baz reg '/b//' '/ar/bar/g' text 
"""
class expression(object):
   
   def __init__(self, token, s_arg, t_arg=False, d_arg=False, g_arg=False):

      self.token = token
      self.t_arg = t_arg
      self.d_arg = d_arg
      self.g_arg = g_arg

      return None

   def apply(self, regs):

      if self.t_arg:
         try:
            """
            expr is a class, it understands the regular expressions very well either the expr class will compile the regular experssion, or else provides the set of methods, which others will use and compile the regular expression. The re uses the expr class to find the sub string. The regexpr.REGEXPR_COMPILE_FLAG makes it compile the pattern. o is an instance of pattern class
            """
	    o = regexpr.expr(regs, regexpr.REGEXPR_COMPILE_FLAG | regexpr.REGEXPR_FOO_FLAG)
            """
            regexpr.REGEXPR_COMPILE_FLAG is not given, to compile regular rexpression make explicit call to 'compile' method seperately
            """
            #o = regexpr.expr(regs, regexpr.REGEXPR_FOO_FLAG)
            
            """
            will compile the previously given pattern(regular expression), if it is already not compiled and returns an instance of dict data type. The key/value pair of dict uses an instance of keys class as a key and value is a single character of the given pattern(regular expression)
            
            """
            # Supports sq_length(). Returns Py_ssize_t 
            # print len(o) 
            # Supports sq_concat(). Returns PyObject* 
            # o + o;
            # Supports sq_repeat(). Returns PyObject*
            # o * 3;
            # Supports sq_item(). Returns PyObject*
            # o[10];
            # Supports sq_slice(). Returns PyObject*
            # o[1:9];
            # Supports sq_contains(). Returns int 
            # 1 is returned when n is in o, otherwise 0 is retuned. It is expected here that on error -1 will be returned and exception is set
            if (10 in o):
                print "Found"
            else:
                print "Not found"

	    dict = o.compile()

            """
            o is an instance of pattern class and it is iterable, x is an instance of keys class and each instance is key of key/value pair 
            """
            for x in o:
                print x
                print dict[x]
            # x and instance of keys. dict = dict.keys()
	    #for x in dict:
               # Processing instance of keys
               #print x()
               # x is an instance of keys. It is used as a key in the dict
               # dict[x] returns a value(dict is a set of key/value pairs
	       #print dict[x]
               #print x

	    #dict2 = o.compile()
            #for x in dict:
            #   print x 
	    #   print dict[x]

       		    
	    #o = regexpr.expr(regs)
	    #o.compile("Hey Dude", regexpr.REGEXPR_COMPILE_FLAG | regexpr.REGEXPR_FOO_FLAG)
	    #o.compile('\0', regexpr.REGEXPR_COMPILE_FLAG)
	    #print "Getting the dict"

            #dict = o.compile('Jill')
	    #dict = o.compile('Jill', regexpr.REGEXPR_COMPILE_FLAG)

	    #dict = o.compile("Sohail", regexpr.REGEXPR_FOO_FLAG)

	    #dict = o.compile("ST", regexpr.REGEXPR_COMPILE_FLAG)

	    #for x in dict.keys():
            #   print x		    
            #   print dict[x]

            #for x in dict.items():
            #   print x		    

            # arg also supports the sequence protocol 
	    #for arg in o:
	    #   arg(phrase="Hello world", pattern="Bye world")
	    #print len(o)
	    #o = o * 10
	    #print o + "ssss"
	    #m = regexpr.re(o, self.token)
            #m()
	 except (regexpr.error, TypeError), e:
            print e		 
		  
           		    
      return self.token	   


def help():
   print "Example usage: ... regedit file [file ...] reg expr [expr ...] [s txt] [d] [g] [text]"
   print "file, name of a file. expr, a regular expression. txt, substitute text. d, is delete and g is for global"
   print "regedit, written by, http://sohail.github.io"
   return None
    	   			       	
def main():

   # Module argsv, class argsv and instanciate it   	
   args = argsv.argsv(sys.argv, "?,-?,/?,h,-h,help,--help#reg#s,-s,substitute,--substitute#d,-d,delete,--delete#g,-g,global,--global#t,-t,text,--text")

   #files = None # If text command is given then it is line of text else
                 # a list holding names of files
   r_arg = None  # Regular expression 
   h_arg = None  # Help 
   s_arg = None  # Substitute
   d_arg = False # Delete
   g_arg = False # Global
   t_arg = None  # Text

   for arg in args:
      h_arg = arg["help"]
      if h_arg:
         print str(h_arg) + " " + str(len(h_arg))	      
	 help()
         return 1

   for arg in args:
      t_arg = arg["text"]
      if t_arg:
         break	      

   if not t_arg:
      files = args(0)
      if not files[1:]:
         help()
	 return 1
   else :
      try:	   
         files = args(0)[1]
      except IndexError:
         help()    
         return 1 
      for txt in args(0)[2:]:
         files = files + " " + txt

   for arg in args:
      r_arg = arg["reg"]
      if r_arg:
         break
    
   if not r_arg or len(r_arg) == 1 and len(r_arg(1)[1:]) == 0:
      help()
      return 1

   for arg in args:
      s_arg = arg["s"]
      if s_arg:
         if len(s_arg(1)) < 2:
            print str(s_arg) + " " + str(len(s_arg))		 
            help()
	    return 1
         else: 
            break 

   for arg in args:
      if arg["d"]:
	 d_arg = True      
         break

   for arg in args:
      if arg["g"]:
         g_arg = True
         break

   i = 1

   if t_arg:
      if s_arg:
         expr = expression(files, s_arg(1)[1], True, d_arg, g_arg)
      else:
	 expr = expression(files, None, True, d_arg, g_arg)

      while i <= len(r_arg):
	 for r in r_arg(i)[1:]:
	    if not expr.apply(r):
	       i = len(r_arg) + 1
	       break	       
	 i = i + 1

   else:      	         	   	   
      for f in files[1:]:
	 if s_arg:    
	    expr = expression(f, s_arg(1)[1], False, d_arg, g_arg)
	 else:
            expr = expression(f, None, False, d_arg, g_arg) 

	 while i <= len(r_arg):
            for r in r_arg(i)[1:]:
	        if not expr.apply(r):
	       	    i = len(r_arg) + 1
	       	    break
                i = i + 1
          
	 i = 1 
	            	   
   return 0	   
    	 	      
"""
Every module has a name and statements in a module can find out the name of its
module. This is specially handy in one particular situation - when a module is
imported for the first time, the main block in that module is run. What if we 
want to run the block only if the program was used by itself and not when it 
was imported from another module? This can be achieved using the __name__ 
attribute of the module.
"""
if __name__ == "__main__":

   import sys

   try:

      import argsv, regexpr

      # single exit point, main()'s return value will be our exit status
      sys.exit(main())
   except (ImportError), e:
      print e	   
	