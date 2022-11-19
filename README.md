# TinyML : Gesture recognition :


![image](https://user-images.githubusercontent.com/101800565/202808649-a887f14e-bbc6-4c87-9155-cd19c9eb7f22.png)
![image](https://user-images.githubusercontent.com/101800565/202809681-8c0353f4-5825-4d64-91ff-a7a354e63418.png)


This project is inspired from the great work of billiyz: https://github.com/billiyz/tinyml-on-the-edge and ShawnHymel: https://github.com/ShawnHymel/tinyml-example-anomaly-detection .


The main goal of this project is to be able to detect the movement done by the IMU, either UPDOWN, RIGHTLEFT or IDLE  using Machine learning directly on the arduino Nano BLE 33 microcontroller and send the result of the classification to my Laptop using UART protocol.


This project will follow the following procedures : 


***- Workflow used :***


  1- Setting up arduino IDE with the necessary libraries


  2- Look into accelerometer operation


  3- Write a sketch to collect accelerometer data at sampling rate =  119Hz


  4- Upload the data to the google colaboratory platform


  5- Extract Features to be used for the training


  6- Train a neural network and Convert the trained model into tensorflow lite model


  7- Write a sketch that do the inference in the arduino nano and send data to the Laptop


***- Materials used:***

  We will work mainly with 2 materials:

   1. Arduino Nano BLE 33 which contains an Accelerometer :
  
  
   ![image](https://user-images.githubusercontent.com/101800565/202809385-bd853fbf-ae1d-48bd-89f6-2a987af54cc7.png)
   
   
***-Setting up arduino IDE :***


To work on this project you need to have 2 important libraries


1. Arduino_TensorFlowLite library
This Arduino library is here to simplify the deployment of Tensorflow Lite for Microcontrollers models to Arduino boards using the Arduino IDE.


2. Arduino_LSM9DS1
Arduino library for LSM9DS1 Nine-Axis using only Accelerometer MEMS MotionTracking™ Device.


To install this 2 libraries, you just need to get this repository where you will find zip files with the names of the 3 libraries.

Include the 2 of them in your project using ***Sketch -> Include library -> Add .ZIP Library...***

Finally you need you arduino IDE to be able to recognize the Arduino Nano BLE 33 so you just need to go to: ***tools -> board -> boards manager*** then write Arduino Nano BLE 33 and install the library.


Accelerometer :

In IMU 3d accelerometer sensor, the sensing mechanism for all three accelerometers is capacitive. The accelerometer measures linear acceleration along the x, y, and z axis.


***-Collect sensor data :***

To capture the accelerometer data, A sketch is written on arduino IDE and uploaded to the board. The sketch in this repository is labelled 
DataCollection. 


![image](https://user-images.githubusercontent.com/101800565/202810830-53b0fa4b-a7ee-4b2f-8ad8-01bb241f4f1a.png)


Make sure you have Ax,Ay,Az, on the serial monitor. With the LSM9DS1 in your hand, make a Move upward to signify 'UP'.Make 20-30 Up movement. On the Serial monitor, 
uncheck Autoscrol button and copy the data. Save the data on your pc as UPDOWN.csv.Make a Move to the right 20-30 times too while checking Serial Monitor and copy the data again, but this time save as RIGHTLEFT.csv and Idle.csv on your PC. 


***-Google colaboratory data upload :***

Open google Colaboratory platorm, Move to Files. Drag and drop your UPDOWN.csv, RIGHTLEFT.csv and IDLE.csv files in the sample_data folder. The Google colaboratory provides a Jupyter notebook that allows us to run our TensorFlow training in a web browser.


***-Extract Features to be used for the training :***
To learn how to decide the best features to use for your training to have the best accuracy, you may walk through this tutoriel done by ShawnHymel which was really inspiring for me: Feature-Extraction.



For my work, I decided to work with Median absolute deviation(MAD which is a robust measure of how spread out a set of data is. It's the best choice when your data has extremely high or extremely low values and non normality


![image](https://user-images.githubusercontent.com/101800565/202811488-5e6d35be-ed2e-4f28-a881-b7cbe850290e.png)


Train neural network, build and train a model.
While on the Google colaboratory platform, go trough the notebook till the end. Convertion of the trained model into tensorflow lite model is done near the end. When the model.h file is created under Files, click on the model.h file to download to your pc. 


![image](https://user-images.githubusercontent.com/101800565/202811970-54e2eec5-82a1-40af-bbe4-a1cb337e1a3a.png)


If you open the model.h file, it will look like the one below :


![image](https://user-images.githubusercontent.com/101800565/202812054-d19a255e-6997-4776-bd9b-e10b4a9550ed.png)


***-Inference in Arduino Nano BLE 33 :***
Open accelerometer clasifier sketch on this repository named as Inference.ino. On the far top right, click on the drop down button and click New Tab. Name the file as model.h when you save it. Open the downloaded model.h file on your favourite editor and copy the content. Paste in your new model.h file you created alongside the classifier sketch and save.
To calculate the Median absolute deviation a C code was written by ShawnHymel and you may find it as utils.h and utils.c You also need to add it to your arduino project.


**like this :**


![image](https://user-images.githubusercontent.com/101800565/202812577-f5ab22bb-cb18-4aeb-bf7d-c9fb72c18aef.png)


***-Results :***

![image](https://user-images.githubusercontent.com/101800565/202811756-dac0592e-f606-4516-9910-548b41cec9f7.png)

