## Convert to all data format (labelme annotation and coco formatted dataset at once)

- Follow the folder structure mentioned below and run the command below. It will create the both dataset
- This two folder (./data_prep/test ./data_prep/train) will be created and deleted in runtime.
- "." is the current working dir
- dataset_BRUMAS_v1 can be replaced with the respective dataset folder name.

```
python prepare.py ./data_prep/test ./data_prep/train . dataset_BRUMAS_v1
```


## mask_to_labelme.py

- This file converts the GT mask to labelMe annotations.
- To convert the GT mask to LabelMe annotation run the following command. But please follow the folder structure as shown below the command. Please use absolute path of the working Dir.

```
python mask_to_labelme.py <Path/of/the/working/dir> dataset_BRUMAS_v1
```

## The folder structure of the dataset and converter script:

```
Cho.Ai
└───dataset_BRUMAS_v1
│   └───done                   # Image
│   │   │   X.jpg
│   │   │   ...
│   └───gt                  # GT
│   │   │   X_GT.jpg
│   │   │   ...
│   └───instance           # LabelMe annotation (created after running the mask_to_labelme.py or the prepare.py
│   │   │   X.json
│   │   │   ...
|   |   
|   └───classesNumber.txt
└───mask_to_labelme.py
└───labelme2cocoInstance.py
└───prepare.py
...
```

## Install LabelMe

```
pip install labelme
```

## Open LabelMe
- open terminal
- Type: labelme and hit enter
- Once the labelme opened just open the JSON file. It will open the image with the annotation

![alt text](https://github.com/humanize-tech/Cho.Ai/blob/main/converters/images/example_labelme.png)
