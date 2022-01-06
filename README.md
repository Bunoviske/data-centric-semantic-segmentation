# Data Framework for Semantic/Instance Segmentation

Bunch of different tools which helps visualizing, transforming and annotating images for semantic/instance segmentation tasks. Check each folder to find these different tools.

## Ground Truth Generation

Labeling tool that creates masks for your semantic segmentation problem. It uses watershed algorithm to boost annotation speed.

## Ground Truth Generation with Object Detection

Labeling tool that leverages some Object Detection Model which already give the masks for your problem 

## Ground Truth Analysis

Checks class distribution from a semantic segmentation dataset

## Data Inspection

Go through your whole dataset and choose which images are good or bad. This is a very important tool if you need clean data and wants to 
build a project with Data-Centric approach

## Dataset Stratification

Multi label dataset stratification can be really hard to execute. I propose a simple approach that keeps the class balance 
of your semantic segmentation dataset

## Class weights

If your dataset suffers from class imbalance, you need to calculate the weights if you want to apply them to your loss function




