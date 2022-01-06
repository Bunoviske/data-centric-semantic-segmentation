# README #

Imagem annotation and ground truth generation for semantic segmentation tasks. Watershed algorithm is used to help in the segmentation.

# Visual tutorial

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/YOUTUBE_VIDEO_ID_HERE/0.jpg)](https://www.youtube.com/watch?v=YOUTUBE_VIDEO_ID_HERE)


# Compilation

Install opencv and latest g++ and c++ 

- mkdir build  
- cd build  
- cmake ..  
- make  


# Execution

Two arguments:  

1- path to images folder  
2- path to classesNumber.txt 

Example  

- ./main ../bruno/ ../../classesConfiguration/classesNumber.txt 


# How to annotate data

1- Give hint marks for the Watershed algorithm  
2- Click 'w' to run watershed and check the segmentation result  
3- Click 'r' to reload and erase the marks  
4- Click 'c' to go to the classification step  
5- When classifying each segmentation region, first click on that region and then insert via terminal the class id  
6- Click 'n' to finalize this image and go to the next one  

AT ANY POINT OF THE PROGRAM, PRESS 'ESC' TO GO TO THE NEXT IMAGE


# Output

Two folders are created inside your images folder: 'done' has all the images already annotated and 'gt' has the ground truth images for semantic segmentation


# Important notes

When you load big images, opencv will not fit the image on your computer screen. I still need to resize these images according to your display dimensions.