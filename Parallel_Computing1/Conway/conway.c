#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int FALSE = 0;
const int TRUE  = 1;


const int iLIM = 10;
const int jLIM = 10;
const int offset = 10;


const int TIMELIMIT = 100; 
int time;


const int LIVE = 0;
const int DEAD = 1;

const char LIVECHAR = '*';
const char DEADCHAR = '-';

typedef struct CellStruc Cell;

struct CellStruc
{
    int state[2];     
};


#define DIMLIM 20


Cell cell[DIMLIM][DIMLIM];

int countNeighbors( int i, int j)
{
    

    int nLive = 0;

    nLive += j-1 >= 0 && cell[i][j-1].state[time%2] == LIVE; /// left
    nLive += i-1 >= 0 && cell[i-1][j].state[time%2] == LIVE; /// above

    nLive += j+1 < DIMLIM && cell[i][j+1].state[time%2] == LIVE; /// right
    nLive += i+1 < DIMLIM && cell[i+1][j].state[time%2] == LIVE; /// below

    nLive += i-1 >= 0 && j-1 >= 0 && cell[i-1][j-1].state[time%2] == LIVE; /// above left
    nLive += i-1 >= 0 && j+1 < DIMLIM && cell[i-1][j+1].state[time%2] == LIVE; /// above right


    nLive += i+1 < DIMLIM && j-1 >= 0 && cell[i+1][j-1].state[time%2] == LIVE; /// below left
    nLive += i+1 < DIMLIM && j+1 < DIMLIM && cell[i+1][j+1].state[time%2] == LIVE; /// below right

    return nLive;
}

void printGrid()
{
    printf( "\n   Grid at time %d\n\n", time );

    int i, j;

    printf( "   " );

    for( j=0; j<DIMLIM; j++ )

        printf( "%d", (j/10)%10 );

    printf( "\n" );

    printf( "   " );

    for( j=0; j<DIMLIM; j++ )

        printf( "%d", j%10 );

    printf( "\n\n" );

    for( i=0; i<DIMLIM; i++ )
    {
       printf( "%d", (i/10)%10 );
       printf( "%d ", i%10 );

        for( j=0; j<DIMLIM; j++ )

            printf( "%c", ( cell[i][j].state[time%2]==DEAD ? DEADCHAR : LIVECHAR ) );

        printf( "\n" );
   }
}

void main()
{
    time = 0;

    

    int i, j;

    for( i=0; i<DIMLIM; i++ )

        for( j=0; j<DIMLIM; j++ )

            cell[i][j].state[time%2] = DEAD;

    

    for( i=offset; i<=offset+iLIM && i<DIMLIM; i++ )

        cell[i][offset].state[time%2] = LIVE;

    for( j=offset; j<=offset+jLIM && j<DIMLIM; j++ )

        cell[offset][j].state[time%2] = LIVE;

    printGrid();


    while( time < TIMELIMIT )
    {
        for( i=0; i<DIMLIM; i++ )

            for( j=0; j<DIMLIM; j++ )
            {
                cell[i][j].state[(time+1)%2] = cell[i][j].state[time%2];

                int nLive = countNeighbors( i, j );

                if( cell[i][j].state[time%2] == LIVE && ( nLive < 2 || nLive > 3 ) ) cell[i][j].state[(time+1)%2] = DEAD;

                else if( cell[i][j].state[time%2] == DEAD && nLive == 3 ) cell[i][j].state[(time+1)%2] = LIVE;
            }

        time++;

        printGrid();

        if( time%5 == 0 ) getchar();
    }
}