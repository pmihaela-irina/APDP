#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "apd-proiect.h"

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *filterData =0 ;
int filter; //filtrul selectat
int dimWindow = 0;
double alpha = 0.0;

static char filename[256] = {0};
static int mainPanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "apd-proiect.uir", MAIN_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	double maxVal = 0.0;
	double minVal = 0.0;
	int maxIndex = 0;
	int minIndex = 0;
	double mean = 0.0;
	double median =0.0;
	double dispersion = 0.0;
	
	switch (event)
	{
		case EVENT_COMMIT:

			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//calculare minim, maxim, mean, mediana, dispersie
			MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
			Mean(waveData,npoints,&mean);
			//Median(waveData,npoints,&median);
			StdDev(waveData,npoints,&mean,&dispersion);
			
			
			SetCtrlVal(panel, MAIN_PANEL_NUM_MAX, maxVal);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MEAN, mean);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MIN, minVal);
			SetCtrlVal(panel, MAIN_PANEL_NUM_MED, median);
			SetCtrlVal(panel, MAIN_PANEL_NUM_DIS, dispersion);
			//SetCtrlVal(panel, MAIN_PANEL_IDC_TIME_START, 0);
			//SetCtrlVal(panel, MAIN_PANEL_IDC_TIME_STOP, (int)(npoints/sampleRate));
			break;
	}
	return 0;
}

int CVICALLBACK OnMainPanelCB (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnPreviousButtonCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int val1 =(int) npoints/sampleRate;
			int val2 = 0;
			int val = 0;
			
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_START, &val);
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_STOP, &val2);
			if(val > 0 && val2 <= val1)
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_START, val-1);
				SetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_STOP, val2-1);
			}		
			
			break;
	}
	return 0;
}

int CVICALLBACK OnNextButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//apasarea butonului Next
			int val1 =(int) npoints/sampleRate;
			int val2 = 0;
			int val = 0;
			
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_START, &val);
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_STOP, &val2);
			if(val >= 0 && val2 < val1)
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_START, val+1);
				SetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_STOP, val2+1);
			}		
			
			break;
	}
	return 0;
}

int CVICALLBACK OnAplicationButtonCB (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(mainPanel, MAIN_PANEL_FILTER_RING, &filter);
			filterData = (double *) calloc(npoints, sizeof(double));
			
			if(filter==1)
			{	
				double s = 0;
				GetCtrlVal(mainPanel, MAIN_PANEL_DIM_WINDOW, &dimWindow);
				
				for (int i = 0; i < npoints; i++)
				{
					for (int k = 0; k <= dimWindow - 1; k++)
					{
						if ((i - k) < 0)
						{
							s += 0;
						}
						else
						{
						s += waveData[i-k];	
						}
					}
					filterData[i] = s/dimWindow;
					s = 0;
				}
			}
			else if(filter == 2)
			{		
				GetCtrlVal(mainPanel, MAIN_PANEL_ALPHA_WINDOW, &alpha);
				
				filterData[0] = waveData[0];
				
				for(int i = 1; i < npoints; i++)
				{
					filterData[i] = (1-alpha)*filterData[i-1] + alpha*waveData[i];
				}
			}
			
			int xScalingMode = 0;
			int yScalingMode = 0;
			double xmin = 0.0;
			double xmax = 0.0;
			double ymin = 0.0;
			double ymax = 0.0;
			GetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, &xScalingMode, &xmin,&xmax, &yScalingMode, &ymin, &ymax);
			SetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_FILTER_DATA, VAL_MANUAL, xmin,xmax, VAL_MANUAL, ymin, ymax);
			
			//citesti secunda start, secunda stop
			int sStart = 0;
			int sStop = 0;
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_STOP, &sStop);
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_TIME_START, &sStart);
			double *p = filterData + (int)(sStart * sampleRate);
			
			DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_FILTER_DATA,-1, VAL_IMMEDIATE_DRAW);
			//PlotY(panel, MAIN_PANEL_IDC_GRAPH_FILTER_DATA, filterData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panel, MAIN_PANEL_IDC_GRAPH_FILTER_DATA, p, (sStop - sStart)*sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);			
			//selectarea intervalului de secunde
			
			//salvarea imaginii
			int m, d, y, h, min, sec;
			int bitmap; 
			GetSystemDate(&m, &d, &y);
			GetSystemTime(&h, &min, &sec);
			GetCtrlDisplayBitmap(mainPanel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, 0,&bitmap);
			sprintf(filename,"images_saved/RawData-%d.%02d.%02d_%02d-%02d-%02d.jpg", y, m, d, h, min, sec); 
			SaveBitmapToJPEGFile(bitmap, filename, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmap);
			
			GetCtrlDisplayBitmap(mainPanel, MAIN_PANEL_IDC_GRAPH_FILTER_DATA, 0,&bitmap);
			sprintf(filename,"images_saved/FilterData-%d.%02d.%02d_%02d-%02d-%02d.jpg", y, m, d, h, min, sec); 
			SaveBitmapToJPEGFile(bitmap, filename, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmap);
			
			break;
	}
	return 0;
}

int CVICALLBACK OnDimensionWinCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK OnFilterClikedCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_FILTER_RING, &filter);
			if(filter == 1)
			{
				SetCtrlAttribute(mainPanel, MAIN_PANEL_DIM_WINDOW, ATTR_VISIBLE, 1);
				SetCtrlAttribute(mainPanel, MAIN_PANEL_ALPHA_WINDOW, ATTR_VISIBLE, 0);
			}
			else if(filter == 2)
			{
				SetCtrlAttribute(mainPanel, MAIN_PANEL_DIM_WINDOW, ATTR_VISIBLE, 0);
				SetCtrlAttribute(mainPanel, MAIN_PANEL_ALPHA_WINDOW, ATTR_VISIBLE, 1);
			}
			

			break;
	}
	return 0;
}
