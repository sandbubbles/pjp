#include <stdio.h>

int writeint ( int x )
{
    printf ( "%d\n", x );
    return 0;
}

int writestring ( const char * x )
{
    printf ( "%s\n", x );
    return 0;
}
/* Not exactly the correct way but I couldn't figure out how to do it the intended way */
int readln ( void ) 
{
    int x;
    scanf ( "%d", &x );
    return x;
}
