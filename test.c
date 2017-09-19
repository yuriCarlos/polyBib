    #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GL/glut.h>

#include "polyBib.h"

/* Functions. */
void Init(void);
void Reshape(int w, int h);
void Display(void);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Drawpoints(void);
void DrawLines();
void desenhaEdge(Edge *e, float r, float g, float b);
void percorreFace(Vertex *a, Vertex *b);
void todosDeUmaOrigem(Vertex *a);

/* Globals */
int winwidth, winheight;

//Poligono *p;
Dcel *dcel;
Edge *e;

/* Defines for transformations. */
#define POINT_INSERT 0
#define POINT_REMOVE 1 

void Init(){

    dcel = criaDcel();

    /* Select clearing color. */
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void Reshape(int w, int h){

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, 512.0, 0.0, 512.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   winwidth  = w;
   winheight = h;
}

void Keyboard(unsigned char key, int x, int y){
    int ponto1, ponto2, tipo;
    Diagonal *diags = NULL;
    

    switch (key){
        case 27:
            exit(0);
            break;
        case 'p':
            printf("Digite os vértices para aresta\n");
            scanf("%d, %d", &ponto1, &ponto2);
            ponto1 --;
            ponto2 --;

            criaAresta(dcel, dcel->vertex[ponto1], dcel->vertex[ponto2]);

            desenhaEdge(dcel->edges[dcel->nEdges - 1], 1, 0, 0);

            /*glColor3f (1.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex2d(dcel->edges[dcel->nEdges - 1]->origin->x, dcel->edges[dcel->nEdges - 1]->origin->y);

            glVertex2d(dcel->edges[dcel->nEdges - 1]->twin->origin->x, dcel->edges[dcel->nEdges - 1]->twin->origin->y);

            glEnd();
            glFlush();*/

            /*printf("Digite o vértice\n");
            scanf("%d", &ponto);
            ponto --;
            p->n = npts;
            if(npts >= 3){
                glBegin(GL_POINTS);
                printf("%d\n", convexo(p, ponto));
                //if(convexo(p, ponto))
                if(ehOrelha(p, ponto))
                    glColor3f (1.0, 0.0, 0.0);
                else
                    glColor3f (0.0, 2.0, 0.0);
                glVertex2d(p->points[ponto].x, p->points[ponto].y);
                glEnd();
                glFlush();

                tipo = classificaPonto(p, ponto);
                if(tipo == 1) printf("start\n");
                if(tipo == 2) printf("split\n");
                if(tipo == 3) printf("end\n");
                if(tipo == 4) printf("merge\n");
                if(tipo == 0) printf("regular\n");
            }*/

            break;
        case 'a':
            break;
        case 'r':
            printf("De a aresta a ser removida\n");
            scanf("%d, %d", &ponto1, &ponto2);
            e = removeAresta(dcel, dcel->vertex[ponto1 - 1], dcel->vertex[ponto2 - 1]);
            if(e != NULL)
                desenhaEdge(e, 1, 1, 1);
            break;
        case 'n':
            printf("De a origem\n");
            scanf("%d", &ponto1);

            todosDeUmaOrigem(dcel->vertex[ponto1 - 1]);
            break;
        case 't':
            e = dcel->faces[0]->startPoint;
            break;
        case 'q':{
                e = dcel->faces[0]->startPoint;
                Edge *aux = e;
                do{
                    printf("%f -> %f\n",aux->origin->x, aux->twin->origin->x);
                    aux = aux->next;
                }while (e != aux);

                printf("De uma aresta da face a ser percorrida\n");
                scanf("%d, %d", &ponto1, &ponto2);
                percorreFace(dcel->vertex[ponto1 - 1], dcel->vertex[ponto2 - 1]);

            }
            break;
        case 'Q':
            exit(0);
            break;
    }
}

void desenhaEdge(Edge *e, float r, float g, float b){
    printf("%f ,%f | %f, %f\n", e->origin->x, e->origin->y, e->twin->origin->x, e->twin->origin->y);

    glColor3f (r,g,b);
    glBegin(GL_LINES);
    glVertex2d(e->origin->x, e->origin->y);
    glVertex2d(e->twin->origin->x, e->twin->origin->y);
    glEnd();
    glFlush();
}

void Mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                if (dcel->nVertex < MAXNPTS)
                {
                    criaVertice(dcel, x, 512.0-y);
                    printf("criou o vertice\n");
					printf("Reading point (%lf,%lf).\n", dcel->vertex[dcel->nVertex - 1]->x, dcel->vertex[dcel->nVertex - 1]->y);
                    if(dcel->nVertex >= 2){
                        desenhaEdge(criaAresta(dcel, dcel->vertex[dcel->nVertex - 2], dcel->vertex[dcel->nVertex - 1]), 1, 0, 0);
                        printf("criou a aresta\n");
                    }
                    
                }

                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                dcel->nFaces = dcel->nEdges = dcel->nVertex = 0;
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

void Drawpoints()
{
    int i;    
    
    glColor3f (0.0, 0.0, 0.0);
    glPointSize((GLdouble) 5);
    glBegin(GL_POINTS);
    //for (i = 0; i < npts; i++)
    //    glVertex2d(p->points[i].x, p->points[i].y);

    for (i = 0; i < dcel->nVertex; i++)
        glVertex2d(dcel->vertex[i]->x, dcel->vertex[i]->y);

    glEnd();
}

void DrawLines()
{
	int i;

    glColor3f (0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (i = 0; i < dcel->nEdges; i++){
        if(dcel->edges[i] != NULL){
            glVertex2d(dcel->edges[i]->origin->x, dcel->edges[i]->origin->y);
            glVertex2d(dcel->edges[i]->twin->origin->x, dcel->edges[i]->twin->origin->y);
        }
    }
    glEnd();
}
    
void Display(void){
    
    /* Clear buffers. */
    glClear(GL_COLOR_BUFFER_BIT);
   
    /* Draw. */
    DrawLines();
    Drawpoints();

    glFlush();
}

int main(int argc, char** argv){

    /* Call glut init functions. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(512, 512); 
    glutCreateWindow("Test");
    Init();

    /* Call glut drawing functions. */
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}

void percorreFace(Vertex *a, Vertex *b){
    Edge *nav = a->incidentEdge, *primeiro;
    int encontrado = 0;
    do{
        if(nav->twin->origin == b){
            encontrado = 1;
            break;
        }
        nav = proximoMesmaOrigem(nav);
    }while(nav != a->incidentEdge);

    if(encontrado){
        primeiro = nav;
        do{
            desenhaEdge(primeiro,0,1,0);
            sleep(1);
            desenhaEdge(primeiro,0,0,1);
            primeiro = primeiro->next;
        }while(primeiro != nav);
    }
}


void todosDeUmaOrigem(Vertex *a){
    if(a->incidentEdge != NULL){
        Edge *nav = a->incidentEdge;
        do{
            desenhaEdge(nav,0,1,0);
            sleep(1);
            desenhaEdge(nav,0,0,1);
            nav = proximoMesmaOrigem(nav);
        }while(nav != a->incidentEdge);
    }

}