#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "apd-proiect.h"

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

			break;
	}
	return 0;
}

