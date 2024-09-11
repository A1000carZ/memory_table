#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define MEMORIA_MAX 80
#define so 20
#define MIN_BLOCK_SIZE 4
#define MAX_BLOCK_SIZE 20

struct NodeMT
{
    int id;
    int size;
    int initialLoc;
    int endLoc;
    int state;
    char access[10];
    int internF;
    NodeMT *right;
};

struct NodeJT
{
    int id;
    int size;
    NodeJT *right;
};

int generateRandomNumber(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

NodeMT *insertMT(NodeMT *nodeListMT, int id, int size, int initialLoc, int state, const char* access, int internF);
NodeMT *createMT();
NodeJT *createJT(NodeJT *nodeListJT);
NodeJT *insertJT(NodeJT *nodeListJT, int size);
void printJT(NodeJT *nodeListJT);
void printMT(NodeMT *nodeListMT);
NodeMT *insertJob(NodeMT *nodeListMT, int jobId, int jobSize);

int main()
{
    randomize();
    NodeMT *nodeListMT = NULL;
    NodeJT *nodeListJT = NULL;

    nodeListMT = createMT();

    while (1)
    {
        int menuAns;
        cout << "Menu\n";
        cout << "1. Crear tablas de tareas\n";
        cout << "2. Insertar tarea\n";
        cout << "3. Ver tabla de tareas\n";
        cout << "4. Ver tabla de memoria\n";
        cout << "5. Salir\n";
        cout << "\nSelecciona una opcion: ";
        cin >> menuAns;

        clrscr();
        switch (menuAns)
        {
        case 1:
            nodeListJT = createJT(nodeListJT);
            break;
        case 2:
        {
            char jobId[10];
            int id;
            cout << "Ingrese el ID de la tarea (por ejemplo, J1, J4, etc.): ";
            cin >> jobId;

            if (sscanf(jobId, "J%d", &id) != 1)
            {
                cout << "ID de tarea invalido. Debe ser en formato Jn.\n";
                break;
            }

            NodeJT *jtNode = nodeListJT;
            int found = 0;
            while (jtNode != NULL)
            {
                if (jtNode->id == id)
                {
                    found = 1;
                    break;
                }
                jtNode = jtNode->right;
            }

            if (!found)
            {
                cout << "La tarea con ID J" << id << " no fue encontrada.\n";
            }
            else
            {
                nodeListMT = insertJob(nodeListMT, jtNode->id, jtNode->size);
            }
        }
        break;
        case 3:
            printJT(nodeListJT);
            break;
        case 4:
            printMT(nodeListMT);
            break;
        case 5:
            return 0;
        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
            break;
        }
    }

    return 0;
}

NodeMT *createMT()
{
    NodeMT *nodeListMT = NULL;
    int totalSize = 0;
    int blockId = 0;

    // Initialize OS block
    nodeListMT = insertMT(nodeListMT, blockId++, so, 0, 1, "SO", 0);
    totalSize += so;

    cout << "Bloque del SO agregado. Tamano: " << so << " Kb\n";

    while (totalSize < MEMORIA_MAX)
    {
        int option;
        cout << "Seleccione una opcion para crear la tabla de memoria:\n";
        cout << "1. Generar bloque aleatorio\n";
        cout << "2. Agregar bloque manualmente\n";
        cin >> option;

        int size;
        if (option == 1)
        {
            size = generateRandomNumber(MIN_BLOCK_SIZE, MAX_BLOCK_SIZE);
        }
        else if (option == 2)
        {
            cout << "Ingrese el tamano del bloque: ";
            cin >> size;
        }
        else
        {
            cout << "Opcion invalida.\n";
            continue;
        }

        if (totalSize + size > MEMORIA_MAX)
        {
            size = MEMORIA_MAX - totalSize;
        }

        nodeListMT = insertMT(nodeListMT, blockId++, size, totalSize, 0, "Libre", 0);
        totalSize += size;

        cout << "Bloque agregado. Tamano total de memoria: " << totalSize << "/" << MEMORIA_MAX << " Kb\n";

        if (totalSize == MEMORIA_MAX)
        {
            cout << "Se ha alcanzado la capacidad maxima de memoria.\n";
            break;
        }
    }

    return nodeListMT;
}

NodeMT *insertMT(NodeMT *nodeListMT, int id, int size, int initialLoc, int state, const char* access, int internF)
{
    NodeMT *newNode = new NodeMT;
    newNode->id = id;
    newNode->size = size;
    newNode->initialLoc = initialLoc;
    newNode->endLoc = initialLoc + size - 1;
    newNode->state = state;
    strcpy(newNode->access, access);
    newNode->internF = internF;
    newNode->right = NULL;

    if (nodeListMT == NULL)
    {
        return newNode;
    }
    
    NodeMT *current = nodeListMT;
    while (current->right != NULL)
    {
        current = current->right;
    }
    current->right = newNode;

    return nodeListMT;
}

NodeJT *createJT(NodeJT *nodeListJT)
{
    int option;
    do
    {
        cout << "Seleccione una opcion para la tabla de tareas:\n";
        cout << "1. Generar tarea aleatoria\n";
        cout << "2. Agregar tarea manualmente\n";
        cout << "3. Terminar\n";
        cout << "Opcion: ";
        cin >> option;

        switch(option)
        {
            case 1:
            {
                int minSize, maxSize;
                cout << "Ingrese el tamano minimo de tarea: ";
                cin >> minSize;
                cout << "Ingrese el tamano maximo de tarea: ";
                cin >> maxSize;

                int size = generateRandomNumber(minSize, maxSize);
                nodeListJT = insertJT(nodeListJT, size);
                cout << "Se ha generado una tarea de tamano " << size << " Kb.\n";
                break;
            }
            case 2:
            {
                int size;
                cout << "Ingrese el tamano de la tarea: ";
                cin >> size;
                if (size > 0)
                {
                    nodeListJT = insertJT(nodeListJT, size);
                    cout << "Se ha agregado una tarea de tamano " << size << " Kb.\n";
                }
                else
                {
                    cout << "Tamano invalido. La tarea no se ha agregado.\n";
                }
                break;
            }
            case 3:
                cout << "Finalizando la creacion de tareas.\n";
                break;
            default:
                cout << "Opcion invalida. Por favor, intente de nuevo.\n";
        }
    } while (option != 3);

    return nodeListJT;
}

NodeJT *insertJT(NodeJT *nodeListJT, int size)
{
    static int id = 1;
    NodeJT *newNode = new NodeJT;
    newNode->id = id++;
    newNode->size = size;
    newNode->right = NULL;

    if (nodeListJT == NULL)
    {
        return newNode;
    }
    NodeJT *current = nodeListJT;
    while (current->right != NULL)
    {
        current = current->right;
    }
    current->right = newNode;
    return nodeListJT;
}

void printJT(NodeJT *nodeListJT)
{
    NodeJT *aux = nodeListJT;
    cout << "Tabla de trabajo\n";
    cout << "Id\tTamano\n";
    while (aux != NULL)
    {
        cout << "J" << aux->id << "\t" << aux->size << " Kb\n";
        aux = aux->right;
    }
    cout << "Presione cualquier tecla para continuar...";
    getch();
}

void printMT(NodeMT *nodeListMT)
{
    NodeMT *aux = nodeListMT;
    cout << "Tabla de memoria\n";
    cout << "Id\tCapacidad\tLoc. Inicial\tLoc. final\tEstado\tAcceso\tFrag. Interna\n";
    while (aux != NULL)
    {
        cout << "B" << aux->id << "\t"
             << aux->size << " Kb\t\t"
             << aux->initialLoc << " Kb\t\t"
             << aux->endLoc << " Kb\t\t"
             << (aux->state ? "Ocupado" : "Libre") << "\t"
             << aux->access << "\t"
             << aux->internF << " Kb\n";
        aux = aux->right;
    }
    cout << "Presione cualquier tecla para continuar...";
    getch();
}

NodeMT *insertJob(NodeMT *nodeListMT, int jobId, int jobSize)
{
    NodeMT *current = nodeListMT;
    while (current != NULL)
    {
        if (current->state == 0 && current->size >= jobSize)
        {
            char buffer[50];
            current->state = 1;
            sprintf(buffer, "%c%d", 'J', jobId);
            strcpy(current->access, buffer);
            current->internF = current->size - jobSize;
            cout << "Tarea J" << jobId << " asignada al bloque B" << current->id << ".\n";
            return nodeListMT;
        }
        current = current->right;
    }
    cout << "No hay espacio suficiente para la tarea J" << jobId << ".\n";
    return nodeListMT;
}
// void CREAR_LISTA(void)
// {
//     int nuevo_elemento, s;
//     do
//     {
//         if (PLista == NULL)
//         {
//             PLista = (Node *)malloc(sizeof(Node));
//             printf("Ingresa el numero: ");
//             scanf("%d", &nuevo_elemento);
//             PLista->number = nuevo_elemento;
//             QLista = PLista;
//         }
//         else
//         {
//             NuevoLista = (Node *)malloc(sizeof(Node));
//             printf("Ingresa el numero: ");
//             scanf("%d", &nuevo_elemento);
//             NuevoLista->number = nuevo_elemento;
//             NuevoLista->right = NULL;
//             QLista->right = NuevoLista;
//             QLista = NuevoLista;
//         }
//         printf("Desea agregar otro elemento si [1] no[0]: ");
//         scanf("%d", &s);
//     } while (s == 1);
// }

// void VER_LISTA(void)
// {
//     Aux = PLista;
//     while (Aux != NULL)
//     {
//         printf("\nDato: %d:", Aux->number);
//         Aux = Aux->right;
//     }
// }