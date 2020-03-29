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

/*
 * Implementation of abstract data type for graphs.
 * @author Shuvra S. Bhattacharyya
 * @version $Id: graph.c 2136 2008-11-29 16:53:26Z ssb $
 *
 */
 
  /*
 	modified 10/23/06 by Robert Ricketts, 
 	added deallocate function
 	added copy function
 
 */
 

#include <stdio.h>

#include "graph.h"
#include "lklst.h"
#include "string_parsing.h"
#include "stlib.h"

/* The maximum number of edges in a graph. */
#define MAXIMUM_EDGES 1024

/* The maximum number of edges in a graph. */
#define MAXIMUM_VERTICES 1024

/* The maximum number of characters in an input line. */
#define MAXIMUM_LINE_LENGTH 256

/* The maximum number of input edges for a graph vertex. */
#define MAXIMUM_VERTEX_INPUTS 16

/* The maximum number of input edges for a graph vertex. */
#define MAXIMUM_VERTEX_OUTPUTS 16

/* The maximum length of vectors that provide edge or vertex attributes. */
#define MAXIMUM_VECTOR_LENGTH 64

/* The character that appears at the beginning of comment lines in the
   input file. 
*/
#define COMMENT_CHARACTER ';'

/* The maximum number of characters in a node attribute specification. 
*/
#define MAXIMUM_NODE_ATTRIBUTE_LENGTH 40

/* The number of node attributes that are specified in generated dot code. 
*/
#define NODE_ATTRIBUTE_COUNT 5

/* The maximum number of characters in a graph attribute specification. 
*/
#define MAXIMUM_GRAPH_ATTRIBUTE_LENGTH 40

/* The number of graph attributes that are specified in generated dot code. 
*/
#define GRAPH_ATTRIBUTE_COUNT 1

/* The maximum number of characters in an edge attribute specification. 
*/
#define MAXIMUM_EDGE_ATTRIBUTE_LENGTH 40

/* The number of edge attributes that are specified in generated dot code. 
*/
#define EDGE_ATTRIBUTE_COUNT 1

/* One unit of indentation (four spaces) in the generated output. 
*/
#define DOT_INDENTATION "    "

/* "Node attributes" in "dot" control the appearance of vertices when graphs are
   drawn. Similarly, "edge attributes" control the appearances of edges.
   The following preprocessor constants pertain to the handling of 
   node attributes and edge attributes in dot.
*/

/* The set of node attributes to use in graphs that are drawn through dot.
   This must be consistent with the related preprocessor constants above.
*/
char nodeAttributes[NODE_ATTRIBUTE_COUNT][MAXIMUM_NODE_ATTRIBUTE_LENGTH] = 
        {"shape = circle", "style = filled", "height = 0.1", "width = 0.1",
        "fontsize = 9"}; 

/* The set of edge attributes to use in graphs that are through dot.
   This must be consistent with the related preprocessor constants above.
*/
char edgeAttributes[EDGE_ATTRIBUTE_COUNT][MAXIMUM_EDGE_ATTRIBUTE_LENGTH] =
        {"fontsize = 9"};

/* The set of graph attributes to use in graphs that are drawn through dot. 
   This must be consistent with the related preprocessor constants above.
*/
char graphAttributes[GRAPH_ATTRIBUTE_COUNT][MAXIMUM_GRAPH_ATTRIBUTE_LENGTH] =
        {"size = \"7.5, 10\""};

/*
 * Data structure for representing an edge in an acyclic SDF graph.
 *
 */
struct _graph_edge {
   graph_vertex_pointer source_vertex;
   graph_vertex_pointer sink_vertex;

   /* Edge index for identification purposes; numbering starts at 0. */
   int number;  
};

/*
 * Data structure for representing an vertex in a graph.
 *
 */
struct _graph_vertex {

   /* The number of predecessors of this vertex. */
   int predecessor_count;

   /* The number of successors of this vertex. */
   int successor_count;

   /* The output edges of this vertex. */
   graph_edge_array output_edges;
   
   /* The input edges of this vertex. */
   graph_edge_array input_edges;

   /* Vertex index for identification purposes; numbering starts at 0. */
   int number;  
};

/* Data structure for representing a graph. */
struct _graph {

  /* The number of vertices in the graph. */
  int vertex_count; 
 
  /* The number of edges in the graph. */
  int edge_count; 

  /* The vertices in the graph. */
  graph_vertex_pointer vertices;  

  /* The edges in the graph. */
  graph_edge_pointer edges;    
};


graph_pointer graph_read(FILE *input_file)
{
    int edge_count = 0; 
    int line_count = 0;
    int edge_source, edge_sink;
    char buffer[MAXIMUM_LINE_LENGTH + 2];
    int edge_sources[MAXIMUM_EDGES];
    int edge_sinks[MAXIMUM_EDGES];
    
    while (read_line(buffer, MAXIMUM_LINE_LENGTH, input_file)) {
        line_count++;
        if ((!blankstr(buffer)) && (buffer[0] != COMMENT_CHARACTER)) {
            if (sscanf(buffer, "%d%d", &edge_source, &edge_sink) != 2) {
                error_fatal("Syntax error on line %d.\n", line_count);
            }
            edge_count++;
            if (edge_count > MAXIMUM_EDGES) {
                error_fatal("Too many edges in graph.\n");
            }
            edge_sources[edge_count - 1] = edge_source;
            edge_sinks[edge_count - 1] = edge_sink;
        }
    }

    return graph_construct(edge_sources, edge_sinks, edge_count);
} /* graph_read */

graph_pointer graph_construct(int *edgeSources, int *edgeSinks, int edgeCount) {
    int vertex_count;
    int i;
    graph_edge_pointer edges;
    graph_vertex_pointer vertices;
    int source, sink; 
    int successor, predecessor;
    graph_pointer newGraph;
    int maximum_vertex_index;

    /* Determine the number of vertices in the graph. */
    maximum_vertex_index = -1;
    for (i = 0; i < edgeCount; i++) {
        if (edgeSources[i] > maximum_vertex_index) {
            maximum_vertex_index = edgeSources[i];
        }
        if (edgeSinks[i] > maximum_vertex_index) {
            maximum_vertex_index = edgeSinks[i];
        }
    }
    vertex_count = maximum_vertex_index + 1;
    if (vertex_count > MAXIMUM_VERTICES) {
        error_fatal("Too many vertices in graph.\n");
    }
  
    /* Set up the information for the graph vertices. */
    vertices = allocate_array(vertex_count, graph_vertex);
    for (i = 0; i < vertex_count; i++) {
        vertices[i].number = i;
        vertices[i].predecessor_count = 0;
        vertices[i].successor_count = 0;
        vertices[i].output_edges = 
                allocate_array(MAXIMUM_VERTEX_OUTPUTS, graph_edge_pointer);
        vertices[i].input_edges = 
                allocate_array(MAXIMUM_VERTEX_INPUTS, graph_edge_pointer);
    }
  
    /* Set up the information for the graph edges. */
    edges = allocate_array(edgeCount, graph_edge);

    /* Fill in the information for the vertices and edges. */
    for(i = 0; i < edgeCount; i++) {
        source = edgeSources[i];
        sink = edgeSinks[i];
        edges[i].source_vertex = &(vertices[source]) ;
        edges[i].sink_vertex = &(vertices[sink]);
        edges[i].number = i;
        vertices[source].successor_count++;
        vertices[sink].predecessor_count++;
        if (vertices[source].successor_count > MAXIMUM_VERTEX_OUTPUTS) {
            error_fatal("Excessive fan-out on vertex %d.\n", source);
        }
        if (vertices[sink].predecessor_count > MAXIMUM_VERTEX_INPUTS) {
            error_fatal("Excessive fan-in on vertex %d.\n", sink);
        }
        successor = vertices[source].successor_count - 1;
        predecessor = vertices[sink].predecessor_count - 1;
        vertices[source].output_edges[successor] = &(edges[i]);
        vertices[sink].input_edges[predecessor] = &(edges[i]);
    }  

    /* Allocate & set up the graph. */
    newGraph = allocate_scalar(graph_type);
    newGraph->vertices = vertices;
    newGraph->vertex_count = vertex_count;
    newGraph->edge_count = edgeCount;
    newGraph->edges = edges;
  
    return newGraph; 
}


void graph_generateDot(graph_pointer graph, FILE *outputFile, char *name) {
    int i;
    graph_vertex_pointer source, sink;
    graph_edge_pointer edge;

    /* Print the graph header. */
    fprintf(outputFile, "digraph %s {\n", name);

    /* Print the graph attribute specification. */
    for (i = 0; i < GRAPH_ATTRIBUTE_COUNT; i++) {
        fprintf(outputFile, "%s", DOT_INDENTATION); 
        fprintf(outputFile, "%s;\n", graphAttributes[i]); 
    }

    /* Print the node attribute specification. */
    fprintf(outputFile, "%snode [", DOT_INDENTATION);
    for (i = 0; i < NODE_ATTRIBUTE_COUNT; i++) {
        fprintf(outputFile, "%s", nodeAttributes[i]); 
        if (i < (NODE_ATTRIBUTE_COUNT - 1)) {
            fprintf(outputFile, ",\n%s%s%s", DOT_INDENTATION, DOT_INDENTATION, 
                    DOT_INDENTATION);
        }
    }
    fprintf(outputFile, "];\n");

    /* Print the edge attribute specification. */
    fprintf(outputFile, "%sedge [", DOT_INDENTATION);
    for (i = 0; i < EDGE_ATTRIBUTE_COUNT; i++) {
        fprintf(outputFile, "%s", edgeAttributes[i]); 
        if (i < (EDGE_ATTRIBUTE_COUNT - 1)) {
            fprintf(outputFile, ",\n%s%s%s", DOT_INDENTATION, DOT_INDENTATION, 
                    DOT_INDENTATION);
        }
    }
    fprintf(outputFile, "];\n");


    /* Print the edges with appropriate labels. */
    for (i = 0; i < graph_edge_count(graph); i++) {
        edge = graph_getEdge(graph, i);
        source = graph_source_vertex(edge);
        sink = graph_sink_vertex(edge);
        fprintf(outputFile, "%s", DOT_INDENTATION);
        fprintf(outputFile, "%d -> %d ", 
                source->number, sink->number);
        fprintf(outputFile, "[label = \"%d\"];\n", 
                graph_edge_number(edge));
    }

    /* Print the footer code. */
    fprintf(outputFile, "%s", DOT_INDENTATION); 
    fprintf(outputFile, "/* The graph contains %d vertices and %d edges. */\n",
            graph_vertex_count(graph), graph_edge_count(graph));
    fprintf(outputFile, "}\n");
}

int *graph_readIntegerAttributes(graph_pointer graph, FILE *input,
        int defaultValue, boolean isEdgeAttribute) {

    int attributeCount, index, *result, line_count = 0, value;
    char buffer[MAXIMUM_LINE_LENGTH + 2];

    /* Allocate and initialize the attribute array. */
    if (isEdgeAttribute) {
        attributeCount = graph_edge_count(graph);
    } else {
        attributeCount = graph_vertex_count(graph);
    }
    result = allocate_array(attributeCount, int);
    for (index = 0; index < attributeCount; index++) {
        result[index] = defaultValue;
    }

    /* Read the attribute values. */
    while (read_line(buffer, MAXIMUM_LINE_LENGTH, input)) {
        line_count++;
        if ((!blankstr(buffer)) && (buffer[0] != COMMENT_CHARACTER)) {
            if (sscanf(buffer, "%d%d", &index, &value) != 2) {
                error_fatal("Syntax error on line %d\n", line_count);
            }
            if ((index < 0) || (index >= attributeCount)) {
                error_fatal("Invalid index at line %d\n", line_count);
            }
            result[index] = value; 
        }
    }

    return result;
}
        
floatmath_vector **graph_readVectorAttributes(graph_pointer graph, FILE *input, 
        floatmath_vector *defaultValue, int isEdgeAttribute) {

    int attributeCount; 
    int index;
    int line_count = 0;
    int vectorLength;
    char buffer[MAXIMUM_LINE_LENGTH + 2];
    floatmath_vector **result;
    double vectorValue[MAXIMUM_VECTOR_LENGTH];
    char *nextField;
    boolean endOfVector;
    double vectorElement;

    /* Allocate and initialize the attribute array. */
    if (isEdgeAttribute) {
        attributeCount = graph_edge_count(graph);
    } else {
        attributeCount = graph_vertex_count(graph);
    }
    result = allocate_array(attributeCount, floatmath_vector *); 
    for (index = 0; index < attributeCount; index++) {
        result[index] = defaultValue;
    }

    /* Read the attribute values. */
    while (read_line(buffer, MAXIMUM_LINE_LENGTH, input)) {
        line_count++;
        if ((!blankstr(buffer)) && (buffer[0] != COMMENT_CHARACTER)) {
            if (sscanf(buffer, "%d", &index) != 1) {
                error_fatal("Missing index on line %d\n", line_count);
            }
            if ((index < 0) || (index >= attributeCount)) {
                error_fatal("Invalid index at line %d\n", line_count);
            }
            nextField = buffer;
            vectorLength = 0;
            endOfVector = FALSE;
            do {
                nextField = sp_nextfield(nextField);
                if (sscanf(nextField, "%lf", &vectorElement) == 1) {
                    vectorLength++;       
                    if (vectorLength > MAXIMUM_VECTOR_LENGTH) {
                        error_fatal("%s on line %d.", 
                                "Maximum vector length exceeded", line_count);
                    }
                    vectorValue[vectorLength - 1] = vectorElement;
                } else {
                    endOfVector = TRUE; 
                } 
            } while (!endOfVector);
            if (vectorLength == 0) {
                error_fatal("Empty vector on line %d.", line_count);
            }  
            result[index] = floatmath_vectorNew(vectorLength, vectorValue);
        }
    }

    return result;
}

void graph_deallocate(graph_pointer graph){
	int i;
	free(graph->edges);
	
	for(i=0;i<graph->vertex_count;i++){
		free(graph->vertices[i].output_edges);	
		free(graph->vertices[i].input_edges);
	}
	free(graph->vertices);	
	free(graph);
	
	
}

graph_pointer graph_copy(graph_pointer oldGraph){
	int numEdges =oldGraph->edge_count;
	int * edgeSources = xmalloc(sizeof(int)*numEdges);
	int * edgeSinks = xmalloc(sizeof(int)*numEdges);
	int i;
    graph_pointer newGraph = NULL;

	graph_edge_pointer e;
	for(i=0;i<numEdges;i++){
		e= (oldGraph->edges)+i;
		edgeSources[i]= e->source_vertex->number;
		edgeSinks[i]= e->sink_vertex->number;
	}
	newGraph =graph_construct(edgeSources,edgeSinks,numEdges);
	free(edgeSources);
	free(edgeSinks);
	return newGraph;
	
	
}

graph_vertex_pointer graph_getVertex(graph_pointer graph, int index) {
    return &(graph->vertices[index]);
}

graph_edge_pointer graph_getEdge(graph_pointer graph, int index) {
    return &(graph->edges[index]);
}

int graph_vertex_count(graph_pointer graph) {
    return graph->vertex_count;
}

int graph_edge_count(graph_pointer graph) {
    return graph->edge_count;
}

int graph_successor_count(graph_vertex_pointer vertex) {
    return vertex->successor_count;
}

int graph_predecessor_count(graph_vertex_pointer vertex) {
    return vertex->predecessor_count;
}

int graph_vertex_number(graph_vertex_pointer vertex) {
    return vertex->number; 
}

graph_edge_array graph_input_edges(graph_vertex_pointer vertex) {
    return vertex->input_edges;
}

graph_edge_array graph_output_edges(graph_vertex_pointer vertex) {
    return vertex->output_edges;
}

int graph_edge_number(graph_edge_pointer edge) {
    return edge->number; 
}

graph_vertex_pointer graph_source_vertex(graph_edge_pointer edge) {
    return edge->source_vertex;
}

graph_vertex_pointer graph_sink_vertex(graph_edge_pointer edge) {
    return edge->sink_vertex;
}

