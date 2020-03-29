#ifndef _graph_h
#define _graph_h

/*******************************************************************
@ddblock_begin copyright 

Copyright (c) 1997-2015
Maryland DSPCAD Research Group, The University of Maryland at College Park 

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

@ddblock_end copyright 
********************************************************************/

/*******************************************************************************
Header file for a directed graph abstract data type.
Author: Shuvra S. Bhattacharyya and Robert Ricketts
*******************************************************************************/
 
#include "floatmath.h"

/* A vertex in a graph. */
typedef struct _graph_vertex graph_vertex;   

/* A pointer to a graph vertex. */
typedef graph_vertex *graph_vertex_pointer;      

/* An edge in a graph. */
typedef struct _graph_edge graph_edge;

/* A pointer to a graph edge. */
typedef graph_edge *graph_edge_pointer;

/* A graph. */
typedef struct _graph graph_type;

/* A pointer to a graph. */
typedef graph_type *graph_pointer;

/* An array of pointers to graph edges. */
typedef graph_edge_pointer *graph_edge_array;


/* ----------------------------  Public functions --------------------- */

/* Given a graph and an index ("vertex number"), returns a pointer to the vertex
at that index. Vertices are numbered starting at zero.
*/
graph_vertex_pointer graph_getVertex(graph_pointer, int);

/* Given a graph and an index ("edge number"), returns a pointer to the edge
at that index. Edges are numbered starting at zero.
*/
graph_edge_pointer graph_getEdge(graph_pointer, int);

/* Returns the number of vertices in a graph. */
int graph_vertex_count(graph_pointer);

/* Returns the number of edges in a graph. */
int graph_edge_count(graph_pointer);

/* Returns the number of output edges of a given vertex. */
int graph_successor_count(graph_vertex_pointer); 

/* Returns the number of input edges of a given vertex. */
int graph_predecessor_count(graph_vertex_pointer); 

/* Returns the integer index of a given vertex. Vertices are labeled with
   successive integers starting at zero. Each vertex has a unique index.
*/
int graph_vertex_number(graph_vertex_pointer);

/* Returns the array of input edges for a given vertex. Each element of
   this array is a pointer to a graph edge. 
*/
graph_edge_array graph_input_edges(graph_vertex_pointer); 

/* Returns the array of output edges for a given vertex. Each element of
   this array is a pointer to a graph edge. 
*/
graph_edge_array graph_output_edges(graph_vertex_pointer);

/* Returns the integer index of a given edge. Edges are labeled with
   successive integers starting at zero. Each edge a unique index.
*/
int graph_edge_number(graph_edge_pointer edge);

/* Returns a pointer to the source vertex of a given edge. */
graph_vertex_pointer graph_source_vertex(graph_edge_pointer);

/* Returns a pointer to the sink vertex of a given edge. */
graph_vertex_pointer graph_sink_vertex(graph_edge_pointer);

/* frees the space used by a graph */
void graph_deallocate(graph_pointer graph);

/*creates an exact copy of a graph*/
graph_pointer graph_copy( graph_pointer);

/* 

Function graph_read()

This function reads the topology of a graph from a file, allocates storage for
a graph and sets the topological properties (vertices and edges) of the graph 
allocated to the topology that was read in.  The function returns a pointer to 
the graph that is created. 

Lines in the input file can have the following form:

-- Blank line (empty line or only white space).

-- A line that begins with ';': comment line

-- A line that contains two integers; the two integers are interpreted as the
source and sink, repspectively of the next edge in the graph. Each integer
must be a number between 0 and N-1, inclusive, where N is the total 
number of vertices in the graph. In other words, when constructing the
vertex set, this function will look for the largest vertex number Z in the
list of edge specifications, and create vertices numbered 0, 1, ..., Z.
If a number in this range is *not* included in at least one edge specification,
then the corresponding vertex will be isolated from the rest of the graph
(i.e., it will not have any incident edges).

-- A line that contains two integers (defining the next edge, as above),
followed by white space and zero or more characters of arbitrary text.  The
arbitrary additional text is ignored, and can be used to provide comments for
the edge declaration.

Argument:

A file pointer for the input file. The file must already be opened for reading.

*/
graph_pointer graph_read(FILE *);

/*

Function graph_construct()

This function reads the topology of a graph from a pair of arrays, allocates
storage for a graph and sets the topological properties (vertices and edges) of
the graph allocated to the topology that was read in through the two arrays.
The arrays provide, respectively the source indices and sink indices of each
edge in the graph.  The function returns a pointer to the graph that is
created. 

Arguments:

-- Argument 1: This is an array of integers. The ith element of the array gives
the source vertex number for edge number i.

-- Argument 2: This is another array of integers. The ith element of the array
gives the sink vertex number for edge number i.

-- Argument 3: The number of edges in the graph (i.e., the number of elements
in each of the arrays of Arguments 1 and 2).

*/
graph_pointer graph_construct(int *, int *, int);

/*

Function graph_generateDot()

This function generates dot code to display a graph using the graphviz graph 
drawing suite.

Arguments:

-- Argument 1: a pointer to the graph for which code is to be generated.

-- Argument 2: the file to which the code should be written. This file
should already be opened for writing.

-- Argument 3: the identifer used to name the graph in the generated dot code.

*/
void graph_generateDot(graph_pointer, FILE *, char *);

/*

Function graph_readIntegerAttributes()

This function reads integer-valued edge or vertex attributes from a file,
allocates an array of integers, and fills the array in with the values that
were read. If edge (vertex) attributes are being read, then the ith element
of the array is filled in with the attribute value for the edge (vertex)
whose index ("number") is i. The syntax of attribute specifications is
similar to that for graph specifications (see (graph_read)).

Lines in the input file can have the following form:

-- Blank line (empty line or only white space).

-- A line that begins with ';': comment line

-- A line that contains two integers; the two integers are interpreted as the
edge (vertex) index and the corresponding attribute value, repspectively.

-- A line that contains two integers (defining an attribute specification, as
above), followed by white space and zero or more characters of arbitrary text.
The arbitrary additional text is ignored, and can be used to provide comments
for the attribute specification.

Arguments:

-- Argument 1: a pointer to the graph for which attributes are to be read.

-- Argument 2: the file from which input should be taken.

-- Argument 3: the default value for this attribute. An edge (vertex) that
does not have an attribute specification in the input will have its attribute
value (corresponding array element) set to this default value.a

-- Argument 4: (Boolean-valued). If this argument is true (nonzero), edge
attributes are read, and the number of elements in the returned array is equal
to the number of graph edges. Otherwise, vertex attributes are read. 

*/
int *graph_readIntegerAttributes(graph_pointer, FILE *, int, int); 

/*

Function graph_readVectorAttributes()

This function reads vector-valued edge or vertex attributes from a file,
allocates an array of pointers to vectors, and fills the array in with the
starting addresses of the vectors that were allocated and filled in.
If edge (vertex) attributes are being read, then the ith element
of the array is filled in with the attribute value for the edge (vertex)
whose index ("number") is i. An array of vector pointers (i.e., array of 
vector starting addresses) is returned by the function.

Lines in the input file can have the following form:

-- Blank line (empty line or only white space).

-- A line that begins with ';': comment line

-- A line that contains an integer followed by one or more floating point
numbers. The leading integers is interpreted as the edge (vertex) index, and
floating point numbers make up the associated vector-valued attribute.  Any
additional text after the vector specification is ignored, so arbitrary
descriptive text (starting with a non-number) can be appended after the vector
specification.

Arguments:

-- Argument 1: a pointer to the graph for which attributes are to be read.

-- Argument 2: the file from which input should be taken.

-- Argument 3: a pointer to the default value for this attribute. An edge
(vertex) that does not have an attribute specification in the input will have
its attribute value (corresponding array element) set to this default value. 

-- Argument 4: (Boolean-valued). If this argument is true (nonzero), edge
attributes are read, and the number of elements in the returned array is equal
to the number of graph edges. Otherwise, vertex attributes are read. 

*/
floatmath_vector **graph_readVectorAttributes(graph_pointer, FILE *, 
        floatmath_vector *, int); 

#endif
