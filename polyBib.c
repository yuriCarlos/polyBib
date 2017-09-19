#include <stdio.h>
#include <stdlib.h>
#include "polyBib.h"

//diz se existe uma interseccao propria
int colinear(point2d a, point2d b, point2d c){
    return areaTriangulo(a,b,c) == 0;
}

Poligono *iniciaPoligono(){
    Poligono *p = (Poligono *) malloc(sizeof(Poligono));
    p->n = 0;
    return p;
}

int convexo(Poligono *p, int i){
    int ant = i - 1 < 0 ? p->n -1 : i - 1;
    //printf("atual %d anterior %d proximo %d \n",i, ant,(i + 1)% p->n);
    return left(p->points[ant], p->points[i], p->points[(i + 1)% p->n]);
}

int classificaPonto(Poligono *p, int i){
    int ant = i - 1 < 0 ? 0 : i - 1;

    //start
    if(convexo(p,i) && abaixo(p->points[ant], p->points[i]) && abaixo(p->points[(i + 1) % p->n], p->points[i])) return 1;
    //split
    if(!convexo(p,i) && abaixo(p->points[ant], p->points[i]) && abaixo(p->points[(i + 1) % p->n], p->points[i])) return 2;
    //end
    if(convexo(p,i) && abaixo(p->points[i], p->points[ant]) && abaixo(p->points[i], p->points[(i + 1) % p->n])) return 3;
    //merge
    if(!convexo(p,i) && abaixo(p->points[i], p->points[ant]) && abaixo(p->points[i], p->points[(i + 1) % p->n])) return 4;
    //regular
    return 0;
}

int abaixo(point2d p1, point2d p2){
    if(p1.y < p2.y) return 1;
    return p1.y == p2.y && p1.x > p2.x;
}


int intersecPropria(point2d a, point2d b, point2d c, point2d e){
    return left(a, b, c) != left(a, b, e) && left(c, e, a) != left(c, e, b) && 
    (!colinear(a, b, c) && !colinear(a, b, e) && !colinear(c, e, a) && !colinear(c, e, b));
}

int left(point2d a, point2d b, point2d c){
    return areaTriangulo(a,b,c) > 0;
}

int leftOn(point2d a, point2d b, point2d c){
    return areaTriangulo(a,b,c) >= 0;
}

int intersecPoligono(point2d *p, int n, point2d a, point2d b){
    int i, numIntersec = 0;
    if(n < 2){
        return -1;
    }

    //conta as interseccoes
    for(i = 1; i < n; i++)
        if(intersecPropria(p[i], p[i - 1], a, b))
            numIntersec++;
    

    //verifica o ultimo ponto com o primeiro
    if(n > 2 && intersecPropria(p[0], p[n - 1], a, b))
        numIntersec++;

    return numIntersec;
}

int intersec(point2d a, point2d b, point2d c, point2d d){
    if(intersecPropria(a,b,c,d)) return 2;
    return intersecImpropria(a,b,c,d);
}

int intersecImpropria(point2d a, point2d b, point2d c, point2d e){
    return between(a,b,c) || between(a,b,e) || between(c,e,a) || between(c,e,b);
}

int between(point2d a, point2d b, point2d c){
    return colinear(a, b, c) && ((c.y <= b.y && c.y >= a.y && c.x <= b.x && c.x >= a.x) || (c.y >= b.y && c.y <= a.y && c.x >= b.x && c.x <= a.x));
}

double areaTriangulo(point2d a, point2d b, point2d c){
    return ((b.x-a.x)*(c.y - b.y) - (c.x - b.x)*(b.y - a.y))/2;
}

int ehOrelha(Poligono *p, int i){
    int ant = i - 1 < 0 ? p->n - 1 : i - 1;
    return convexo(p, i) && !intersecPoligono(p->points, p->n, p->points[ant], p->points[(i + 1)% p->n]);
}

Diagonal *triangulariza(Poligono *p){
    int *estados = malloc(sizeof(int) * p->n), i, numOrelhas = 0, anterior, esquerda, direita, qualquer, naoConvexos = 0;
    Diagonal *diagonais = NULL;
    int maisBaixo = 0;

    numOrelhas = p->n;
    
    //marca todos que sao ou nao orelhas e pega o ponto mais baixo
    for(i = 0; i < p->n; i++){
        if(ehOrelha(p, i))
            estados[i] = 1;
        else
            estados[i] = 0;
        
        if(!convexo(p, i))
            naoConvexos ++;

        printf("%d\n", estados[i]);

        if(i != 0 && abaixo(p->points[i], p->points[maisBaixo]) && estados[i])
            maisBaixo = i;
    }

    if(naoConvexos > p->n/2) return NULL;


    //registra as diagonais
    while(numOrelhas > 3){

        esquerda = maisBaixo - 1 < 0? p->n - 1 : maisBaixo - 1;
        direita = (maisBaixo + 1) % p->n;
        //pega o ponto anterior e proximo ao atual que ainda nao foram triangularizados
        while(estados[esquerda] == 2)
            esquerda = esquerda - 1 < 0? p->n - 1: esquerda - 1;
        while(estados[direita] == 2)
            direita = (direita + 1) % p->n;

        //printf("esquerda %d direita %d\n", esquerda, direita);


        //printf("esquerda %d direita %d\n", convexo(p, maisBaixo), !intersecPoligono(p->points, p->n, p->points[esquerda], p->points[direita])   );
        //verifica se eh uma orelha


        printf("%d - %d - %d\n", esquerda, maisBaixo, direita);
        if(estados[maisBaixo] != 2 && left(p->points[esquerda], p->points[maisBaixo], p->points[direita]) && !intersecPoligono(p->points, p->n, p->points[esquerda], p->points[direita])){
            Diagonal *d = malloc(sizeof(Diagonal));
            d->i = esquerda;
            d->f = direita;
            d->next = diagonais;
            diagonais = d;

            //diminui de usados
            numOrelhas--;
            //marca como usado
            estados[maisBaixo] = 2;
            //printf("usouuu\n");
        }

        maisBaixo = direita;
        
        //scanf("%d", &qualquer);
    }

    return diagonais;
}



//=======================================| DCEL |====================================
Dcel *criaDcel(){
    Dcel *dcel = (Dcel *)malloc(sizeof(Dcel));
    //dcel->vertex = NULL;
    //dcel->edges = NULL;
    //dcel->faces = NULL;
    dcel->nFaces = dcel->nVertex = dcel->nEdges = 0;

    return dcel;
}

Vertex *criaVertice(Dcel* dcel, float x, float y){
    //cria o vertice
    Vertex *v = malloc(sizeof(Vertex));
    v->x = x;
    v->y = y;
    v->incidentEdge = NULL;

    dcel->vertex[dcel->nVertex ++] = v;

    return v;
}


Edge *criaAresta(Dcel *dcel, Vertex *a, Vertex *b){
    Edge *menorEsq, *nav;
    float valor = -9999999999999, valorAtual;

    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->origin = a;
    
    edge->twin = (Edge *)malloc(sizeof(Edge));
    edge->twin->origin = b;

    edge->twin->twin = edge;
    
    //liga os dois
    if(a->incidentEdge == NULL){
        printf("ola caso a\n");
        edge->prev = edge->twin;
        edge->twin->next = edge;
    }else{
        
        nav = a->incidentEdge;
        do{
            valorAtual = anguloVetores(a, b, nav->twin->origin);
            if(valorAtual > valor){
                valor = valorAtual;
                menorEsq = nav;
            }
            nav = proximoMesmaOrigem(nav);

        }while(nav != a->incidentEdge);

        if(leftD(a, b, menorEsq->twin->origin)){
            menorEsq = menorEsq->twin;

            edge->prev = menorEsq;
            edge->twin->next = menorEsq->next;

            menorEsq->next->prev = edge->twin;
            menorEsq->next = edge;
        }else{
            
            edge->twin->next = menorEsq;
            edge->prev = menorEsq->prev;

            menorEsq->prev->next = edge;
            menorEsq->prev = edge->twin;
        }

        edge->face = edge->twin->face = edge->prev->face;
    }

    valor = -9999999999999;
    if(b->incidentEdge == NULL){
        edge->next = edge->twin;
        edge->twin->prev = edge;

    }else{
        nav = b->incidentEdge;
        do{
            valorAtual = anguloVetores(b, a, nav->twin->origin);
            if(valorAtual > valor){
                valor = valorAtual;
                menorEsq = nav;
            }
            nav = proximoMesmaOrigem(nav);

        }while(nav != b->incidentEdge);

        if(leftD(b, a, menorEsq->twin->origin)){
            menorEsq = menorEsq->twin;

            edge->twin->prev = menorEsq;
            edge->next = menorEsq->next;

            menorEsq->next->prev = edge;
            menorEsq->next = edge->twin;
        }else{

            edge->next = menorEsq;
            edge->twin->prev = menorEsq->prev;

            menorEsq->prev->next = edge->twin;
            menorEsq->prev = edge;
        }
        edge->face = edge->twin->face = edge->twin->prev->face;
    }

    //se ambos forem nullos cria uma face
    if(b->incidentEdge == NULL && b->incidentEdge == NULL){
        a->incidentEdge = edge;
        b->incidentEdge = edge->twin;
        //cria a face externa
        Face *f = malloc(sizeof(Face));
        f->pos = dcel->nFaces;
        f->startPoint = edge;
        //coloca os dois pra apontar na mesma face
        edge->face = 
        edge->twin->face = f;
        //cria na lista da dcel;
        dcel->faces[dcel->nFaces ++] = f;
    }

    edge->twin->pos = edge->pos = dcel->nEdges;
    dcel->edges[dcel->nEdges ++] = edge;

    //verifica se existe um ciclo, se existir então cria uma nova face
    int ciclo = 1;
    nav = edge;
    do{
        if(nav->next == nav->twin){
            ciclo = 0;
            break;
        }
        nav = nav->next;
    }while(nav != edge);
    
    //se existe um ciclo cria uma nova face
    if(ciclo){
        Face *f = malloc(sizeof(Face));
        f->startPoint = edge;
        f->pos = dcel->nFaces;
        //cria na lista da dcel;
        dcel->faces[dcel->nFaces ++] = f;

        edge->face = f;
        //marca os pertencentes a face
        nav = edge;
        do{
            nav->face - f;
            nav = nav->next;
        }while(nav != edge);
    }

    return edge;
}


Edge * removeAresta(Dcel *dcel, Vertex *a, Vertex *b){
    Edge *nav = a->incidentEdge, *edgeEncontrada;
    int encontrado = 0;
    do{
        if(nav->twin->origin == b){
            encontrado = 1;
            break;
        }
        nav = proximoMesmaOrigem(nav);
    }while(nav != a->incidentEdge);

    //arruma o incidentEdge dos vertices
    printf("%f %f\n", a->incidentEdge->twin->origin->x, b->incidentEdge->twin->origin->x);
    a->incidentEdge = a->incidentEdge == proximoMesmaOrigem(a->incidentEdge)? NULL : proximoMesmaOrigem(a->incidentEdge);
    printf("ai\n");
    b->incidentEdge = b->incidentEdge == proximoMesmaOrigem(b->incidentEdge)? NULL : proximoMesmaOrigem(b->incidentEdge);
    //printf("%f %f\n", a->incidentEdge->twin->origin->x, b->incidentEdge->twin->origin->x);

    if(encontrado){
        edgeEncontrada = nav;
        //ajeita o sentido
        edgeEncontrada->next->prev = edgeEncontrada->twin->prev;
        edgeEncontrada->twin->prev->next = edgeEncontrada->next;

        edgeEncontrada->prev->next = edgeEncontrada->twin->next;
        edgeEncontrada->twin->next->prev = edgeEncontrada->prev;

        //se existem duas faces libera uma das faces
        if(edgeEncontrada->face != edgeEncontrada->twin->face){
            dcel->faces[edgeEncontrada->twin->face->pos] = NULL;
            free(edgeEncontrada->twin->face);

            //percorre ajeitando as faces
            nav = edgeEncontrada->next;
            do{
                nav->face = edgeEncontrada->face;
                nav = nav->next;
            }while(nav != edgeEncontrada->next);
        }

        dcel->edges[edgeEncontrada->pos] = NULL;

        return edgeEncontrada;
    }

    return NULL;
}

float anguloVetores(Vertex *v1, Vertex *v2, Vertex *v3){
    float   xa = v2->x - v1->x,
            ya = v2->y - v1->y,
            xb = v3->x - v1->x,
            yb = v3->y - v1->y;

    printf("%f | %f | %f\n", v1->x, v2->x, v3->x);
    return (xa * xb + ya * yb) / (sqrt(xa * xa + ya * ya) * sqrt(xb* xb + yb * yb)); 
}

double areaTrianguloD(Vertex *a, Vertex *b, Vertex *c){
    return ((b->x-a->x)*(c->y - b->y) - (c->x - b->x)*(b->y - a->y))/2;
}

int leftD(Vertex *a, Vertex *b, Vertex *c){
    return areaTrianguloD(a,b,c) > 0;
}

int leftOnD(Vertex *a, Vertex *b, Vertex *c){
    return areaTrianguloD(a,b,c) >= 0;
}

Edge *proximoMesmaOrigem(Edge *e){
    //if(e->prev != NULL)
        return e->prev->twin;
}

float angulo(Edge* e, Edge* d){
    point2d a, b;
    a.x = e->twin->origin->x - e->origin->x;
    a.y = e->twin->origin->y - e->origin->y;
    b.x = d->twin->origin->x - d->origin->x;
    b.y = d->twin->origin->y - d->origin->y;

    return (a.x * b.x + a.y * b.y)/(sqrt(a.x * a.x + a.y*a.y)*sqrt(b.x*b.x+b.y*b.y));
}