int transp;//Fo transparency
OSL_COLOR *fade;//To hold the colour of the rectangle blitted

/*This was originally coded by Blade_punk. I modified it to add the X, and Y bits. */

void fadeOut(OSL_IMAGE* bg,int x, int y)//Name and params taken
{
	for(transp = 0; transp < 255; transp += 3)//Sets the transparency to 0 aka invisible. Then it increases by 3 until it equals 255 aka completely visible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
	}
}
void fadeIn(OSL_IMAGE* bg, int x, int y)//Name and params taken
{
	for(transp = 255; transp > 0; transp -= 3)//Sets the transparency to 255 aka completely visible. Then it decreases by 3 until it equals 255 aka invisible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
	}
}

