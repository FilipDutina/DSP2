#include "processing.h"
#include "iir.h"

void calculateShelvingCoeff(float c_alpha, Int16* output)
{
	/* Your code here */
    //32767 -> 1
    //-32768 -> -1

    output[0] = c_alpha * 32768;
    output[1] = -32768;
    output[2] = 32767;
    output[3] = -c_alpha * 32768;
}

void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
	/* Your code here */

    output[0] = c_alpha * 32768;
    output[1] = -c_beta * (1 + c_alpha) * (32768 / 2);
    output[2] = 32767;

    output[3] = 32767;
    output[4] = -c_beta * (1 + c_alpha) * (32768 / 2);
    output[5] = c_alpha * 32768;
}

Int16 shelvingHP(Int16 input, Int16* coef, Int16* z_x, Int16* z_y, Int16 k)
{
	/* Your code here */

    Int16 filtered = first_order_IIR(input, coef, z_x, z_y);
    Int32 Output = (Int32)_smpy((filtered + input), k) + ((input - filtered) >> 1);
    return (Int16)Output;
}

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 k)
{
	/* Your code here */

    Int16 filtered = first_order_IIR(input, coeff, z_x, z_y);
    Int32 Output = (((Int32)filtered + input) >> 1) + _smpy((input - filtered), k);
    return (Int16)Output;
}

Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 k)
{
	/* Your code here */

    Int16 filtered = second_order_IIR(input, coeff, z_x, z_y);
    Int32 Output = (((Int32)filtered + input) >> 1) + (Int32)_smpy((input - filtered), k);
    return (Int16)Output;
}
