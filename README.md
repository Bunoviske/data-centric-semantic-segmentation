# Data Centric Framework for Semantic/Instance Segmentation

Bunch of different tools which helps visualizing, transforming and annotating images for semantic/instance segmentation tasks. Check each folder to find these different tools.

## Install dependencies

In general, the tools were developed in python, so here are some steps in order to configure your environment. I recommend installing Miniconda3 (https://docs.conda.io/en/latest/miniconda.html) and do the following:

```
conda create -n myenv python=3.8
conda activate myenv
pip install -r requirements.txt
```

## Tools list


### Ground Truth Generation

Labeling tool that creates masks for your semantic segmentation problem. It uses watershed algorithm to boost annotation speed (check inside the folder for more details).

### Ground Truth Generation with Object Detection

Labeling tool that leverages some Object Detection Model which already give the masks for your problem. Then you just need to assign the classes
for each generated mask (check inside the folder for more details).

### Ground Truth Analysis

Checks class histogram from a semantic segmentation dataset and verify images size distribution.

### Data Inspection

Go through your whole dataset and choose which images are good or bad. This is a very important tool if you need clean data and wants to 
build a project with a Data-Centric approach.

Sometimes it won't work in VSCODE, so start Jupyterlab to run the notebooks from this folder: `jupyter lab --no-browser`

### Dataset Stratification

Multi label dataset stratification can be really tricky. I propose a simple approach that keeps the class balance 
of your trainset and testset.

### Class weights

If your dataset suffers from class imbalance, you need to calculate the weights if you want to apply them to your loss function or 
your Dataloader Sampler.


### Any question you can get in contact

Linkedin: https://www.linkedin.com/in/brunofcarvalho1996/
Email: brunofcarvalho1996@gmail.com
