#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**

Conway game of life

The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,
each of which is in one of two possible states, alive or dead.

Every cell interacts with its eight neighbours,
which are the cells that are horizontally, vertically, or diagonally adjacent.

At each step in time, the following transitions occur:

Any live cell with fewer than two live neighbours dies, as if caused by under-population.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by over-population.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

The initial pattern constitutes the seed of the system.

The first generation is created by applying the above rules simultaneously to every cell in the seed—births and deaths occur simultaneously,
and the discrete moment at which this happens is sometimes called a tick (in other words,
each generation is a pure function of the preceding one).
The rules continue to be applied repeatedly to create further generations.

**/

FILE* file;

#define FILEPATH "outputLife.txt"

int printFrameInd = 1;  /// print frame indicator
int shrinkFrameInd = 1; /// shrink frame indicator

const int FALSE = 0;
const int TRUE  = 1;

/// live cell initialization parameters

const int iLIM = 4;
const int jLIM = 3;
const int offset = 10;

/// bounding frame on live cell locations

int iMin, iMax, jMin, jMax;
int iMinNext, iMaxNext, jMinNext, jMaxNext;

/// time step 0, 1, 2, ...

const int TIMELIMIT = 100; /// # time steps to execute
int time;

/// values of cell state

const int DEAD = 0;
const int LIVE = 1;

/// print chars

const char DEADCHAR = ' ';
const char LIVECHAR = '*';

/// print chars for dead frame cells

const char HORZCHAR = '-';
const char VERTCHAR = '|';

typedef struct CellStruc Cell;

struct CellStruc
{
    int state[2];     /// state is LIVE or DEAD, alternate between using state[0] and state[1]
};

/// grid dimension limit - for this program, grid is finite

#define DIMLIM 20

/// 2D grid

Cell cell[DIMLIM][DIMLIM];

int min( int i, int j)
{
    return ( i<=j ? i : j );
}

int max( int i, int j)
{
    return ( i>=j ? i : j );
}

int countNeighbors( int i, int j)
{
    /// counts # live neighbors of cell i,j

    int nLive = 0;

    nLive += j-1 >= 0 && cell[i][j-1].state[time%2] == LIVE; /// left
    nLive += i-1 >= 0 && cell[i-1][j].state[time%2] == LIVE; /// above

    nLive += j+1 < DIMLIM && cell[i][j+1].state[time%2] == LIVE; /// right
    nLive += i+1 < DIMLIM && cell[i+1][j].state[time%2] == LIVE; /// below

    nLive += i-1 >= 0 && j-1 >= 0 && cell[i-1][j-1].state[time%2] == LIVE;     /// above left
    nLive += i-1 >= 0 && j+1 < DIMLIM && cell[i-1][j+1].state[time%2] == LIVE; /// above right


    nLive += i+1 < DIMLIM && j-1 >= 0 && cell[i+1][j-1].state[time%2] == LIVE;     /// below left
    nLive += i+1 < DIMLIM && j+1 < DIMLIM && cell[i+1][j+1].state[time%2] == LIVE; /// below right

    return nLive;
}

void shrinkFrame()
{
    /// shrink bounding frame if possible

    int i, j, liveCell;

    /// iMin

    i = iMin;

    while( i == iMin )
    {
        /// check for live cell in row i

        liveCell = 0;

        for( j=jMin; j<=jMax; j++ )
        {
            liveCell = cell[i][j].state[time%2] == LIVE;

            /// if live cell is in row i, break from for-loop

            if( liveCell ) break;
        }

        /// if live cell is not in row i, increment iMin if possible

        if( !liveCell ) iMin = min( iMin+1, DIMLIM-1 );

        /// if iMin was not incremented, break from while-loop

        if( i == iMin ) break;

        /// else iMin was incremented

        /// init row i in time+1 to DEAD since this row will lie outside the new frame, so it won't get inited in the processing loop in main()

        for( j=jMin; j<=jMax; j++ )

            cell[i][j].state[(time+1)%2] = DEAD;

        /// set i and iMinNext to new value of iMin

        i = iMinNext = iMin;

        /// iterate while-loop
    }

    /// iMax

    i = iMax;

    while( i == iMax )
    {
        /// check for live cell in row i

        liveCell = 0;

        for( j=jMin; j<=jMax; j++ )
        {
            liveCell = cell[i][j].state[time%2] == LIVE;

            /// if live cell is in row i, break from for-loop

            if( liveCell ) break;
        }

        /// if live cell is not in row i, decrement iMax if possible

        if( !liveCell ) iMax = max( iMax-1, 0 );

        /// if iMax was not decremented, break from while-loop

        if( i == iMax ) break;

        /// else iMax was decremented

        /// init row i in time+1 to DEAD since this row will lie outside the new frame, so it won't get inited in the processing loop in main()

        for( j=jMin; j<=jMax; j++ )

            cell[i][j].state[(time+1)%2] = DEAD;

        /// set i and iMaxNext to new value of iMax

        i = iMaxNext = iMax;

        /// iterate while-loop
    }

    /// jMin

    j = jMin;

    while( j == jMin )
    {
        /// check for live cell in col j

        liveCell = 0;

        for( i=iMin; i<=iMax; i++ )
        {
            liveCell = cell[i][j].state[time%2] == LIVE;

            /// if live cell is in col j, break from for-loop

            if( liveCell ) break;
        }

        /// if live cell is not in col j, increment jMin if possible

        if( !liveCell ) jMin = min( jMin+1, DIMLIM-1 );

        /// if jMin was not incremented, break from while-loop

        if( j == jMin ) break;

        /// else jMin was incremented

        /// init col j in time+1 to DEAD since this col will lie outside the new frame, so it won't get inited in the processing loop in main()

        for( i=iMin; i<=iMax; i++ )

            cell[i][j].state[(time+1)%2] = DEAD;

        /// set j and jMinNext to new value of jMin

        j = jMinNext = jMin;

        /// iterate while-loop
    }

    /// jMax

    j = jMax;

    while( j == jMax )
    {
        /// check for live cell in col j

        liveCell = 0;

        for( i=iMin; i<=iMax; i++ )
        {
            liveCell = cell[i][j].state[time%2] == LIVE;

            /// if live cell is in col j, break from for-loop

            if( liveCell ) break;
        }

        /// if live cell is not in col j, decrement jMax if possible

        if( !liveCell ) jMax = max( jMax-1, 0 );

        /// if jMax was not decremented, break from while-loop

        if( j == jMax ) break;

        /// else jMax was decremented

        /// init col j in time+1 to DEAD since this col will lie outside the new frame, so it won't get inited in the processing loop in main()

        for( i=iMin; i<=iMax; i++ )

            cell[i][j].state[(time+1)%2] = DEAD;

        /// set j and jMaxNext to new value of jMax

        j = jMaxNext = jMax;

        /// iterate while-loop
    }
}

void printGrid()
{
    int printFrameLocal = printFrameInd && time > 0;

    fprintf( file, "\n   Grid at time %d\n\n", time );

    int i, j;

    fprintf( file, "   " );

    for( j=0; j<DIMLIM; j++ )

        fprintf( file, "%d", (j/10)%10 );

    fprintf( file, "\n" );

    fprintf( file, "   " );

    for( j=0; j<DIMLIM; j++ )

        fprintf( file, "%d", j%10 );

    fprintf( file, "\n\n" );

    for( i=0; i<DIMLIM; i++ )
    {
       fprintf( file, "%d", (i/10)%10 );
       fprintf( file, "%d ", i%10 );

        for( j=0; j<DIMLIM; j++ )
        {
            if( printFrameLocal && ( i==iMin || i==iMax ) && j>=jMin && j<=jMax )

                fprintf( file, "%c", ( cell[i][j].state[time%2]==DEAD ? HORZCHAR : LIVECHAR ) );

            else if( printFrameLocal && ( j==jMin || j==jMax ) && i>=iMin && i<=iMax )

                fprintf( file, "%c", ( cell[i][j].state[time%2]==DEAD ? VERTCHAR : LIVECHAR ) );

            else

                fprintf( file, "%c", ( cell[i][j].state[time%2]==DEAD ? DEADCHAR : LIVECHAR ) );
        }

        fprintf( file, "\n" );
   }
}

void main()
{
    file = fopen( FILEPATH, "w" );

    time = 0;

    /// print grid legend

    fprintf( file, "Grid legend\n\n" );

    fprintf( file, "Dead cell: '%c'\n", DEADCHAR );
    fprintf( file, "Live cell: '%c'\n\n", LIVECHAR );

    if( printFrameInd ) fprintf( file, "Dead cell in bounding frame: '%c', '%c'\n",HORZCHAR, VERTCHAR );
    if( printFrameInd ) fprintf( file, "Live cell in bounding frame: '%c'\n",LIVECHAR );

    /// init grid

    int i, j;

    for( i=0; i<DIMLIM; i++ )

        for( j=0; j<DIMLIM; j++ )
        {
            cell[i][j].state[time%2] = DEAD;
            cell[i][j].state[(time+1)%2] = DEAD;
        }

    /// change initial pattern of live cells to change program activity pattern

/**
    for( i=0; i<iLIM; i++ )

        cell[i][0].state[time%2] = LIVE;

    for( j=0; j<jLIM; j++ )

            cell[0][j].state[time%2] = LIVE;
**/

    for( i=offset/2; i<=offset/2+iLIM && i<DIMLIM; i++ )

        cell[i][offset].state[time%2] = LIVE;

    for( j=offset; j<=offset+jLIM && j<DIMLIM; j++ )

        cell[offset][j].state[time%2] = LIVE;

    printGrid();

    /// set initial frame limits

    iMin = jMin = DIMLIM-1;
    iMax = jMax = 0;

    for( i=0; i<DIMLIM; i++ )

        for( j=0; j<DIMLIM; j++ )

            if( cell[i][j].state[time%2] == LIVE )
            {
                iMin = min( iMin, i );
                iMax = max( iMax, i );

                jMin = min( jMin, j );
                jMax = max( jMax, j );
            }

    /// iterate processing loop till time limit is reached or all cells are dead

    int liveCell = 1;

    iMinNext = iMin; iMaxNext = iMax; jMinNext = jMin; jMaxNext = jMax;

    while( time < TIMELIMIT && liveCell )
    {
        liveCell = 0;

        for( i=max(iMin-1,0); i<=min(iMax+1,DIMLIM-1); i++ )

            for( j=max(jMin-1,0); j<=min(jMax+1,DIMLIM-1); j++ )
            {
                cell[i][j].state[(time+1)%2] = cell[i][j].state[time%2]; /// init cell i,j in next time step

                int nLive = countNeighbors( i, j );

                if( cell[i][j].state[time%2] == LIVE && ( nLive < 2 || nLive > 3 ) )

                    cell[i][j].state[(time+1)%2] = DEAD;

                else if( cell[i][j].state[time%2] == DEAD && nLive == 3 )
                {
                    cell[i][j].state[(time+1)%2] = LIVE;

                    /// expand bounding frame if necessary

                    iMinNext = min( iMinNext, i );
                    iMaxNext = max( iMaxNext, i );

                    jMinNext = min( jMinNext, j );
                    jMaxNext = max( jMaxNext, j );
                }

                liveCell = liveCell || ( cell[i][j].state[(time+1)%2] == LIVE );
            }

        /// move to next time step

        time++;

        iMin = iMinNext; iMax = iMaxNext; jMin = jMinNext; jMax = jMaxNext;

        /// shrink bounding frame if possible

        if( shrinkFrameInd ) shrinkFrame();

        printGrid();
    }

    fclose(file);
}
