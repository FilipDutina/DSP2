
#include "ImageProcessing.h"
#include "ImageInterpolation.h"

#include <QDebug>

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();

	int pom;

	/* NOTE: Calculate output image resolution and construct output image object */

	if(progName == "Sample and hold") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* TO DO: Calculate output image resolution and construct output image object */
		int newXSize, newYSize;

		newXSize = floor(params[1] * X_SIZE);
		newYSize = floor(params[0] * Y_SIZE);

		pom = newXSize / 4 + 1;
		newXSize = pom * 4;
		pom = newYSize / 4 + 1;
		newYSize = pom * 4;
		
		uchar *output = (uchar*)malloc(3 * sizeof(uchar) * newXSize * newYSize);

		/* TO DO: Perform Sample and hold interpolation  */
		
		sampleAndHold((uchar*)inImgs->bits(), X_SIZE, Y_SIZE, output, newXSize, newYSize);
		QImage img(output, newXSize, newYSize, inImgs->format());
		*outImgs = img;


	}
	else if (progName == "Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* TO DO: Calculate output image resolution and construct output image object */
		int newXSize, newYSize;

		newXSize = floor(params[1] * X_SIZE);
		newYSize = floor(params[0] * Y_SIZE);

		pom = newXSize / 4 + 1;
		newXSize = pom * 4;
		pom = newYSize / 4 + 1;
		newYSize = pom * 4;

		uchar *output = (uchar*)malloc(3 * sizeof(uchar) * newXSize * newYSize);

		/* TO DO: Perform Bilinear interpolation  */

		bilinearInterpolate((uchar*)inImgs->bits(), X_SIZE, Y_SIZE, output, newXSize, newYSize);
		QImage img(output, newXSize, newYSize, inImgs->format());
		*outImgs = img;
	}
	else if (progName == "Bicubic")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* TO DO: Calculate output image resolution and construct output image object */

		/* TO DO: Perform Bicubic interpolation  */

	}
	else if(progName == "Rotation") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Rotation angle in degrees is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */

		/* TO DO: Construct output image object */
		uchar* output = (uchar*)malloc(3 * sizeof(uchar)*X_SIZE*Y_SIZE);

		/* TO DO: Perform image rotation */
		imageRotate((uchar*)inImgs->bits(), X_SIZE, Y_SIZE, output, X_SIZE / 2, Y_SIZE / 2, params[0]);

		QImage img(output, X_SIZE, Y_SIZE, inImgs->format());
		*outImgs = img;
	
	}
	else if (progName == "Rotation Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Rotation angle in degrees is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */

		/* TO DO: Construct output image object */
		uchar* output = (uchar*)malloc(3 * sizeof(uchar) * X_SIZE * Y_SIZE);

		/* TO DO: Perform image rotation with bilinear interpolation */
		imageRotateBilinear((uchar*)inImgs->bits(), X_SIZE, Y_SIZE, output, 126, 126, params[0]);
		QImage img(output, X_SIZE, Y_SIZE, inImgs->format());
		*outImgs = img;
	}

}

