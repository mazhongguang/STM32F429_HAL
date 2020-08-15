#include "GUI.h" 
/********************************************************************* * *

Defines * ********************************************************************** */ 

// // Recommended memory to run the sample with adequate performance 
//
#define RECOMMENDED_MEMORY (1024L * 5) 
/********************************************************************* * *       
Public code * ********************************************************************** 
*/
/********************************************************************* * *       MainTask */
void MainTask(void) 
{  
	/* float f = 123.4567;
	char *stemwin; */
	/* int i;
	char actext[] = "this example demostrates text wrapping ! ";
	GUI_RECT rect = {200, 240, 259, 299};
	GUI_WRAPMODE awm[] = 
						{
							GUI_WRAPMODE_NONE,
							GUI_WRAPMODE_CHAR,
							GUI_WRAPMODE_WORD
						}; */

	GUI_Init(); 

//   
// Check if recommended memory for the sample is available   
//  
	if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) 
	{     
		GUI_ErrorOut("Not enough memory available.");      
		return;   
	} 

	 GUI_SetBkColor(GUI_WHITE);
	 GUI_Clear();
	 GUI_SetFont(&GUI_Font24_ASCII);

	 /* 文本测试 */
	 #if 0
	 /* GUI_SetColor(GUI_YELLOW);
	 GUI_DispString("Hello World !\r\n");
	 GUI_DispString("123456");
	 GUI_DispString("123456");

	 GUI_SetFont(&GUI_Font8x16);
	 GUI_SetPenSize(10);
	 GUI_SetColor(GUI_RED);
	 GUI_DrawLine(300, 50, 500, 130);
	 GUI_DrawLine(300, 130, 500, 50);
	 GUI_SetBkColor(GUI_BLACK);
	 GUI_SetColor(GUI_WHITE);
	 GUI_SetTextMode(GUI_TM_NORMAL);
	 GUI_DispStringHCenterAt("GUI_TM_NORMAL", 400, 50);

	 GUI_SetTextMode(GUI_TM_REV);
	 GUI_DispStringHCenterAt("GUI_TM_REV", 400, 66);

	 GUI_SetTextMode(GUI_TM_TRANS);
	 GUI_DispStringHCenterAt("GUI_TM_TRANS", 400, 82);

	 GUI_SetTextMode(GUI_TM_XOR);
	 GUI_DispStringHCenterAt("GUI_TM_XOR", 400, 98);

	 GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
	 GUI_DispStringHCenterAt("GUI_TM_TRANS | GUI_TM_REV", 400, 114);

	 GUI_SetTextMode(GUI_TM_TRANS);
	 for (i = 0; i < 3; i++)
	 {
		 GUI_SetColor(GUI_WHITE);
		 GUI_FillRectEx(&rect);
		 GUI_SetColor(GUI_BLACK);
		 GUI_DispStringInRectWrap(actext, &rect, GUI_TA_LEFT, awm[i]);
		 rect.x0 += 70;
		 rect.x1 += 70;
	 } */
#endif
/* 数值测试 */
#if 0
	GUI_SetColor(GUI_RED);
	GUI_DispStringHCenterAt("DEC TEST !!!", 399, 0);
	GUI_SetColor(GUI_WHITE);

	GUI_DispStringAt("GUI_DispDec():", 0, 20);
	GUI_DispDec(123, 4);
	GUI_DispString("   ");
	GUI_DispDec(-123, 4);

	GUI_DispStringAt("GUI_DispDecAt():      ", 0, 44);
	GUI_DispDecAt(100, 192, 44, 3);
	GUI_DispDecAt(-564, 252, 44, 4);

	GUI_DispStringAt("GUI_DispDecShift():", 0, 68);
	GUI_DispDecShift(1236, 5, 2);
	GUI_DispString("  ");
	GUI_DispDecShift(-1236, 6, 1);

	GUI_DispStringAt("GUI_DispDecSpace():", 0, 92);
	GUI_DispDecSpace(3265, 5);

	GUI_DispStringAt("GUI_DispSDec():   ", 0, 116);
	GUI_DispSDec(201, 4);
	GUI_DispString("  ");
	GUI_DispSDec(-201, 4);

	GUI_DispStringAt("GUI_DispSDecShift():   ", 0, 140);
	GUI_DispSDecShift(123456, 8, 3);

	GUI_SetColor(GUI_RED);
	GUI_DispStringHCenterAt("FLOAT TEST !!!", 399, 164);
	GUI_SetColor(GUI_WHITE);

	GUI_DispStringAt("GUI_DispFloat():\r\n", 0, 188);
	GUI_DispFloat(f, 9);
	GUI_GotoX(120);
	GUI_DispFloat(-f, 9);

	GUI_DispStringAt("GUI_DispFloatFix():\n", 0, 236);
	GUI_DispFloatFix(f, 9, 2);
	GUI_GotoX(120);
	GUI_DispFloatFix(-f, 9, 2);

	GUI_DispStringAt("GUI_DispSFloatFix():\n", 0, 284);
	GUI_DispSFloatFix(f, 9, 2);
	GUI_GotoX(120);
	GUI_DispFloatFix(-f, 9, 2);

	GUI_DispStringAt("GUI_DispFloatMin():\n", 0, 332);
	GUI_DispFloatMin(f, 3);
	GUI_GotoX(120);
	GUI_DispFloatMin(-f, 3);

	GUI_DispStringAt("GUI_DispSFloatMin():\n", 0, 380);
	GUI_DispSFloatMin(f, 3);
	GUI_GotoX(120);
	GUI_DispFloatMin(-f, 3);

	GUI_DispStringHCenterAt("GUI_GetVersionString():", 237, 428);
	stemwin = (char *)GUI_GetVersionString();
	GUI_DispStringHCenterAt(stemwin, 399, 428);
#endif

/* 2D 绘图 */

	GUI_SetColor(GUI_RED);
	GUI_DispStringHCenterAt(" 2D TEST !!!", 399, 0);

	GUI_SetBkColor(GUI_YELLOW);
	GUI_ClearRect(5, 20, 95, 110);
	GUI_SetBkColor(GUI_BLUE);

	GUI_DrawGradientH(100, 20, 190, 110, 0x4117bb, 0xc6b6f5);
	GUI_DrawGradientRoundedV(195, 20, 285, 110, 30, 0xffffff, 0);
	GUI_DrawRect(290, 20, 380, 110);
	GUI_FillRect(385, 20, 475, 110);
	GUI_FillRoundedRect(480, 20, 570, 110, 30);

	GUI_SetColor(GUI_RED);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt("Alpha TEST !!!", 399, 120);
	
	GUI_EnableAlpha(1);
	GUI_SetColor(GUI_BLACK);
	GUI_DispStringHCenterAt("alpha blending ", 110, 260);
	GUI_SetAlpha(40);
	GUI_SetColor(GUI_RED);
	GUI_FillRect(10, 170, 110, 270);
	GUI_SetAlpha(40);
	GUI_SetColor(GUI_GREEN);
	GUI_FillRect(60, 220, 160, 320);
	GUI_SetAlpha(40);
	GUI_SetColor(GUI_BLUE);
	GUI_FillRect(110, 270, 210, 370);
	GUI_EnableAlpha(0);


	 while(1);
	
}