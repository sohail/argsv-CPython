/* ****
 * Allah, The ONE and the ONLY ONE and ALL MIGHTY.
 * *******************************************************************
 * argsv.c
 * Written by, Sohail Qayum Malik
 * ******* */

#include "argsv.h"

/* Forward declarations */
static PyTypeObject argsv;

/* ------------------------------------------------------------------------ */
/*              PyTypeObject argsv, functions start here                    */
/* ------------------------------------------------------------------------ */

static void argsv_PyTypeObject_destructor(argsv_object *self)
{  
   struct commands_list *clnk = NULL;

   /* This could be problematic, I'm not certain yet */
   while((clnk=get_a_lnk((struct just_a_link **)&(self->cmds_lst), (struct just_a_link *)clnk)) != NULL)
   { 
      free_list((struct just_a_link **)&(clnk->map)); 
   }
       
   free_list((struct just_a_link **)&(self->cmds_lst));

   PyObject_Del(self);
}


static int argsv_PyTypeObject_printfunc(argsv_object *self, FILE *fp, int flags)
{
   Py_ssize_t i = 0;	 
   struct commands_list *clnk = NULL;

   while ((clnk=get_a_lnk((struct just_a_link **)&(self->cmds_lst), (struct just_a_link *)clnk)) != NULL)
   {
      /* I die a little, when ever I have to put statements like these */
      if (i > 0) 
         fprintf(fp, "%c", COMMANDS_DELIMITER);	      
      for (i = 0;  i < clnk->cmds_size; i++)
         fprintf(fp, "%c", clnk->cmds[i]);	            
   }	   
   	
   return 0;	
}

static PyObject *argsv_PyTypeObject_reprfunc(argsv_object *self)
{
   char *cmds = NULL;

   //size_t size;

   struct commands_list *clnk = NULL;

   char *cmds_delimiter = NULL;
 
   cmds_delimiter = reallocate_mem_for_string(&cmds_delimiter, " ", 1);
   if (cmds_delimiter == NULL)
      return NULL; 

   cmds_delimiter[0] = COMMANDS_DELIMITER;

   while ((clnk=get_a_lnk((struct just_a_link **)&(self->cmds_lst), \
				  (struct just_a_link *)clnk)) != NULL) {

      if (cmds != NULL) {
         cmds = reallocate_mem_for_string(&cmds, cmds_delimiter, \
			                          strlen(cmds_delimiter));
	 if (cmds == NULL)
            return NULL;		 
      }	 

      cmds = reallocate_mem_for_string(&cmds, clnk->cmds, clnk->cmds_size);
      if (cmds == NULL)
         return NULL;	      
   }

   free(cmds_delimiter);

   //printf("----> %s\n", cmds);

   return Py_BuildValue("s", cmds);
}

/* Okhey now lets be clear about one thing here, the following code is ugly 
   I mean really really ugly */
static PyObject *argsv_PyTypeObject_ternaryfunc(argsv_object *self, \
		                     PyObject *args, PyObject *keywords) {
   int idx = 0, i = 0, j;	
   PyObject *ptr = NULL;

   struct map *mlnk = NULL;
   struct commands_list *clnk = NULL;

   if (keywords != NULL) {
      PyErr_SetString(PyExc_TypeError, \
		           "ternaryfunc(): dict keywords is non null");
      return NULL;
   }

   if (!PyArg_ParseTuple(args, "i", &idx)) 
      return NULL;

   if (idx == 0) {

      return (PyObject *)PyList_GetSlice((PyObject *)self->argv, \
		                                  0, self->common_argc);
   }      
   else if (self->found == 1) {

      i = 0;	   

      while ((clnk=get_a_lnk((struct just_a_link **)&(self->cmds_lst), \
				    (struct just_a_link *)clnk)) != NULL) {

	 while ((mlnk=get_a_lnk((struct just_a_link **)&(clnk->map), \
				    (struct just_a_link *)mlnk)) != NULL) {            		    		 
            if (mlnk->f == 0) {

               i++;
	       if (i == idx) 
                  return (PyObject *)PyList_GetSlice((PyObject *)self->argv, mlnk->idx, mlnk->idx + mlnk->argc);		       
            }
	    else {

               for (j = 0; j < mlnk->f; j++) {
                  i++; 
		  if (i == idx) 
		     return (PyObject *)PyList_GetSlice((PyObject *)self->argv, mlnk->idx, mlnk->idx + mlnk->argc); 
               }		                      
            } 
         }		 
      }	      
   }

   Py_INCREF(Py_None); 	    
   return (PyObject *)Py_None; 	
}

static PyObject *argsv_PyTypeObject_getiterfunc(argsv_object *self) {

   /* printf("Got iterator\n"); */

   /* This is important at this point */
   self->clnk = NULL;

   Py_XINCREF(self);

   return (PyObject *)self;	
}

static PyObject *argsv_PyTypeObject_iternextfunc(argsv_object *self) {

   self->clnk = get_a_lnk((struct just_a_link **)&(self->cmds_lst), \
		                        (struct just_a_link *)(self->clnk));

   if (self->clnk == NULL) 
      return NULL;
         
   /* For debugging purposes */
   /* debug_map(self->clnk->map); */
 
   Py_XINCREF(self);

   return (PyObject *)self;	
}

static int argsv_PyTypeObject_initproc(argsv_object *self, \
		                       PyObject *args, PyObject *keywords) {

   self->found = 0;	
	
   self->common_argc = PyList_Size((PyObject *)(self->argv));

   //printf("%d\n", self->common_argc);

   self->cmds_lst = NULL;
   self->cmds_lst = build_commands_list(&(self->cmds_lst), \
		    self->argv, self->commands, strlen(self->commands), \
		                 0, self->common_argc, &(self->common_argc),\
				 REMOVE_EMPTY_MAPS);

   //printf("-----> List Size  = %d\n", PyList_GET_SIZE(self->argv));       

   /* for (int k = 0; k < PyList_GET_SIZE(self->argv); k++)
   {
      PyObject* item = PyList_GET_ITEM(self->argv, k);

      char* str = PyBytes_AS_STRING(item);

      printf("tp_name = %s", item->ob_type->tp_name);

      printf("-----> str = %s\n", str);

      if (PyUnicode_Check(item) == TRUE)
      {
         //printf("It is Unicode\n");

         PyObject *foo = PyUnicode_AsASCIIString(item);

         str = PyBytes_AS_STRING(foo);

         printf("-----> %s\n", str);
      }
   } */   
   
   /* cmds_lst can be null, when there are no arguments given on the 
      command line then cmds_lst can be null 
      The other consequence is that cmds_lst will have only as many links
      as there are arguments on the command line */
   if (self->cmds_lst == NULL && PyErr_Occurred()) 
      return -1;
      
   /* For debugging purposes */
   /* debug_commands_list(self->cmds_lst); */

   return 0; 	
}

static PyObject *argsv_PyTypeObject_newfunc(PyTypeObject *ptr_argsv, \
				     PyObject *args, PyObject *keywords) {

   argsv_object *self = NULL;
   
   if (ptr_argsv != &argsv) {
      PyErr_SetString(PyExc_Exception, "argsv_new(): unusual call to PyTypeObject::tp_new, the self->ob_type will point to wrong instance of type PyTypeObject");
      return NULL;   
   }

   /* All the arguments should be in args tuple */
   if (!_PyArg_NoKeywords("argsv_new()", keywords)) {
      PyErr_SetString(PyExc_Exception, \
          "argsv_new(): function _PyArg_NoKeyWords() unexpectedly returned 0");
      return NULL;
   } 

   self = PyObject_New(argsv_object, &argsv);
   if (self == NULL) 
      return NULL;

   /* PyObject **p;
      p = va_arg(*p_va, PyObject **); // Get the address of argsv_object::argv
      *p = arg; // Put in the address of sys.argv in argsv_object::argv 
      What I want to emphasise/emphasize here that donot increment/decrement 
      the ref counts donot try to free/delete them, they are not yours just 
      look at the few statements of C part of these comments */
   if (!PyArg_ParseTuple(args, "Os", &(self->argv), &(self->commands))) 
      return NULL;
      	 
   if (!PyList_Check((PyObject *)(self->argv))) {	      
      PyErr_SetString(PyExc_Exception, \
		      "argsv_new(): argv is not an instance of type list"); 
      return NULL;
   }    
   
   if ((PyList_Size((PyObject *)(self->argv)) < 1) || !strlen(self->commands)) {
        PyErr_SetString(PyExc_Exception, \
			 "argsv_new(): please form arguments carefully");
       return NULL;
   }
     
   /* 
    * Some comments on self...
    * At this point the self->ob_refcnt is 1(believe me, it is),
    * (self->ob_type == ptr_argsv) is true,
    * The other two members the self->_ob_next, self->_ob_prev are pointers
    * to support doubly-linked list and are only part of self when Python
    * is compiled with Py_TRACE_REFS
    */
 
   return (PyObject *)self;   	
}

/* ---------------------------------------------------------------------- */
/*                 PyTypeObject argsv, functions END here                 */
/* ---------------------------------------------------------------------- */

static int argsv_nonzero(argsv_object *self) {
	
   return self->found; /* 0 when Not found, otherwise 1 */
}

static PyNumberMethods argsv_as_number = {

   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   /*(inquiry)argsv_nonzero,*/
   (unaryfunc)argsv_nonzero,
};

static Py_ssize_t argsv_PyMappingMethods_lenfunc(argsv_object *self) {

   Py_ssize_t f = 0;

   struct commands_list *clnk = NULL;
   struct map *mlnk = NULL;

   while ((clnk=get_a_lnk((struct just_a_link **)&(self->cmds_lst), (struct just_a_link *)clnk)) != NULL) {

      while ((mlnk=get_a_lnk((struct just_a_link **)&(clnk->map), (struct just_a_link *)mlnk)) != NULL) {

	 f++;

	 if (mlnk->f > 0)
	    f += (mlnk->f - 1);	 
      }
   }

   if (self->found == 0)
      f+=self->common_argc;	   

   return f;	
}

static PyObject *argsv_PyMappingMethods_binaryfunc(argsv_object *self, \
		                                          PyObject *item) {

   argsv_object *new_self;
   struct commands_list *clnk;
   struct map *outer_mlnk = NULL, *inner_mlnk;

   /*char *command = PyString_AsString(item);*/
   char *command = PyBytes_AS_STRING(item);
   
   char *str_outer = command, *str_inner = NULL;

   size_t str_outer_size = 0, str_inner_size = 0;
 
   if (self->clnk == NULL) {

      PyErr_SetString(PyExc_Exception, "argsv_PyMappingMethods_binaryfunc(): mapping only possible during iteration");
      return NULL;  
   }

   str_inner = self->clnk->cmds;

   while((str_outer_size=parse(command, strlen(command), &str_outer, str_outer_size, COMMAND_DELIMITER)) != 0) {
        	   
      while((str_inner_size=parse(self->clnk->cmds, self->clnk->cmds_size, &str_inner, str_inner_size, COMMAND_DELIMITER)) != 0) {

	 if (str_outer_size == str_inner_size && !strncmp(str_outer, str_inner, str_inner_size)) 
            break;
	    
      }

      if (str_inner_size != 0) 
         break;
   }

   /* Found it, now do some thing with it */
   if (str_inner_size && str_outer_size) {

      new_self = PyObject_New(argsv_object, &argsv);

      /* Exception has alrady been generated, just pass it back to the 
         caller. The caller of the binaryfun() should deal with it very
	 carefully. The execution of the caller should be stoped */
      if (new_self == NULL) 
         return NULL;

      new_self->found = 1;
      new_self->argv = self->argv;
      new_self->common_argc = self->common_argc;
      new_self->commands = self->commands;
      new_self->cmds_lst = NULL;
      new_self->clnk = NULL;

      /* self->clnk has the link of commands_list for which we requested the
      current search */ 

      clnk = add_to_list((struct just_a_link **)&(new_self->cmds_lst), sizeof(struct commands_list));

      if (clnk == NULL)
         return NULL;

      clnk->cmds = self->clnk->cmds;
      clnk->cmds_size = self->clnk->cmds_size;
      clnk->map = NULL;

      while ((outer_mlnk=get_a_lnk((struct just_a_link **)&(self->clnk->map), (struct just_a_link *)outer_mlnk)) != NULL) {

	 inner_mlnk = add_to_list((struct just_a_link **)&(clnk->map), sizeof(struct map));

	 inner_mlnk->idx = outer_mlnk->idx;
	 inner_mlnk->argc = outer_mlnk->argc;
	 inner_mlnk->f = outer_mlnk->f;
      }

      /* For debugging purposes */
      /* debug_commands_list(new_self->cmds_lst); */
       
      Py_XINCREF(new_self);

      self = new_self;
   }
   else {

      Py_INCREF(Py_None);
      self = (argsv_object *)Py_None;
   }	   
     
   /* If command is not found, then self is same as object being iterated */    
   return (PyObject *)self;	
}	

/* Include/object.h */
static PyMappingMethods argsv_as_mapping = {

   (lenfunc)argsv_PyMappingMethods_lenfunc,
   (binaryfunc)argsv_PyMappingMethods_binaryfunc,
   (objobjargproc)0,
};

/* Include/object.h, Objects/typeobject.c */
/* A new class, this class will inherit from class object. The function 
 * PyType_Ready()(Objects/typeobject.c) will assign the address of 
 * PyBaseObject_Type(an instance of PyTypeObject Objects/typeobject.c) 
 * to PyTypeObject::tp_base. The PyTypeObject PyBaseObject { }; is our class
 * named object
 * The above detail can be summerised as following Python statements... 
 *    class argsv(object):
 *       def __init__(self):
 *          print "Hello World" */
static PyTypeObject argsv = {
   
   /* Include/object.h */	
   /*PyObject_HEAD_INIT(NULL)*/
   PyVarObject_HEAD_INIT(NULL, 0)
   /*0,*/						/* ob_size */
   "argsv.argsv",                            	/* tp_name, module.typename */ 
   sizeof(argsv_object),			/* sizeof(*self) */
   0,	  					/* tp_itemsize */
   /* Methods to implement standard operations(Include/object.h) */
   (destructor)argsv_PyTypeObject_destructor,	/* tp_dealloc */
   (printfunc)argsv_PyTypeObject_printfunc, 	/* tp_print */
   0, 						/* tp_getattr */
   0, 						/* tp_setattr */
   0,						/* tp_compare */
   0,						/* tp_repr */
   /* Methods suits for standard classes(Include/object.h) */ 
   &argsv_as_number,				/* tp_as_number, \
						       PyNumberMethods* */
   0, 						/* tp_as_sequence, 
						       PySequenceMethods* */
   &argsv_as_mapping,				/* tp_as_mapping, 
						       PyMappingMethods* */
   /*More standard operations(here for binary compatibility)(Include/object.h*/
   0,						/* tp_hash */
   (ternaryfunc)argsv_PyTypeObject_ternaryfunc, /* tp_call */
   (reprfunc)argsv_PyTypeObject_reprfunc,	/* tp_str */
   0,						/* tp_getattro */
   0,						/* tp_setattro */
   /* Functions to access object as input/output buffer(Include/object.h) */
   0,						/* tp_as_buffer, \
						        PyBufferProcs * */
   /*Flags to define presence of optional/expanded features(Include/object.h)*/
   Py_TPFLAGS_DEFAULT,				/* tp_flags */
   argsv_doc,					/* tp_doc, document string */
   /* Assigned meaning in 2.0 */
   /* a call function for all accessible objects(Include/object.h) */
   0,						/* tp_traverse */
   /* delete refrences to the contained objects(Include/object.h) */
   0,						/* tp_clear */
   /* Assigned meaning in release 2.1 */
   /* rich comparisons(Include/object.h) */
   0,						/* tp_richcompare */
   /* weak reference enabler */
   0,						/* tp_weaklistoffset */
   /* Addes in release 2.2 */
   /* Iterators */
   (getiterfunc)argsv_PyTypeObject_getiterfunc,	  /* tp_iter */
   (iternextfunc)argsv_PyTypeObject_iternextfunc, /* tp_iternext */
   /* Attribute descriptor and subclassing stuff */
   0,						/* tp_methods, PyMethodDef* */
   0,						/* tp_members, PyMemeberDef* */
   0,						/* tp_getset, PyGetSetDef* */
   0,						/* tp_base, _typeobject* */
   0,						/* tp_dict, PyObject */
   0,						/* tp_descr_get */
   0, 						/* tp_descr_set */
   0,						/* tp_dictoffset, Py_ssize_t */
   (initproc)argsv_PyTypeObject_initproc,	/* tp_init */
   0,						/* tp_alloc */
   argsv_PyTypeObject_newfunc,			/* tp_new */
   0,						/* tp_free, Low-Level \
						   free-memory routine */
   0,						/* tp_is_gc, For \
						   PyObject_IS_GC */
   0,						/* tp_bases, PyObject* */
   0,						/* tp_mro, method resolution \
						   order. PyObject* */
   0,						/* tp_cache, PyObject* */
   0,						/* tp_subclasses, PyObject* */
   0,						/* tp_weaklist, PyObject* */
   0,						/* tp_del */  
};

static PyMethodDef argsvMethods [] = {
   {NULL},	
};	

static void
add_define(PyObject *dict, const char *key, long value)
{
    PyObject *k = PyUnicode_FromString(key);
    PyObject *v = PyLong_FromLong(value);
    if (v && k) {
        PyDict_SetItem(dict, k, v);
    }
    Py_XDECREF(k);
    Py_XDECREF(v);
}

static struct PyModuleDef argsvmodule = {
    PyModuleDef_HEAD_INIT,
    "argsv",
    argsv_doc, 
    -1, 
    argsvMethods,
    NULL,
    NULL,
    NULL,
    NULL    
};

PyMODINIT_FUNC PyInit_argsv(void)
{

    /* This module */
   PyObject *module_argsv;

   if (PyType_Ready(&argsv) < 0)
      return NULL;	   

   /*
    * The name of the module, the module methods and any document string 
    * There is a one to one corresspondence between the name of the module,
    * the initialization function's name and finally the name of this file
    */    
   module_argsv = PyModule_Create(&argsvmodule);
   if (module_argsv == NULL)
       return NULL;

   /* Add the instance of TypeObject to the module dict */
   PyModule_AddObject(module_argsv, "argsv", (PyObject *)&argsv);

    /*PyObject *dict;
    PyObject *module = PyModule_Create(&argsvmodule);
    if (module == NULL)
        return NULL;
    dict = PyModule_GetDict(module);*/

    /*ADD_DEFINE(SND_ASYNC);
    ADD_DEFINE(SND_NODEFAULT);
    ADD_DEFINE(SND_NOSTOP);
    ADD_DEFINE(SND_NOWAIT);
    ADD_DEFINE(SND_ALIAS);
    ADD_DEFINE(SND_FILENAME);
    ADD_DEFINE(SND_MEMORY);
    ADD_DEFINE(SND_PURGE);
    ADD_DEFINE(SND_LOOP);
    ADD_DEFINE(SND_APPLICATION);

    ADD_DEFINE(MB_OK);
    ADD_DEFINE(MB_ICONASTERISK);
    ADD_DEFINE(MB_ICONEXCLAMATION);
    ADD_DEFINE(MB_ICONHAND);
    ADD_DEFINE(MB_ICONQUESTION);*/

    //PyModule_AddObject(module, "argsv", (PyObject *)&argsv);

    return module_argsv;
}

SORT_A_LIST(int);

/* Helper functions */
/* --------------------------------------------------------------------- */
/* HELPER FUNCTION, Functions related to allocation and deallocation mem */    
/* --------------------------------------------------------------------- */

char *reallocate_mem_for_string(char **ptrptr, char *str, \
		                                   Py_ssize_t str_size) {

   char *ptr = *ptrptr;

   Py_ssize_t ptr_size = (ptr != NULL) ? strlen(ptr) : 0;

   *ptrptr = malloc(ptr_size + str_size + 1);
   if (*ptrptr == NULL) {
      
      PyErr_SetString(PyExc_MemoryError, \
		         "reallocate_mem_for_string(): malloc failed");
      return NULL;
   }
 
   strncpy(*ptrptr, ptr, ptr_size);
   strncpy(*ptrptr + ptr_size, str, str_size);
   *(*ptrptr + ptr_size + str_size) = '\0';

   if (ptr != NULL)
      free(ptr);

   return *ptrptr;	
}

/* ------------------------------------------------------------------- */
/* HELPER FUNCTION, Functions related to building struct commands_list */
/* ------------------------------------------------------------------- */

void *build_commands_list(struct commands_list **cmds_lst, PyListObject *argv,\
            char *commands, Py_ssize_t commands_size, \
	                     int argv_idx, int argc, int *common_argc, \
			         int remove_empty_maps) {
   
   char *cmds = commands, *cmd;
   Py_ssize_t cmds_size = 0, cmd_size = 0;

   int f = 0, i, j;
   char *str;
   PyObject *item;

   /* commands_redundency_short and commands_redundency_long */
   struct cmd_redundency *crs_head = NULL, *crl_head = NULL;

   struct commands_list *clnk; /* commands_list_lnk */
   struct cmd_redundency *rlnk = NULL; /* cmd_redundency_lnk */

   struct map *map;
/*
   if (!strcmp(commands, "w")) {
	  
      printf("Yes in the build_commands_list() : ");  	   
   }
*/   

   while ((cmds_size=parse(commands, commands_size, &cmds, \
				      cmds_size, COMMANDS_DELIMITER)) != 0) {


                  
      clnk = add_to_list((struct just_a_link **)cmds_lst, \
		                          sizeof(struct commands_list));
      
      if (clnk == NULL)
         return NULL;

      clnk->map = NULL;
      clnk->cmds = cmds;
      clnk->cmds_size = cmds_size;
 
      cmd = cmds;

      //printf("cmd = %s, cmd-size = %d\n", clnk->cmds, clnk->cmds_size);

      while((cmd_size=parse(cmds, cmds_size, &cmd, \
      				        cmd_size, COMMAND_DELIMITER)) != 0) {

          //printf("cmd_size = %d\n", cmd_size);                                                
/*
	 if (cmd_size == 1 && !strncmp(cmd, "w", 1))
            printf("-> w\n");
*/	    

	 if (cmd_size == 1) {

            /* Process short command, the 'h' or '-h' */		 
            while ((rlnk=get_a_lnk((struct just_a_link **)&crs_head, \
				      (struct just_a_link *)rlnk)) != NULL) {

                  //printf("--------> rlnk = %s", rlnk->cmd);  

	       /* Look if 'h' is is already used */	    
               if (rlnk->cmd_size == 1 && rlnk->cmd[0] == *cmd)
	          break;

	       /* You've given 'h' and now look if you've used '-h' */
	       if (rlnk->cmd_size == 2 && rlnk->cmd[0] == '-' \
			                      && rlnk->cmd[1] == *cmd)
		  break;     
	       	    
            }

	    /* No it is first time you've used 'h' */
	    if (rlnk == NULL) {

	       rlnk=add_to_list((struct just_a_link **)&crs_head, \
			                     sizeof(struct cmd_redundency));
	       if (rlnk == NULL)
		  return NULL;

	       rlnk->cmd = cmd;
	       rlnk->cmd_size = cmd_size;
            }

	    rlnk = NULL;

            /* Process the 'help' or 'h' not the short command */
	    while ((rlnk=get_a_lnk((struct just_a_link **)&crl_head, \
				     (struct just_a_link *)rlnk)) != NULL) {

	       if (rlnk->cmd_size == 1 && rlnk->cmd[0] == *cmd)
                  break;
	    }

	    /* No there is no redundency, store this in the redundency list */
	    if (rlnk == NULL) {
		
	       rlnk=add_to_list((struct just_a_link **)&crl_head, \
			                  sizeof(struct cmd_redundency));

	       if (rlnk == NULL)
		  return NULL;

	       rlnk->cmd = cmd;
	       rlnk->cmd_size = cmd_size;
            }		    
         }
	 else if (cmd_size == 2 && cmd[0] == '-') {

            while ((rlnk=get_a_lnk((struct just_a_link **)&crs_head, \
				 (struct just_a_link *)rlnk)) != NULL) {

	       if (rlnk->cmd_size == 1 && rlnk->cmd[0] == cmd[1])
		  break;

	       if (rlnk->cmd_size == 2 && !strncmp(rlnk->cmd, cmd, 2))
		  break;
	    }

	    if (rlnk == NULL) {

	       rlnk=add_to_list((struct just_a_link **)&crs_head, \
			                   sizeof(struct cmd_redundency));

	       if (rlnk == NULL)
		  return NULL;

	       rlnk->cmd = cmd + 1;
	       rlnk->cmd_size = cmd_size;
            }

	    rlnk = NULL;

	    while ((rlnk=get_a_lnk((struct just_a_link **)&crl_head, \
				       (struct just_a_link *)rlnk)) != NULL)
	       if (rlnk->cmd_size == 2 && !strncmp(rlnk->cmd, cmd, 2))
		  break;

	    if (rlnk == NULL) {

               rlnk=add_to_list((struct just_a_link **)&crl_head, \
			                     sizeof(struct cmd_redundency)); 		    
	       if (rlnk == NULL)
	          return NULL;

	       rlnk->cmd = cmd;
	       rlnk->cmd_size = cmd_size;
	    }   	       
         }
	 else {

            while ((rlnk=get_a_lnk((struct just_a_link **)&crl_head, \
				    (struct just_a_link *)rlnk)) != NULL) 
	       if (rlnk->cmd_size == cmd_size && \
			              !strncmp(rlnk->cmd, cmd, cmd_size))
		  break;       
            

	    if (rlnk == NULL) {

	       rlnk=add_to_list((struct just_a_link **)&crl_head, \
			                  sizeof(struct cmd_redundency));

	       if (rlnk == NULL)
		  return NULL;

	       rlnk->cmd = cmd;
	       rlnk->cmd_size = cmd_size;
            }		    
         }

         rlnk = NULL;
      }

      for (i = 1; i < argc; i++) {

	 item = PyList_GET_ITEM(argv, i + argv_idx);

	 /* str = PyString_AsString(item); */
     str = PyBytes_AS_STRING(PyUnicode_AsASCIIString(item));

     /* printf("-----> %s\n", str); */

	 if (!it_is_short_command(str))
            continue;

         /* Build short commands */
         while((rlnk=get_a_lnk((struct just_a_link **)&crs_head, \
				     (struct just_a_link *)rlnk)) != NULL) {

            for (j = 1; j < strlen(str); j++) {

	       if (str[j] == rlnk->cmd[0])
		  f++;     
            }

	    if (f != 0) {

	       map = add_to_list((struct just_a_link **)&(clnk->map), \
			                               sizeof(struct map));

	       if (map == NULL)
		  return NULL;

	       map->idx = i + argv_idx; 
	       map->argc = 0; 
	       map->f = f;

	       f = 0;

	       /* Bit excessive(read lot) */
	       if (sort_a_list_int((struct just_a_link **)&(clnk->map), offsetof(struct map, idx), sizeof(struct map)) == NULL)
	          return NULL;	       
	    } 	    
         }  	 
      }

      /* sort_a_list_int((struct just_a_link **)&(clnk->map), offsetof(struct map, idx), sizeof(struct map)); */
       
      /* Build long commands */      
      for (i = 0; i < argc; i++) {

	 item = PyList_GET_ITEM(argv, i + argv_idx);

	 /*str = PyString_AsString(item);*/
     str = PyBytes_AS_STRING(PyUnicode_AsASCIIString(item));

	 if (it_is_short_command(str))
            continue;		 

         while((rlnk=get_a_lnk((struct just_a_link **)&crl_head, \
				      (struct just_a_link *)rlnk)) != NULL) {

            if (rlnk->cmd_size != strlen(str) || \
			           strncmp(rlnk->cmd, str, rlnk->cmd_size))
               continue;

	    map = add_to_list((struct just_a_link **)&(clnk->map), \
			                               sizeof(struct map));
	    if (map == NULL)
               return NULL;

	    map->idx = i + argv_idx;
	    map->argc = 0;
	    map->f = 0;

	    /* Bit excessive(read lot) */
            if (sort_a_list_int((struct just_a_link **)&(clnk->map), offsetof(struct map, idx), sizeof(struct map)) == NULL)
	       return NULL;	    
         } 
      }

      /* sort_a_list_int((struct just_a_link **)&(clnk->map), \ 
		   offsetof(struct map, idx), sizeof(struct map)); */

      if (clnk->map == NULL && remove_empty_maps) 
	 remove_a_lnk((struct just_a_link **)cmds_lst, \
			              (struct just_a_link **)&clnk);
      	      
      free_list((struct just_a_link **)&crs_head);
      free_list((struct just_a_link **)&crl_head);
   }

   /* Success means that we return the address of cmds_lst otherwise NULL */	
   return update_map_argc(cmds_lst, argv_idx, argc, common_argc);
}	

/* *str should atleast be cmd and cmd should not be NULL 
   The string is returned in str and its size is the return value 
   *str is Null when there is no more cmd to parse */
Py_ssize_t parse(char *cmd, Py_ssize_t cmd_size, \
		           char **str, Py_ssize_t str_size, char delimiter) {

   Py_ssize_t i = (*str + str_size) - cmd;

   str_size = 0;

   if (i >= cmd_size) {

      *str = cmd;
      return 0;
   }

   if (cmd[i] == delimiter)
      i++;	   

   *str = (cmd + i);

   while (i < cmd_size && cmd[i] != delimiter) {	   
      	   
      i++;
      str_size++;
   } 	   

   return str_size;   	
}

struct commands_list *update_map_argc(struct commands_list **head, \
		                 int argv_idx, int argc, int *common_argc) {

   int idx;	
	   	
   struct commands_list *clnk = NULL; /* struct commands_list's link*/

   struct smap *smap = NULL, *outer_smlnk = NULL, *inner_smlnk = NULL;
   struct map *mlnk = NULL; /* struct map's link*/
  
   /* Basically we are just building a doubly linked list of type 
      struct smap, the s is for sorted map(which it is not). It merely is
      a list in which each link stores a pointer to type struct map */
   while ((clnk=get_a_lnk((struct just_a_link **)head, \
				    (struct just_a_link *)clnk)) != NULL) {

      while ((mlnk=get_a_lnk((struct just_a_link **)&(clnk->map), \
				    (struct just_a_link *)mlnk)) != NULL) {

	 outer_smlnk = add_to_list((struct just_a_link **)&smap, \
			                            sizeof(struct smap));
	 if (outer_smlnk == NULL)
            return NULL;

         outer_smlnk->map = mlnk;      
      }	      
   }

   outer_smlnk = NULL;
   *common_argc = argc + argv_idx; 

   while ((outer_smlnk=get_a_lnk((struct just_a_link **)&smap, \
			   (struct just_a_link *)outer_smlnk)) != NULL) {

      idx = argc + argv_idx; 
	 
      while ((inner_smlnk=get_a_lnk((struct just_a_link **)&smap, \
			    (struct just_a_link *)inner_smlnk)) != NULL) {

	 if (inner_smlnk->map->idx > outer_smlnk->map->idx && \
			           inner_smlnk->map->idx < idx)
	    idx = inner_smlnk->map->idx;	 
      }

      outer_smlnk->map->argc = idx - outer_smlnk->map->idx;

      if (outer_smlnk->map->idx < *common_argc)
	 *common_argc = outer_smlnk->map->idx;     
   }

   *common_argc = *common_argc - argv_idx;

   /* debug_smap(smap, *common_argc); */

   free_list((struct just_a_link **)&smap);

   return *head;	
}	

int it_is_short_command(const char *cmd) {

  int ret = 0;

  if (strlen(cmd) > 2 && cmd[0] == '-' && cmd[1] != '-')
     ret = 1;	  

  return ret;   	
}

/* Doubly Linked List helper functions */
void *add_to_list(struct just_a_link **head, Py_ssize_t size) {

   void *lnk = malloc(size);
   if (lnk == NULL) {
      PyErr_SetString(PyExc_MemoryError, "add_to_list(): malloc failed");
      return NULL; 	   
   }	   

   if (*head == NULL) {

      *head = lnk;
      ((struct just_a_link *)lnk)->prev = lnk;
      ((struct just_a_link *)lnk)->next = lnk;
   }  
   else {

      (*head)->prev->next = lnk; 
      ((struct just_a_link *)lnk)->next = *head;
      ((struct just_a_link *)lnk)->prev = (*head)->prev;
      (*head)->prev = lnk;
   }

   return lnk;
}

void free_list(struct just_a_link **head) {

   struct just_a_link *lnk;

   if (*head == NULL) 
      return;
   
   lnk = (*head)->prev;

   while (lnk != *head) {

      lnk = lnk->prev;
      free(lnk->next);      	   
   };

   free(*head);

   *head = NULL;
}

/*
 lnk = NULL;
 lnk = get_a_lnk(&some_list, lnk);
 if (lnk != NULL) { }
*/
void *get_a_lnk(struct just_a_link **head, struct just_a_link *lnk) {

   if (*head != NULL ) {

      if (lnk != NULL && lnk->next == *head) 
         lnk = NULL;
      else if (lnk == NULL)
	 lnk = *head;
      else 
	 lnk = lnk->next;            	   
   }
   else 
      lnk = NULL;
        
   return lnk;
}

void remove_a_lnk(struct just_a_link **head, struct just_a_link **lnk) {

   struct just_a_link *prev, *next;

   if (*lnk == NULL || *head == NULL) 
      return;

   if (*head == *lnk) {

      prev = (*head)->prev;
      next = (*head)->next;

      if (next == *head && prev == *head)
	 *head = NULL;
      else 
         *head = next;	      
   }
   else {

      prev = (*lnk)->prev;
      next = (*lnk)->next;
   }

   prev->next = next;
   next->prev = prev;

   free(*lnk); 

   *lnk = NULL;
}

/* ------------------------------------------------------------------------ */
/* 
 * Doubly Linked List debug functions, they will be gone when the module 
 * is done
 */
/* ------------------------------------------------------------------------ */
void debug_smap(struct smap *smap, int common_argc) {

   struct smap *lnk = NULL;

   if (smap == NULL) 
      printf("smap is NULL and common_argc = %d\n", common_argc);	   
  	   

   while ((lnk = get_a_lnk((struct just_a_link **)&smap, \
				     (struct just_a_link *)lnk)) != NULL) {
      printf("idx = %d, argc = %d, common_argc = %d\n", lnk->map->idx, lnk->map->argc, common_argc);	   
   }	   
}	

void debug_map(struct map *map) {

   struct map *ap = map;

   if (map == NULL)
      return;	   

   do {

      printf("idx = %d, argc = %d, f = %d\n", ap->idx, ap->argc, ap->f); 

      ap = ap->next;
   }while(map != ap);	   
}	

void debug_commands_list(struct commands_list *cmds_lst) {

   Py_ssize_t i;	
   struct commands_list *lst = cmds_lst;
   
   if (lst == NULL)
      return;	   
   
   do {

      for (i = 0; i<lst->cmds_size; i++) 
	 printf("%c", lst->cmds[i]);

      printf("\n");
      
      debug_map(lst->map);
  	   
      lst = lst->next; 	   
   }while(cmds_lst != lst);	   	
}
