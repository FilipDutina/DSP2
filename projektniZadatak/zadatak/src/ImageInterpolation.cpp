#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"
#include <math.h>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	uchar *YBuff = (uchar*)malloc(newXSize * newYSize * sizeof(uchar));
	char *UBuff = (char*)malloc(newXSize / 4 * newYSize * sizeof(char));
	char *VBuff = (char*)malloc(newXSize / 4 * newYSize * sizeof(char));

	uchar *YBuff1 = (uchar*)malloc(newXSize * newYSize * sizeof(uchar));
	char *UBuff1 = (char*)malloc(newXSize / 4 * newYSize * sizeof(char));
	char *VBuff1 = (char*)malloc(newXSize / 4 * newYSize * sizeof(char));

	RGBtoYUV420(input, xSize, ySize, YBuff1, UBuff1, VBuff1);

	float fx = (float)newXSize / xSize;	//odnos izlazne i ulazne dimenzije slike
	float fy = (float)newYSize / ySize;
	int a, b;
	float xi, yj;
	int i, j;

	for (i = 0; i < newXSize; i++)
	{
		xi = i / fx;	//odnos trenutne pozicije i odnosa izlaza i ulaza

		if (xi - floorf(xi) < 0.5)
			a = (int)floorf(xi);
		else
			a = (int)ceilf(xi);

		for (j = 0; j < newYSize; j++)
		{
			yj = j / fy;

			if (yj - floor(yj) < 0.5)
				b = (int)floorf(yj);
			else
				b = (int)ceilf(yj);

			YBuff[j * newXSize + i] = YBuff1[b * xSize + a];	
		}
	}

	for (i = 0; i < newXSize / 2; i++)
	{
		double xi = i / fx;

		if (xi - floorf(xi) < 0.5)
			a = (int)floorf(xi);
		else
			a = (int)ceilf(xi);

		for (j = 0; j < newYSize / 2; j++)
		{
			double yj = j / fy;

			if (yj - floor(yj) < 0.5)
				b = (int)floorf(yj);
			else
				b = (int)ceilf(yj);

			UBuff[j * newXSize / 2 + i] = UBuff1[b * xSize / 2 + a];
		}
	}

	for (i = 0; i < newXSize / 2; i++)
	{
		double xi = i / fx;

		if (xi - floorf(xi) < 0.5)
			a = (int)floorf(xi);
		else
			a = (int)ceilf(xi);

		for (j = 0; j < newYSize / 2; j++)
		{
			double yj = j / fy;

			if (yj - floor(yj) < 0.5)
				b = (int)floorf(yj);
			else
				b = (int)ceilf(yj);

			VBuff[j * newXSize / 2 + i] = VBuff1[b * xSize / 2 + a];
		}
	}

	YUV420toRGB(YBuff, UBuff, VBuff, newXSize, newYSize, output);
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	uchar *YBuff = (uchar*)malloc(newXSize * newYSize * sizeof(uchar));
	char *UBuff = (char*)malloc(newXSize * newYSize * sizeof(char));
	char *VBuff = (char*)malloc(newXSize * newYSize * sizeof(char));

	uchar *YBuff1 = (uchar*)malloc(newXSize * newYSize * sizeof(uchar));
	char *UBuff1 = (char*)malloc(newXSize * newYSize * sizeof(char));
	char *VBuff1 = (char*)malloc(newXSize * newYSize * sizeof(char));

	double fx = (double)newXSize / xSize;	//opet odnos velicine izlaza i ulaza
	double fy = (double)newYSize / ySize;
	double a, b;
	int Af, Bf, Ac, Bc;

	RGBtoYUV420(input, xSize, ySize, YBuff1, UBuff1, VBuff1);
	int i, j;

	for (i = 0; i < newXSize; i++)
	{
		for (j = 0; j < newYSize; j++)
		{
			a = i / fx - floor(i / fx);
			b = j / fy - floor(j / fy);
			Af = floor(i / fx);
			Bf = floor(j / fy);
			Ac = ceil(i / fx);
			Bc = ceil(j / fy);


			YBuff[j * newXSize + i] = (1 - a) * (1 - b) * YBuff1[Bf * xSize + Af] +
				a * (1 - b) * YBuff1[Bf * xSize + Ac] +
				(1 - a) * b * YBuff1[Bc * xSize + Af] +
				a * b * YBuff1[Bc * xSize + Ac];
		}
	}

	for (i = 0; i < newXSize / 2; i++)
	{
		for (j = 0; j < newYSize / 2; j++)
		{
			a = i / fx - floor(i / fx);
			b = j / fy - floor(j / fy);
			Af = floor(i / fx);
			Bf = floor(j / fy);
			Ac = ceil(i / fx);
			Bc = ceil(j / fy);


			UBuff[j * newXSize / 2 + i] = (1 - a) * (1 - b) * UBuff1[Bf * xSize / 2 + Af] +
				a * (1 - b) * UBuff1[Bf * xSize / 2 + Ac] +
				(1 - a) * b * UBuff1[Bc * xSize / 2 + Af] +
				a * b * UBuff1[Bc * xSize / 2 + Ac];
		}
	}

	for (i = 0; i < newXSize / 2; i++)
	{
		for (j = 0; j < newYSize / 2; j++)
		{
			a = i / fx - floor(i / fx);
			b = j / fy - floor(j / fy);
			Af = floor(i / fx);
			Bf = floor(j / fy);
			Ac = ceil(i / fx);
			Bc = ceil(j / fy);


			VBuff[j * newXSize / 2 + i] = (1 - a) * (1 - b) * VBuff1[Bf * xSize / 2 + Af] +
				a * (1 - b) * VBuff1[Bf * xSize / 2 + Ac] +
				(1 - a) * b * VBuff1[Bc * xSize / 2 + Af] +
				a * b * VBuff1[Bc * xSize / 2 + Ac];
		}
	}

	YUV420toRGB(YBuff, UBuff, VBuff, newXSize, newYSize, output);
}

void bicubicInterpolate(uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
	uchar *YBuff = (uchar*)malloc(xSize * ySize * sizeof(uchar));
	char *UBuff = (char*)malloc(xSize * ySize * sizeof(char));
	char *VBuff = (char*)malloc(xSize * ySize * sizeof(char));

	uchar *YBuff1 = (uchar*)malloc(xSize * ySize * sizeof(uchar));
	char *UBuff1 = (char*)malloc(xSize * ySize * sizeof(char));
	char *VBuff1 = (char*)malloc(xSize * ySize * sizeof(char));

	RGBtoYUV420(input, xSize, ySize, YBuff1, UBuff1, VBuff1);

	angle = angle / 180 * PI;
	int a, b;
	int i, j;

	for (i = 0; i < xSize; i++)
	{
		for (j = 0; j < ySize; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m;
			double yj = j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n;

			if (xi - floor(xi) < 0.5)
				a = floor(xi);
			else
				a = ceil(xi);
			if (yj - floor(yj) < 0.5)
				b = floor(yj);
			else
				b = ceil(yj);

			if (a < xSize && b < ySize && a >= 0 && b >= 0)
				YBuff[j * xSize + i] = YBuff1[b * xSize + a];
			else
				YBuff[j * xSize + i] = 0;
		}
	}

	for (i = 0; i < xSize / 2; i++)
	{
		for (j = 0; j < ySize / 2; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - (m / 2) * cos(angle) + (n / 2) * sin(angle) + m / 2;
			double yj = j * cos(angle) + i * sin(angle) - (m / 2) * sin(angle) - (n / 2) * cos(angle) + n / 2;

			if (xi - floor(xi) >= 0.5)
				a = ceil(xi);
			else
				a = floor(xi);
			if (yj - floor(yj) >= 0.5)
				b = ceil(yj);
			else
				b = floor(yj);

			if (a < xSize / 2 && b < ySize / 2 && a >= 0 && b >= 0)
				UBuff[j * xSize / 2 + i] = UBuff1[(b * xSize + 1) / 2 + a];
			else
				UBuff[j * xSize / 2 + i] = 0;
		}
	}

	for (i = 0; i < xSize / 2; i++)
	{
		for (j = 0; j < ySize / 2; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - (m / 2) * cos(angle) + (n / 2) * sin(angle) + m / 2;
			double yj = j * cos(angle) + i * sin(angle) - (m / 2) * sin(angle) - (n / 2) * cos(angle) + n / 2;

			if (xi - floor(xi) >= 0.5)
				a = ceil(xi);
			else
				a = floor(xi);
			if (yj - floor(yj) >= 0.5)
				b = ceil(yj);
			else
				b = floor(yj);

			if (a < xSize / 2 && b < ySize / 2 && a >= 0 && b >= 0)
				VBuff[j * xSize / 2 + i] = VBuff1[(b * xSize + 1) / 2 + a];
			else
				VBuff[j * xSize / 2 + i] = 0;
		}
	}

	YUV420toRGB(YBuff, UBuff, VBuff, xSize, ySize, output);
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
	uchar *YBuff = (uchar*)malloc(xSize * ySize * sizeof(uchar));
	char *UBuff = (char*)malloc(xSize * ySize * sizeof(char));
	char *VBuff = (char*)malloc(xSize * ySize * sizeof(char));

	uchar *YBuff1 = (uchar*)malloc(xSize * ySize * sizeof(uchar));
	char *UBuff1 = (char*)malloc(xSize * ySize * sizeof(char));
	char *VBuff1 = (char*)malloc(xSize * ySize * sizeof(char));

	RGBtoYUV420(input, xSize, ySize, YBuff1, UBuff1, VBuff1);

	angle = angle / 180 * PI;
	float fx, fy, xi, yj;
	int Af, Bf, Ac, Bc;

	int i, j;

	for (i = 0; i < xSize; i++)
	{
		for (j = 0; j < ySize; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m;
			double yj = j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n;

			fx = xi - floor(xi);
			fy = yj - floor(yj);
			Af = floor(xi);
			Bf = floor(yj);
			Ac = ceil(xi);
			Bc = ceil(yj);

			if (Af < xSize && Bf < ySize && Af >= 0 && Bf >= 0 && Ac < xSize && Bc < ySize && Ac >= 0 && Bc >= 0)
			{
				YBuff[j * xSize + i] = (1 - fx) * (1 - fy) * YBuff1[Bf * xSize + Af] +
					fx * (1 - fy) * YBuff1[Bf * xSize + Ac] +
					(1 - fx) * fy * YBuff1[Bc * xSize + Af] +
					fx * fy * YBuff1[Bc * xSize + Ac];
			}
			else
				YBuff[j * xSize + i] = 0;
		}
	}

	m = m / 2;
	n = n / 2;

	for (i = 0; i < xSize / 2; i++)
	{
		for (j = 0; j < ySize / 2; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m;
			double yj = j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n;

			fx = xi - floor(xi);
			fy = yj - floor(yj);
			Af = floor(xi);
			Bf = floor(yj);
			Ac = ceil(xi);
			Bc = ceil(yj);

			if (Af < xSize / 2 && Bf < ySize / 2 && Af >= 0 && Bf >= 0 && Ac < xSize / 2 && Bc < ySize / 2 && Ac >= 0 && Bc >= 0)
			{
				UBuff[j * xSize / 2 + i] = (1 - fx) * (1 - fy) * UBuff1[Bf * xSize / 2 + Af] +
					fx * (1 - fy) * UBuff1[Bf * xSize / 2 + Ac] +
					(1 - fx) * fy * UBuff1[Bc * xSize / 2 + Af] +
					fx * fy * UBuff1[Bc * xSize / 2 + Ac];
			}
			else
				UBuff[j * xSize / 2 + i] = 0;
		}
	}

	for (i = 0; i < xSize / 2; i++)
	{
		for (j = 0; j < ySize / 2; j++)
		{
			double xi = i * cos(angle) - j * sin(angle) - m * cos(angle) + n * sin(angle) + m;
			double yj = j * cos(angle) + i * sin(angle) - m * sin(angle) - n * cos(angle) + n;

			fx = xi - floor(xi);
			fy = yj - floor(yj);
			Af = floor(xi);
			Bf = floor(yj);
			Ac = ceil(xi);
			Bc = ceil(yj);

			if (Af < xSize / 2 && Bf < ySize / 2 && Af >= 0 && Bf >= 0 && Ac < xSize / 2 && Bc < ySize / 2 && Ac >= 0 && Bc >= 0)
			{
				VBuff[j * xSize / 2 + i] = (1 - fx) * (1 - fy) * VBuff1[Bf * xSize / 2 + Af] +
					fx * (1 - fy) * VBuff1[Bf * xSize / 2 + Ac] +
					(1 - fx) * fy * VBuff1[Bc * xSize / 2 + Af] +
					fx * fy * VBuff1[Bc * xSize / 2 + Ac];
			}
			else
				VBuff[j * xSize / 2 + i] = 0;
		}
	}

	YUV420toRGB(YBuff, UBuff, VBuff, xSize, ySize, output);
}