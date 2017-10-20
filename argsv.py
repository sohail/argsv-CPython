from distutils.core import setup, Extension
# argsv, module name
setup(name="argsv", version="1.0", 
		ext_modules=[
	           # Take help on Extension module		
	           # To toggle between cross compilation comment or uncomment
		   # the following lines
		   Extension("argsv", ["argsv.c"])])		   
		   #Extension("argsv", ["argsv.c"], extra_compile_args=["-m32"], extra_link_args=["--format=elf32-i386", "-m elf-i386", "-oformat elf32-i386"])])
