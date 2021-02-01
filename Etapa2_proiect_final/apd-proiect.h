/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ACQ_PANEL                        1       /* callback function: OnAcquisitionPanelCB */
#define  ACQ_PANEL_IDC_GRAPH_FILTER_DATA  2       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_IDC_GRAPH_RAW_DATA     3       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_LOAD_BUTTON            4       /* control type: command, callback function: OnLoadButtonCB */
#define  ACQ_PANEL_NUM_ZEROS              5       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_NUM_DIS                6       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_NUM_MED                7       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_NUM_MAX                8       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_NUM_MIN                9       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_NUM_MEAN               10      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_APLICATION_BUTTON      11      /* control type: command, callback function: OnAplicationButtonCB */
#define  ACQ_PANEL_NEXT_BUTTON            12      /* control type: command, callback function: OnNextButtonAQCB */
#define  ACQ_PANEL_PREV_BUTTON            13      /* control type: command, callback function: OnPreviousButtonAQCB */
#define  ACQ_PANEL_IDC_TIME_STOP          14      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_IDC_TIME_START         15      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_FILTER_RING            16      /* control type: ring, callback function: OnFilterClikedCB */
#define  ACQ_PANEL_TEXTMSG                17      /* control type: textMsg, callback function: (none) */
#define  ACQ_PANEL_IDC_SWITCH_PANEL       18      /* control type: binary, callback function: OnSwitchPanelCB */
#define  ACQ_PANEL_DIM_WINDOW             19      /* control type: slide, callback function: (none) */
#define  ACQ_PANEL_ALPHA_WINDOW           20      /* control type: scale, callback function: (none) */

#define  FRQ_PANEL                        2       /* callback function: OnFrequencyPanelCB */
#define  FRQ_PANEL_IDC_GRAPH_SPEC_FILTER  2       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_IDC_GRAPH_WINDOW       3       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_IDC_GRAPH_FILTRED      4       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_IDC_GRAPH_SPEC_WIN     5       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_IDC_GRAPH_SPECTRU      6       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_IDC_GRAPH_RAW_DATA     7       /* control type: graph, callback function: (none) */
#define  FRQ_PANEL_NEXT_BUTTON            8       /* control type: command, callback function: OnNextButtonFQCB */
#define  FRQ_PANEL_PREV_BUTTON            9       /* control type: command, callback function: OnPreviousButtonFQCB */
#define  FRQ_PANEL_IDC_TIME_STOP          10      /* control type: numeric, callback function: (none) */
#define  FRQ_PANEL_IDC_FREQ_PEAK          11      /* control type: numeric, callback function: (none) */
#define  FRQ_PANEL_IDC_POWER_PEAK         12      /* control type: numeric, callback function: (none) */
#define  FRQ_PANEL_FREQ_BUTTON            13      /* control type: command, callback function: OnFrequencyButtonCB */
#define  FRQ_PANEL_IDC_TIME_START         14      /* control type: numeric, callback function: (none) */
#define  FRQ_PANEL_TEXTMSG                15      /* control type: textMsg, callback function: (none) */
#define  FRQ_PANEL_IDC_SWITCH_PANEL       16      /* control type: binary, callback function: OnSwitchPanelCB */
#define  FRQ_PANEL_NUMERIC_FFT            17      /* control type: numeric, callback function: (none) */
#define  FRQ_PANEL_ID_FILTER_TYPE         18      /* control type: ring, callback function: OnChoseFilterCB */
#define  FRQ_PANEL_IDC_FILTER_RIPPLE      19      /* control type: numeric, callback function: OnChoseFilterRippleCB */
#define  FRQ_PANEL_IDC_FILTER_ORDER       20      /* control type: numeric, callback function: OnChoseFilterOrderCB */
#define  FRQ_PANEL_IDC_WINDOW_TYPE        21      /* control type: ring, callback function: OnChoseWindowCB */
#define  FRQ_PANEL_FILTER_BUTTON          22      /* control type: command, callback function: OnFilterButtonClikedCB */


     /* Control Arrays: */

#define  INTERVAL_ARRAY                   1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnAcquisitionPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAplicationButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChoseFilterCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChoseFilterOrderCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChoseFilterRippleCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChoseWindowCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButtonClikedCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterClikedCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButtonAQCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButtonFQCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPreviousButtonAQCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPreviousButtonFQCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif