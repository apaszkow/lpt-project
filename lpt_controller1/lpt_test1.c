#include <sys/io.h>
#include <stdio.h>
#include <stdlib.h>

#define BASEPORT 0x378 // Adres portu lpt

int main()
{
	//inicjalizacja dostępu do portu
    //wykorzystujemy jeden bajt portu lpt
	//ustawiamy port lpt w tryb nadawania danych
	if( ioperm(BASEPORT, 1, 1) ) {
		perror( "ioperm" );
		exit( 1 );
	}
 
	int tab[8];   // tablica przechowująca wartości 0/1 dla odpowiednich bitów w wykorzystywanym bajcie portu lpt
	tab[0] = 1;   // 00000001
	tab[1] = 2;   // 00000010
	tab[2] = 4;   // 00000100
	tab[3] = 8;   // 00001000
	tab[4] = 16;  // 00010000
	tab[5] = 32;  // 00100000
	tab[6] = 64;  // 01000000
	tab[7] = 128; // 10000000
 
	int i;
    int counter = 0;
	while( counter < 100 )
	{
		//pętla która umożliwia ustawianie w stan wysoki kolejnych pinów portu lpt
		for( i=0; i<8; i++)
		{
			outb( tab[i], BASEPORT );
			usleep( 100000 ); //po wysłaniu danych odczekaj 100ms
			printf( "status: %d\n", tab[i] );
		}
		counter++;
	}
}