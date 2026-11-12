/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                                                                                                         */                      
/*   Copyright (C) 2014 Thorsten Koch, Daniel Rehfeldt, Renato Werneck. All Rights Reserved                                                                */
/*                                                                                                                                                         */
/*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),    */
/*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,    */
/*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:            */
/*                                                                                                                                                         */
/*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.                        */
/*                                                                                                                                                         */
/*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER    */
/*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER           */ 
/*   DEALINGS IN THE SOFTWARE.                                                                                                                             */
/*                                                                                                                                                         */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#pragma comment(linker, "/STACK:10000000") /* large stack for graph_trail */
#define R_OK 4  /* checks for read only */  
#include <io.h>
#define access _access
#else
#include <sys/times.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>         /* errno */
#include <assert.h>
#include <stdarg.h>        /* message: va_list etc */
#include <float.h>
/*#include <unistd.h>*/        /* R_OK  */


#define MINIMUM_TIME_PER_INSTANCE       5.0        /* minimum total time we must run for each instance */
#define MST_COMPUTATIONS                50         /* number of roots from which to start a MST computation (per instance) */
#define VERSION_NUMBER                  20140902


/*
 * Maths
 */
#define EPSILON                  1e-06
#define EPS_ZERO                (DBL_EPSILON * 1e4)
#define LT(a, b)              (((a) - (b))     < -EPS_ZERO)
#define GT(a, b)              (((a) - (b))     >  EPS_ZERO)



/*
 * Graph
 */
#define EAT_FREE     -1
#define EAT_LAST     -2
#define EAT_HIDE     -3

#define CONNECT       0
#define UNKNOWN     (-1)
#define FARAWAY       1e15

#define NO_CHANGE    -10

#define Is_term(a)   ((a) >= 0)
#define Edge_anti(a) ((((a) % 2) > 0) ? (a) - 1 : (a) + 1)



/*
 * Graph reader
 */
#define STP_MAGIC       0x33d32945
#define VERSION_MAJOR   1
#define VERSION_MINOR   0

#define TEST_INSTANCES "stp_test_instances.txt"

#define MSG_FATAL       0
#define MSG_ERROR       1
#define MSG_WARN        2
#define MSG_INFO        3
#define MSG_DEBUG       4

#define VERBOSE         MSG_INFO

#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

#ifndef SUCCESS
#define SUCCESS         0
#endif
#ifndef FAILURE
#define FAILURE       (-1)
#endif

#if defined(MSDOS) || defined(WIN32)
#define  DIRSEP "\\"
#else
#define  DIRSEP "/"
#endif

/* Try to get the maximum length of a path.
 *
 * WARNING: if a path found or build during the scanning process is
 *          longer than defined below the programm will probably
 *          crash, because scanf() will overwrite some memory.
 */
#if defined(PATH_MAX)                     /* Found this on SCO UNIX */
#define MAX_PATH_LEN PATH_MAX
#elif defined(_MAX_PATH)                  /* Watcom C on MSDOS */
#define MAX_PATH_LEN _MAX_PATH
#elif defined(_POSIX_PATH_MAX)            /* Maybe POSIX */
#define MAX_PATH_LEN _POSIX_PATH_MAX
#else
#define MAX_PATH_LEN 1024
#endif

/* Extension separators
 */
#define EXTSEP             '.'

#define MAX_LINE_LEN             1024
#define MAX_KEYWORD_LEN          64
#define MAX_STRING_LEN           256
#define MAX_ARGUMENTS            8

#define KEY_SECTION              0001
#define KEY_EOF                  9999
#define KEY_END                  9998

#define KEY_COMMENT_NAME         1001
#define KEY_COMMENT_DATE         1002
#define KEY_COMMENT_CREATOR      1003
#define KEY_COMMENT_REMARK       1004

#define KEY_GRAPH_NODES          2001
#define KEY_GRAPH_EDGES          2002
#define KEY_GRAPH_E              2003
#define KEY_GRAPH_A              2004

#define KEY_TERMINALS_TERMINALS  3001
#define KEY_TERMINALS_T          3002
#define KEY_TERMINALS_TP         3003
#define KEY_TERMINALS_ROOT       3004

#define KEY_COORDINATES_DD       4001
#define KEY_COORDINATES_DDD      4002
#define KEY_COORDINATES_GRID     4003

#define KEY_SOLUTION_VALUE       4001
#define KEY_SOLUTION_DATE        4002
#define KEY_SOLUTION_TIME        4003
#define KEY_SOLUTION_STEINER     4004
#define KEY_SOLUTION_S           4005

#define KEY_PRESOLVE_DATE        5001
#define KEY_PRESOLVE_FIXED       5002
#define KEY_PRESOLVE_LOWER       5003
#define KEY_PRESOLVE_UPPER       5004
#define KEY_PRESOLVE_TIME        5005

#define KEY_NODEWEIGHTS_NW       6000

#define KEY_PRIZECOLL_MD         7000

#define KEY_MAXDEGS_MD           8000

#define FLAG_OPTIONAL            1
#define FLAG_REQUIRED            2

#define SECTION_MISSING          0
#define SECTION_EXISTEND         1



/*
 * Graph structs
 */

typedef struct
{
   /* Knots
    */
   int     ksize;  /* Count of allocated knot slots               */
   int     knots;  /* Count of knots in graph                     */
   int     terms;  /* Count of terminals                          */
   int     source; /* The root                                    */
   int*    term;   /* Array [0..knots-1] of networknumber for     */
                   /* knot [i], -1 if [i] is never a terminal     */
   int*    mark;   /* Array [0..knots-1], normaly TRUE or FALSE   */
                   /* to mark knots for inclusion in the shortest */
                   /* path / minimum spanning tree routine        */
   int*    grad;   /* Array [0..knots-1] with degree of knot [i]  */
   int*    inpbeg; /* Array [0..knots-1] with starting slot index */
                   /* for the ieat array, -1 if not used          */
   int*    outbeg; /* Array [0..knots-1] with starting slot index */
                   /* for the oeat array, -1 if not used          */

   /* Edges
    */
   int     esize;  /* Count of allocated edge slots               */
   int     edges;  /* Count of edges in the graph                 */
   double* cost;   /* Array [0..edges-1] of positiv edge costs    */
   int*    tail;   /* Array [0..edges-1] of knot-number of tail   */
                   /* of edge [i]                                 */
   int*    head;   /* Array [0..edges-1] of knot-number of head   */
                   /* of edge [i]                                 */
   /* Knots/Edges
    */
   int*    ieat;   /* Array [0..edges-1], incomming edge          */
                   /* allocation table                            */
   int*    oeat;   /* Array [0..edges-1], outgoing edge           */
                   /* allocation table                            */

   /* data for math and mst computation
    */
   int* path_heap;
   int* path_state;
} GRAPH;

typedef struct presolve_info
{
   double fixed;
   double upper;
   double lower;
   int    time;
} PRESOL;

/* ONE segment of a path
 */
typedef struct mst_unit
{
   double       dist;         /* Distance to the end of the path             */
   signed int   edge;         /* First edge to go                            */
} MST;



/*
 * Graph reader structs
 */

struct key
{
   const char* keyword;
   int         sw_code;
   const char* format;
};

static const struct key keyword_table[] =
   {
      /*
       * *** The keywords MUST be sorted alphabetically ! ***
       */
      {  ".eof",                KEY_EOF,                 NULL   },
      {  ".section",            KEY_SECTION,             NULL   },

      {  "comment.creator",     KEY_COMMENT_CREATOR,     "s"    },
      {  "comment.date",        KEY_COMMENT_DATE,        "s"    },
      {  "comment.end",         KEY_END,                 NULL   },
      {  "comment.name",        KEY_COMMENT_NAME,        "s"    },
      {  "comment.remark",      KEY_COMMENT_REMARK,      "s"    },

      {  "coordinates.dd",      KEY_COORDINATES_DD,      "nnn"  },
      {  "coordinates.ddd",     KEY_COORDINATES_DDD,     "nnnn" },
      {  "coordinates.end",     KEY_END,                 NULL   },
      {  "coordinates.grid",    KEY_COORDINATES_GRID,    NULL   },

      {  "graph.a",             KEY_GRAPH_A,             "nnnn" },
      {  "graph.e",             KEY_GRAPH_E,             "nnn"  },
      {  "graph.edges",         KEY_GRAPH_EDGES,         "n"    },
      {  "graph.end",           KEY_END,                 NULL   },
      {  "graph.nodes",         KEY_GRAPH_NODES,         "n"    },

      {  "maximumdegrees.end",  KEY_END,                 NULL   },
      {  "maximumdegrees.md",   KEY_MAXDEGS_MD,          "n"    },

      {  "nodeweights.end",     KEY_END,                 NULL   },
      {  "nodeweights.nw",      KEY_NODEWEIGHTS_NW,      "n"    },

      {  "presolve.date",       KEY_PRESOLVE_DATE,       "s"    },
      {  "presolve.end",        KEY_END,                 NULL   },
      {  "presolve.fixed",      KEY_PRESOLVE_FIXED,      "n"    },
      {  "presolve.lower",      KEY_PRESOLVE_LOWER,      "n"    },
      {  "presolve.time",       KEY_PRESOLVE_TIME,       "n"    },
      {  "presolve.upper",      KEY_PRESOLVE_UPPER,      "n"    },

      {  "solution.date",       KEY_SOLUTION_DATE,       "s"    },
      {  "solution.end",        KEY_END,                 NULL   },
      {  "solution.s",          KEY_SOLUTION_S,          "n"    },
      {  "solution.steiner",    KEY_SOLUTION_STEINER,    "n"    },
      {  "solution.time",       KEY_SOLUTION_TIME,       "n"    },
      {  "solution.value",      KEY_SOLUTION_VALUE,      "n"    },

      {  "terminals.end",       KEY_END,                 NULL   },
      {  "terminals.root",      KEY_TERMINALS_ROOT,      "n"    },
      {  "terminals.t",         KEY_TERMINALS_T,         "n"    },
      {  "terminals.terminals", KEY_TERMINALS_TERMINALS, "n"    },
      {  "terminals.tp",        KEY_TERMINALS_TP,        "nn"    },
   };

struct section
{
   const char* name;
   const char* extension;
   const int   flag;
   int         mark;
};


/* Extension NULL = no separate file possible !
 */
static struct section section_table[] =
   {
      { "",            "stp", FLAG_REQUIRED, SECTION_EXISTEND },

      /*
       * *** The section names MUST be sorted alphabetically ! ***
       */
      { "comment",     NULL,  FLAG_REQUIRED, SECTION_MISSING },
      { "coordinates", "crd", FLAG_OPTIONAL, SECTION_MISSING },
      { "graph",       "grp", FLAG_REQUIRED, SECTION_MISSING },
      { "maximumdegrees", "mdg", FLAG_OPTIONAL, SECTION_MISSING },
      { "nodeweights", "nwg", FLAG_OPTIONAL, SECTION_MISSING },
      { "presolve",    "prs", FLAG_OPTIONAL, SECTION_MISSING },
      { "solution",    "slt", FLAG_OPTIONAL, SECTION_MISSING },
      { "terminals",   "trm", FLAG_REQUIRED, SECTION_MISSING },
   };

typedef struct current_file
{
   char   filename[MAX_PATH_LEN];
   int    line;
   FILE*  fp;
   struct section* section;
} CURF;

typedef union parameter
{
   double n;  /* Could be long long */
   char   s[MAX_STRING_LEN];
} PARA;



/*
 * Graph methods
 */

/** initialize a graph */
static GRAPH* graph_init(
   int ksize,
   int esize)
{
   GRAPH* p;
   int    i;

   assert(ksize > 0);
   assert(esize > 0);

   p = (GRAPH*)malloc(sizeof(*p));

   assert(p != NULL);

   p->ksize  = ksize;
   p->knots  = 0;
   p->terms  = 0;
   p->source = -1;

   p->term   = (int*)malloc((size_t)ksize * sizeof(int));
   p->mark   = (int*)malloc((size_t)ksize * sizeof(int));
   p->grad   = (int*)malloc((size_t)ksize * sizeof(int));
   p->inpbeg = (int*)malloc((size_t)ksize * sizeof(int));
   p->outbeg = (int*)malloc((size_t)ksize * sizeof(int));

   p->esize = esize;
   p->edges = 0;
   p->cost  = (double*)malloc((size_t)esize * sizeof(double));
   p->tail  = (int*)malloc((size_t)esize * sizeof(int));
   p->head  = (int*)malloc((size_t)esize * sizeof(int));

   p->ieat  = (int*)malloc((size_t)esize * sizeof(int));
   p->oeat  = (int*)malloc((size_t)esize * sizeof(int));

   p->path_heap = NULL;
   p->path_state = NULL;

   assert(p->term   != NULL);
   assert(p->mark   != NULL);
   assert(p->grad   != NULL);
   assert(p->inpbeg != NULL);
   assert(p->outbeg != NULL);
   assert(p->cost   != NULL);
   assert(p->tail   != NULL);
   assert(p->head   != NULL);
   assert(p->ieat   != NULL);
   assert(p->oeat   != NULL);

   return(p);
}

/** free the graph */
static void graph_free(
   GRAPH* p)
{
   assert(p != NULL);

   free(p->term);
   free(p->mark);
   free(p->grad);
   free(p->inpbeg);
   free(p->outbeg);
   free(p->cost);
   free(p->tail);
   free(p->head);
   free(p->ieat);
   free(p->oeat);

   free(p);
}

/** add a node */
static void graph_knot_add(
   GRAPH* p,
   int    term)
{
   assert(p        != NULL);
   assert(p->ksize >  p->knots);

   p->term  [p->knots] = term;
   p->mark  [p->knots] = TRUE;
   p->grad  [p->knots] = 0;
   p->inpbeg[p->knots] = EAT_LAST;
   p->outbeg[p->knots] = EAT_LAST;

   if (Is_term(term))
   {
      p->terms++;
   }
   p->knots++;
}

/** change the terminal property of the node */
static void graph_knot_chg(
   GRAPH* p,
   int    knot,
   int    term)
{
   assert(p      != NULL);
   assert(knot   >= 0);
   assert(knot   < p->knots);

   if (term != NO_CHANGE)
   {
      if (term != p->term[knot])
      {
         if (Is_term(p->term[knot]))
         {
            p->terms--;
         }
         p->term[knot] = term;

         if (Is_term(p->term[knot]))
         {
            p->terms++;
         }
      }
   }
}

/** add an edge */
static void graph_edge_add(
   GRAPH* p,
   int    tail,
   int    head,
   double cost1,
   double cost2)
{
   int    e;

   assert(p      != NULL);
   assert(tail   >= 0);
   assert(tail   <  p->knots);
   assert(head   >= 0);
   assert(head   <  p->knots);

   assert(p->esize >= p->edges + 2);

   e = p->edges;

   p->grad[head]++;
   p->grad[tail]++;

   p->cost[e]      = cost1;
   p->tail[e]      = tail;
   p->head[e]      = head;
   p->ieat[e]      = p->inpbeg[head];
   p->oeat[e]      = p->outbeg[tail];
   p->inpbeg[head] = e;
   p->outbeg[tail] = e;

   e++;

   p->cost[e]      = cost2;
   p->tail[e]      = head;
   p->head[e]      = tail;
   p->ieat[e]      = p->inpbeg[tail];
   p->oeat[e]      = p->outbeg[head];
   p->inpbeg[tail] = e;
   p->outbeg[head] = e;

   p->edges += 2;
}

/** mark all nodes connected to node i */
static void graph_trail(
   const GRAPH* p,
   int          i)
{
   int   k;

   assert(p      != NULL);
   assert(i      >= 0);
   assert(i      <  p->knots);

   if (!p->mark[i])
   {
      p->mark[i] = TRUE;

      for(k = p->outbeg[i]; k != EAT_LAST; k = p->oeat[k])
         if (!p->mark[p->head[k]])
            graph_trail(p, p->head[k]);
   }
}

/** check whether the graph is valid */
static int graph_valid(
   const GRAPH* p)
{
   const char* error1  = "*** Graph Validation Error: Head invalid, Knot %d, Edge %d, Tail=%d, Head=%d\n";
   const char* error2  = "*** Graph Validation Error: Tail invalid, Knot %d, Edge %d, Tail=%d, Head=%d\n";
   const char* error3  = "*** Graph Validation Error: Source invalid, Layer %d, Source %d, Terminal %d\n";
   const char* error4  = "*** Graph Validation Error: FREE invalid, Edge %d/%d\n";
   const char* error5  = "*** Graph Validation Error: Anti invalid, Edge %d/%d, Tail=%d/%d, Head=%d/%d\n";
   const char* error6  = "*** Graph Validation Error: Knot %d with Grad 0 has Edges\n";
   const char* error7  = "*** Graph Validation Error: Knot %d not connected\n";
   const char* error8  = "*** Graph Validation Error: Wrong Terminal count, count is %d, should be %d\n";

   int    k;
   int    l = 0;
   int    e;
   int    terms;

   assert(p      != NULL);

   terms  = p->terms;

   for(k = 0; k < p->knots; k++)
   {
      if (Is_term(p->term[k]))
      {
         terms--;
      }
      for(e = p->inpbeg[k]; e != EAT_LAST; e = p->ieat[e])
         if (p->head[e] != k)
            break;

      if (e != EAT_LAST)
         return((void)fprintf(stderr, error1, k, e, p->tail[e], p->head[e]), FALSE);

      for(e = p->outbeg[k]; e != EAT_LAST; e = p->oeat[e])
         if (p->tail[e] != k)
            break;

      if (e != EAT_LAST)
         return((void)fprintf(stderr, error2, k, e, p->tail[e], p->head[e]), FALSE);
   }

   if (terms != 0)
      return((void)fprintf(stderr, error8, p->terms, p->terms - terms), FALSE);

   if ((p->source < 0)
      || (p->source >= p->knots)
      || (p->term[p->source] != 0))
      return((void)fprintf(stderr, error3,
            l, p->source, p->term[p->source]), FALSE);

   for(e = 0; e < p->edges; e += 2)
   {
      if ((p->ieat[e    ] == EAT_FREE) && (p->oeat[e    ] == EAT_FREE)
         && (p->ieat[e + 1] == EAT_FREE) && (p->oeat[e + 1] == EAT_FREE))
         continue;

      if ((p->ieat[e] == EAT_FREE) || (p->oeat[e] == EAT_FREE)
         || (p->ieat[e + 1] == EAT_FREE) || (p->oeat[e + 1] == EAT_FREE))
         return((void)fprintf(stderr, error4, e, e + 1), FALSE);

      if ((p->head[e] != p->tail[e + 1]) || (p->tail[e] != p->head[e + 1]))
         return((void)fprintf(stderr, error5,
               e, e + 1, p->head[e], p->tail[e + 1],
               p->tail[e], p->head[e + 1]), FALSE);
   }
   for(k = 0; k < p->knots; k++)
      p->mark[k] = FALSE;

   graph_trail(p, p->source);

   for(k = 0; k < p->knots; k++)
   {
      if ((p->grad[k] == 0)
         && ((p->inpbeg[k] != EAT_LAST) || (p->outbeg[k] != EAT_LAST)))
         return((void)fprintf(stderr, error6, k), FALSE);

      if (!p->mark[k] && (p->grad[k] > 0))
         return((void)fprintf(stderr, error7, k), FALSE);
   }
   return(TRUE);
}



/*
 * Graph reader methods
 */

/*---------------------------------------------------------------------------*/
/*--- Name     : String to Lower                                          ---*/
/*--- Function : Converts a string to lower case.                         ---*/
/*--- Arguments: Pointer to string.                                       ---*/
/*--- Returns  : The argument, but now pointing to a lower case string.   ---*/
/*---------------------------------------------------------------------------*/
static char* strlower(
   char* s)
{
   char* t;

   for(t = s; *s != '\0'; s++)
      *s = (char)tolower(*s);

   return(t);
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Print Message                                            ---*/
/*--- Function : Prints a message on stderr.                              ---*/
/*--- Arguments: Type of message, Info about filename and line number,    ---*/
/*---            printf format string and parameters to be printed.       ---*/
/*--- Returns  : Nothing                                                  ---*/
/*---------------------------------------------------------------------------*/
static void message(
   unsigned int type,
   const CURF*  curf,
   const char*  msg,
   ...)
{
   va_list params;

   const char* intro[] = { "Fatal Error", "Error      ", "Warning    ",
                           "Info       ", "Debug      "
   };
   const char* header  = "*** %s File \"%s\" line %05d: ";

   assert(type           <  sizeof(intro) / sizeof(char*));
   assert(curf           != NULL);
   assert(curf->filename != NULL);
   assert(curf->line     >= 0);
   assert(msg            != NULL);

   va_start(params, msg);

   if (type <= VERBOSE)
   {
      (void)fprintf(stderr, header, intro[type], curf->filename, curf->line);
      (void)vfprintf(stderr, msg, params);
      (void)fputc('\n', stderr);
   }
   va_end(params);
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Key Compare                                              ---*/
/*--- Function : Compares the key with an element of the list.            ---*/
/*--- Parameter: Pointer to key, pointer to element                       ---*/
/*--- Returns  : <0 : key<elem, =0 : key=elem, >0 : key>elem              ---*/
/*---------------------------------------------------------------------------*/
static int key_cmp(
   const void* key,
   const void* elem)
{
   assert(key                                != NULL);
   assert(elem                               != NULL);
   assert(((const struct key*)elem)->keyword != NULL);
#if 0
   (void)fprintf(stderr, "key [%s] elem [%s]\n",
      (const char*)key, ((const struct key*)elem)->keyword);
#endif
   return(strcmp((const char*)key, ((const struct key*)elem)->keyword));
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Section Compare                                          ---*/
/*--- Function : Compares the key with an section name.                   ---*/
/*--- Parameter: Pointer to key, pointer to section                       ---*/
/*--- Returns  : <0 : key<sec, =0 : key=sec, >0 : key>sec                 ---*/
/*---------------------------------------------------------------------------*/
static int sec_cmp(
   const void* key,
   const void* section)
{
   assert(key                                    != NULL);
   assert(section                                != NULL);
   assert(((const struct section*)section)->name != NULL);

   return(strcmp((const char*)key, ((const struct section*)section)->name));
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Get Arguments                                            ---*/
/*--- Function : Extract the arguments following a keyword.               ---*/
/*--- Parameter: Current file info, argument format string, input line,   ---*/
/*---            pointer to array of MAX_ARGUMENTS PARA's.                ---*/
/*--- Returns  : 0 for success and < 0 for failure.                       ---*/
/*---------------------------------------------------------------------------*/
static int get_arguments(
   const CURF* curf,
   const char* format,
   const char* s,
   PARA*       para)
{
   const char* err_missmatch_v = "Wrong Syntax";
   const char* msg_hello_ss    = "get_arguments(\"%s\", \"%s\")";

   int missmatch = FALSE;
   int i;

   assert(format != NULL);
   assert(s      != NULL);
   assert(para   != NULL);

   message(MSG_DEBUG, curf, msg_hello_ss, format, s);

   /* We try until we run out of input or have enough arguments.
    */
   while((*s != '\0') && (*format != '\0') && !missmatch)
   {
      missmatch = TRUE;

      switch(*format)
      {
      case 'n' :  /* Numeric */
         /* Go to next digit.
          */
         while((*s != '\0') && !isdigit(*s))
            s++;

         /* Someting left ?
          */
         if (*s != '\0')
         {
            assert(isdigit(*s));

            /* Get it.
             */
            para->n = 0;

            while(isdigit(*s))
            {
               para->n = para->n * 10 + (*s - '0');
               s++;
            }
            missmatch = FALSE;
         }
         break;
      case 's' :  /* String */
         /* Go to the beginning of the string.
          */
         while((*s != '\0') && (*s != '\"'))
            s++;

         /* Someting left ?
          */
         if (*s != '\0')
         {
            assert(*s == '\"');

            /* Get the String.
             */
            i = 0;
            s++;

            while((*s != '\0') && (*s != '\"') && (i < MAX_STRING_LEN - 1))
               para->s[i++] = *s++;

            para->s[i] = '\0';
            missmatch  = FALSE;
         }
         break;
      case 'b' :  /* Bool */
      case 'd' :  /* Date */
      default  :
         /* CONSTCOND */
         assert(FALSE);
         break;
      }
      if (missmatch)
         message(MSG_ERROR, curf, err_missmatch_v);
      else
      {
         para++;
         format++;
      }
   }
   return((*format == '\0') ? SUCCESS : FAILURE);
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Open File                                                ---*/
/*--- Function : Opens a file, processes the file header and secures it's ---*/
/*---            the right file.                                          ---*/
/*--- Arguments: Info about filename and wanted section (file type).      ---*/
/*--- Returns  : 0 for success with curf->fp and curf->line filled or     ---*/
/*---            or < 0 for failure.                                      ---*/
/*---------------------------------------------------------------------------*/
static int open_file(
   CURF* curf)
{
   const char* err_cantopen_s   = "%s.";
   const char* err_noheader_v   = "Wrong file header.";
   const char* err_nomagic_d    = "Wrong Magic-Number %d.";
   const char* err_wrongtype_ss = "Wrong file type. Found %s, expected %s.";
   const char* msg_version_dd   = "Format Version %d.%d.";

   char         type[4];
   unsigned int magic;
   int          version_major;
   int          version_minor;
   int result = FAILURE;

   assert(curf           != NULL);
   assert(curf->filename != NULL);
   assert(curf->section  != NULL);

   /* Prepare the result.
    */
   curf->line = 1;
   curf->fp   = NULL;

   /* Try to open the file...
    */
   if ((curf->fp = fopen(curf->filename, "r")) == NULL)
      message(MSG_FATAL, curf, err_cantopen_s, strerror(errno));

   /* Read Header...
    */
   else if (fscanf(curf->fp, "%8x %3s File, STP Format Version %2d.%2d \n",
         &magic, type, &version_major, &version_minor) != 4)
      message(MSG_FATAL, curf, err_noheader_v);

   /* Test Magic...
    */
   else if (magic != STP_MAGIC)
      message(MSG_FATAL, curf, err_nomagic_d, magic);

   /* Did we get the right type of file ?
    */
   else if (strcmp(strlower(type), curf->section->extension))
      message(MSG_FATAL, curf, err_wrongtype_ss, type, curf->section->extension);
   else
   {
      /* Succeeded. Just give a warning if the file has a different
       * version number than the reader and hope it will be ok.
       */
      if ((version_major != VERSION_MAJOR) || (version_minor != VERSION_MINOR))
         message(MSG_WARN, curf, msg_version_dd, version_minor, version_major);

      result = SUCCESS;
   }
   return(result);
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Start a new Section                                      ---*/
/*--- Function : Starts a new section, maybe in another file.             ---*/
/*--- Parameter: Path for files, Basename, pointer to actual file info,   ---*/
/*---            Pointer to where to save actual file info, inputline.    ---*/
/*--- Returns  : 0 for success and < 0 for failure.                       ---*/
/*---------------------------------------------------------------------------*/
static int start_section(
   const char* pathname,
   const char* basename,
   CURF*       curf,
   CURF*       save,
   const char* s)
{
   const char* err_missing_v   = "Section name missing";
   const char* err_badsect_s   = "Unknown section name [%s]";
   const char* err_required_s  = "Can't access required file [%s]";
   const char* err_duplicate_s = "Duplicate Section [%s]";

   CURF            temp;
   char            inclname[MAX_PATH_LEN];
   char            sectname[MAX_KEYWORD_LEN];
   char            dummy   [MAX_KEYWORD_LEN];
   int             tokens;
   int             ret = FAILURE;

   assert(pathname != NULL);
   assert(basename != NULL);
   assert(curf     != NULL);
   assert(save     != NULL);
   assert(s        != NULL);

   sectname[0] = '\0';
   inclname[0] = '\0';

   /* Extract names
    */
   if ((tokens = sscanf(s, "%63s %63s %s", sectname, dummy, inclname)) < 1)
      message(MSG_FATAL, curf, err_missing_v);
   else
   {
      /* Known section ?
       */
      temp.section = (struct section*)bsearch(strlower(sectname),
         &section_table[1],
         (sizeof(section_table) / sizeof(struct section)) - 1,
         sizeof(struct section), sec_cmp);

      if (temp.section == NULL)
         message(MSG_FATAL, curf, err_badsect_s, sectname);
      else
      {
         if (temp.section->mark & SECTION_EXISTEND)
            message(MSG_FATAL, curf, err_duplicate_s, sectname);
         else
         {
            /* Is this section in a separate file ?
             */
            if (tokens == 1)
            {
               curf->section        = temp.section;
               curf->section->mark |= SECTION_EXISTEND;
               ret                  = SUCCESS;
            }
            else
            {
               (void)sprintf(temp.filename, "%s%s%c%s",
                  pathname,
                  (*inclname == '\0') ? basename : inclname,
                  EXTSEP,
                  temp.section->extension);

               if (access(temp.filename, R_OK))
               {
                  /* We can't access the include file.
                   * If the section is optional, we just ignore
                   * the whole thing, otherwise we have a problem.
                   */
                  if (temp.section->flag & FLAG_REQUIRED)
                     message(MSG_FATAL, curf, err_required_s, temp.filename);
                  else
                  {
                     temp.section = &section_table[0];
                     ret          = SUCCESS;
                  }
               }
               else
               {
                  if (!open_file(&temp))
                  {
                     *save                = *curf;
                     *curf                = temp;
                     curf->section->mark |= SECTION_EXISTEND;

                     ret                  = SUCCESS;
                  }
               }
            }
         }
      }
   }
   return(ret);
}

/*---------------------------------------------------------------------------*/
/*--- Name     : Steiner Tree Problem Load                                ---*/
/*--- Function : Reads a file in STP format and parses it.                ---*/
/*--- Parameter: Pointer to filename, Pointer to presolve struct          ---*/
/*--- Returns  : 0 for success, < 0 for failure                           ---*/
/*---------------------------------------------------------------------------*/
static GRAPH* graph_load(
   const char*  file,
   PRESOL*      presol)
{
   const char*  err_unknown_s   = "Unknown keyword [%s]";
   const char*  err_include_v   = "Include in included file";
   const char*  err_missing_s   = "Required section %s missing";
   const char*  msg_newsect_s   = "Processing Section %s";
   const char*  msg_keyword_sd  = "Found Keyword \"%s\", code = %d";
   const char*  err_badedge_ddd = "Bad edge %d-%d (%d nodes)";
   const char*  err_badroot_dd  = "Bad root %d (%d nodes)";
   const char*  err_baddeg_dd   = "More degree constraints (%d) than nodes (%d)";
   const char*  msg_finish_dddd = "Knots: %d  Edges: %d  Terminals: %d  Source=%d\n";

   const char*  endofline = "#;\n\r";
   const char*  separator = " \t:=";

   static CURF  curf_null = { "", 0, NULL, NULL };

   GRAPH*       g       = NULL;
   CURF         curf;
   CURF         save;
   PARA         para    [MAX_ARGUMENTS];
   double       nodeweight;
   char         buffer  [MAX_LINE_LEN];
   char         pathname[MAX_PATH_LEN];
   char         basename[MAX_PATH_LEN];
   char         keyword [MAX_KEYWORD_LEN];
   int          stop_input = FALSE;
   int          ret        = FAILURE;
   char*        s;
   char*        t;
   struct key*  p;
   int          i;
   int          terms = 0;
   int          nodes = 0;
   int          edges = 0;

   assert(file != NULL);

   /* No section loaded so far.
    */
   for(i = 1; i < (int)(sizeof(section_table) / sizeof(section_table[0])); i++)
      section_table[i].mark = SECTION_MISSING;

   /* Get the names...
    */
   (void)strcpy(pathname, file);

   /* Did we get a path ?
    */
   if ((s = strrchr(pathname, DIRSEP[0])) == NULL)
   {
      /* No, no path.
       */
      (void)strcpy(basename, pathname);
      pathname[0] = '\0';
   }
   else
   {
      /* Yes, there is a path in front.
       */
      s++;
      (void)strcpy(basename, s);
      *s = '\0';
   }

   /* Strip of the extension
    */
   if ((s = strrchr(basename, EXTSEP)) != NULL)
      *s = '\0';

   /* Build filename
    */
   curf.section     = &section_table[0];
   save             = curf_null;

   (void)sprintf(curf.filename, "%s%s.%s",
      pathname,
      basename,
      curf.section->extension);

   /* Open the file...
    */
   if (!open_file(&curf))
   {
      /* We read while a file is open...
       */
      while((curf.fp != NULL) && !stop_input)
      {
         /* Read a line.
          */
         if ((s = fgets(buffer, sizeof(buffer), curf.fp)) == NULL)
         {
            /* No more lines available, so we can close the file.
             */
            (void)fclose(curf.fp);

            /* If we are in an included file, we come back to our
             * main file. Otherwise fp_save is NULL and than fp will
             * get NULL so we're finished.
             */
            curf = save;
            save = curf_null;

            continue;
         }

         /* Count line number
          */
         curf.line++;

         /* Find the start of the interesting part of an inputline.
          */
         while(isspace(*s))
            s++;

         /* Find the end of the interesting portion of an input line.
          * Either the start of a comment or the final NL or CR.
          * Since all lines but the last must have at least a NL
          * t is nearly never NULL.
          */
         if ((t = strpbrk(s, endofline)) != NULL)
            *t = '\0';

         /* Is there an interesting part left ?
          */
         if (*s == '\0')
            continue;

         /* Build a keyword of form "sectionname.keyword"
          */
         (void)strcpy(keyword, curf.section->name);

         i          = (int)strlen(keyword);
         keyword[i] = '.';

         for(i++;
             (i < MAX_KEYWORD_LEN - 1) && (isalpha(*s) || (*s == '_'));
             i++, s++)
            keyword[i] = (char)tolower(*s);

         keyword[i] = '\0';

         /* Skip junk following the keyword.
          */
         while((*s != '\0') && (strchr(separator, *s) != NULL))
            s++;

         /* Did we know the keyword ?
          */
         p = (struct key*)bsearch(keyword, keyword_table,
            sizeof(keyword_table) / sizeof(struct key),
            sizeof(struct key), key_cmp);

         if (p == NULL)
            message(MSG_ERROR, &curf, err_unknown_s, keyword);
         else
         {
            assert(p != NULL);

            message(MSG_DEBUG, &curf, msg_keyword_sd, p->keyword, p->sw_code);

            /* Yes, so lets get the rest of the line if possible
             */
            if ((p->format == NULL)
               || !get_arguments(&curf, p->format, s, para))
            {
               /* Now, what should we do ?
                */
               switch(p->sw_code)
               {
               case KEY_SECTION : /* a new Section starts. */
                  if (save.fp == NULL)
                     stop_input = start_section(pathname, basename,
                        &curf, &save, s);
                  else
                  {
                     message(MSG_FATAL, &curf, err_include_v);
                     stop_input = TRUE;
                  }
                  break;
               case KEY_END : /* END found. */

                  curf.section = &section_table[0];
                  break;
               case KEY_EOF : /* EOF found */
                  ret        = SUCCESS;
                  stop_input = TRUE;

                  /* Test if all required section were found.
                   */
                  for(i = 0; (unsigned)i < sizeof(section_table) / sizeof(struct section); i++)
                  {
                     if ((section_table[i].flag & FLAG_REQUIRED)
                        && !(section_table[i].mark & SECTION_EXISTEND))
                     {
                        message(MSG_FATAL, &curf, err_missing_s, section_table[i].name);
                        ret = FAILURE;
                     }
                  }
                  break;
               case KEY_COMMENT_NAME :
               case KEY_COMMENT_DATE :
               case KEY_COMMENT_CREATOR :
               case KEY_COMMENT_REMARK :
                  break;
               case KEY_GRAPH_NODES :
                  nodes = (int)para[0].n;
                  break;
               case KEY_GRAPH_EDGES :
                  edges = (int)para[0].n;
                  break;
               case KEY_GRAPH_A :
               case KEY_GRAPH_E :
                  if (g == NULL)
                  {
                     g = graph_init(nodes, edges * 2);

                     assert(g != NULL);

                     for(i = 0; i < nodes; i++)
                        graph_knot_add(g, -1);
                  }
                  if (((int)para[0].n <= nodes) && ((int)para[1].n <= nodes))
                  {
                     graph_edge_add(g, (int)para[0].n - 1, (int)para[1].n - 1,
                        (double)para[2].n,
                        (p->sw_code == KEY_GRAPH_E)
                        ? (double)para[2].n
                        : (double)para[3].n);
                  }
                  else
                  {
                     message(MSG_FATAL, &curf, err_badedge_ddd,
                        (int)para[0].n, (int)para[1].n, nodes);
                     ret = FAILURE;
                  }
                  break;
	       case KEY_MAXDEGS_MD :
	       case KEY_NODEWEIGHTS_NW :
               case KEY_TERMINALS_TERMINALS :
		  terms = (int)para[0].n;
		  assert(terms > 0);
                  break;
               case KEY_TERMINALS_ROOT :
                  assert(g != NULL);

                  if ((int)para[0].n <= nodes)
                     g->source = (int)para[0].n - 1;
                  else
                  {
                     message(MSG_FATAL, &curf, err_badroot_dd,
                        (int)para[0].n, nodes);
                     ret = FAILURE;
                  }
                  break;
               case KEY_TERMINALS_T :
                  graph_knot_chg(g, (int)para[0].n - 1, 0);
                  break;
	       case KEY_TERMINALS_TP :
                  graph_knot_chg(g, (int)para[0].n - 1, 0);
                  break;
               case KEY_COORDINATES_DD :
                  break;
               case KEY_COORDINATES_DDD :
                  break;
               case KEY_COORDINATES_GRID :
                  break;
               case KEY_PRESOLVE_FIXED :
                  break;
               case KEY_PRESOLVE_DATE :
                  break;
               case KEY_PRESOLVE_LOWER :
                  break;
               case KEY_PRESOLVE_UPPER :
                  break;
               case KEY_PRESOLVE_TIME :
                  break;
               default :
                  /* CONSTCOND */
                  assert(FALSE);
                  break;
               }
            }
         }
      }
   }

   /* Was there an error in an incuded file ?
    * Than close the main file.
    */
   if (save.fp != NULL)
      (void)fclose(save.fp);

   /* Close the actual file anyway. Since we stop at encountering
    * a line with "EOF" on it, this will be the normal case.
    */
   if (curf.fp != NULL)
      (void)fclose(curf.fp);

   if (ret == SUCCESS)
   {
      assert(g != NULL);

      if (g->source == -1)
      {
         for(i = 0; i < g->knots; i++)
            if ((g->term[i] == 0)
               && ((g->source < 0) || (g->grad[i] > g->grad[g->source])))
               g->source = i;
      }

      assert(graph_valid(g));
   }
   return((ret == SUCCESS) ? g : NULL);
}



/*
 * Minimum Spanning Tree methods
 */

static int mst_heap_nearest(
   int* heap,
   int* state,
   int* count,    /* pointer to store the number of elements on the heap */
   const MST* path)
{
   int   k;
   int   t;
   int   c;
   int   j;

   /* Heap shift down
    */
   k              = heap[1];
   j              = 1;
   c              = 2;
   heap[1]        = heap[(*count)--];
   state[heap[1]] = 1;

   if ((*count) > 2)
      if (LT(path[heap[3]].dist, path[heap[2]].dist))
         c++;

   while((c <= (*count)) && GT(path[heap[j]].dist, path[heap[c]].dist))
   {
      t              = heap[c];
      heap[c]        = heap[j];
      heap[j]        = t;
      state[heap[j]] = j;
      state[heap[c]] = c;
      j              = c;
      c             += c;

      if ((c + 1) <= (*count))
         if (LT(path[heap[c + 1]].dist, path[heap[c]].dist))
            c++;
   }
   return(k);
}

static void mst_heap_correct(
   int* heap,
   int* state,
   int* count,    /* pointer to store the number of elements on the heap */
   MST*  path,
   int    l,
   int    k,
   int    e,
   double cost)
{
   int    t;
   int    c;
   int    j;

   path[l].dist = cost;
   path[l].edge = e;

   /* Has the node not been processed yet?
    */
   if (state[l] == UNKNOWN)
   {
      heap[++(*count)] = l;
      state[l]      = (*count);
   }

   /* Heap shift up
    */
   j = state[l];
   c = j / 2;

   while((j > 1) && GT(path[heap[c]].dist, path[heap[j]].dist))
   {
      t              = heap[c];
      heap[c]        = heap[j];
      heap[j]        = t;
      state[heap[j]] = j;
      state[heap[c]] = c;
      j              = c;
      c              = j / 2;
   }
}

/** initialize a Minimum Spanning Tree on the graph */
static void mst_init(
   GRAPH* p)
{
   assert(p->path_heap  == NULL);
   assert(p->path_state == NULL);

   p->path_heap  = (int*)malloc((size_t)p->knots * sizeof(int));
   p->path_state = (int*)malloc((size_t)p->knots * sizeof(int));

   assert(p->path_heap  != NULL);
   assert(p->path_state != NULL);
}

/** free the Minimum Spanning Tree data structures on the graph */
static void mst_exit(
   GRAPH* p)
{
   assert(p->path_heap  != NULL);
   assert(p->path_state != NULL);

   free(p->path_heap);
   free(p->path_state);

   p->path_heap  = NULL;
   p->path_state = NULL;
}

/** execute a Minimum Spanning Tree computation */
static void mst_exec(
   const GRAPH*  p,
   int           start,
   double*       cost,
   MST*          path)
{
   int   k;
   int   m;
   int   i;
   int* heap;
   int* state;
   int count;

   assert(p      != NULL);
   assert(start  >= 0);
   assert(start  <  p->knots);
   assert(p->path_heap   != NULL);
   assert(p->path_state  != NULL);
   assert(path   != NULL);
   assert(cost   != NULL);

   /* if there are no nodes, terminate
    */
   if (p->knots == 0)
      return;

   heap = p->path_heap;
   state = p->path_state;

   /* initalize all needed data structures
    */
   for(i = 0; i < p->knots; i++)
   {
      state[i]     = UNKNOWN;
      path[i].dist = FARAWAY;
      path[i].edge = -1;
   }
   /* add the starting node to the heap
    */
   k            = start;
   path[k].dist = 0.0;

   /* at least two nodes are necessary for the heap to work
    */
   if (p->knots > 1)
   {
      count       = 1;
      heap[count] = k;
      state[k]    = count;

      /* as soon as the heap is empty the computation has been finished
       */
      while( count > 0 )
      {
         /* get a nearest node
          */
         k = mst_heap_nearest(heap, state, &count, path);

         /* mark the node as scanned
          */
         state[k] = CONNECT;

         /* correct adjacent nodes
          */
         for(i = p->outbeg[k]; i != EAT_LAST; i = p->oeat[i])
         {
            m = p->head[i];

            /* has the node not been scanned yet?
             * is it forbidded?
             */
            if ((state[m]) && (p->mark[m])

               /* is the new connection shorter than the currently best known?
                */
               && (GT(path[m].dist, cost[i])))
               mst_heap_correct(heap, state, &count, path, m, k, i, cost[i]);
         }
      }
   }
}


static void trim_right(
   char* s)
{
   size_t i;
   i = 0;
   while(s[i] != '\0' && isspace((unsigned char)s[i]))
      i++;
   memmove(s, s + i, strlen(s) - i + 1);
}

static void trim_left(
   char* s)
{
   size_t i;
   i = strlen(s);
   while(i > 0 && isspace((unsigned char)s[i - 1]))
      i--;
   s[i] = '\0';
}

/* delete whitespace both from the left- and the right-hand side */
static void trim(
   char* s)
{
   trim_right(s);
   trim_left(s);
}



/*
 * Clock
 */
static double clock_cputime2sec(
   clock_t cputime)             /* clock ticks for CPU time */
{
   clock_t clocks_per_second;

#if defined(_WIN32) || defined(_WIN64)
   clocks_per_second = 100;
#else
#ifndef CLK_TCK
   clocks_per_second = sysconf(_SC_CLK_TCK);
#else
   clocks_per_second = CLK_TCK;
#endif
#endif

   return (double)(cputime) / (double)clocks_per_second;
}


static void clock_start(
   clock_t* cputime)
{
   assert(cputime != NULL);

   *cputime = 0;
#if defined(_WIN32) || defined(_WIN64)
   FILETIME creationtime;
   FILETIME exittime;
   FILETIME kerneltime;
   FILETIME usertime;

   GetProcessTimes(GetCurrentProcess(), &creationtime, &exittime, &kerneltime, &usertime);
   *cputime -= usertime.dwHighDateTime * 42950 + usertime.dwLowDateTime / 100000L;
#else
   struct timeval tp;
   struct tms now;
   (void)times(&now);
   *cputime -= now.tms_utime;
#endif

}


static double clock_stop(
   clock_t* cputime)
{
   double result;
   assert(cputime != NULL);
   result = 0.0;
#if defined(_WIN32) || defined(_WIN64)
   FILETIME creationtime;
   FILETIME exittime;
   FILETIME kerneltime;
   FILETIME usertime;

   GetProcessTimes(GetCurrentProcess(), &creationtime, &exittime, &kerneltime, &usertime);
   result = clock_cputime2sec(*cputime + usertime.dwHighDateTime * 42950 + usertime.dwLowDateTime / 100000L);
#else
   struct timeval tp;
   struct tms now;

   (void)times(&now);
   result = clock_cputime2sec(*cputime + now.tms_utime);
#endif

   return result;
}


/*
 * Main
 */
int main()
{
   GRAPH*     graph;
   MST*       mst;
   FILE*      fp;
   clock_t    cputime;
   double     score;
   double     acc_time; 
   double     geo_mean = 1.0;
   double     exec_time;
   double     step_size;
   double     total_time = 0.0;
   double     cur_avg_time;
   int        i;
   int        pass; 
   int        root;
   int        nnodes;
   int        ninstances = 0;
   int        timed_trees;
   int        ncomputations = 0;
   int        warm_up_cache = 1; 
   int        min_timed_pass;
   char*      filename;
   char*      s;
   char       buffer[MAX_LINE_LEN];

   printf ("STP Benchmark version %d\n", VERSION_NUMBER);
   printf ("Running MST computations from %d roots for at least %.1f seconds.\n", MST_COMPUTATIONS, MINIMUM_TIME_PER_INSTANCE);
   printf ("Reading files from file ['%s'].\n", TEST_INSTANCES);
   fflush(stdout);

   /* read the test-instances file */
   if ((fp = fopen(TEST_INSTANCES, "r")) == NULL)
   {
      printf("Test-instances file missing \n");
      return 1;
   }

   /* read the file, containing the test instance locations, line per line */
   while(fp != NULL)
   {
      if ((filename = fgets(buffer, sizeof(buffer), fp)) == NULL)
      {
         (void)fclose(fp);
         break;
      }

      /* delete trailing whitespace */
      trim(filename);

      if (strcmp(filename, "EOF") == 0)
      {
	 (void)fclose(fp);
         break;
      }
      if (*filename == '\0')
         continue;

      /* load the graph */
      graph = graph_load(filename, NULL);
      if (graph == NULL)
         continue;

      /* the number of nodes */
      nnodes = graph->knots;

      if (nnodes >= MST_COMPUTATIONS)
	 ncomputations = MST_COMPUTATIONS;
      else
	 ncomputations = nnodes;

      if (ncomputations > 1)
         step_size = (double) (nnodes - 1) / (double) (ncomputations - 1);
      else
         step_size = 0;

      /* initialize a MST on the current graph */
      mst = (MST*)malloc((size_t) nnodes * sizeof(*mst));
      mst_init(graph);
      printf ("Input '%s' (%d nodes, %d edges, %d roots)... ", filename, nnodes, graph->edges, ncomputations);

      /* compute MSTs for ncomputations many (relatively) equidistant root nodes */
      acc_time = 0.0; //accumulated time for this instance
      warm_up_cache = 1; //do not time the first run (to warm up the cache)
      timed_trees = 0;
      pass = 0; //
      min_timed_pass = warm_up_cache ? 1 : 0; //

      /* run the entire experiment repeatedly until the total accumulated time
       * is at least MINIMUM_TIME_PER_INSTANCE; this gets around precision issues with the timer */
      clock_start(&cputime);
      while (GT(MINIMUM_TIME_PER_INSTANCE, acc_time)) {
         /* initialize the timer when appropriate */
         if (pass == min_timed_pass) 
            clock_start(&cputime);

         /* perform MST computations */
         for(i = 0; i < ncomputations; i++) {
            root = step_size * i;
            mst_exec(graph, root, graph->cost, mst);
         }

         /* if this is a timed pass, update counters and check current time */
         if (pass >= min_timed_pass) {
            timed_trees += ncomputations;
            acc_time = clock_stop(&cputime);

         }
         pass++;
      }

      cur_avg_time = 1000000.0 * MINIMUM_TIME_PER_INSTANCE / timed_trees; //scaled to milliseconds

      if (GT(cur_avg_time, 0.0)) { //this should always be true
         ninstances++;
         geo_mean = geo_mean * cur_avg_time;
         total_time += cur_avg_time;
      }
      else {
         printf("%s won't be used for the gemetric mean computation \n", filename);
      }

      printf("%.2f microseconds/tree (%d trees, %d passes)\n", cur_avg_time, timed_trees, pass);
	  fflush(stdout);

      /* free memory */
      mst_exit(graph);
      free(mst);
      graph_free(graph);
      graph = NULL;
   }
   geo_mean = pow(geo_mean, 1.0 / (double)ninstances); //scaled to milliseconds
   printf("\nGeometric mean over %d instances: %.2f microseconds/tree.\n", ninstances, geo_mean);
   score = 1000000.0 / geo_mean;
   printf("This corresponds to %.6f trees/second.\n", score);
   printf("\nSCORE: %.6f \n", score);

   return 0;
}