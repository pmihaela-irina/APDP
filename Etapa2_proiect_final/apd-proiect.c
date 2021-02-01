#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "apd-proiect.h"
#include "toolbox.h"

// Constants
#define SAMPLE_RATE			0
#define NPOINTS				1
#define FLTR_MEDITION		0
#define FLTR_ALPHA 			1
#define FLTR_BW_HPF			0
#define FLTR_INV_CH_BPF		1

//Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements

	//frecventa de esantionare
double sampleRate = 0.0;
	//numarul de esantioane
int npoints = 0;
	//vectorul ce contine datele semnalului audio
double *waveData = 0;
	//vectorul ce contine datele semnalului filtrat in domeniul timp
double *filterTimeData =0 ;
	//vectorul ce contine datele semnalului filtrat Butterworth in domeniul frecventa
double *filteredSignalBw =0 ;
	//vectorul ce contine datele semnalului filtrat Chebyshev in domeniul frecventa
double *filteredSignalCh =0 ;
	//tip filtru
int filterType; 
	//tip fereastra
int windowType = 0;
	//dimensiunea ferestrei
int dimWindow = 0;
	//valoarea parametrului alpha
double alpha = 0.0;
	//ordinul filtrului
int filterOrder = 1;
	//Stop band attenuation in decibels
double stopBandAttendb = 40.0;
	//secunda de start
int sStart = 0;
	//secunda de stop
int sStop = 0;

	//vector ce contine spectrul semnalului convertit la volti
double *convertedSpectrum;
	//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
double powerPeak = 0.0;
	//valoarea maxima din spectru de putere (din autoSpectrum)
double freqPeak = 0.0;
	//variabila ce reprezinta pasul in domeniul frecventei
double df = 0.0;
	//voltage signal - descriere a semnalului achizitionat
char unit[32] = "V";
	//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
double *autoSpectrum;
WindowConst winConst;
double dt;

static char filename[256] = {0};
static int frqPanel = 0;
static int acqPanel = 0;

int acquisition = 0;
int N = 0;

int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (acqPanel = LoadPanel (0, "apd-proiect.uir", ACQ_PANEL));
	errChk (frqPanel = LoadPanel (0, "apd-proiect.uir", FRQ_PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (acqPanel));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (acqPanel > 0)
		DiscardPanel (acqPanel);
	return 0;
}

/*************************************************************************************
	functie de callback pentru butonul de LoadExternalModule
**************************************************************************************/
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

			LaunchExecutable("python main.py");
			
				//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
				
				//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
				//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
				//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
				//afisare pe grapf
			DeleteGraphPlot(acqPanel, ACQ_PANEL_IDC_GRAPH_RAW_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(acqPanel, ACQ_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			DeleteGraphPlot(frqPanel, FRQ_PANEL_IDC_GRAPH_RAW_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_RAW_DATA, waveData, sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
				//calculare minim, maxim, mean, mediana, dispersie
			MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
			Mean(waveData,npoints,&mean);
			Median(waveData,npoints,&median);
			StdDev(waveData,npoints,&mean,&dispersion);
			
			
			SetCtrlVal(panel, ACQ_PANEL_NUM_MAX, maxVal);
			SetCtrlVal(panel, ACQ_PANEL_NUM_MEAN, mean);
			SetCtrlVal(panel, ACQ_PANEL_NUM_MIN, minVal);
			SetCtrlVal(panel, ACQ_PANEL_NUM_MED, median);
			SetCtrlVal(panel, ACQ_PANEL_NUM_DIS, dispersion);
			//SetCtrlVal(panel, ACQ_PANEL_IDC_TIME_START, 0);
			//SetCtrlVal(panel, ACQ_PANEL_IDC_TIME_STOP, (int)(npoints/sampleRate));
			
				//calculare valori de zero-crrosing
			int zero_crossing = 0;
			for ( int i = 2; i < npoints; i++)
			{
				if((waveData[i] * waveData[i-1]) < 0)
				{
					zero_crossing = zero_crossing + 1;
				}
			}
			SetCtrlVal(panel, ACQ_PANEL_NUM_ZEROS, zero_crossing);
			
			acquisition = 1;
			break;
	}
	return 0;
}

int CVICALLBACK OnAcquisitionPanelCB (int panel, int event, void *callbackData,
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

/**************************************************************************************
	functie de callback pentru butonul Previous din panoul de achizitie
***************************************************************************************/
int CVICALLBACK OnPreviousButtonAQCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
			//apasarea butonului Previous
		case EVENT_COMMIT:
			int val1 =(int) npoints/sampleRate;
			int val2 = 0;
			int val = 0;
			
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_START, &val);
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_STOP, &val2);
			if(val > 0 && val2 <= val1)
			{
				SetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_START, val-1);
				SetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_STOP, val2-1);
			}		
			
			break;
	}
	return 0;
}

/**************************************************************************************
	functie de callback pentru butonul Next din panoul de achizitie
***************************************************************************************/
int CVICALLBACK OnNextButtonAQCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//apasarea butonului Next
			int val1 =(int) npoints/sampleRate;
			int val2 = 0;
			int val = 0;
			
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_START, &val);
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_STOP, &val2);
			if(val >= 0 && val2 < val1)
			{
				SetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_START, val+1);
				SetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_STOP, val2+1);
			}		
			
			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback a butonului de aplicare (procesare in domeniul timp)
*********************************************************************************/
int CVICALLBACK OnAplicationButtonCB (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(acqPanel, ACQ_PANEL_FILTER_RING, &filterType);
			filterTimeData = (double *) calloc(npoints, sizeof(double));
			
			if(filterType== FLTR_MEDITION)
			{	
				double s = 0;
				
				GetCtrlVal(acqPanel, ACQ_PANEL_DIM_WINDOW, &dimWindow);
				
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
					filterTimeData[i] = s/dimWindow;
					s = 0;
				}
			}
			else if(filterType == FLTR_ALPHA)
			{		
				GetCtrlVal(acqPanel, ACQ_PANEL_ALPHA_WINDOW, &alpha);
				
				filterTimeData[0] = waveData[0];
				
				for(int i = 1; i < npoints; i++)
				{
					filterTimeData[i] = (1-alpha)*filterTimeData[i-1] + alpha*waveData[i];
				}
			}
			
				//selectarea intervalului de secunde
			int xScalingMode = 0;
			int yScalingMode = 0;
			double xmin = 0.0;
			double xmax = 0.0;
			double ymin = 0.0;
			double ymax = 0.0;
			GetAxisRange(panel, ACQ_PANEL_IDC_GRAPH_RAW_DATA, &xScalingMode, &xmin,&xmax, &yScalingMode, &ymin, &ymax);
			SetAxisRange(panel, ACQ_PANEL_IDC_GRAPH_FILTER_DATA, VAL_MANUAL, xmin,xmax, VAL_MANUAL, ymin, ymax);
			
				//citesti secunda start, secunda stop
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_STOP, &sStop);
			GetCtrlVal(acqPanel, ACQ_PANEL_IDC_TIME_START, &sStart);
			double *p = filterTimeData + (int)(sStart * sampleRate);
			
				//afisarea semnalului filtrat
			DeleteGraphPlot(panel, ACQ_PANEL_IDC_GRAPH_FILTER_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, ACQ_PANEL_IDC_GRAPH_FILTER_DATA, p, (sStop - sStart)*sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);			
			
				//salvarea imaginii
			int m, d, y, h, min, sec;
			int bitmap; 
			GetSystemDate(&m, &d, &y);
			GetSystemTime(&h, &min, &sec);
			GetCtrlDisplayBitmap(acqPanel, ACQ_PANEL_IDC_GRAPH_RAW_DATA, 0,&bitmap);
			sprintf(filename,"images_saved/RawData-%d.%02d.%02d_%02d-%02d-%02d.jpg", y, m, d, h, min, sec); 
			SaveBitmapToJPEGFile(bitmap, filename, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmap);
			
			GetCtrlDisplayBitmap(acqPanel, ACQ_PANEL_IDC_GRAPH_FILTER_DATA, 0,&bitmap);
			sprintf(filename,"images_saved/FilterData-%d.%02d.%02d_%02d-%02d-%02d.jpg", y, m, d, h, min, sec); 
			SaveBitmapToJPEGFile(bitmap, filename, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmap);
			
			
			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback a butonului de alegerea a filtrului din domeniul timp
*********************************************************************************/
int CVICALLBACK OnFilterClikedCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(acqPanel, ACQ_PANEL_FILTER_RING, &filterType);
			if(filterType == FLTR_MEDITION)
			{
				SetCtrlAttribute(acqPanel, ACQ_PANEL_DIM_WINDOW, ATTR_VISIBLE, 1);
				SetCtrlAttribute(acqPanel, ACQ_PANEL_ALPHA_WINDOW, ATTR_VISIBLE, 0);
			}
			else if(filterType == FLTR_ALPHA)
			{
				SetCtrlAttribute(acqPanel, ACQ_PANEL_DIM_WINDOW, ATTR_VISIBLE, 0);
				SetCtrlAttribute(acqPanel, ACQ_PANEL_ALPHA_WINDOW, ATTR_VISIBLE, 1);
			}
			

			break;
	}
	return 0;
}

/***************************************************************************************************
	functie de callback a butonului de switch intre cele doua paneluri (de achizitie si frecventa)
****************************************************************************************************/
int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(acquisition == 1)
			{
				
				if(panel == acqPanel)
				{
					SetCtrlVal(frqPanel, FRQ_PANEL_IDC_SWITCH_PANEL, 1);
					DisplayPanel(frqPanel);
					HidePanel(panel);
				}
				else
				{
					SetCtrlVal(acqPanel, FRQ_PANEL_IDC_SWITCH_PANEL, 0);
					DisplayPanel(acqPanel);
					HidePanel(panel);
				}
			}
			else
			{
				MessagePopup("ERROR!", "Load wave file!");	
			}
			
			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback a butonului de prelucrare in frecventa
*********************************************************************************/
int CVICALLBACK OnFrequencyButtonCB (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
		//extragem dimensiunea ferestrei
	GetCtrlVal(frqPanel, FRQ_PANEL_NUMERIC_FFT, &N);
	convertedSpectrum = (double *) calloc(sampleRate, sizeof(double));
	autoSpectrum = (double *) calloc(sampleRate, sizeof(double));
	dt = 1/sampleRate;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
				//citesti secunda start, secunda stop
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, &sStop);
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, &sStart);
			double *p = waveData + (unsigned int)(sStart *sampleRate);
			double *newData = waveData + (unsigned int)(sStart *sampleRate);
			
			//afisare semnal nefiltrat pe secunde
			DeleteGraphPlot(frqPanel, FRQ_PANEL_IDC_GRAPH_RAW_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_RAW_DATA, newData, (sStop-sStart)*sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
				//aplatizare forma semnal la capetele intervalului
			ScaledWindow(p,N, RECTANGLE_, &winConst);
				//partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
			AutoPowerSpectrum(p,N, dt, autoSpectrum, &df); 
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului  
			PowerFrequencyEstimate(autoSpectrum, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
				//afisez pe interfata valorile determinate				    
			SetCtrlVal( frqPanel, FRQ_PANEL_IDC_FREQ_PEAK, freqPeak);
			SetCtrlVal(frqPanel, FRQ_PANEL_IDC_POWER_PEAK, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			SpectrumUnitConversion(autoSpectrum,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum,unit);
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    PlotWaveform( frqPanel, FRQ_PANEL_IDC_GRAPH_SPECTRU, convertedSpectrum, 22050 ,VAL_DOUBLE, 1.0, 0.0, 0.0, 1,
		                                    VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);		
				
			break;
	}
	return 0;
}

int CVICALLBACK OnFrequencyPanelCB (int panel, int event, void *callbackData,
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

/********************************************************************************
	functie de callback a butonului Previous din panoul de frecventa
*********************************************************************************/
int CVICALLBACK OnPreviousButtonFQCB (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
			case EVENT_COMMIT:
				int val1 =(int) npoints/sampleRate;
				int val2 = 0;
				int val = 0;
				
				GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, &val);
				GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, &val2);
				if(val > 0 && val2 <= val1)
				{
					SetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, val-1);
					SetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, val2-1);
				}
				break;
	}
	return 0;
}

/********************************************************************************
	functie de callback a butonului Next din panoul de frecventa
*********************************************************************************/
int CVICALLBACK OnNextButtonFQCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//apasarea butonului Next
			int val1 =(int) npoints/sampleRate;
			int val2 = 0;
			int val = 0;
			
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, &val);
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, &val2);
			if(val >= 0 && val2 < val1)
			{
				SetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, val+1);
				SetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, val2+1);
			}		
			
			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback pentru alegerea tipului de fereastra
********************************************************************************/
int CVICALLBACK OnChoseWindowCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_WINDOW_TYPE, &windowType);
			break;
	}
	return 0;
}

/***********************************************************************************
	functie de callback a butonului de alegere a filtrului din domeniul frecventa
************************************************************************************/
int CVICALLBACK OnChoseFilterCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				//obtin tipul filtrului
			GetCtrlVal(frqPanel, FRQ_PANEL_ID_FILTER_TYPE, &filterType);
			switch (filterType)
			{
				case FLTR_BW_HPF:
					SetCtrlAttribute(frqPanel, FRQ_PANEL_IDC_FILTER_ORDER, ATTR_VISIBLE, 1);
					SetCtrlAttribute(frqPanel, FRQ_PANEL_IDC_FILTER_RIPPLE, ATTR_VISIBLE, 0);
					break;
				case FLTR_INV_CH_BPF:
					SetCtrlAttribute(frqPanel, FRQ_PANEL_IDC_FILTER_ORDER, ATTR_VISIBLE, 0);
					SetCtrlAttribute(frqPanel, FRQ_PANEL_IDC_FILTER_RIPPLE, ATTR_VISIBLE, 1);
					break;
			}

			break;
	}
	return 0;
}
/********************************************************************************
	functie de callback pentru amplitude of the stop band ripple in decibels
*********************************************************************************/
int CVICALLBACK OnChoseFilterRippleCB (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_FILTER_RIPPLE, &stopBandAttendb);

			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback pentru alegerea ordinului filtrului
********************************************************************************/
int CVICALLBACK OnChoseFilterOrderCB (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				//obtin ordinul filtrului
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_FILTER_ORDER, &filterOrder);

			break;
	}
	return 0;
}

/********************************************************************************
	functie de callback pentru alegerea filtrului in domeniul frecventa
********************************************************************************/
int CVICALLBACK OnFilterButtonClikedCB (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	double *newWave;
	double *signalWindowed;
	double *window;
	signalWindowed = (double *)calloc(sampleRate, sizeof(double));
		
	switch (event)
	{
			case EVENT_COMMIT:
			//WindowConst constants;
			//extragem dimensiunea ferestrei
			GetCtrlVal(frqPanel, FRQ_PANEL_NUMERIC_FFT, &N);
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_STOP, &sStop);
			GetCtrlVal(frqPanel, FRQ_PANEL_IDC_TIME_START, &sStart);
			
			//ferestruire
			newWave = waveData;
			window = newWave + (unsigned int)(sStart *sampleRate);
			LinEv1D (window, sampleRate, 1, 0, signalWindowed);
			switch (windowType)
			{
				case 0:
				{
					FlatTopWin (signalWindowed, sampleRate);
					DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_WINDOW, signalWindowed, (sStop-sStart)*sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
					
			 		break;
				}
			 	case 1:
				{
					WelchWin (signalWindowed, sampleRate);
					DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_WINDOW, signalWindowed, (sStop-sStart)*sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			
			 		break;
				}
			}
			
				//calculare spectru dupa ferestruire
			double *autoSpectrumWindow = (double *) calloc(sampleRate,sizeof(double));
			double *convertedSpectrumWindow = (double *) calloc(sampleRate, sizeof(double));
			dt=1/sampleRate;
			
			AutoPowerSpectrum(window,N, dt, autoSpectrumWindow, &df); 
			PowerFrequencyEstimate(autoSpectrumWindow, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
		    SpectrumUnitConversion(autoSpectrumWindow,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrumWindow,unit);
			DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_WIN, -1, VAL_IMMEDIATE_DRAW);	
			PlotWaveform( frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_WIN, convertedSpectrumWindow, 22050 ,VAL_DOUBLE, 1.0, 0.0, 0.0, 1,
		                                    VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
			//aplicare filtru
			filteredSignalBw = (double *) calloc(sampleRate, sizeof(double));
			filteredSignalCh = (double *) calloc(sampleRate, sizeof(double));
			switch (filterType)
			{
				case FLTR_BW_HPF:
					{
						Bw_HPF(signalWindowed, sampleRate, sampleRate, sampleRate/6, filterOrder, filteredSignalBw);
						//Bw_LPF(window, npoints, sampleRate, sampleRate/6, filterOrder, filterFrqData);
							//afisare semnal filtrat
						DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_FILTRED, -1, VAL_IMMEDIATE_DRAW);
						PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_FILTRED, filteredSignalBw, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);

							//afisarea spectrului semnalului filtrat
						double *autoSpectrumFilter = (double *) calloc(sampleRate,sizeof(double));
						double *convertedSpectrumFilter = (double *) calloc(sampleRate, sizeof(double));
						dt=1/sampleRate;
						
						AutoPowerSpectrum(filteredSignalBw,N, dt, autoSpectrumFilter, &df); 
						PowerFrequencyEstimate(autoSpectrumFilter, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
					    SpectrumUnitConversion(autoSpectrumFilter,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrumFilter,unit);
						DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_FILTER, -1, VAL_IMMEDIATE_DRAW);	
						PlotWaveform( frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_FILTER, convertedSpectrumFilter, 22050 ,VAL_DOUBLE, 1.0, 0.0, 0.0, 1,
					                                    VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
						
						break;
					}
				case FLTR_INV_CH_BPF:
				{
					InvCh_BPF (signalWindowed, sampleRate, sampleRate, sampleRate/8, sampleRate/4, stopBandAttendb, 5, filteredSignalCh);
						//afisare semnal filtrat
					DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_FILTRED, -1, VAL_IMMEDIATE_DRAW);
					PlotY(frqPanel, FRQ_PANEL_IDC_GRAPH_FILTRED, filteredSignalCh, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);

						//afisarea spectrului semnalului filtrat
					double *autoSpectrumFilter = (double *) calloc(sampleRate,sizeof(double));
					double *convertedSpectrumFilter = (double *) calloc(sampleRate, sizeof(double));
					dt=1/sampleRate;
						
					AutoPowerSpectrum(filteredSignalCh,N, dt, autoSpectrumFilter, &df); 
					PowerFrequencyEstimate(autoSpectrumFilter, N/2, -1.0, winConst, df,7, &freqPeak,&powerPeak);
				    SpectrumUnitConversion(autoSpectrumFilter,N/2, 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrumFilter,unit);
					DeleteGraphPlot (frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_FILTER, -1, VAL_IMMEDIATE_DRAW);	
					PlotWaveform( frqPanel, FRQ_PANEL_IDC_GRAPH_SPEC_FILTER, convertedSpectrumFilter, 22050 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
				                                    VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
					break;
				}
			}
				
				
			break;
	}
	return 0;
}
