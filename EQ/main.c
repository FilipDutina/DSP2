//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2017
// *
// * Zadatak: Ekvalizacija audio signala
// * Autor: Filip Dutina RA43/2014
// *                                                                          
// *                                                                          
/////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "math.h"

#include "iir.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];
short tempBuff[128];

Int16 lp_coeffs[4];
Int16 hp_coeffs[4];
Int16 sh_coeffs[6];
Int16 z_x[3];
Int16 z_y[3];

void main( void )
{   
    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Ekvalizacija audio signala \n");
		
    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();
    
    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    int i;
    for(i = 0; i < 3; i++)
    {
        z_x[i] = 0;
        z_y[i] = 0;
    }

    calculateShelvingCoeff(0.3, lp_coeffs);	//low pass
    calculateShelvingCoeff(-0.3, hp_coeffs);	//high pass
    calculatePeekCoeff(0.7, 0.0, sh_coeffs);	//peek

    while(1)
    {
    	aic3204_read_block(sampleBufferL, sampleBufferR);

    	/* Your code here */

    	for(i = 1; i < AUDIO_IO_SIZE; i++)
    	    sampleBufferL[i] = 0;

    	//Int16 shelvingLP(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 k)
    	//Int16 shelvingHP(Int16 input, Int16* coef, Int16* z_x, Int16* z_y, Int16 k)
    	//Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 k)

    	for(i = 0; i < AUDIO_IO_SIZE; i++)
    		//sampleBufferR[i] = shelvingLP(sampleBufferL[i], lp_coeffs, z_x, z_y, 32767);
    		//sampleBufferR[i] = shelvingHP(sampleBufferL[i], hp_coeffs, z_x, z_y, 32767);
    		//sampleBufferR[i] = shelvingPeek(sampleBufferL[i], sh_coeffs, z_x, z_y, 32767);

		aic3204_write_block(sampleBufferR, sampleBufferR);
	}

    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


