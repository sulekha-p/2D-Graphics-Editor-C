#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef struct
{
    int id;
    int type;       // 1-Line, 2-Rectangle, 3-Triangle, 4-Circle

    int x1, y1;
    int x2, y2;
    int x3, y3;

    int radius;

} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas Functions ---------- */

void initCanvas()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas()
{
    int i, j;

    printf("\n");

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
}

void plot(int x, int y)
{
    if(x >= 0 && x < ROWS && y >= 0 && y < COLS)
    {
        canvas[x][y] = '*';
    }
}

/* ---------- Drawing Functions ---------- */

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        plot(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    drawLine(x1, y1, x1, y2);
    drawLine(x1, y2, x2, y2);
    drawLine(x2, y2, x2, y1);
    drawLine(x2, y1, x1, y1);
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void drawCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(x <= y)
    {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);

        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        if(d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }

        x++;
    }
}

/* ---------- Redraw Canvas ---------- */

void redraw()
{
    int i;

    initCanvas();

    for(i = 0; i < objectCount; i++)
    {
        switch(objects[i].type)
        {
            case 1:
                drawLine(objects[i].x1, objects[i].y1,
                         objects[i].x2, objects[i].y2);
                break;

            case 2:
                drawRectangle(objects[i].x1, objects[i].y1,
                              objects[i].x2, objects[i].y2);
                break;

            case 3:
                drawTriangle(objects[i].x1, objects[i].y1,
                             objects[i].x2, objects[i].y2,
                             objects[i].x3, objects[i].y3);
                break;

            case 4:
                drawCircle(objects[i].x1,
                           objects[i].y1,
                           objects[i].radius);
                break;
        }
    }
}

/* ---------- Add Object ---------- */

void addObject()
{
    Object obj;

    printf("\nChoose Object Type\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Triangle\n");
    printf("4. Circle\n");

    printf("Enter choice: ");
    scanf("%d", &obj.type);

    obj.id = objectCount + 1;

    switch(obj.type)
    {
        case 1:
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &obj.x1, &obj.y1,
                  &obj.x2, &obj.y2);
            break;

        case 2:
            printf("Enter top-left and bottom-right coordinates:\n");
            printf("x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &obj.x1, &obj.y1,
                  &obj.x2, &obj.y2);
            break;

        case 3:
            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &obj.x1, &obj.y1,
                  &obj.x2, &obj.y2,
                  &obj.x3, &obj.y3);
            break;

        case 4:
            printf("Enter centerX centerY radius: ");
            scanf("%d%d%d",
                  &obj.x1,
                  &obj.y1,
                  &obj.radius);
            break;

        default:
            printf("Invalid Type!\n");
            return;
    }

    objects[objectCount++] = obj;

    redraw();

    printf("Object Added Successfully.\n");
}

/* ---------- Delete Object ---------- */

void deleteObject()
{
    int id;
    int i, j;

    printf("Enter Object ID to delete: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            for(j = i; j < objectCount - 1; j++)
            {
                objects[j] = objects[j + 1];
            }

            objectCount--;

            redraw();

            printf("Object Deleted Successfully.\n");
            return;
        }
    }

    printf("Object Not Found.\n");
}

/* ---------- Modify Object ---------- */

void modifyObject()
{
    int id;
    int i;

    printf("Enter Object ID to modify: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            printf("Enter New Values:\n");

            switch(objects[i].type)
            {
                case 1:
                    printf("x1 y1 x2 y2: ");
                    scanf("%d%d%d%d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2);
                    break;

                case 2:
                    printf("x1 y1 x2 y2: ");
                    scanf("%d%d%d%d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2);
                    break;

                case 3:
                    printf("x1 y1 x2 y2 x3 y3: ");
                    scanf("%d%d%d%d%d%d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2,
                          &objects[i].x3,
                          &objects[i].y3);
                    break;

                case 4:
                    printf("centerX centerY radius: ");
                    scanf("%d%d%d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].radius);
                    break;
            }

            redraw();

            printf("Object Modified Successfully.\n");
            return;
        }
    }

    printf("Object Not Found.\n");
}

/* ---------- List Objects ---------- */

void listObjects()
{
    int i;

    printf("\nCurrent Objects:\n");

    for(i = 0; i < objectCount; i++)
    {
        printf("ID = %d  Type = ",
               objects[i].id);

        switch(objects[i].type)
        {
            case 1:
                printf("Line");
                break;
            case 2:
                printf("Rectangle");
                break;
            case 3:
                printf("Triangle");
                break;
            case 4:
                printf("Circle");
                break;
        }

        printf("\n");
    }

    if(objectCount == 0)
        printf("No Objects Present.\n");
}

/* ---------- Main ---------- */

int main()
{
    int choice;

    initCanvas();

    while(1)
    {
        printf("\n=================================\n");
        printf("      2D GRAPHICS EDITOR\n");
        printf("=================================\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}