/***** 
 * Allah, The ONE and the ONLY ONE and ALL MIGHTY. 
 * *******************************************************************
 * argsv.h
 * Written by, Sohail Qayum Malik[sqm@hackers.pk]
 * ********/

#include <Python.h>
/* Well not every thing got included/imported by the Python.h */
#include <stddef.h>

/* 
 * A Python module initialization function. Module init functions outside the
 * Python core must be exported(they should've external linkage)
 */
#ifndef		PyMODINIT_FUNC	
#define		PyMODINIT_FUNC			void
#endif

#define		ARGSVVERSIONDOC	   "Version 0.1 - Written by, Sohail Qayum Malik[sqm@hackers.pk]"

#define		CAST(x)	           (argsv_object *)(x)	

#define		REMOVE_EMPTY_MAPS  	1
#define		DONOT_REMOVE_EMPTY_MAPS 0

/* This will go in the PyTypeObject::tp_doc */
PyDoc_STRVAR(argsv_doc, "argsv, Helps a command line application to manage arguments.\n");
PyDoc_STRVAR(argsv_a_cmd_doc, "argsv_a_cmd, a composite to manage the command.\n");

/* Forward declaration */
struct map;

/* This is self, the name of type of argsv_object->ob_type is argsv */
typedef struct {

   PyObject_HEAD

   /* Type specific fields go here */
   PyListObject *argv;
   int common_argc;

   char *commands;

   struct commands_list *cmds_lst, *clnk;

   int found;
}argsv_object;


typedef struct {

   PyObject_HEAD
  
   /* Type specific fields go here */
   struct map *map;
}argsv_a_cmd_object;

typedef struct {

   PyObject_HEAD
  
   /* Type specific fields go here */

   struct commands_lis *clnk_ptr; 	   
   struct commands_list *cmds_lst;	   

}argsv_iter_object;

typedef struct {

   PyObject_HEAD

   /* Type specific fields go here */

}argsv_iterated_cmd_object;	

/* Linked list related details */

#define		COMMANDS_DELIMITER		'#'
#define		COMMAND_DELIMITER		','

struct just_a_link {

   struct just_a_link *prev, *next;
};	

struct cmd_redundency {

   struct cmd_redundency *prev, *next;
   char *cmd;
   Py_ssize_t cmd_size;
};	

struct map {  
   struct map *prev, *next;
   int idx, argc, f; /* f is frequency, thanks to the one subject I hate */
};

struct smap {

   struct smap *prev, *next;	
   struct map *map;
};	

struct commands_list {

   struct commands_list  *prev, *next;
   char *cmds;
   Py_ssize_t cmds_size;
   struct map *map;
}__attribute__((packed));

//void *build_commands_list(void);
void *build_commands_list(struct commands_list **, PyListObject *, char *, \
		                                   Py_ssize_t, int, int, int *,\
						   int remove_empt_maps);
Py_ssize_t parse(char *, Py_ssize_t, char **, Py_ssize_t, char);
void *match_item(const char *);
struct commands_list *update_map_argc(struct commands_list **, int, int, int *);
int it_is_short_command(const char *);

void *add_to_list(struct just_a_link **, Py_ssize_t);
void free_list(struct just_a_link **);
void *get_a_lnk(struct just_a_link **, struct just_a_link *);
void remove_a_lnk(struct just_a_link **, struct just_a_link **);

char *reallocate_mem_for_string(char **, char *, Py_ssize_t);

/* Generic list "bubble sorting" function */
#define		SORT_A_LIST(type)  void *sort_a_list_##type(struct \
		                   just_a_link **, size_t, size_t);\
	        void *sort_a_list_##type(struct just_a_link **head, \
				     size_t offset, size_t size) { \
			                                        \
                   struct just_a_link *olnk=NULL, *ilnk=NULL;   \
                                                                \
	           if (size < sizeof(struct just_a_link))       \
	              size = sizeof(struct just_a_link);        \
                                                                \
	           size-=sizeof(struct just_a_link);  	        \
                                                                \
                   char *ptr = malloc(size);                    \
	           if (ptr == NULL) {                           \
		      PyErr_SetString(PyExc_MemoryError,        \
		           "sort_a_list(): malloc failed");     \
	              return NULL;                              \
		   }                                            \
			                                        \
		   while ((olnk=get_a_lnk(head, olnk)) != NULL) {\
		         	                                 \
		      ilnk = olnk;  	 			 \
		      while((ilnk=get_a_lnk(head, ilnk)) != NULL) {\
			  		   			   \
			 if (*((type *)((char *)olnk + offset)) >  \
			         *((type *)((char *)ilnk + offset)) ) {\
                                                                   \
			    memcpy(ptr, olnk + 1, size);	   \
		            memcpy(olnk + 1, ilnk + 1, size);	   \
			    memcpy(ilnk + 1, ptr, size);           \
			 }		                           \
		      }                                            \
                   }                                               \
		   free(ptr);					   \
	           return *head;	                           \
		}                                                  \


/* List debug functions */
void debug_commands_list(struct commands_list *);
void debug_map(struct map *);
void debug_smap(struct smap *, int);
