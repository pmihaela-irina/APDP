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

#define  MAIN_PANEL                       1       /* callback function: OnMainPanelCB */
#define  MAIN_PANEL_IDC_GRAPH_FILTER_DATA 2       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_GRAPH_RAW_DATA    3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_LOAD_BUTTON           4       /* control type: command, callback function: OnLoadButtonCB */
#define  MAIN_PANEL_NUM_DIS               5       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MED               6       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MAX               7       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MIN               8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUM_MEAN              9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_APLICATION_BUTTON     10      /* control type: command, callback function: OnAplicationButtonCB */
#define  MAIN_PANEL_NEXT_BUTTON           11      /* control type: command, callback function: OnNextButtonCB */
#define  MAIN_PANEL_PREV_BUTTON           12      /* control type: command, callback function: OnPreviousButtonCB */
#define  MAIN_PANEL_IDC_TIME_STOP         13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_TIME_START        14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_FILTER_RING           15      /* control type: ring, callback function: OnFilterClikedCB */
#define  MAIN_PANEL_ALPHA_WINDOW          16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DIM_WINDOW            17      /* control type: numeric, callback function: OnDimensionWinCB */
#define  MAIN_PANEL_TEXTMSG               18      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

#define  INTERVAL_ARRAY                   1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnAplicationButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDimensionWinCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterClikedCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPreviousButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif