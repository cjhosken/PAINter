
# A Beginner’s Guide to PAINter
*There’s nothing more painful than trying to use a basic paint program.* 

Christopher Hosken \
L4 Programming Principles \
2023/2024


## Introduction
Welcome to PAINter! Your beginner-friendly alternative to MSPaint (for Linux!). This guide will walk you through how to install, run, and use PAINter. And by the end, you should be a pro. So, let's jump right in!
## Starting PAINter
To run PAINter, you first need to download a [release](https://github.com/cjhosken/PAINter/releases). Currently, there is only support for Linux operating systems. Once you have installed and unpacked the release, navigate into the folder and find PAINter. Double-clicking on the file should run PAINter. If not, report a user bug [here](https://github.com/cjhosken/PAINter/issues).

Once PAINter starts, all you need to do is start painting!

## Using PAINter
PAINter has some tools that can aid you in your 2D graphics journey.
Importing, Creating, Saving
When you start PAINter, there are two optional command-line arguments that you can add to load and export images: -f and -o.

### Load Image (-f)
-f is the file that you plan on editing. If you leave this blank, PAINter will generate a blank 512x512 image instead.

### Save Image (-o)
-o is the name of the output save file. If you leave it blank, the image will save as output.png

To save the file in PAINter, press the save icon in the top navigation bar.

### Command Line Arguments
Running the application from the command line would look something like this. All paths are relative to the application runfile.

PAINter -f landscape.png -o saved.png


## Viewport Navigation
Viewport navigation is relatively simple (similar to how you may navigate the 2D viewport in Nuke), with an extra button for recentering the canvas.
### Panning (Middle Mouse Down)
Hold down your middle mouse button and move the mouse to pan across the canvas. Zooming is done with the scroll wheel and will target your mouse cursor.
### Home (H)
Pressing H will recenter the canvas.

## Thickness and Color
There are multiple ways to control color and brush thickness in PAINter. Currently, you can only choose brush thicknesses between 1px and 64px.
### Picker (P)
Pressing P will instantly activate the color picker.
### Color Builder (W)
Pressing W will open the color builder. Use the corresponding red, green, and blue sliders to build colors.
### Change Brush Size (-/+)
You can change your brush size by using - to make it smaller and + to make it larger.


## The Paint Tools
You have a wide range of paint tools available to you. From brushes to geometric shapes, there’s no limit to your creative freedom.
### Brush (B)
Pressing B or selecting the brush in the sidebar will enter you into brush mode. You can then draw on the canvas with the specified brush size and color.
### Eraser (E)
Pressing E or selecting the eraser will enter you into erase mode. Erasing will remove the paint you’ve added from the canvas, leaving your base image untouched.
### Fill (F)
Pressing F will let you use the fill tool. Click on an area and let the fill algorithm do its magic! 
### Line (L)
Pressing L will let you use the line tool. Click to specify a starting point and drag your mouse to make your line. Once you’re happy, let go of your mouse to confirm your line.
### Circle (C)
Pressing C will let you use the circle tool. Click to specify a center point and hold and drag your mouse to control the radius. Once you’re happy, let go of your mouse to confirm your circle.
### Square (S)
Pressing S will let you use the square tool. Click to specify the top-left corner of the square and drag your mouse to control where to place the bottom-right corner. Once you’re happy, let go of your mouse to confirm your square.

If you ever feel the need to start again, you can wipe the canvas by pressing the square in the top left of PAINter.

## Conclusion
Congratulations! You know now how to use PAINter. I look forward to seeing the art that you’ll go on to create. If you come across any bug or issue, don't be afraid to report it. Good luck painting!

This program was written by me, Christopher Hosken, for L4 Programming Principles at Bournemouth University. If you wish to get in contact with me, you can do so by messaging my on [LinkedIn](https://www.linkedin.com/in/christopher-hosken/) or sending an email to [hoskenchristopher@gmail.com]().