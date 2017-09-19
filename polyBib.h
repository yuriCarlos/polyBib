#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXNPTS 1000

/* Types. */
typedef struct point_2d
{
    double x;
    double y;

} point2d;


typedef struct poligono
{
    int n;
    point2d points[MAXNPTS];

} Poligono;

typedef struct diagonal{
	int i;
	int f;
	struct diagonal *next;
}Diagonal;



//===============================| DCEL |=================================

typedef struct edge{
	struct vertex *origin;
	struct edge *twin;

	struct face *face;
	struct edge*next;
	struct edge *prev;

	int pos;

} Edge;

typedef struct vertex{
	float x;
	float y;

	Edge *incidentEdge;
} Vertex;

typedef struct face{
	Edge *startPoint;
	int pos;

}Face;

typedef struct list{
	void *val;
	struct list *next;
}List;

typedef struct dcel{
	Vertex *vertex[MAXNPTS];
	Edge *edges[MAXNPTS];
	int nEdges;
	int nVertex;
	int nFaces;
	Face *faces[MAXNPTS];
}Dcel;


Dcel *criaDcel();
Vertex *criaVertice(Dcel* dcel, float x, float y);
Edge *criaAresta(Dcel *dcel, Vertex *a, Vertex *b);
float anguloVetores(Vertex *v1, Vertex *v2, Vertex *v3);
double areaTrianguloD(Vertex *a, Vertex *b, Vertex *c);
int leftD(Vertex *a, Vertex *b, Vertex *c);
int leftOnD(Vertex *a, Vertex *b, Vertex *c);
Edge *proximoMesmaOrigem(Edge *e);
Edge * removeAresta(Dcel *dcel, Vertex *a, Vertex *b);



//retorna as diagonais de um poligono
Diagonal *triangulariza(Poligono *p);

double areaTriangulo(point2d a, point2d b, point2d c);
//diz se o ponto C esta ou não a esquerda da aresta AB
int left(point2d a, point2d b, point2d c);
//diz se esta a esquerda ou em cima
int leftOn(point2d a, point2d b, point2d c);

//diz se existe uma interseccao propria
int intersecPropria(point2d a, point2d b, point2d c, point2d e);

//diz se existe uma interseccao inpropria
int intersecImpropria(point2d a, point2d b, point2d c, point2d e);

//diz se os pontos são colineares
int colinear(point2d a, point2d b, point2d c);

//verifica se o ponto c esta entre a e b
int between(point2d a, point2d b, point2d c);

//verifica se existe alguma interseccao seja ela propria(2) ou impropria(2)
int intersec(point2d a, point2d b, point2d c, point2d d);

//verifica se a reta AB intersecta algum ponto de p
int intersecPoligono(point2d *p, int n, point2d a, point2d b);

Poligono *iniciaPoligono();

//verifica se o vertice i eh convexo
int convexo(Poligono *p, int i);

//verifica se existe uma orelha
int ehOrelha(Poligono *p, int i);

//verifica se p1 esta abaixo de p2
int abaixo(point2d p1, point2d p2);


//classifica um ponto de um poligono cmo:
//start: 1
//split: 2
//end : 3
//merge: 4
//regular: 0
int classificaPonto(Poligono *p, int i);